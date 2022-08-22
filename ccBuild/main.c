# include <time.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <ctype.h>
#include <ftw.h>
#include <errno.h>
#include "macros.h"

#define SIZE 32

int stack[SIZE];
int top = -1;

void systemInfo(void);
void localTime(void);
void currentPath(void);
void systemMsg(void);
void get(char *fileName);
int isOperator(const char *operator);
void put(char **tokenContainer, int expressionSize);
void push(int num);
int pop();
void prefix(char *const *tokenContainer, char *exp, int inputSize);
void tokenizeInput(char *input, char *token, char **tokenContainer);

int main()
{
    char *options[] = {"calc", "time", "path", "sys", "put", "get"};
    char input[SIZE] = {};
    char *token = NULL;
    char *tokenContainer[SIZE] = {};
    char expression[SIZE] = {};

    printf("Welcome to the shell.\n");
    systemMsg();

    while(strcmp(input, "quit") != 0)
    {
        printf("> ");
        fgets(input, SIZE, stdin);

        // ensure single inputs works
        input[strcspn(input, "\n")] = 0;

        // tokenize string and fill token array
        tokenizeInput(input, token, tokenContainer);

        int inputSize = 1;
        size_t optLen = sizeof(options)/sizeof(options[0]);

        // find the number of items in token array
        while(tokenContainer[inputSize] != NULL){inputSize++;}

        int n = 0;
        for(int i = 0; i < optLen; i++)
        {
            if(strcmp(tokenContainer[0], options[i]) == 0)
            {
                switch(i)
                {
                    case 0:
                        prefix(tokenContainer, expression, inputSize);
                        break;

                    case 1:
                        if(inputSize >= 2)
                        {
                            printf("ERROR.\n");
                        }
                        else {localTime();}
                        break;

                    case 2:
                        if(inputSize >= 2)
                        {
                            printf("ERROR.\n");
                        }
                        else {currentPath();}
                        break;

                    case 3:
                        if(inputSize >= 2)
                        {
                            printf("ERROR.\n");
                        }
                        else {systemInfo();}
                        break;

                    case 4:
                        put(tokenContainer, inputSize);
                        exit(1);

                    case 5:
                        get(tokenContainer[1]);
                        break;

                    default:
                        exit(-1);
                }
            }
            else{n++;}

            if((n == 6) and strcmp(tokenContainer[0], "quit") != 0)
            {
                printf("ERROR.\n");
                n = 0;
                break;
            }
        }
        //systemMsg();
        bzero(expression, SIZE);
        bzero(tokenContainer, SIZE);
        bzero(stack, SIZE);
    }
    printf("Program quit.");
}

