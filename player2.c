// Player 2
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> // For uintptr_t
#include <time.h> // Include this header for time()

#include "shm_common.h"

int main () {
    
    // Allocation of memory segment.
    int shmid1;	//shared memory id, returned from shmget
	
	shmid1= shmget((key_t)SHM_KEY1, sizeof(arr), 0666 | IPC_CREAT);

    if (shmid1 == -1) {
        fprintf(stderr, "shmget failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    
    int shmid2;	//shared memory id, returned from shmget
	
	shmid2= shmget((key_t)SHM_KEY2, sizeof(arr), 0666 | IPC_CREAT);

    if (shmid2 == -1) {
        fprintf(stderr, "shmget failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    

    // Attach memory segment
    int *arr = (int *) shmat(shmid1, (void *)0, 0);	//address of first byte of the shared memory, returned from shmat
    int *turn = (int *) shmat(shmid2, (void *)0, 0);	//address of first byte of the shared memory, returned from shmat

    if (arr == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

    if (turn == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

    printf("Memory for game attached at %lX\n",  (unsigned long)(uintptr_t)arr);
    printf("Memory for turn attached at %lX\n",  (unsigned long)(uintptr_t)turn);
    
    printf("\nNOTE :: Every time when your turn arrives, you will enter two numbers, the row and the column where you want to put your mark\n");
    
    turn[4] = 1; 
    turn[0] = 0;
    int count = 0;
    while(true){
        if(turn[0] == -1){
            break;
        }
        else if(turn[0] == 1){
            if (count%3 == 0) printf("\n>>> Waiting for Player 1 to respond..");
            sleep(1);
            count +=1;
        }
        else if(turn[0] == 0){
            if(count%3==0) printf("\n>>> Waiting for Validator...");
            sleep(1);
            count+=1;
        }
        else{
        	count = 0;
            int x,y;
            printf("\n\nEnter your move : ");
            scanf("%d",&x);
            scanf("%d",&y);
            if (arr[3*(x-1) + (y-1)] != ' ') {
                printf("\n>>> This position is already marked. Enter a vaild position") ;
                continue;
            }
            arr[3*(x-1) + (y-1)] = 'O';
            turn[0] = 0;
        }
    }

    if (turn[2] == 1){
    	printf("\n\n########################\n");
        printf("#   YOU WIN. Congrats  #\n");
        printf("########################\n\n");
    }
    else if(turn[1] == 1 ){
        printf("\n\n####################################\n");
        printf("# YOU LOSE. Better luck next time. #\n");
        printf("####################################\n\n");
    }
    else{
    	printf("\n\n##################################\n");
        printf("# THE GAME WAS DRAW. Well played #\n");
        printf("##################################\n\n");
    }

    // detach
    if (shmdt(arr) == -1) {
        fprintf(stderr, "shmdt failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    if (shmdt(turn) == -1) {
        fprintf(stderr, "shmdt failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    
    return 0 ;
}
