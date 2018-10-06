#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

// This program has to be run with root privileges [See the sample run below] otherwise u will get
// operation not permitted error or some other error of that sort

/*The program below demonstrates the use of  pthread_setschedparam()  and  pthread_getschedparam(), 
	as well as the use of a number of other scheduling-related pthreads functions. */

#define error_exit_en(en, msg) \
	do { errno=en; perror(msg); exit(EXIT_FAILURE); } while(0)

/* The main thread sets its scheduling policy to SCHED_FIFO with a priority of 10, and initializes a 
   thread attributes object with a scheduling policy attribute of SCHED_RR and a scheduling  priority  
   attribute of 20. The program then sets (using pthread_attr_setinheritsched()) the inherit scheduler attribute 
   of the thread attributes object to PTHREAD_EXPLICIT_SCHED, meaning that threads created using this 
   attributes object should take their scheduling attributes from the thread attributes object. The program then 
   creates a thread using the thread attributes object, and that thread displays its scheduling policy and priority.
*/

static void usuage(char *prog_name, char *msg){
	if(msg!=NULL)
		fputs(msg, stderr);
	fprintf(stderr, "Usuage: %s [options]\n", prog_name);
	fprintf(stderr, "Options are: \n");

	#define fpe(str) fprintf(stderr, "\t%s", str)  // makes the code shorter
		// fpe - fprintf to error stream
	fpe("-a<policy><prio> Set scheduling policy and priority in\n");
    fpe("                 thread attributes object\n");
    fpe("                 <policy> can be\n");
    fpe("                     f  SCHED_FIFO\n");
    fpe("                     r  SCHED_RR\n");
    fpe("                     o  SCHED_OTHER\n");
    fpe("-A               Use default thread attributes object\n");
    fpe("-i {e|s}         Set inherit scheduler attribute to\n");
    fpe("                 'explicit' or 'inherit'\n");
    fpe("-m<policy><prio> Set scheduling policy and priority on\n");
    fpe("                 main thread before pthread_create() call\n");
    exit(EXIT_FAILURE);

}

static int get_policy(char p, int *policy){
	switch(p){
		case 'f': *policy = SCHED_FIFO; return 1;
		case 'r': *policy = SCHED_RR; return 1;
		case 'o': *policy = SCHED_OTHER; return 1;
		default: return 0;   // in this programme we are not providing option for SCHED_BATCH
	}									// and SCHED_IDLE but they do exist
}

static void display_sched_attr(int policy, struct sched_param *param){
	// make the way the printf statment is used below
	printf(" policy = %s, priority = %d\n", 
		 (policy==SCHED_FIFO) ? "SCHED_FIFO" :
		 (policy==SCHED_OTHER) ? "SCHED_OTHER":
		 (policy==SCHED_RR) ? "SCHED_RR":
		 "???", param->sched_priority);
}
static void display_thread_sched_attr(char *msg){  // See how these messages are effectively used to increase
															// run time understanding/claraity
	int policy, s;
	struct sched_param param;

	s = pthread_getschedparam(pthread_self(), &policy, &param);
	if(s!=0)
		error_exit_en(s, "pthread_getschedparam");
	
	printf("%s\n", msg);
	display_sched_attr(policy, &param);
													
	
}

static void* thread_start(void *arg){
	display_thread_sched_attr("Schedular attributes of new thread");

	return NULL;
}

