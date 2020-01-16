/* 
 * bsh - the Bowdoin Shell
 * 
 * Built by: Kim Hancock, Sam Harder
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */
#define MAXBUF     1024   /* max buffer size for safe_printf */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char** environ;      /* defined in libc */
char prompt[] = "bsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

typedef struct job_t {      /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
} job_t;
job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char* cmdline);


/*
 * Implements the bg and fg built-in commands
 */
void do_bgfg(char** argv);

/*
 * Waits for a foreground to complete (similar to waitpid ?)
 */
void waitfg(pid_t pid);

/*
 * Handler for SIGINT (ctrl-c) signals. This sends a SIGINT to the shell, which will then pass it along to our foreground processes
 * (if one exists). After calling fork but before child calls execve, child should call setpgif(0, 0) which puts child in a new process
 * group whose group ID is equal to the child's PID
 */
void sigint_handler(int sig);

/*
 * Handler for SIGSTP (ctrl-z) signals
 */
void sigtstp_handler(int sig);

/*
 * Handler for SIGCHILD signals
 */
void sigchld_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char* cmdline, char** argv); 
void sigquit_handler(int sig);

void clearjob(job_t* job);
void initjobs(job_t* jobs);
int maxjid(job_t* jobs); 
int addjob(job_t* jobs, pid_t pid, int state, char* cmdline);
int deletejob(job_t* jobs, pid_t pid); 
pid_t fgpid(job_t* jobs);
job_t* getjobpid(job_t* jobs, pid_t pid);
job_t* getjobjid(job_t* jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(job_t* jobs);

int builtin_cmd(char** argv);
void usage(void);
void unix_error(char* msg);
void app_error(char* msg);
void safe_printf(const char* format, ...);
typedef void handler_t(int);
handler_t* Signal(int signum, handler_t* handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char** argv) {
  char c;
  char cmdline[MAXLINE];
  int emit_prompt = 1; /* emit prompt (default) */

  /* Redirect stderr to stdout (so that driver will get all output
   * on the pipe connected to stdout) */
  dup2(1, 2);

  /* Parse the command line */
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
      case 'h':             /* print help message */
        usage();
        break;
      case 'v':             /* emit additional diagnostic info */
        verbose = 1;
        break;
      case 'p':             /* don't print a prompt */
        emit_prompt = 0;  /* handy for automatic testing */
        break;
      default:
        usage();
        break;
    }
  }

  /* Install the signal handlers */

  /* These are the ones you will need to implement */
  Signal(SIGINT,  sigint_handler);   /* ctrl-c */
  Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
  Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

  /* This one provides a clean way to kill the shell */
  Signal(SIGQUIT, sigquit_handler); 

  /* Initialize the job list */
  initjobs(jobs);

  /* Execute the shell's read/eval loop */
  while (1) {

    /* Read command line */
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      app_error("fgets error");
    }
    if (feof(stdin)) { /* End of file (ctrl-d) */
      fflush(stdout);
      exit(0);
    }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char* cmdline) {
	/* parse command line into its arguments */  
  char** argv = (char**) malloc(sizeof(char*)*MAXLINE);
  int bg = parseline(cmdline, argv);

	/* Create mask */ 
	sigset_t mask; // signal bit vector
	sigemptyset(&mask); // clear all bits 
	sigaddset(&mask, SIGCHLD); // set SIGCHLD bit
	
	/* We catch errors when no commandline arguments are given */ 
	if(argv[0] == NULL){
		return;
	}

  int is_builtin = builtin_cmd(argv);

	/* If it was not a builtin command, then we have to fork and exec */ 
	if(!is_builtin){

		sigprocmask(SIG_BLOCK, &mask, NULL); //block SIGCHLD

	  pid_t pid = fork();
	  
		/* Catch errors in fork() */ 
		if(pid < 0){
			unix_error("Forking failed");
		}
		/* Child process */ 
		else if(pid == 0){
			/* Since pid == 0, this sets pgid to the pid of the calling process */
			setpgid(0,0);

			/* We must unblock signals before executing the new program */ 
			
			if(sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0){
				unix_error("Unblocking of signal mask failed");				
			}

			/* Now we execute the desired program */ 
			if(execve(argv[0], argv, environ) < 0){
				printf("%s: Command not found\n", argv[0]);
				return;
			}
		}	
		/* Parent (shell) process */ 
		else{
			/* If foreground, wait for it */ 
			if(!bg){
				addjob(jobs, pid, FG, cmdline);
				
				/* Unblock before calling waitfg
				 * We don't want to have signals be blocked for waitfg, because 
				 * otherwise we will never clear the foreground command from our 
				 * job list, and never leave waitfg */ 
				if(sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0){
					unix_error("Unblocking of signal mask failed");				
				}

				waitfg(pid);
				//printf("We were finished waiting!\n");
			}
			/* If background, add it to our joblist. We have to 
			 * take care, since if we are not careful, we might have tried to 
			 * delete the job before we add it, therefore we block any 
			 * signals that could cause this issue.  */ 
			else{
				/* Block the SIGCHLD handler */ 
				addjob(jobs, pid, BG, cmdline);
			
				/* verbose print out */ 	
				job_t* bgjob = getjobpid(jobs, pid);
				printf("[%d] (%d) %s", bgjob->jid, bgjob->pid, cmdline);
				
			}
			// Unblock signal mask 
			if(sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0){
				unix_error("Unblocking of signal mask failed");			
			}
		}
	}

	free(argv);
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char* cmdline, char** argv) {
  static char array[MAXLINE]; /* holds local copy of command line */
  char* buf = array;          /* ptr that traverses command line */
  char* delim;                /* points to first space delimiter */
  int argc;                   /* number of args */
  int bg;                     /* background job? */

  strcpy(buf, cmdline);
  buf[strlen(buf) - 1] = ' ';  /* replace trailing '\n' with space */
  while (*buf && (*buf == ' ')) { /* ignore leading spaces */
    buf++;
  }

  /* Build the argv list */
  argc = 0;
  if (*buf == '\'') {
    buf++;
    delim = strchr(buf, '\'');
  } else {
    delim = strchr(buf, ' ');
  }
  while (delim) {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) { /* ignore spaces */
      buf++;
    }

    if (*buf == '\'') {
      buf++;
      delim = strchr(buf, '\'');
    } else {
      delim = strchr(buf, ' ');
    }
  }
  argv[argc] = NULL;

  if (argc == 0) {  /* ignore blank line */
    return 1;
  }

  /* should the job run in the background? */
  if ((bg = (*argv[argc-1] == '&')) != 0) {
    argv[--argc] = NULL;
  }
  return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately. We return 1 if it was a built-in command, and 0
 *    if it was not.  
 */
