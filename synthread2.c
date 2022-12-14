#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/*
 *	Declare a mutex, condition variable, and a predicate variable
 *	These three will work together to synchronize occurence of the
 *	required events.
 * */

pthread_mutex_t display_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutexattr_t display_mutex_attr;

pthread_cond_t cond_var_display = PTHREAD_COND_INITIALIZER;

int turn = 1;


void display(char *str) {

	char *tmp;

	for (tmp=str;*tmp;tmp++)
	{
		write(1,tmp,1);
		usleep(1);
	}
}

void* run(void *params)
{
	char* thread_name;
	int i;

	thread_name = (char*) params;

	if( strcmp(thread_name, "thread1") == 0)
	{
		for(i=0; i<10; i++)
		{
			pthread_mutex_lock(&display_mutex);
			while( turn == 0 )
			{
				pthread_cond_wait( &cond_var_display, &display_mutex);
			}
			display("ab");
			turn = 0;
			pthread_cond_signal(&cond_var_display);
			pthread_mutex_unlock(&display_mutex);
		}
	}
	else
	{
		for(i=0; i<10; i++)
		{
			pthread_mutex_lock(&display_mutex);
			while( turn == 1)
			{
				pthread_cond_wait(&cond_var_display, &display_mutex);
			}
			display("cd\n");
			turn = 1;
			pthread_cond_signal(&cond_var_display);
			pthread_mutex_unlock(&display_mutex);
		}

	}
	return NULL;
	//printf("%s", thread_name);
}


int main() {

	char thread1_name[8] = "thread1";
	char thread2_name[8] = "thread2";

	/* return value of pthread_create()*/
	int thread_ret_val;


	/* declare two threads */
	pthread_t thread1_id, thread2_id;
	/* declare two thread attributes */
	pthread_attr_t th1_attr, th2_attr;


	/* initialize the mutex attribute */
	pthread_mutexattr_init( &display_mutex_attr );
	/* initialize the mutex */
	pthread_mutex_init( &display_mutex, &display_mutex_attr);


	/* initalize attributes of thread1 */
	pthread_attr_init(&th1_attr);
	/* initialize attributes of thread2 */
	pthread_attr_init(&th2_attr);


	/* create thread1 */
	thread_ret_val = pthread_create(&thread1_id, &th1_attr, run, (void*)thread1_name);
	if( thread_ret_val != 0 )
	{
		fprintf(stderr, "could not create thread\n");
		return 0;
	}
	/* create thread2 */
	thread_ret_val = pthread_create(&thread2_id, &th1_attr, run, (void*)thread2_name);
	if( thread_ret_val != 0 )
	{
		fprintf(stderr, "could not create thread\n");
		return 0;
	}


	/* main thread will wait for thread1 to finish */
	pthread_join(thread1_id, NULL);
	/* main thread will wait for thread2 to finish */
	pthread_join(thread2_id, NULL);

	/* destroy the display_mutex*/
	pthread_mutex_destroy(&display_mutex);
	return 0;
}
