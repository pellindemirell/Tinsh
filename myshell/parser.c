#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
char *trim_whitespace(char *str)
{
    if (!str) return NULL;
    while (*str && isspace((unsigned char)*str))
        str++;
    if (*str == '\0')
        return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';  
    return str;
}
int parse_input(char *input, char **tokens)
{
    int count = 0;   
    char *p   = input;
    p = trim_whitespace(p);
    while (*p != '\0' && count < MAX_TOKENS - 1) {
        while (*p && isspace((unsigned char)*p))
            p++;
        if (*p == '\0')
            break;  
        if (*p == '"' || *p == '\'') {
            char quote = *p;    
            p++;                
            tokens[count++] = p;    
            while (*p && *p != quote)
                p++;
            if (*p == '\0') {
                fprintf(stderr, "mysh: syntax error: unclosed quote\n");
                return -1;
            }
            *p = '\0';  
            p++;        
        } else {
            tokens[count++] = p;
            while (*p && !isspace((unsigned char)*p))
                p++;
            if (*p) {
                *p = '\0';  
                p++;        
            }
        }
    }
    tokens[count] = NULL;  
    return count;
}