int builtin_cmd(char** argv) {
  /* The builtin command would be the first argument */ 
	char *command = argv[0];
  if (strcmp(command, "quit")== 0){
		exit(1);
	}
	else if (strcmp(command, "jobs") == 0){
		listjobs(jobs);
	}
	else if (strcmp(command, "bg") == 0){
		do_bgfg(argv);
	}
	else if (strcmp(command, "fg") == 0){
		do_bgfg(argv);
	}
	/* If not a built in program, we must fork and exec it */ 
	else{
		return 0;
	}
	return 1;
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char** argv) {
	/* We first parse the state that we are hoping
	 * to change */ 
	int state;
	char *jobid = argv[1];
	if (jobid == NULL) {
		printf("%s command requires PID or %%jobid argument\n", argv[0]);
		return;
	}
	if (strcmp(argv[0], "bg") == 0) {
		state = BG;
	}
	else {
		state = FG;
	}

	/* jids are prefixed by a % and pids are just
	 * single integers. */	
	int is_jid = (*argv[1] == '%');

	/* First, we look for the defined job */ 
	job_t* job; 
  if(is_jid){
	  	/* We grab the JID and then get the corresponding job from the jobs list */
		int jid = atoi(&argv[1][1]);
		job = getjobjid(jobs, jid);
		if (job == NULL) {
			/* Check for invalid job ID as input   */
			printf("%s: No such job\n", argv[1]);
			return;
		}
	}
	else{
		pid_t pid = (pid_t) atoi(argv[1]);
		job = getjobpid(jobs, pid);
		if (job == NULL && atoi(argv[1])) {
			/* Check for invalid job ID as input */
			printf("(%s): No such process\n", argv[1]);
			return;
		}		
	}

	/* If the job was not found, return an error */ 
	if(job == NULL){
		printf("%s: argument must be a PID or %%jobid \n", argv[0]);
		return;
	}

	// Update the state of our job. 
	job->state = state;

	/* Resume the specified job, move it to foreground if specified, 
	 * and then wait for all child processes to finish if it is in 
	 * the foreground. We will make sure that we run everything
	 * in the job by sending the signal to the entire process group. 
	 * */
	if(state == BG){
		printf("[%d] (%d) %s",job->jid, job->pid, job->cmdline);
	}

	if(kill(-1*(job->jid), SIGCONT) < 0){
		unix_error("Failed to continue stopped process");
	}

	/* Now, if our job is in the foreground, we want to wait 
	 * until it finishes */ 
	if(state == FG){
		waitfg(job->pid);
	}
	
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
	/* While foreground command is running we wait. Once it is finished, 
	 * the sigchid handler will reap it and delete the job from the
	 * job list. At that point we will execute the loop and return. */ 
	sigset_t mask;
	sigemptyset(&mask);

	while(fgpid(jobs) != 0 ){
		// we suspend so we don't loop tirelessly
		sigsuspend(&mask);
	}
}
/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) {
	//safe_printf("We received a SIGCHILD handler.\n");
	int pid;
	int status;

	// We delete all zombie processes as well as handle processes that have 
	// stopped or terminated themselves.  
  while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0){
		// If process stopped itself, then we should switch its status
		if(WIFSTOPPED(status)){
			job_t* job = getjobpid(jobs,pid);
			int signal = WSTOPSIG(status);
			job->state = ST;
			safe_printf("Job [%d] (%d) stopped by signal %d\n",job->jid, job->pid, signal);
		}
		// If proccess terminated we should remove it from job list 
		else if(WIFSIGNALED(status)){
			job_t* job = getjobpid(jobs, pid);
			int signal = WTERMSIG(status);
			safe_printf("Job [%d] (%d) terminated by signal %d\n",job->jid, job->pid, signal);
			deletejob(jobs,pid);
		}
		// Otherwise just reap the normally terminated job
		else{
			deletejob(jobs, pid); // delete job from job list
		}
	}
	// Check waitpid for errors
	if(pid < -1){
		unix_error("Waitpid failed.");
	}
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenever the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) {
	int pid;
	// For each process in the foreground group, terminate. 
	if((pid = fgpid(jobs)) != 0){
		// get job object for foreground
		//job_t* fgjob = getjobpid(jobs, pid);
		// kill everything in process group
		if(kill(-pid, SIGINT) < 0){
			unix_error("Failed to terminate process");				
		}
		//safe_printf("Job [%d] (%d) terminated by signal %d\n", fgjob->jid, fgjob->pid, sig);
	}
  return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) {
	int pid;
/* We first check to see if there is a foreground job */ 
	if((pid = fgpid(jobs)) != 0){
		/* If there is, we change its status to stop, STOP it, then print a 
		 * message */ 
		//job_t* fgjob = getjobpid(jobs, pid);
		//fgjob->state = ST;
		if(kill(-pid, SIGTSTP) < 0){
			unix_error("Failed to stop process");
		}
		//safe_printf("Job [%d] (%d) stopped by signal %d\n",fgjob->jid, fgjob->pid, sig); 
	}
  return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(job_t* job) {
  job->pid = 0;
  job->jid = 0;
  job->state = UNDEF;
  job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(job_t* jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++) {
    clearjob(&jobs[i]);
  }
}

/* maxjid - Returns largest allocated job ID */
int maxjid(job_t* jobs) {
  int i, max = 0;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].jid > max) {
      max = jobs[i].jid;
    }
  }
  return max;
}

