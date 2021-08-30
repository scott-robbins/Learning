#!/bin/bash
ls *.zip | while read n;
do 
	unzip -q $n; 
done

ffmpeg -loglevel panic -i frames/img%03d.jpeg -r 2 footage.mp4 >> /dev/null
clear

# Cleaning up frame folder
rm -rf frames

# play video 
vlc footage.mp4
#EOF
