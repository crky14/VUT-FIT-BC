/*
	Subor: 	 childcare.c
	Datum:	 2016/2017
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


void print_error(int error_code) {
	switch(error_code) {
		case ARG_ERR :
			fprintf(stderr, "Wrong parameter\n");
			break;	
		case ARGC_ERR :
			fprintf(stderr, "Too many or not enough parameters, programme need 6 parameters\n");
			break;	
		case FILE_ERR :
			fprintf(stderr, "Error accured while opening a file\n");
			break;	
		case SHM_ERR :
			fprintf(stderr, "Error accured while creating shared memory\n");
			break;	
		case SEM_ERR :
			fprintf(stderr, "Error accured while creating shared memory for semaphores\n");
			break;	
		case INIT_ERR:
			fprintf(stderr, "Error accured while initializing semaphores\n");
			break;
		case KEY_ERR :
			fprintf(stderr, "Error accured while creating uniqe key for shared memory\n");
			break;
	}
}

s_params get_params(int argc, char *argv[]){
	//initialization of returned structure
	s_params result = {
		.A = 0,
		.C = 0,
		.AGT = 0,
		.CGT = 0,
		.AWT = 0,
		.CWT = 0,
		.error = OK,
		.output = NULL
	};
	char *endptr;
	if(argc == 7) {
		result.A = strtol(argv[1], &endptr, 10);
		if((result.A > INT_MAX) || (result.A < 1) || (errno != 0) || *endptr != '\0')
		{
			result.error = ARG_ERR;
		}
		result.C = strtol(argv[2], &endptr, 10);
		if((result.C > INT_MAX) || (result.C < 1) || (errno != 0) || *endptr != '\0')
		{
			result.error = ARG_ERR;
		}
		result.AGT = strtol(argv[3], &endptr, 10);
		if((result.AGT < 0) || (result.AGT > 5000) || (errno != 0) || *endptr != '\0')
		{
			result.error = ARG_ERR;
		}
		result.CGT = strtol(argv[4], &endptr, 10);
		if((result.CGT < 0) || (result.CGT > 5000) || (errno != 0) || *endptr != '\0')
		{
			result.error = ARG_ERR;
		}
		result.AWT = strtol(argv[5], &endptr, 10);
		if((result.CGT < 0) || (result.AWT > 5000) || (errno != 0) || *endptr != '\0')
		{
			result.error = ARG_ERR;
		}
		result.CWT = strtol(argv[6], &endptr, 10);
		if((result.CGT < 0) || (result.CWT > 5000) || (errno != 0) || *endptr != '\0')
		{
			result.error = ARG_ERR;
		}
	}
	else{
		result.error = ARGC_ERR;
	}

	return result;
}

int init_sems(s_params *params, s_sem *semph) {
	if(sem_init(&semph->memory, 1, 1)) {
      	return INIT_ERR;
	}
	if(sem_init(&semph->adult_Q, 1, 0)) {
      	return INIT_ERR;
	}
	if(sem_init(&semph->child_Q, 1, 0)) {
      	return INIT_ERR;
	}	
	if(sem_init(&semph->barrier, 1, 0)) {
      	return INIT_ERR;
    }
    if(params->AGT == 0) {
    	if(sem_init(&semph->a_spawn, 1, 0)) {
      		return INIT_ERR;
    	}
    }
    if(params->AGT == 0) {
    	if(sem_init(&semph->c_spawn, 1, 0)) {
      		return INIT_ERR;
    	}
    }
	return 0;
}

void destroy_sems(s_params *params, s_sem *semph) {
	sem_destroy(&semph->memory);
	sem_destroy(&semph->adult_Q);
	sem_destroy(&semph->child_Q);
	sem_destroy(&semph->barrier);
	if(params->AGT == 0) {
		sem_destroy(&semph->a_spawn);
    }
	if(params->CGT == 0) {
		sem_destroy(&semph->c_spawn);
    }
}

void clear_shm(int shmem_id, int semph_id, s_shm *shmem, s_sem *semph) {
	shmdt(shmem);
	shmctl(shmem_id, IPC_RMID, NULL);
	shmdt(semph);
	shmctl(semph_id, IPC_RMID, NULL);
}

void adult(s_params *params, s_shm *shmem, s_sem *semph) {
	setbuf(params->output, NULL);
	int adult_num;
	int queued = 3;
	//adult created
	sem_wait(&semph->memory);
	shmem->adult++;
	adult_num = shmem->adult;
	shmem->action++;
	fprintf(params->output,"%d\t: A %d\t: started\n", shmem->action, adult_num);
	sem_post(&semph->memory);
	//barrier to generate all adults at once
	if(params->AGT == 0) {
		if(shmem->adult == params->A) {
			sem_post(&semph->a_spawn);
		}
		sem_wait(&semph->a_spawn);
		sem_post(&semph->a_spawn);
	}
	//adult enters center, if there are children waiting letting max 3 of them to come in
	sem_wait(&semph->memory);
	shmem->action++;
	fprintf(params->output, "%d\t: A %d\t: enter\n", shmem->action, adult_num);
	shmem->adult_count++;
	if(shmem->comming > 0) {
		if(shmem->comming < 3) {
			 queued = shmem->comming;
		}
		for(int i = 0; i < queued; i++) 
			sem_post(&semph->child_Q);
		shmem->comming -= queued;
		shmem->child_count += queued;
		shmem->queue += queued;
	}
	sem_post(&semph->memory);
	//simulating work in center
	if(params->AWT > 0) {
		usleep((rand () % ((params->AWT + 1))) * 1000);
	}
	//trying to leave if possible, if not waiting
	sem_wait(&semph->memory);
	shmem->action++;
	// making child process wait again if didnt enter until adult was valid
	if(shmem->queue > 0) {
		shmem->child_count -= shmem->queue;
		shmem->comming += shmem->queue;
		shmem->block = shmem->queue;
	}
	//adult leaving center, if cant leave waiting till children leave
	fprintf(params->output, "%d\t: A %d\t: trying to leave\n", shmem->action, adult_num);
	if(shmem->child_count <= ((shmem->adult_count - 1) * 3)) {
		shmem->adult_count--;
		shmem->action++;
		fprintf(params->output, "%d\t: A %d\t: leave\n", shmem->action, adult_num);
		sem_post(&semph->memory);
	}
	else{
		shmem->leaving++;
		shmem->action++;
		fprintf(params->output, "%d\t: A %d\t: waiting : %d : %d\n", shmem->action, adult_num, shmem->adult_count, shmem->child_count);
		sem_post(&semph->memory);
		sem_wait(&semph->adult_Q);
		sem_wait(&semph->memory);
		shmem->action++;
		fprintf(params->output, "%d\t: A %d\t: leave\n", shmem->action, adult_num);
		sem_post(&semph->memory);
	}
	//letting all waiting children come in if last adult left 
	sem_wait(&semph->memory);
	if((shmem->adult_count == 0 && shmem->adult == params->A) && (shmem->comming > 0)) {
		for(int i = 0; i < shmem->comming; i++) 
			sem_post(&semph->child_Q);
		shmem->child_count += shmem->comming;
		shmem->comming = 0;
		shmem->block = 0;
	}
	shmem->barrier_count++;
	sem_post(&semph->memory);
	//waiting at barrier for all other processes
	if(shmem->barrier_count == (params->A + params->C)) {
		sem_post(&semph->barrier);
	}
	sem_wait(&semph->barrier);
	sem_post(&semph->barrier);
	sem_wait(&semph->memory);
	shmem->action++;
	fprintf(params->output, "%d\t: A %d\t: finished\n", shmem->action, adult_num);
	sem_post(&semph->memory);
}

void first_adult(s_params *params, s_shm *shmem, s_sem *semph) {
	pid_t pid;
	for(int i = 0; i < params->A; i++) {
		if(params->AGT > 0) {							
		usleep (rand () % ((params->AGT * 1000)));
		}	
		pid = fork();
		if(pid == 0) {
			adult(params, shmem, semph);
			fclose(params->output);
			exit(0);
		}
	}
	while ((pid = wait(NULL)) > 0);
}

void child(s_params *params, s_shm *shmem, s_sem *semph) {
	setbuf(params->output, NULL);
	int child_num;
	//child created
	sem_wait(&semph->memory);
	shmem->child++;
	child_num = shmem->child;
	shmem->action++;   
	fprintf(params->output, "%d\t: C %d\t: started\n", shmem->action, child_num);
	sem_post(&semph->memory);
	//barrier to generate all adults at once
	if(params->AGT == 0) {
		if(shmem->child == params->C) {
			sem_post(&semph->c_spawn);
		}
		sem_wait(&semph->c_spawn);
		sem_post(&semph->c_spawn);
	}
	//child enter center if possible, if not child is waiting 
	sem_wait(&semph->memory);
	if(shmem->child_count < (shmem->adult_count * 3) || (shmem->adult_count == 0 && shmem->adult == params->A )) {
		shmem->child_count++;
		sem_post(&semph->memory);
	}
	else {
		shmem->action++;
		fprintf(params->output, "%d\t: C %d\t: waiting : %d : %d\n", shmem->action, child_num, shmem->adult_count, shmem->child_count);
		shmem->comming++;
		sem_post(&semph->memory);
		sem_wait(&semph->child_Q);
	}
	// block process again if adult which inblocked it want to leave
	if(shmem->block > 0) {
		sem_wait(&semph->memory);
		shmem->block--;
		sem_post(&semph->memory);
		sem_wait(&semph->child_Q);
	}
	// child enter center
	sem_wait(&semph->memory);
	shmem->action++;
	if(shmem->queue > 0) {
		shmem->queue--;
	}
	fprintf(params->output, "%d\t: C %d\t: enter\n", shmem->action, child_num);
	sem_post(&semph->memory);
	// simulating work in center
	if(params->CWT > 0) {
		usleep((rand () % ((params->AWT + 1))) * 1000);
	}
	// process leaving , letting waiting adults leave
	sem_wait(&semph->memory);
	shmem->action++;
	fprintf(params->output, "%d\t: C %d\t: trying to leave\n", shmem->action, child_num);
	shmem->action++;
	shmem->child_count--;
	fprintf(params->output, "%d\t: C %d\t: leave\n", shmem->action, child_num);
	if(shmem->leaving > 0 && shmem->child_count <= ((shmem->adult_count - 1) * 3)) {
		shmem->adult_count--;
		shmem->leaving--;
		sem_post(&semph->adult_Q);
	}
	shmem->barrier_count++;
	sem_post(&semph->memory);
	//process waiting at barrier for all other processes
	if(shmem->barrier_count == (params->A + params->C)) {
		sem_post(&semph->barrier);
	}
	sem_wait(&semph->barrier);
	sem_post(&semph->barrier);
	sem_wait(&semph->memory);
	shmem->action++;
	fprintf(params->output, "%d\t: C %d\t: finished\n", shmem->action, child_num);
	sem_post(&semph->memory);
}


// funcia vytvara procesy child
void first_child(s_params *params, s_shm *shmem, s_sem *semph) {
	pid_t pid;
	for(int i = 0; i < params->C; i++) {
		if(params->CGT > 0) {
		usleep (rand () % (((params->CGT + 1) * 1000)));
		}	
		pid = fork();
		if(pid == 0) {
			child(params, shmem, semph);
			fclose(params->output);
			exit(0);
		}
	}
	while ((pid = wait(NULL)) > 0);	
}