/* addjob - Add a job to the job list */
int addjob(job_t* jobs, pid_t pid, int state, char* cmdline) {
  int i;

  if (pid < 1) {
    return 0;
  }

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == 0) {
      jobs[i].pid = pid;
      jobs[i].state = state;
      jobs[i].jid = nextjid++;
      if (nextjid > MAXJOBS) {
        nextjid = 1;
      }
      strcpy(jobs[i].cmdline, cmdline);
      if(verbose){
        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
      }
      return 1;
    }
  }
  printf("Tried to create too many jobs\n");
  return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(job_t* jobs, pid_t pid) {
  int i;

  if (pid < 1) {
    return 0;
  }

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
      clearjob(&jobs[i]);
      nextjid = maxjid(jobs)+1;
      return 1;
    }
  }
  return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(job_t* jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].state == FG) {
      return jobs[i].pid;
    }
  }
  return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
job_t* getjobpid(job_t* jobs, pid_t pid) {
  int i;

  if (pid < 1) {
    return NULL;
  }
  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
      return &jobs[i];
    }
  }
  return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
job_t* getjobjid(job_t* jobs, int jid) {
  int i;

  if (jid < 1) {
    return NULL;
  }
  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].jid == jid) {
      return &jobs[i];
    }
  }
  return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) {
  if (pid < 1) {
    return 0;
  }
  for (int i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
      return jobs[i].jid;
    }
  }
  return 0;
}

