#include <stdio.h>
#include "dict.h"

void main(){
	// Create Dictioanry 
	printf("[-] Creating Dictionary\n");
	Dictionary* d = createDictionary();

	// Add Someting to Dictionary
	printf("[+] Adding example values to dictionary\n");
	addLabel(d, "Example", "test");
	addLabel(d, "Test", "1..2..3..");

	// Retrieve a value from dictionary 
	printf("%s\n",getItem(d, "Example"));
	printf("%s\n",getItem(d, "Test"));

	// Destroy Dictionary (cleans up memory)
	printf("[x] Destroying Dictionary\n");
	destroy(d);
}