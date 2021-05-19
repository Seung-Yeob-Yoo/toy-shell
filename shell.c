#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <dirent.h>

#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	30

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;

    
    while (true) {
        char* s;
        int len;
	char buf[255];

	getcwd(buf,255);
        
	char hostname[LEN_HOSTNAME + 1];
    	memset(hostname, 0x00, sizeof(hostname));

    	gethostname(hostname, LEN_HOSTNAME);

        printf("%s@%s:%s$ ",getpwuid(getuid())->pw_name,hostname,buf);
	
	s = fgets(command, MAX_LEN_LINE, stdin);

	//exit 구현
	if (strcmp(command,"exit\n")==0){
		printf("%c[1;31m",27);
		printf("프로그램 종료!\n");
		printf("%c[0m\n",27);
		break;
	}

	//ls 구현
	if (strcmp(command,"ls\n")==0){
		char* cwd=(char*)malloc(sizeof(char)*255);
		DIR* dir=NULL;
		struct dirent* entry=NULL;

		getcwd(cwd,255);

		if ((dir=opendir(buf))==NULL)
		{
				printf("디렉토리 에러 발생!");
				exit(1);
		}

		while((entry=readdir(buf)) != NULL){
		printf("%s\n",entry->d_name);
		
		}

		free(cwd);
		closedir(dir);
	}


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
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
