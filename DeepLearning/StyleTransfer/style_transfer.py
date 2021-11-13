# Adapted from the TensorflowExamples repo by aymericdamien
# https://github.com/aymericdamien/TensorFlow-Examples
from tqdm import tqdm
import numpy as np
import argparse
import utils
import time
import sys
import os
import re

# PyTorch Model Imports 
from torch.utils.tensorboard import SummaryWriter
from transformer_net import TransformerNet
from torch.utils.data import DataLoader
from torchvision import transforms
from torchvision import datasets
from torch.optim import Adam
from PIL import ImageFile
from vgg import Vgg16
import torch

ImageFile.LOAD_TRUNCATED_IMAGES = True
writer = SummaryWriter()

def check_paths(args):
	try:
		if not os.path.exists(args.save_model_dir):
			os.makedirs(args.save_model_dir)
		if args.checkpoint_model_dir is not None and not (os.path.exists(args.checkpoint_model_dir)):
			os.makedirs(args.checkpoint_model_dir)
	except OSError as e:
		print(e)
		sys.exit(1)

def training(args):
	# Define How to interact with host/cuda 
	device = torch.device("cuda" if args.cuda else "cpu")

	# Initialize PyTorch 
	np.random.seed(args.seed)
	torch.manual_seed(args.seed)
	print(f'[+] Using Seed: {args.seed}')

	# Build Model to Train
	transform = transforms.Compose([
		transforms.Resize(args.image_size),
		transforms.CenterCrop(args.image_size),
		transforms.ToTensor(),
		transforms.Lambda(lambda x: x.mul(255))
	])
	train_dataset = datasets.ImageFolder(args.dataset, transform)
	train_loader = DataLoader(train_dataset, batch_size=args.batch_size)

	# Create Style Transformer
	transformer = TransformerNet().to(device)
	optimizer = Adam(transformer.parameters(), args.lr)
	mse_loss = torch.nn.MSELoss()
	vgg = Vgg16(requires_grad=False).to(device)
	
	# Build Style transformer
	style_transform = transforms.Compose([
		transforms.ToTensor(),
		transforms.Lambda(lambda x: x.mul(255))
	])
	style = utils.load_image(args.style_image, size=args.style_size)
	style = style_transform(style)
	style = style.repeat(args.batch_size, 1, 1, 1).to(device)
	features_style = vgg(utils.normalize_batch(style))
	gram_style = [utils.gram_matrix(y) for y in features_style]

	# Create Optimizer for Training and Loss Function 
	device = torch.device("cuda" if args.cuda else "cpu")
	transformer = TransformerNet().to(device)
	optimizer = Adam(transformer.parameters(), args.lr)
	mse_loss = torch.nn.MSELoss()

	# create progress bar because training can take a tong time
    progress = tqdm(total=args.epochs*len(train_loader))
    start = time.time()
    # Begin Training :D
    for e in range(args.epochs):
    	transformer.train()
    	agg_content_loss = 0. 
    	agg_style_loss = 0. 
    	count = 0 

    	# Iterate over each batch of training set 
    	for batch_id, (x, _) in enumerate(train_loader):
    		n_batch = len(x)
    		count += n_batch
    		optimizer.zero_grad()

    		x = x.to(device)
    		y = transformer(x)
    		y = utils.normalize_batch(y)
    		x = utils.normalize_batch(x)

    		features_y = vgg(y)
    		features_x = vgg(x)

    		content_loss = args.content_wegiht * mse_loss(features_y.relu2_2,
    													  features_x.relu2_2)
    		style_loss = 0.
    		for ft_y, gm_s in zip(features_y, gram_style):
    			gm_y = utils.gram_matrix(ft_Y)
    			style_loss += mse_loss(gm_y, gm_s[:n_batch, :, :])
    		style_loss += args.style_weight

    		# Add up Loss and Backpropgate 
    		total_loss = content_loss + style_loss
    		total_loss.backward()
    		optimizer.step()

    		agg_content_loss += content_loss.item()
    		agg_style_loss += style_loss.item()
    		writer.add_scalar("Style Loss/train", agg_style_loss / (batch_id + 1), batch_id+1)
			writer.add_scalar("Content Loss/train", agg_content_loss / (batch_id + 1), batch_id+1)
			writer.add_scalar("Total/train", (agg_content_loss + agg_style_loss) / (batch_id + 1), batch_id+1)
            
			if (batch_id +1) % 1000 == 0:
				writer.flush()
			# Iteration of training loop completed, update the progress bar 
			progress.update(1)     

	# FINISHED, save and close shop
	print('[+] *** TRAINING COMPLETED *** [%ss Elapsed]' % str(time.time() - start))
	transformer.eval().cpu()
	
	tstamp = str(time.ctime()).replace(' ', '_') + "_"
	save_model_path = os.path.join(args.save_model_dir, f'epoch_{str(args.epochs)}_{tstamp}.model')
	torch.save(transformer.state_dict(), save_model_path)
	print("Model saved to %s" % save_model_path)	
	writer.close()

