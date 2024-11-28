// validator.c

//A code file which acts as the checker which will take the input from both players and will check which of them won...

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> // For uintptr_t
#include <time.h> // Include this header for time()

#include "shm_common.h"

bool wins_X (int* arr) {
    if (arr[0]=='X' && arr[1]=='X' && arr[2]=='X') {
        return true;
    }
    else if (arr[3]=='X' && arr[4]=='X' && arr[5]=='X') {
        return true;
    }
    else if (arr[6]=='X' && arr[7]=='X' && arr[8]=='X') {
        return true;
    }
    else if (arr[0]=='X' && arr[3]=='X' && arr[6]=='X') {
        return true;
    }
    else if (arr[1]=='X' && arr[4]=='X' && arr[7]=='X') {
        return true;
    }
    else if (arr[2]=='X' && arr[5]=='X' && arr[8]=='X') {
        return true;
    }
    else if (arr[0]=='X' && arr[4]=='X' && arr[8]=='X') {
        return true;
    }
    else if (arr[2]=='X' && arr[4]=='X' && arr[6]=='X') {
        return true;
    }
    else{
        return false;
    }
    
}

bool wins_O (int* arr) {
    if (arr[0]=='O' && arr[1]=='O' && arr[2]=='O') {
        return true;
    }
    else if (arr[3]=='O' && arr[4]=='O' && arr[5]=='O') {
        return true;
    }
    else if (arr[6]=='O' && arr[7]=='O' && arr[8]=='O') {
        return true;
    }
    else if (arr[0]=='O' && arr[3]=='O' && arr[6]=='O') {
        return true;
    }
    else if (arr[1]=='O' && arr[4]=='O' && arr[7]=='O') {
        return true;
    }
    else if (arr[2]=='O' && arr[5]=='O' && arr[8]=='O') {
        return true;
    }
    else if (arr[0]=='O' && arr[4]=='O' && arr[8]=='O') {
        return true;
    }
    else if (arr[2]=='O' && arr[4]=='O' && arr[6]=='O') {
        return true;
    }
    else{
        return false;
    }
}

void print_game(int* arr){
    printf("\n\n----------------\n");
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(arr[3*i+j] == 'X'){
                printf("| ❌ ");
            }
            else if(arr[3*i+j] == 'O'){
                printf("| ⭕ ");
            }
            else{
                printf("|    ");
            }
        }
        printf("|\n----------------\n");
    }
}

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

    //initialising the tic-tac-toe by using space..

    for(int i=0;i<9;i++){
        arr[i] = ' ';
    }
    for(int i=0;i<5;i++){
    	turn[i] = 0;
    }

    printf("Memory for game attached at %lX\n",  (unsigned long)(uintptr_t)arr);
    printf("Memory for turn attached at %lX\n",  (unsigned long)(uintptr_t)turn);
    
	int count = 0;
    int turns = 0;
    turn[0] = 0;
    while(turn[3] != 1 || turn[4] != 1){
    	printf("\n>>> Waiting for both players to join....");
    	sleep(3);
    }
    
    turn[0] = 1;
	
	printf("\n\nTHE INITIAL STATE OF TIC-TAC-TOE\n");
    printf("--------------------------------");
    print_game(arr);
        
    while(turns<9){
        if(turns%2==0){
            while(turn[0]==1){
                if(count%3==0) printf("\n>>> Waiting for Player1 to respond....");
                sleep(1);
                count+=1;
            }
            print_game(arr);
            count = 0;
            if(wins_X(arr)){
                turn[1] = 1;
                turn[0] = -1;
                break;
            }
            else{
            	turn[0] = 2;
                turns++;
            }
        }
        else{
            while(turn[0]==2){
                if(count%3==0) printf("\n>>> Waiting for Player2 to respond....");
                sleep(1);
                count+=1;
            }
            print_game(arr);
            count = 0;
            if(wins_O(arr)){
                turn[2] = 1;
                turn[0] = -1;
                break;
            }
            else{
            	turn[0] = 1;
                turns++;
            }
        }
    }
    turn[0] = -1;
    
    if (turn[1] == 1){
    	printf("\n\n##########################\n");
        printf("# PLAYER 1 WINS THE GAME #\n");
        printf("##########################\n\n");
    }
    else if(turn[2] == 1 ){
        printf("\n\n##########################\n");
        printf("# PLAYER 2 WINS THE GAME #\n");
        printf("##########################\n\n");
    }
    else{
    	printf("\n\n#####################\n");
        printf("# THE GAME WAS DRAW #\n");
        printf("#####################\n\n");
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
    
    // Delete
    if (shmctl(shmid1, IPC_RMID, 0) == -1) { //Assuming shmid if (shmctl(shmid1, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    if (shmctl(shmid2, IPC_RMID, 0) == -1) { //Assuming shmid if (shmctl(shmid2, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);

    return 0 ;
}
