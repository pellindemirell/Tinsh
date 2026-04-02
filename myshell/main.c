#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    
#include <signal.h>    
#include <errno.h>     
#include "parser.h"
#include "builtins.h"
#include "executor.h"
static void sigint_handler(int sig)
{
    (void)sig;  
    const char msg[] = "\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}
static void print_prompt(void)
{
    char cwd[1024];
    const char *user = getenv("USER");
    if (!user) user = "user";
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\033[1;32m%s@myshell\033[0m:\033[1;34m%s\033[0m$ ", user, cwd);
    } else {
        printf("mysh> ");
    }
    fflush(stdout);
}
int main(void)
{
    char  input[MAX_INPUT];          
    char *tokens[MAX_TOKENS];        
    int   token_count;               
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("mysh: signal");
        return 1;
    }
    while (1) {
        print_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (errno == EINTR) {
                errno = 0;
                continue;
            }
            printf("\nmysh: goodbye!\n");
            break;
        }
        token_count = parse_input(input, tokens);
        if (token_count <= 0)
            continue;
        if (is_builtin(tokens[0])) {
            run_builtin(token_count, tokens);
        } else {
            run_external(token_count, tokens);
        }
    }
    return 0;
}