def stylize(args):
	device = torch.device("cuda" if args.cuda else "cpu")

	content_image = utils.load_image(args.content_image, scale=args.content_scale)
	content_transform = transforms.Compose([
		transforms.ToTensor(),
		transforms.Lambda(lambda x: x.mul(255))
	])
	content_image = content_transform(content_image)
	content_image = content_image.unsqueeze(0).to(device)

	with torch.no_grad():
		style_model = TransformerNet()
		state_dict = torch.load(args.model)
		# remove saved deprecated running_* keys in InstanceNorm from the checkpoint
		for k in list(state_dict.keys()):
			if re.search(r'in\d+\.running_(mean|var)$', k):
				del state_dict[k]
		style_model.load_state_dict(state_dict)
		style_model.to(device)
		output = style_model(content_image).cpu()
	utils.save_image(args.output_image, output[0])


def main():
main_arg_parser = argparse.ArgumentParser(description="parser for fast-neural-style")
	subparsers = main_arg_parser.add_subparsers(title="subcommands", dest="subcommand")
	# Check for args regarding training a model
	train_arg_parser = subparsers.add_parser("train", help="parser for training arguments")
	train_arg_parser.add_argument("--epochs", type=int, default=2,
								  help="number of training epochs, default is 2")
	train_arg_parser.add_argument("--batch-size", type=int, default=4,
								  help="batch size for training, default is 4")
	train_arg_parser.add_argument("--dataset", type=str, required=True,
								  help="path to training dataset, the path should point to a folder "
									   "containing another folder with all the training images")
	train_arg_parser.add_argument("--style-image", type=str, default="images/style-images/mosaic.jpg",
								  help="path to style-image")
	train_arg_parser.add_argument("--save-model-dir", type=str, required=True,
								  help="path to folder where trained model will be saved.")
	train_arg_parser.add_argument("--checkpoint-model-dir", type=str, default=None,
								  help="path to folder where checkpoints of trained models will be saved")
	train_arg_parser.add_argument("--image-size", type=int, default=256,
								  help="size of training images, default is 256 X 256")
	train_arg_parser.add_argument("--style-size", type=int, default=None,
								  help="size of style-image, default is the original size of style image")
	train_arg_parser.add_argument("--cuda", type=int, required=True,
								  help="set it to 1 for running on GPU, 0 for CPU")
	train_arg_parser.add_argument("--seed", type=int, default=42,
								  help="random seed for training")
	train_arg_parser.add_argument("--content-weight", type=float, default=1e5,
								  help="weight for content-loss, default is 1e5")
	train_arg_parser.add_argument("--style-weight", type=float, default=1e10,
								  help="weight for style-loss, default is 1e10")
	train_arg_parser.add_argument("--lr", type=float, default=1e-3,
								  help="learning rate, default is 1e-3")
	# Check for Args regarding Evaluating existing Model 
	eval_arg_parser = subparsers.add_parser("eval", help="parser for evaluation/stylizing arguments")
	eval_arg_parser.add_argument("--content-image", type=str, required=True,
								 help="path to content image you want to stylize")
	eval_arg_parser.add_argument("--output-image", type=str, required=True,
								 help="path for saving the output image")
	eval_arg_parser.add_argument("--model", type=str, required=True,
								 help="saved model to be used for stylizing the image.")
	eval_arg_parser.add_argument("--cuda", type=int, required=True,
								 help="set it to 1 for running on GPU, 0 for CPU")
	
	args = main_arg_parser.parse_args()

	if args.subcommand is None:
		print("ERROR: specify either train or eval")
		sys.exit(1)
	if args.cuda and not torch.cuda.is_available():
		print("ERROR: cuda is not available, try running on CPU")
		sys.exit(1)

	if args.subcommand == "train":
		check_paths(args)
		train(args)
	else:
		stylize(args)



if __name__ == '__main__':
	main()
