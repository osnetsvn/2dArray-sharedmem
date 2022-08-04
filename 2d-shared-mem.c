/* This is the implementation of hashtable using 2D array
* The code uses SHA-256 to compute the hash and creates a hashtable with Y*Y (Y is the size)
* Copyright 2022 Roja Eswaran, PhD Candidate <reswara1@binghamton.edu>
* Copyright 2022 Prf Kartik Gopalan <kartik@binghamton.edu>
*/


#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "./sha-256.h"
#define Size 10000
long mem_size = 1024;



struct hash_value{
	long pfn_key;
	long pfn_value;
};


struct shared_mem{

	struct hash_value (*ht)[Size];
};



struct shared_mem *ptr;
unsigned long hash_function(char* str) {

        uint8_t hash[32];
        char string [10000];


        unsigned long i = 0;



        calc_sha_256(hash, str, strlen(str));
        hash_to_string(string, hash);

        for (long j=0; string[j]; j++)
                i += string[j];

  //      printf("I:%ld\n", i);
        return i % Size;
}



long singled_array_insert(long value, long *arr){
        long y = 0; 

        while(1){

                if(!arr[y]){
                        arr[y] = value;
                        return y;

                }
                
               y++;

               if(y == Size)
                        return -1;

        }
}


int singled_array_search(long value, long *arr){
        long y = 0; 

        while(1){
                if(arr[y]== value)
                        return 1;
                else  if(!arr[y])
                        return -1;
 
                
               y++;
        }
}


int array_search(long x, long key, struct hash_value(*arr)[Size]){
        long y = 0; 

        while(1){
                if(arr[x][y].pfn_key == key)
                        return arr[x][y].pfn_value;
                else  if(arr[x][y].pfn_key==-1)
                        return -1;
 
                
               y++;
        }
}

long array_insert(long x, long key, long value, struct hash_value(*arr)[Size]){

        long y = 0; 

        while(1){

		printf("X:%ld Y:%ld\n", x, y);
                if(arr[x][y].pfn_key == -1){
			printf("Inserting:%ld %ld\n", x, y);
                        arr[x][y].pfn_key = key;
                        arr[x][y].pfn_value = value;
                        return y;

                }
                
               y++;

               if(y == Size)
                        return -1;

        }
}


int hash_search( long key, struct hash_value (*arr)[Size]){
    
        char str_key[10000];
        sprintf(str_key, "%lx", key);
        long index = hash_function(str_key);
        
        return array_search(index, key, arr);



}


int hash_insert(long key, long value, struct hash_value (*arr)[Size]){
        char str_key[10000];
        sprintf(str_key, "%lx", key);
        long index = hash_function(str_key);

       
        return array_insert(index, key, value, arr);

}



int main()
{



	key_t key;
	int shmid1;

	ptr = (struct shared_mem *)malloc(sizeof(struct shared_mem)); 
	mem_size = mem_size *1024* 1024;
 




	key = ftok("./pfn_array",16);
	shmid1 = shmget(key, mem_size, 0666 | IPC_CREAT);
	if (shmid1 < 0) {
		printf("*** shmget error (server) pfn ***\n");
		exit(1);
	}

	ptr->ht = shmat(shmid1, NULL, 0);



	long value;
	long l_key = 123;
	hash_insert(l_key, 111, (ptr->ht));
	if((value=hash_search(l_key, ptr->ht))!=-1)
		printf("Elt present:%ld\n", value);
	
	

	return 0;

}
