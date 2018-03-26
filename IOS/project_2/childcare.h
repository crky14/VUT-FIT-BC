/**
	Subor: childcare.h
	Datum: 2016/2017
	Autor:   Jakub Crkoň, xcrkon00@stud.fit.vutbr.cz
	Projekt: Synchronizacia paralelnych procesov, proj2 do predmetu IOS
	Popis:   Riesenie problemu child-care center
*/

/** @struct s_shm
 *  @brief This structure contains all shared memory variables.
 */
typedef struct 
{
	int adult;			// Count of started adult processes
	int child;			// Count of started child processes
	int action;			// Count of performed actions
	int comming;		// Count of waiting children
	int leaving;		// Count of waiting adults
	int queue;
	int block;
	int child_count;	// Count of children in center
	int adult_count;	// Count of adults in center
	int barrier_count;	// Count of processes at barrier
} s_shm;

/** @struct s_params
 *  @brief This structure contains parsed paramters.
 * 	Function contains FILE pointer for easier passing this pointer to other functions.
 *	Error contains error code if there is something wrong with parameters.
 */
typedef struct { 
	int A;			// number of adult processes
	int C;			// number of child processes
	int AGT;		// max watining time for generating adults
	int CGT;		// max watining time for generating childs
	int AWT;		// max time which will adult spend in center
	int CWT;		// max time which will child spend in center
	int error; 		// error code
	FILE *output;	// file for output
} s_params;
/** @struct s_sem
 *  @brief This structure contains all semaphores.
 */
typedef struct {
	sem_t memory;		// Mutual exclusion for exclusive acees to shared variables and file
	sem_t child_Q;		// Waiting childer queue
	sem_t adult_Q;		// Waiting adult queue
	sem_t barrier;		// Barrier making processes end at the same time
	sem_t a_spawn;		// Barrier make process generate at the same time if AGT == 0
	sem_t c_spawn;		// Barrier make process generate at the same time if CGT == 0
} s_sem; 

/* Error codes */
enum errors {
	OK,			// OK
	ARG_ERR,	// Wrong parameter
	ARGC_ERR,	// Too many or not enough parameters
	FILE_ERR,	// Error while opening a file
	SEM_ERR,	// Error while creating semaphores shared memory 
	SHM_ERR,	// Error while creating shared memory
	INIT_ERR,	// Error while initializing semaphores
	KEY_ERR,	// Error while creating uniq key
};
/**
 * @brief Prints error message to stderr.
 */
void print_error(int error_code);

/**
 * @brief Loads parameters to structure s_params.
 * @param argc number of arguments
 * @param argv arguments	
 * @return struct containing parsed parameters
 */
s_params get_params(int argc, char *argv[]);

/**
 * @brief Initialize semaphores
 * @param semph pointer to shared memory containing semaphores	
 * @return zero on succes, otherwise error code
 */
int init_sems(s_params *params, s_sem *semph);

/**
 * @brief Destroy semaphores before dealocation of memory which contains them
 * @param semph pointer to shared memory containing semaphores	
 */
void destroy_sems(s_params *params, s_sem *semph);

/**
 * @brief Dealoc created shared memory
 * @param shmem_id id of shared memory
 * @param int semph_id id of shared memory containign semaphores
 * @param shmem pointer to shared memory 
 * @param semph pointer to shared memory containing semaphores	
 */
void clear_shm(int shmem_id, int semph_id, s_shm *shmem, s_sem *semph);

/**
 * @brief Function containing actions of adult process
 * @param params structure containing params form command line and FILE pointer
 * @param shmem pointer to shared memory
 * @param semph pointer to shared memory containing semaphores
 */
void adult(s_params *params, s_shm *shmem, s_sem *semph);

/**
 * @brief Function creating adults processes
 * @param params structure containing params form command line and FILE pointer
 * @param shmem pointer to shared memory
 * @param semph pointer to shared memory containing semaphores
 */
void first_adult(s_params *params, s_shm *shmem, s_sem *semph);
/**
 * @brief Function containing actions of child process
 * @param params structure containing params form command line and FILE pointer
 * @param shmem pointer to shared memory
 * @param semph pointer to shared memory containing semaphores
 */
void child(s_params *params, s_shm *shmem, s_sem *semph);

/**
 * @brief Function creating childs processes
 * @param params structure containing params form command line and FILE pointer
 * @param shmem pointer to shared memory
 * @param semph pointer to shared memory containing semaphores
 */
void first_child(s_params *params, s_shm *shmem, s_sem *semph);
