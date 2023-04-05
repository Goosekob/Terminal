#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 10  

void onCtrlC(int sig) 
{
    printf("Список запущенных процессов:\n");
    system("ps -e");
    printf("Выберите PID процесса, который хотите остановить: ");
    pid_t pid;
    scanf("%d", &pid);
    kill(pid, SIGTERM);
    exit(1);
}

int main() 
{
    char input[100];
    char *arguments[MAX + 2];
    int status, count;
    pid_t pid;

    signal(SIGINT, onCtrlC);

    while (1) 
    {
        printf("\n%s > ", getlogin());
        fgets(input, sizeof(input), stdin);
        count = 0;
        arguments[count++] = strtok(input, " \n");
        while ((arguments[count++] = strtok(NULL, " \n")) != NULL) 
        {
            if (count > MAX + 1) 
            {
                printf("Слишком много аргументов.\n");
                count = 0;
                break;
            }
        }
        
        if (count == 1) 
        {
            continue;
        }
        arguments[count] = NULL;

        if (strcmp(arguments[0], "ls") == 0) 
        {
            if ((pid = fork()) == 0) 
            {
                execvp(arguments[0], arguments);
                exit(1);
            }
            wait(&status);
        } else 
        if (strcmp(arguments[0], "cat") == 0) 
        {
            if (arguments[1] == NULL)
            {
                printf("Не указан файл.\n");
                exit(1);
            }

            if ((pid = fork()) == 0) 
            {
                execvp(arguments[0], arguments);
                exit(1);
            }
            wait(&status);
        } else 

        if (strcmp(arguments[0], "exit") == 0)
        {
            return 0;
        } else
        {
            printf("Неизвестная команда");
        }
    }
    return 0;
}