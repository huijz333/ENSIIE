# include <stdio.h> //printf
# include <string.h> //strcmp
# include <unistd.h> //dup2
# include <stdlib.h> //wait
# include <sys/types.h> //wait
# include <sys/wait.h> //wait
# include <fcntl.h> //open

# define CMD_ERROR (2)
# define CMD_SUCCESS (1)
# define CMD_EXIT (0)
# define CMD_UNKNOWN (-1)

# define STDIN_FD (0)
# define STDOUT_FD (1)
# define STDERR_FD (2)

/* commande struct */
typedef struct	s_cmd {
	const char 	* s;		//command name
	int 		(*f)(void);	//command action
}		t_cmd;

/* exit commande */
static int f_exit(void) {
	return (CMD_EXIT);
}

/** pid command */
static int f_pid(void) {
	printf("shell pid: %d\n", getpid());
	return (CMD_SUCCESS); 
}

/** last status command */
static int last_status = 0;
static int f_last_status(void) {
	printf("last command status: %d\n", last_status);
	return (CMD_SUCCESS);
}

/* global variable, available commandes */
static t_cmd cmds[] = {
	{"exit", f_exit},
	{"$$", f_pid},
	{"$?", f_last_status}
};

/* numner of commands */
static size_t MAX_CMDS = sizeof(cmds) / sizeof(t_cmd);

/* file redirection */
char * nextIn = NULL;
char * nextOut = NULL;

/** generic commands wrapper */
static int shell(char * cmd) {
	if (cmd[0] == '<') {
		nextIn = strdup(cmd + 1);
		return (CMD_SUCCESS);
	} else if (cmd[0] == '>') {
		nextOut = strdup(cmd + 1);
		return (CMD_SUCCESS);
	}

	unsigned int i;
	for (i = 0 ; i < MAX_CMDS ; i++) {
		/** on cherche la commande a executer */
		if (strcmp(cmds[i].s, cmd) == 0) {
			/** on cree un processus pour l executer */
			pid_t pid = fork();
			if (pid == -1) {
				return (CMD_ERROR);
			}
			if (pid == 0) {
				/** on redirige les flux */
				if (nextIn != NULL) {
					int fd = open(nextIn, O_RDONLY);
					if (fd == -1 || dup2(fd, STDIN_FD) == -1) {
						printf("couldn't redirect stdout.\n");
						exit(EXIT_FAILURE);
					}
					free(nextIn);
					nextIn = NULL;
				}

				if (nextOut != NULL) {
					int fd = open(nextOut, O_WRONLY | O_CREAT, 0644);
					if (fd == -1 || dup2(fd, stdout) == -1) {
						printf("couldn't redirect stdout.\n");
						exit(EXIT_FAILURE);
					}
					free(nextOut);
					nextOut = NULL;
				}
				
				/** on execute la commande */
				cmds[i].f();
				exit(EXIT_SUCCESS);
			}
			wait(&last_status);
			return (CMD_SUCCESS);
		}
	}
	return (CMD_UNKNOWN);
}

int main(void) {
	char cmd[1000], line[1000];
	while (!feof(stdin)) {
		printf("shell > ");
		if (fgets(line, 100, stdin) == 0 || sscanf(line, "%s", cmd) != 1) {
			continue ;
		}
		int r = shell(cmd);
		if (r == CMD_UNKNOWN) {
			printf("unknown command: %s\n", cmd);
			printf("%lu available commands:\n", MAX_CMDS);
			size_t i;
			for (i = 0 ; i < MAX_CMDS ; i++) {
				printf("\t%s\n", cmds[i].s);
			}
		} else if (r == CMD_ERROR) {
			printf("an error occured\n");
		}
	}
	printf("\n");
	return (0);
}