/* listjobs - Print the job list */
void listjobs(job_t* jobs) {
  for (int i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid != 0) {
      printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
      switch (jobs[i].state) {
        case BG: 
          printf("Running ");
          break;
        case FG: 
          printf("Foreground ");
          break;
        case ST: 
          printf("Stopped ");
          break;
        default:
          printf("listjobs: Internal error: job[%d].state=%d ", 
              i, jobs[i].state);
          break;
	}
      
      printf("%s", jobs[i].cmdline);
    }
  }
}

/***********************
 *  Other helper routines
 * ********************/

/* 
 * usage - print a help message
 */
void usage(void){
	printf("Usage: shell [-hvp]\n");
	printf("    -h print this message\n");
	printf("    -v print additional diagnostic information \n");
	printf("    -p do not emit a command prompt\n");
	exit(1);
}

/*
 * unix erorr (we accidentally deleted this )
 */
void unix_error(char* msg){
	fprintf(stdout, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

/*
 * app_error - application style error routine
 */
void app_error(char* msg){
	fprintf(stdout, "%s\n", msg);
	exit(1);
}

/*
 *    Use this rather than printf in signal handlers.
 *    Use this rather than printf in signal handlers.
 */
void safe_printf(const char* format, ...) {
  char buf[MAXBUF];
  va_list args;

  va_start(args, format);
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  write(1, buf, strlen(buf)); /* write is async-signal-safe */
}

/*
 * Signal - wrapper for the sigaction function.
 *    Associates the given signal to the given signal handler function.
 */
handler_t* Signal(int signum, handler_t* handler) {
  struct sigaction action, old_action;

  action.sa_handler = handler;  
  sigemptyset(&action.sa_mask); /* block sigs of type being handled */
  action.sa_flags = SA_RESTART; /* restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0) {
    unix_error("Signal error");
  }
  return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) {
  printf("Terminating after receipt of SIGQUIT signal\n");
  exit(1);
}


