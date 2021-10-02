# Operating Systems 
Before diving into a full operating system, we need to probably start with a device driver or a kernel module. Either way we will want a way to spin up custom virtual machines easily, and some other tools to help development. 


## Getting Started
The following is my working through the project hosted at: https://jsandler18.github.io/. I will include snippets so you can follow along. I'll then build on from there and customize a bit! 

## Environemnt 
First we will need a way to test up a system for virtualization that allows us to run the custom OS or an OS with custom modules added. QEMU is a popular open source tool for this. You can install it with apt-get using the following:
```bash
sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin
```

Create a Virtual hard disk image for the VM.
```bash
qemu-img create ubuntu_custom.img 16G
```

Boot the Virtual machine:
```bash
qemu-system-x86_64 -name TEST -cpu Nehalem -m 1024 -cdrom ubuntu-16.04.7-desktop-amd64.iso -drive format=raw,file=ubuntu_custom.img
```

## Boot Process
For a barebones kernel we will need the follow
```language
    boot.S - kernel entry point that sets up the processor environment
    kernel.c - kernel routines
    linker.ld - for linking the files above 
```

For compiling the code for the OS we need a special version of GCC:
`apt install gcc-arm-none-eabi`

### boot.S


### Kernel.c
The bulk of the code is for setting up the hardware for basic I/O. The I/O is done through the UART hardware, which allows us to send and recieve text data through the serial ports. 

The only way to take advantage of this on actual hardware is to get a USB to TTL serial cable. Since I don’t have one of those cables, I am going interact with the kernel through the VM until we can get more sophisticated I/O like HDMI out and USB keyboard.

Aside from the hardware setup, there are a few helper functions to abstract away the hardware, and, of course, the main function.

### linker.ld
There are, broadly speaking, three main steps in the C compilation process. The first is preproccessing, where all of your #define statements are expanded. The second is compilation to object files, where the individual code files are converted to individual binaries called object files. The third is linking, where these individual object files are tied together into a single executable.

By default, GCC links your program as if it were user level code. We need to override the default, because our kernel is not an ordinary user program. We do this with a linker script. Here is the linker script we will be using:

### Testing
To compile and link together these three files into an image that can be run in the VM, use:
```language
arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o
arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra
arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
```
To run the code in the VM, execute this command:
```language
qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf
```

### Better Building 
we are going to do it is to separate C files, header files, and compilation files into separate directories: src, include, and build respectively. src and include are going to be structured very similarly, so when I talk about one, assume it holds for the other unless otherwise stated.

`src` and `include` are going to have subdirectories `kernel` and common, where `kernel` is for files that are exclusive to the kernel, and common is for files that contain standard functionality that may not be exclusive to the kernel, such as `memcpy`

build is going to contain our linker file, and a makefile. During compilation, the makefile will put all object files, as well as the compiled kernel binary in the build directory.

Here is the code for the makefile:
```language
# Don't use normal gcc, use the arm cross compiler
CC = ./arm-none-eabi-gcc

# Set any constants based on the raspberry pi model.  Version 1 has some differences to 2 and 3
ifeq ($(RASPI_MODEL),1)
    CPU = arm1176jzf-s
    DIRECTIVES = -D MODEL_1
else
    CPU = cortex-a7
endif

CFLAGS= -mcpu=$(CPU) -fpic -ffreestanding $(DIRECTIVES)
CSRCFLAGS= -O2 -Wall -Wextra
LFLAGS= -ffreestanding -O2 -nostdlib

# Location of the files
KER_SRC = ../src/kernel
KER_HEAD = ../include
COMMON_SRC = ../src/common
OBJ_DIR = objects
KERSOURCES = $(wildcard $(KER_SRC)/*.c)
COMMONSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KERSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(COMMONSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))
HEADERS = $(wildcard $(KER_HEAD)/*.h)

IMG_NAME=kernel.img

build: $(OBJECTS) $(HEADERS)
    echo $(OBJECTS)
    $(CC) -T linker.ld -o $(IMG_NAME) $(LFLAGS) $(OBJECTS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
    mkdir -p $(@D)
    $(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
    mkdir -p $(@D)
    $(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
    mkdir -p $(@D)
    $(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

clean:
    rm -rf $(OBJ_DIR)
    rm $(IMG_NAME)

run: build
    qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel kernel.img
```
The Folder structure to use for this project should look like this:
```language
raspi-kernel/
L__ src/
    L__ common/
    L__ kernel/
        L__ kernel.c
        L__ boot.S
L__ include/
    L__ common/
    L__ kernel/
L__ build/
    L__ Makefile
    L__ linker.ld
```
But this is not well organized, as the entirety of the C code for handling everyhting we need is in kernel.c, and as we add more features we will want to spread this functionality out across many more files. 

Right now, kernel.c contains all of the C source code for the entire kernel. It contains the logic for setting up UART, and for doing IO. Let’s factor that stuff out into logical files.

First, we are going to put that that big enum from kernel.c that describes the UART peripheral, and all of the function signatures related to UART into include/kernel/uart.h. Then we are going to move all of the uart code to src/kernel/uart.c. This should leave kernel.c with just kernel_main in it.

Now, we are going to write some library code. We create the files src/common/stdlib.c and src/common/stdio.c and corresponding header files.

In stdlib.c, we define the functions memcpy and bzero, as these will come in handy later, and we define itoa (integer to ascii) to make debugging easier.

In stdio.c, we define getc, putc, gets and puts as general purpose IO functions. We do this even though uart.c had uart_putc and uart_puts because later we are going to want to swap out uart_putc for a function that renders text to an actuall screen, and it will be easier to replace one call to uart_putc here than many possible places.

```language
raspi-kernel/
L__ src/
    L__ common/
        L__ stdio.c
        L__ stdlib.c
    L__ kernel/
        L__ kernel.c
        L__ boot.S
        L__ uart.c
L__ include/
    L__ common/
        L__ stdio.h
        L__ stdlib.h
    L__ kernel/
        L__ uart.h
L__ build/
    L__ Makefile
    L__ linker.ld
```
The user can then use the conventional build & install commands, and our makefile will be able to act accordingly:

```
./configure --prefix=/usr/local --disable-debug
make
sudo make install
```