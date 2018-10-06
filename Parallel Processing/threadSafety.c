#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// All reenterant functions are thread-safe. The vice-versa is not true. 
// Any function that does not use static data or other shared resource is trivially thread safe.

// Some suggestions for writing a reenterant function:
		// i) Avoid "static" variables since they are allocated per-process and not per-thread.
		// ii) Avoid use of global variables as well. Global data should be maintaind per thread or encapsulated.
		// iii) Do not make a function call to a non-reenterant function.
		// iv) Do not return static fields. Can return dynamically allocated storage or can use caller-specified storage. The former
		// requires the user to handle freeing up the storage and thus the latter is recommended.
		
// v) Make use of synchronization primitives such as locking to make a function thread-safe. This doesn't guarantee re-enterancy but.

// Examples of thread unsafe functions:
	// i) i/o functions without locking such as getchar_unlocked. Manual locking has to be performed to use them.
	// ii) rand - maintains seed value state information. Use rand_r instead or random_r.
	// iii) strtok and any other function that returns a static field.


// _r suffix is used to differentiate reenterant functions. You can use the same scheme to name your functions.
// When working in a multi-threaded environment, use the reenterant version of the functions wherever possible and necessary.


// ########## Examples #########


// non-reenterant version of strtoupper

char* strtoupper(char* str) {
	static char result[MAX_SIZE];

	int i;
	for (i = 0; str[i]; ++i)
		result[i] = toupper(str[i]);
	result[i] = '\0';

	return result;
}

// reenterant version of strtoupper using dynamically allocated storage

char* strtoupper(char* str) {
	char* result = (char*)malloc(MAX_SIZE);
	if(result != NULL) {
		int i;
		for (i = 0; str[i]; ++i)
			result[i] = toupper(str[i]);
		result[i] = '\0';
		return result;		// caller will be responsible for freeing
	}
	return NULL;
}

// reenterant version of strtoupper using caller specified storage

char* strtoupper(char* oldStr, char* newStr) {
	int i;
	for (i = 0; oldStr[i]; ++i)
		newStr[i] = toupper(oldStr[i]);
	newStr[i] = '\0';

	return newStr;
}


// Now say you have a function that deals with string buffers and maintains a static variable to hold the index. To make a reenterant
// version of it, either you can extend the function prototype to include a int *index parameter (a user provided integer pointer) or you
// can return the index so that in the next call the user can pass the string at an offset (like str + ind).



// Non-reenterant increement counter

int increement_ctr() {
	static int count = 0;
	++count;
	return count;
}

// To make the above code thread-safe, the static counter must be protected by a "static lock".

int increement_ctr() {
	static int count = 0;
	static pthread_mutex_t counterLock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(counterLock);
	++count;
	pthread_mutex_unlock(counterLock);
	return count;
}

// The same workaround can be used for external library functions that may be non-reenterant.

					// lock(library_lock);    // use a global lock for the library
					// library_call();
					// unlock(library_lock);


// As noted above we made the mutex static and the lock global since that's the only way to have them shared among all the threads.
// Making a local lock serves no purpose since each thread will have it's own lock then.