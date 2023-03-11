#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int semid;

void
hndlr(int s)
{
    semctl(semid, 0, IPC_RMID, (int) 0); // удаление массива семафоров
    exit(0);
}

void
son_proc(char *str, int type, int N)
{
    signal(SIGINT, SIG_DFL);
    // подключение ресурса
    key_t key = ftok(str, 'a');
    int semid_son = semget(key, 1, IPC_CREAT | 0666); // создаем массив из 1 семафоров
    int cur_num = type;
    struct sembuf semb;
    semb.sem_num = 0;
    semb.sem_flg = 0;

    while (cur_num < N) {
        if (type == 1) {
            semb.sem_op = 0;
        } else {
            semb.sem_op = -1;
        }

        semop(semid_son, &semb, 1); // изменение значения семафора
        printf("%d\n", cur_num);
        fflush(stdout);
        cur_num += 2;

        if (type == 1) {
            semb.sem_op = 2;
        } else {
            semb.sem_op = -1;
        }

        semop(semid_son, &semb, 1); // изменение значения семафора
    }
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, hndlr);
    int number;
    // создание ресурса
    key_t key = ftok(argv[0], 'a');
    semid = semget(key, 1, IPC_CREAT | 0666); // создаем массив из 2 семафоров
    scanf("%d", &number);
    // установление начального значения семафора в 2
    semctl(semid, 0, SETVAL, (int) 2);
    if (fork() == 0) {
        son_proc(argv[0], 0, number);
        exit(0);
    }
    if (fork() == 0) {
        son_proc(argv[0], 1, number);
        exit(0);
    }

    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID); // удаление массива семафоров
    return 0;
}