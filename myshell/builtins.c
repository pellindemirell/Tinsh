#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   
#include <errno.h>    
#include "builtins.h"
static const char *BUILTIN_NAMES[] = {
    "cd", "pwd", "echo", "exit", "help", "whoami", NULL
};
int is_builtin(const char *command)
{
    if (!command) return 0;
    for (int i = 0; BUILTIN_NAMES[i] != NULL; i++) {
        if (strcmp(command, BUILTIN_NAMES[i]) == 0)
            return 1;
    }
    return 0;
}
int run_builtin(int argc, char **argv)
{
    if (!argv || !argv[0]) return -1;
    if (strcmp(argv[0], "cd")   == 0) return builtin_cd(argc, argv);
    if (strcmp(argv[0], "pwd")  == 0) return builtin_pwd(argc, argv);
    if (strcmp(argv[0], "echo") == 0) return builtin_echo(argc, argv);
    if (strcmp(argv[0], "exit") == 0) return builtin_exit(argc, argv);
    if (strcmp(argv[0], "help") == 0) return builtin_help(argc, argv);
    if (strcmp(argv[0], "whoami") == 0) return builtin_whoami(argc, argv);
    fprintf(stderr, "mysh: unknown built-in: %s\n", argv[0]);
    return -1;
}
int builtin_cd(int argc, char **argv)
{
    const char *target;
    if (argc < 2 || argv[1] == NULL) {
        target = getenv("HOME");
        if (!target) {
            fprintf(stderr, "mysh: cd: HOME not set\n");
            return -1;
        }
    } else {
        target = argv[1];
    }
    if (chdir(target) != 0) {
        perror("mysh: cd");
        return -1;
    }
    return 0;
}
int builtin_pwd(int argc, char **argv)
{
    (void)argc; 
    (void)argv;
    char buf[4096]; 
    if (getcwd(buf, sizeof(buf)) == NULL) {
        perror("mysh: pwd");
        return -1;
    }
    printf("%s\n", buf);
    return 0;
}
int builtin_echo(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        if (i > 1) putchar(' ');   
        fputs(argv[i], stdout);
    }
    putchar('\n');
    fflush(stdout);
    return 0;
}
int builtin_exit(int argc, char **argv)
{
    int code = 0;   
    if (argc >= 2 && argv[1] != NULL) {
        code = atoi(argv[1]);
    }
    printf("mysh: goodbye!\n");
    exit(code);
    return 1;  
}
int builtin_help(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("\n");
    printf("  myshell — a minimal Unix shell\n");
    printf("  ================================\n\n");
    printf("  Built-in commands:\n\n");
    printf("    cd [dir]       Change directory (defaults to $HOME)\n");
    printf("    pwd            Print current working directory\n");
    printf("    echo [args]    Print arguments to stdout\n");
    printf("    exit [code]    Exit the shell (optional exit code)\n");
    printf("    help           Show this help message\n");
    printf("    whoami         Print the current user's name\n\n");
    printf("  Any other command is searched for on your PATH and run\n");
    printf("  as an external process via fork + execvp.\n\n");
    printf("  Signals:\n");
    printf("    Ctrl+C (SIGINT)  Cancel current input, stay in shell\n");
    printf("    Ctrl+D (EOF)     Exit the shell gracefully\n\n");
    return 0;
}

int builtin_whoami(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    const char *user = getenv("USER");
    if (user) {
        printf("%s\n", user);
    } else {
        fprintf(stderr, "mysh: whoami: USER environment variable not set\n");
    }
    return 0;
}
