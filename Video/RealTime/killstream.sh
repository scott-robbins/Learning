#!/bin/bash
kill -9 $(ps aux | grep streamer.py | awk '{print $2; exit}') 
#EOF