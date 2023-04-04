#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 10  // максимальное количество аргументов

void onCtrlC(int sig) 
{
    printf("Список запущенных процессов:\n");
    system("ps -e");
    // запрашиваем у пользователя идентификатор процесса
    printf("Выберите PID процесса, который хотите остановить: ");
    pid_t pid;
    scanf("%d", &pid);
    // останавливаем процесс с помощью команды kill
    kill(pid, SIGTERM);
    // завершаем работу программы
    exit(0);
}

int main() 
{
    char input[100];           // буфер для ввода команды
    char *args[MAX_ARGS + 2];  // массив аргументов команды
    int status, count;
    pid_t pid;

    signal(SIGINT, onCtrlC);  // обработка сигнала SIGINT

    while (1) 
    {
        printf("\n> ");                      // приглашение командной строки
        fgets(input, sizeof(input), stdin);  // получаем ввод пользователя
        count = 0;
        args[count++] = strtok(input, " \n");  // первый аргумент - имя команды
        while ((args[count++] = strtok(NULL, " \n")) != NULL) 
        {
            if (count > MAX_ARGS + 1) 
            {
                printf("Слишком много аргументов.\n");
                count = 0;
                break;
            }
        }
        if (count == 1) 
        {  // отсутствие аргументов
            continue;
        }
        args[count] = NULL;  // последний аргумент - NULL

        if (strcmp(args[0], "ls") == 0) 
        {  // команда "ls"
            if ((pid = fork()) == 0) 
            {
                execvp(args[0], args);
                exit(1);
            }
            wait(&status);
        } else if (strcmp(args[0], "cat") == 0) 
        {  // команда "cat"
            if ((pid = fork()) == 0) 
            {
                execvp(args[0], args);
                exit(1);
            }
            wait(&status);
        } else 
        {  // неизвестная команда
            printf("Неизвестная команда:");
        }
    }
    return 0;
}