#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	30
#define MAX_DIR_LINE    100

int main(void)
{
    char command[MAX_LEN_LINE];
    char *path;
    char *road;
    char *args[] = {command, path, road, NULL};

    int ret, status;

    pid_t pid, cpid;
    char hostname[LEN_HOSTNAME + 1];


    memset(hostname, 0x00, sizeof(hostname));
    printf("username: %s\n", getpwuid(getuid())->pw_name);

    gethostname(hostname, LEN_HOSTNAME);
    printf("hostname: %s\n", hostname);


    while (true) {
        char *s;
        int len;
    	char cwd[MAX_DIR_LINE];
    	getcwd(cwd,sizeof(cwd));
        printf("\033[34mpresentLee $ \033[32m%s/\033[32m%s $ :\033[0m ", hostname, cwd);
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);

	if (!strcmp(command, "exit")){
		printf("It's over!!\n");
		break;
	}

	path = strtok(command," ");
	if(!strcmp(args[0], "cd"))
	{
		path = strtok(NULL, " ");
	    	if(chdir(path)<0){
			printf("No Directory [%s].\n", path);
		}
		else{
			printf("Correct!\n");
		}
	}
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
	
        }

        else {  /* child */
	    if(!strcmp(command, "ls")){
		path = strtok(NULL, " ");
		args[0] = "/bin/ls";
		args[1] = path;
            } 

	    if(!strcmp(command, "mkdir")){
		path = strtok(NULL, " ");
		args[0] ="/bin/mkdir";
		args[1] = path;
		road = strtok(NULL, " ");
		args[2] = road;

	    }

	    if(!strcmp(command, "rmdir")){
		path = strtok(NULL, " ");
		args[0] ="/bin/rmdir";
		args[1] = path;
		road = strtok(NULL, " ");
		args[2] = road;
	    }

            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }

        }
    }
    return 0;

}

