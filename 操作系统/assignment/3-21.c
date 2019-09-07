#include <sys/types.h> 
#include <stdio.h> 
#include <unistd.h>
int main(int argc,char *argv[]) 
{
    int number = atoi(argv[1]); 
	pid_t pid;

	/* fork a child process */ 
	pid = fork();
	
	if (pid < 0) { /* error occurred */ 
		fprintf(stderr, "Fork Failed"); 
		return 1; 
	} 
	else if (pid == 0) { /* child process */ 
		while(number != 1){
			printf("%d ",number);
			if(number % 2){
				number = 3 * number + 1;
			} // number is odd
			else{
				number = number / 2;
			} //number is even
		}
		printf("1\n");
    } 
    else { /* parent process */ 
    	/* parent will wait for the child to complete */
    	wait(NULL);
    	printf("Complete\n");
    } 
    
    return 0;
}