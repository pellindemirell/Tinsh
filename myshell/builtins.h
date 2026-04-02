#ifndef BUILTINS_H
#define BUILTINS_H
int is_builtin(const char *command);
int run_builtin(int argc, char **argv);
int builtin_cd(int argc, char **argv);    
int builtin_pwd(int argc, char **argv);   
int builtin_echo(int argc, char **argv);  
int builtin_exit(int argc, char **argv);  
int builtin_help(int argc, char **argv);  
int builtin_whoami(int argc, char **argv);
#endif