void tokenizeInput(char *input, char *token, char **tokenContainer) {
    token = strtok(input, " ");
    int k = 0;
    while(token != NULL)
    {
        //printf("%s", token);
        tokenContainer[k] = token;
        token = strtok(NULL, " ");
        k++;
    }
}
int deleteSubDirs(const char *path, const struct stat *sb, int t, struct FTW *ftw)
{
    return remove(path);
}
void put(char **tokenContainer, int expressionSize) {
    char path[1024] = {};
    getcwd(path, sizeof(path));

    // append the new file name
    strcat(path, "/");
    strcat(path, tokenContainer[1]);

    if(strcmp(tokenContainer[expressionSize-1], "-f") == 0)
    {
        // has -f
        tokenContainer[expressionSize-1] = NULL;
        nftw(path, deleteSubDirs, 10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS);
        mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);

        // add files
        int j = 2;
        while(tokenContainer[j] != NULL)
        {
            strcat(path, "/");
            strcat(path, tokenContainer[j]);

            fclose(fopen(path, "a"));
            j++;
            bzero(path, sizeof(path));
            getcwd(path, sizeof(path));
            strcat(path, "/");
            strcat(path, tokenContainer[1]);
        }
    }
    else
    {
        // no -f - returns error
        if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == -1){
            printf("Error: %s\n", strerror(errno));
            exit(1);
        }

        // add files
        int j = 2;
        while(tokenContainer[j] != NULL)
        {
            strcat(path, "/");
            strcat(path, tokenContainer[j]);

            fclose(fopen(path, "a"));
            j++;
            bzero(path, sizeof(path));
            getcwd(path, sizeof(path));
            strcat(path, "/");
            strcat(path, tokenContainer[1]);
        }
    }
}
void systemInfo(void)
{
    struct utsname uts;
    uname(&uts);
    printf("System name - %s\n", uts.sysname);
    printf("Version     - %s \n", uts.version);
    printf("Hardware    - %s \n", uts.machine);
}
void localTime(void)
{
    time_t t;
    struct tm *ptr;
    char arr[64];

    time(&t);
    ptr = localtime(&t);
    if(strftime(arr, 64, "Time and Date: %r - %d/%b/%Y", ptr) != 0){
        printf("%s\n", arr);
    }
}
void currentPath(void){
    char p1[1024] = {};
    if(getcwd(p1, sizeof(p1)) != NULL)
        printf("%s\n", p1);
}
void systemMsg(void){
    printf("Input a command:\n");
    printf("- calc <prefix exp>\n- time\n- path\n- sys\n- put <dirname> <filenames> <[-f]>\n- get filename(including path)\n- quit\n");
}
void get(char *fileName)
{
    char io[6];
    FILE *fp = NULL;
    char *line = NULL;
    size_t size = 0;
    int lineNum = 0, k = 0;
    char pathStr[1024] = {};

    getcwd(pathStr, sizeof(pathStr));
    strcat(pathStr, "/");
    strcat(pathStr, fileName);

    fp = fopen(fileName, "r");

    here:
    while(getline(&line, &size, fp) != -1)
    {
        printf("%d:%s", lineNum, line);
        lineNum++, k++;

        if(k == 40)
        {
            printf("<Press any key to continue>\n");
            fgets(io, 6, stdin);
            k = 0;
            goto here;
        }
    }
    printf("<EOF reached>\n\n");
    line = NULL;
    fclose(fp);
    systemMsg();
}
void push(int num)
{
    top++;
    stack[top] = num;
}
int pop()
{
    int num;
    num = stack[top];
    top--;
    return num;
}
int isOperator(const char *operator)
{
    if (*operator == '+'){return 1;}
    if (*operator == '-'){return 2;}
    return -1;
}
void prefix(char *const *tokenContainer, char *exp, int inputSize) {
    int num1 = 0, num2 = 0, operator = 0, r = 0;
    if(inputSize > 2)
    {
        // prefix eval on expression with spaces
        while(inputSize --> 1) {
            if (isnumber(*tokenContainer[inputSize]) == 0) {
                // operator
                operator = isOperator(tokenContainer[inputSize]);
                // pop 2 do eval push to stack
                num1 = pop();
                num2 = pop();

                switch (operator) {
                    case 1:
                        // add
                        r = num2 + num1;
                        printf("%d + %d = %d\n", num2, num1, r);
                        break;
                    case 2:
                        // sub
                        r = num2 - num1;
                        printf("%d - %d = %d\n", num2, num1, r);
                        break;
                    default:
                        exit(1);
                }
                push(r);
            }
            else
            {
                // number
                int num = atoi(tokenContainer[inputSize]);
                push(num);
                num = 0;
            }
        }
        printf("result:%d\n", stack[top]);
    }
    else
    {
        // expression is all 1 string
        strcpy(exp, tokenContainer[1]);
        size_t expLen = strlen(exp);
        while(expLen --> 0)
        {
            if(isnumber(exp[expLen]) == 0)
            {
                // operator
                operator = isOperator(&exp[expLen]);
                // pop 2 do eval push to stack
                num1 = pop();
                num2 = pop();

                switch(operator)
                {
                    case 1:
                        // add
                        r = num2 + num1;
                        printf("%d + %d = %d\n", num2, num1, r);
                        break;
                    case 2:
                        // sub
                        r = num2 - num1;
                        printf("%d - %d = %d\n", num2, num1, r);
                        break;
                    default:
                        exit(1);
                }
                push(r);

            }
            else
            {
                // number
                int num = (exp[expLen] - '0');
                push(num);
                num = 0;
            }
        }
        printf("result:%d\n", stack[top]);
    }
}