int main(int argc, char *argv[])
{
	int s, opt, inheritType, do_default_attr, policy;

	pthread_t t;
	pthread_attr_t attr;
	pthread_attr_t *attrp;

	char *sched_attr_str, *sched_main_str, *sched_inherit_str;  // read it as scheduler <*> string

	struct sched_param param;

	do_default_attr = 0;
	sched_attr_str = NULL;   // setting up the default versions
	sched_main_str = NULL;
	sched_inherit_str = NULL;

	while((opt=getopt(argc, argv, "a:Ai:m:"))!=-1){
		switch(opt){  // although opt is int, it can very well be used to switch for characters
							// the reason why we use int instead of char is that getopt returns an int
			case 'a': sched_attr_str = optarg; break;
			case 'A': do_default_attr = 1; break;
			case 'i': sched_inherit_str = optarg; break;
			case 'm': sched_main_str = optarg;  break;
			default: usuage(argv[0], "Unrecognized option\n");  // argv[0] is the programe name
		}
	}

	if(do_default_attr && 
		 (sched_inherit_str !=NULL || sched_attr_str!=NULL) )
		usuage(argv[0], "Can't specify -A with -i or -a\n");

	/*Setting the policy and priority for the main thread*/

	if(sched_main_str!=NULL){
		if(!get_policy(sched_main_str[0], &policy))
			usuage(argv[0], "Bad policy for main thread (-s)\n");  // calling usuage exits the program
		param.sched_priority = strtol(&sched_main_str[1], NULL, 0);  // study the usuage to understand the semantics
		
		s = pthread_setschedparam(pthread_self(), policy, &param);
		if(s!=0)
			error_exit_en(s, "pthread_setschedparam");
	}

	display_thread_sched_attr("Scheduler settings of main thread");
	puts("");

	/* Initializing thread attributes object according to options */

	attrp = NULL;

	if(!do_default_attr){
		s = pthread_attr_init(&attr);
		if(s!=0)
			error_exit_en(s, "pthread_attr_init");
		attrp = &attr;
	}

	if(sched_inherit_str!=NULL){   // the inheritance string from the options
		if(sched_inherit_str[0]=='e')
			inheritType = PTHREAD_EXPLICIT_SCHED;
		else if(sched_inherit_str[0]=='i')
			inheritType = PTHREAD_INHERIT_SCHED;
		else
			usuage(argv[0], "Value for -i must be 'e' or 'i'\n");

		s = pthread_attr_setinheritsched(&attr, inheritType);
		if(s!=0)
			error_exit_en(s, "pthread_attr_setinheritsched");
	}

	if(sched_attr_str!=NULL){  // the attribute string from the options
		if(!get_policy(sched_attr_str[0], &policy))
			usuage(argv[0], "Bad policy for 'attr' (-a)\n");
		param.sched_priority = strtol(&sched_attr_str[1], NULL, 0);

		s = pthread_attr_setschedpolicy(&attr, policy);
		if(s!=0)
			error_exit_en(s, "pthhread_attr_setschedparam");

		s = pthread_attr_setschedparam(&attr, &param);
		if(s!=0)
			error_exit_en(s, "pthhread_attr_setschedparam");
	}

	/* If we initialized a thread attributes object, display
        the scheduling attributes that were set in the object */

	if(attrp!=NULL){  //may be the only use of attrp in this program (but still a clever one to check whether
								// attr has been modified)
		s = pthread_attr_getschedparam(&attr, &param);
		if(s!=0)
			error_exit_en(s, "pthread_getschedparam");
		s = pthread_attr_getschedpolicy(&attr, &policy);
		if(s!=0)
			error_exit_en(s, "pthread_attr_getschedpolicy");

		puts("Scheduler settings in 'attr'");
		display_sched_attr(policy, &param);

		s = pthread_attr_getinheritsched(&attr, &inheritType);
		if(s!=0)
			error_exit_en(s, "pthread_attr_getinheritsched");
		printf("Inheritance Type is %s\n",
			    (inheritType==PTHREAD_INHERIT_SCHED) ? "PTHREAD_INHERIT_SCHED" :
			    (inheritType==PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
			    "???");
		puts("");
	}

	/* Creating the thread n doing other stuff finally */

	s = pthread_create(&t, attrp, thread_start, NULL);
	if(s!=0)
		error_exit_en(s, "pthread_create");

	s = pthread_attr_destroy(&attr);
	if(s!=0)
		error_exit_en(s, "pthread_attr_destroy");

	s = pthread_join(t, NULL);
	if(s!=0)
		error_exit_en(s, "pthread_join");

	exit(EXIT_SUCCESS);  // the threads gets terminated on their own

	return 0;
}


/* SAMPLE run:
           $ su      # Need privilege to set real-time scheduling policies
           Password:
           # ./a.out -mf10 -ar20 -i e
           Scheduler settings of main thread
               policy=SCHED_FIFO, priority=10

           Scheduler settings in 'attr'
               policy=SCHED_RR, priority=20
               Inheritance Type is EXPLICIT

           Scheduler attributes of new thread
               policy=SCHED_RR, priority=20

       The  next  run is the same as the previous, except that the inherit scheduler attribute is
       set to PTHREAD_INHERIT_SCHED, meaning that threads created  using  the  thread  attributes
       object  should  ignore  the  scheduling  attributes specified in the attributes object and
       instead take their scheduling attributes from the creating thread.

           # ./a.out -mf10 -ar20 -i i
           Scheduler settings of main thread
               policy=SCHED_FIFO, priority=10

           Scheduler settings in 'attr'
               policy=SCHED_RR, priority=20
               Inheritance Type is INHERIT

           Scheduler attributes of new thread
               policy=SCHED_FIFO, priority=10
*/