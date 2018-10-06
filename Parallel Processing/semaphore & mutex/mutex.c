#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// link with -pthread

int balance = 100;

pthread_mutex_t bal = PTHREAD_MUTEX_INITIALIZER;  // macro used to initialize "static" mutexes 
pthread_t t1,t2;							// pthread_mutex_init() cud have been used as well
	
void deposit(int val){
	pthread_mutex_lock(&bal);  // locking the shared resources so no other thread can access it before it
										// is unlocked
	int cur_balance = balance;   // reading balance since its value could have been changed by some other thread
	cur_balance += val;
	balance = cur_balance;   // writing balance
	printf("\nBALANCE: %d\n", balance);

	pthread_mutex_unlock(&bal);  // releasing the lock so that other threads can lock it
}

void withdraw(int val){
	pthread_mutex_lock(&bal);
	
	int cur_balance = balance;  // reading balance
	if(cur_balance>=val){
		cur_balance -= val;
		balance = cur_balance;
		printf("\nBALANCE: %d\n", balance);
	}

	else
		puts("Insufficient Balance");

	pthread_mutex_unlock(&bal);
}

void* thread_init(void *tid){

	if((long)tid==1){
		printf("Enter the amount to deposit: ");
		int val;
		scanf("%d",&val);
		deposit(val);
	}

	else{
		void* res;
		pthread_join(t1, &res);  // will wait for t1 to terminate before starting the second thread
		free(res);  // note this step
		printf("Enter the amount to withdraw: ");
		int val;
		scanf("%d",&val);
		withdraw(val);
	}

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	pthread_create(&t1, NULL, thread_init, (void*)1);
	pthread_create(&t2, NULL, thread_init, (void*)2);
	
	pthread_exit(NULL);
	pthread_mutex_destroy(&bal);  // remember to destory what you create

	return 0;
}