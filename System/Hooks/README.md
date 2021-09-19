# Hooks 
The execution of a compiled binary can be examined or entirely changed if we compile a shared libary that contains functions with the same names, and then  
use `LD_PRELOAD` to have this library loaded as an environment variable for the executable. Take a really basic example below. 

## Introduction to Hooks
Here is a really simple function, saved as badlogin.c. It checks if the user has provided (as an argument input) a string which matches a harcoded password. 
In this case a reverse engineer would easily be able to reverse the program to see the string, but that would be done with "static analysis". That's a process 
where you examine the file itself, piecing together what the code will do or looking for things like this hardcoded string. Here we can simply run our vulnerable 
program with our sneaky library loaded and have the strcmp call simply print the values being compared:
```
$ LD_PRELOAD=./hook.so ./login HAH!
=== Checking Your Credentials ===
[strcmp : HAH!,	0x53c2e7]
	~| A C C E S S  D E N I E D |~
```

```
$ LD_PRELOAD=./hook.so ./login 0x53c2e7
=== Checking Your Credentials ===
[strcmp : 0x53c2e7,	0x53c2e7]
	*** ACCESS GRANTED ***
```

## Running real applications
By intercepting `strlen()` calls for example, I can wait to see the `Untitled Document 1` string to come through (for the window title) and change the pointer to reference and entirely different string!


![hooked](https://raw.githubusercontent.com/scott-robbins/Learning/main/System/hooked.gif)
