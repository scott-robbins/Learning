# Memory 
C is great because it gives you extreme low level control for your programming, but with all power comes responsibility. 

Take the two programs included here, incorrect.c and correct.c, they both just create an MxN array and print onscreen.

Because the user defines the size of the array though, we could allocate more memory for the array than our program is inherently given. This leads to a fault because our program tried to use memory that doesn't belong to it. 

Using `malloc` to allocate memory for our dynamically sized array, and `free` to give it back when finished, no more faults :D

   