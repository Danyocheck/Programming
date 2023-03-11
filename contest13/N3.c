#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int shmid, semid, shmid_help;

void
hndlr(int s)
{
    shmctl(shmid_help, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, (int) 0);
    exit(0);
}

void
son_proc(char *str, int type, int N)
{
    signal(SIGINT, SIG_DFL);
    int start_pos, stop_pos, i;
    if (type == 0) {
        start_pos = 0;
        stop_pos = N - 1;
        i = 1;
    } else {
        start_pos = N - 1;
        stop_pos = 0;
        i = -1;
    }

    key_t key1 = ftok(str, 'a');
    int shmid_son = shmget(key1, sizeof(int) * N, IPC_CREAT | 0666);
    int *shmaddr = shmat(shmid, NULL, 0);

    key_t key2 = ftok(str, 'b');
    int semid_son = semget(key2, 1, IPC_CREAT | 0666);

    key_t key3 = ftok(str, 'c');
    int shmid_help1 = shmget(key3, sizeof(int), IPC_CREAT | 0666);
    int *shmaddr_help = shmat(shmid_help1, NULL, 0);

    struct sembuf semb;
    semb.sem_num = 0;
    semb.sem_flg = 0;

    while (start_pos != stop_pos) {
        if (shmaddr_help[0] == 0) {
            break;
        } else if (shmaddr_help[0] == 1) {
            int stop_one_proc = 0;
            if (type == 0) {
                semb.sem_op = -1;
            } else {
                semb.sem_op = 0;
                stop_one_proc = 1;
            }
            semop(semid_son, &semb, 1);
            if (stop_one_proc) {
                break;
            }
            ++shmaddr[start_pos];
            --shmaddr_help[0];
            semb.sem_op = 1;
            semop(semid_son, &semb, 1);
            break;
        }
        ++shmaddr[start_pos];
        --shmaddr_help[0];
        start_pos += i;
    }
    shmdt(shmaddr);
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, hndlr);
    int number;
    scanf("%d", &number);

    key_t key1 = ftok(argv[0], 'a');
    shmid = shmget(key1, sizeof(int) * number, IPC_CREAT | 0666);

    key_t key2 = ftok(argv[0], 'b');
    semid = semget(key2, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, (int) 1);

    key_t key3 = ftok(argv[0], 'c');
    shmid_help = shmget(key3, sizeof(int), IPC_CREAT | 0666);
    int *shmaddr_help = shmat(shmid, NULL, 0);
    shmaddr_help[0] = number;

    int *shmaddr = shmat(shmid, NULL, 0);
    for (int i = 0; i < number; ++i) {
        shmaddr[i] = i;
    }

    if (fork() == 0) {
        son_proc(argv[0], 1, number);
        exit(0);
    }
    if (fork() == 0) {
        son_proc(argv[0], 0, number);
        exit(0);
    }

    while (wait(NULL) > 0) {}
    if (number <= 10000) {
        for (int i = 0; i < number; ++i) {
            printf("%d\n", shmaddr[i]);
        }
    }

    shmctl(shmid_help, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, (int) 0);
    return 0;
}