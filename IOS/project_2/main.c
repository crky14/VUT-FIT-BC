/*
	Subor: main.c
	Datum: 2016/2017
	Autor:   Jakub Crkoň, xcrkon00@stud.fit.vutbr.cz
	Projekt: Synchronizacia paralelnych procesov, proj2 do predmetu IOS
	Popis:   Riesenie problemu child-care center
*/
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "childcare.h"

int main(int argc, char *argv[]) {

	// variables initialization
	int shmem_id = 0;
	int semph_id = 0;
	s_shm *shmem = NULL;
	s_sem *semph = NULL;
	pid_t pid, main_parent;


	// loading parameters to struct params                        
	s_params params = get_params(argc, argv);
	if(params.error != OK) {
		print_error(params.error);
		return 1;
	}

	// creating uniqe key for shared memory creation      
	key_t key_shm = ftok("proj2", 1);  
	if (errno != 0) {
		return 2;
	}
	//creating shared memory										
	if((shmem_id = shmget(key_shm, sizeof(s_shm), IPC_CREAT | 0666)) < 0){
		return 2;	
	}
	//assigning created memory to shmem
	if((shmem = (s_shm *) shmat(shmem_id, NULL, 0)) == (void *) -1) {
		clear_shm(shmem_id, 0, NULL, NULL);
		return 2;
	}
	// shared memory variables initialization
	shmem->adult = 0;
	shmem->child = 0;
	shmem->action = 0;  
	shmem->comming = 0;
	shmem->leaving = 0;
	shmem->queue =0;
	shmem->child_count = 0;
	shmem->adult_count = 0;
	shmem->barrier_count = 0;
	// creating uniqe key for semaphores shared memory
	key_t key_sem = ftok("proj2", 2);
	if (errno != 0) {
		clear_shm(shmem_id, 0, shmem, NULL);
		return 2;
	}
	// creating semaphores shared memory
	if((semph_id = shmget(key_sem, sizeof(s_sem), IPC_CREAT | 0666)) < 0) {
		clear_shm(shmem_id, 0, shmem, NULL);
		return 2;
	}

	// assign shared memory to semph
	if((semph = (s_sem *) shmat(semph_id, NULL, 0)) == (void *) -1) {
		clear_shm(shmem_id, semph_id, shmem, NULL);
		return 2;
	}
	// semaphores initialization
	if((params.error = init_sems(&params, semph)) != 0) {
		print_error(params.error);
		return 2;
	}
	//opening file for writing
	params.output = fopen("proj2.out", "w");
	if(params.output == NULL) {
		print_error(FILE_ERR);
		return 2;
	}

	srand (time (NULL));
	// creating main adult and main child process
	main_parent = getpid();     
	pid = fork();
	if(pid > 0) {
		pid = fork();
		if(pid == 0){
			first_child(&params, shmem, semph);
			fclose(params.output);
			exit(0);
		}
	}
	else{
		first_adult(&params, shmem, semph);
		fclose(params.output);
		exit(0);
	}
	if(main_parent == getpid()){
		//waiting for first child and first adult to end
		while((pid = wait(NULL)) > 0);
	}
	// dealocation of memory and closing file
	fclose(params.output);
	destroy_sems(&params, semph);
	clear_shm(shmem_id, semph_id, shmem, semph);
	return 0;
}