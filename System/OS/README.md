# Operating Systems
Diving into how a linux based operating system works. 

## Setting Up
To run the operating system being developed, we can use QEMU. It's a great commandline tool for launching a VM instance of our tiny OS.

I installed qemu using:
```bash
sudo apt-get install qemu-system-x86
```

Some other things you'll want to download for building, compiling and running some simple operating systems:
```
apt install nasm
apt-get install lgcc
apt-get install libc6-dev-i386.
apt-get install qemu-system-i386
apt-get install nasm xorriso mtools grub-pc-bin
```

## Basic BootSector

[TODO: DEFINE A BOOTSECTOR]

Compile the bootsector assembly code into a binary using nasm as follows:
```bash
nasm -fbin boot.asm -o boot.bin
```
Then launching using qemu, we can see the greeting! The basic boot sector is making use of the hardware. You can try using the following command to boot:
```bash
qemu-system-x86_64 boot.bin 
```

But this is a pretty useless OS. We want to load in a kernel so we can tie together all of the components of the computer and also create and support basic functions for making use of the computer, the fundamental concept of what an *operating system* is!.

I found a really clean example that I was able to wrap my head around over here from ![RockoonTecknologies](https://github.com/RockoonTechnologies/Kernel)
<!-- ![alt](https://github.com/cirosantilli/x86-bare-metal-examples/tree/master/multiboot/osdev).  -->

## Example [Kernel/]
Working with this, I'll see what I can learn, modify and build from for myself!

Checking it out and building it, it's pretty neat! You actually choose the OS to boot into, and are greeted with the OS window. The console is... wonky. At least on my system it didnt really function. But this is a pretty great starting point I think becausee the fundamentals are there.

### Booting The Code 
I Added a script for building this code and then tried to make some modifications to customize it a bit. The way it's written uses globals that seem to get corrupted, so after you clear terminal for example some stray letters remain. It's definitely got some work to be done but it is definitely a super clean and easy to understand intro! Thanks to RockoonTechnologies for the neat example! 




### Resources 
- [Boot Process](https://web.mit.edu/rhel-doc/4/RH-DOCS/rhel-rg-en-4/s1-boot-init-shutdown-process.html)
- ![OS Dev]((https://github.com/cirosantilli/x86-bare-metal-examples/tree/master/multiboot/osdev))