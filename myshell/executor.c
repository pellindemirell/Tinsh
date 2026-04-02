#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <errno.h>      
#include "executor.h"
int run_external(int argc, char **argv)
{
    (void)argc;  
    pid_t pid;   
    int status;  
    pid = fork();
    if (pid < 0) {
        perror("mysh: fork");
        return -1;
    }
    if (pid == 0) {
        execvp(argv[0], argv);
        if (errno == ENOENT) {
            fprintf(stderr, "mysh: %s: command not found\n", argv[0]);
        } else {
            perror("mysh: execvp");
        }
        exit(127);
    }
    if (waitpid(pid, &status, 0) == -1) {
        perror("mysh: waitpid");
        return -1;
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        fprintf(stderr, "mysh: process killed by signal %d\n",
                WTERMSIG(status));
    }
    return -1;
}
