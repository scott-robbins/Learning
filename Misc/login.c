#include <stdio.h>
#include <string.h>

# define ADMIN "admin"
# define PASSW "b4rr1cud4"

struct User {
	char *name;
	char *password;
	unsigned int balance; /*multiplied by 100 to always be positive integer*/
};


int checkCredentials(const char *u, const char *p){
	int verified = 1;
	if(strcmp(ADMIN, u) && strcmp(PASSW, p)){
		verified = 0;
	}

	return verified;
}


void showMenu(){
	char moveon[40];
	while (strcmp(moveon, "Y")){
		printf("[%d] - Check Balance\n", 1);
		printf("[%d] - Withdrawal\n", 2);
		printf("[%d] - Deposit\n", 3);
		printf("[%d] - Account Info\n", 4);
		// Let user choose
		int choice;
		printf("Enter a selection: \n");
		scanf("%d", &choice);
		// Determine option chosen and handle it
		switch(choice) {
			case 1  :
				printf("Balance\n");
				break; 
			case 2  :
				printf("Make a Withdrawal\n");
				break; 
			case 3  :
				printf("Making a Deposit\n");
				break; 
			case 4  :
				printf("Account Info\n");
				break;
		  	default : /* Optional */
				printf("Please Enter a valid selection.\n");
}
		// Let User Exit
		printf("Would you Like to Exit? [Y/N]:\n");
		scanf("%s", moveon);
	}
}


int main(int argc, char *argv[]){
	// check arg formatting is correct
	if(argc < 3){
		printf("[!!] Usage: login <username> <password>\n");
		return -1;
	}

	char uname[8];
	char password[12];

	// check size! 
	if (strlen(argv[1]) > 8){
		printf("[!!] Username Provided Appears too long. Exiting...\n");
		return -1;
	}
	if (strlen(argv[2]) > 12){
		printf("[!!] Password Provided Appears too long. Exiting...\n");
		return -1;
	}
	strcpy(uname, argv[1]);
	strcpy(password, argv[2]);
	// Try to Login
	if (checkCredentials(uname, password)){
		printf("~ Welcome to Linux Bank %s~\n", ADMIN);
		// Show the bank menu
		showMenu();

	}else{
		printf("ACCESS DENIED\n");
		return -1;
	}

}