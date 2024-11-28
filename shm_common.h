#define SIZE 9 // size of array
#define SHM_KEY1 1234 // key for the shared memory segment shared among player1, player2, checker.c
#define SHM_KEY2 4321 // key for the shared memory segment shared among player1.c, player2.c and checker.c

char *arr; //this is the return address by shared memory attach function and so it can serve as the base address of array
int *turn;

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> 
#include <time.h> 
#include <stdbool.h>
