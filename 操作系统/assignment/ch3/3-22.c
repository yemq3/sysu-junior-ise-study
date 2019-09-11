#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc,char *argv[]) 
{
    const int SIZE = 4096;
    const char *name = "3-22";
    int fd;
    int *ptr;

    int number = atoi(argv[1]); 
	pid_t pid;

	/* fork a child process */ 
	pid = fork();
	
	if (pid < 0) { /* error occurred */ 
		fprintf(stderr, "Fork Failed"); 
		return 1; 
	} 
	else if (pid == 0) { /* child process */ 
        fd = shm_open(name,O_CREAT|O_RDWR,0666);
        ftruncate(fd, SIZE);
        ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		while(number != 1){
			*ptr = number;
            ptr++;
			if(number % 2){
				number = 3 * number + 1;
			} // number is odd
			else{
				number = number / 2;
			} //number is even
		}
		*ptr = 1;
		printf("Child Complete\n");
    } 
    else { /* parent process */ 
    	/* parent will wait for the child to complete */
    	wait(NULL);
        fd = shm_open(name, O_RDONLY , 0666);
        ptr = (int *) mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        while(*ptr != 1){
            printf("%d ",*ptr);
            ptr++;
        }
		printf("%d\n", *ptr);
        shm_unlink(name);
    	printf("Parent Complete\n");
    } 
    
    return 0;
}