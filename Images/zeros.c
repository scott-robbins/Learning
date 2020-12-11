#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	printf("%s\n", argv[1]);
	
	if(argc > 2){
		int M = atoi(argv[1]);
		int N = atoi(argv[2]);
		printf("Creading [%dx%d] Array\n", M, N);

		int mat[N][M];
		int i = 0; int j = 0; int total_elemnts = 0;
		for(i=0;i < N; i++){
			for(j=0;j < M; j++){
				mat[N][M] = 0;
				total_elemnts = total_elemnts + 1;
			}
			
		}
		printf("%d Total Elements in Array\n", total_elemnts);
		// free up that memory now that we are done
		// free(&mat);
		
		
  	}

	return 0;
}