#ifndef PARSER_H
#define PARSER_H
#define MAX_TOKENS  128   
#define MAX_INPUT   1024  
int parse_input(char *input, char **tokens);
char *trim_whitespace(char *str);
#endif 
