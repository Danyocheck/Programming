#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

enum { N = 1024 };

int msgid, shmid, semid;

union semun {
    int val; // значение одного семафора
    struct semid_ds *buf; // параметры массива семафоров в целом
    ushort *array; // массив значений семафоров
};

void
hndlr(int s)
{
    shmctl(shmid, IPC_RMID, NULL); // удаление разделяемой памяти
    semctl(semid, 0, IPC_RMID, (int) 0); // удаление массива семафоров
    msgctl(msgid, IPC_RMID, NULL); // удаление очереди сообщений
    exit(0);
}

void
proc(char *str)
{
    signal(SIGINT, SIG_DFL);
    key_t key1 = ftok(str, 'a');
    key_t key2 = ftok(str, 'b');
    key_t key3 = ftok(str, 'c');
    msgget(key1, IPC_CREAT | 0666); // создание очереди сообщений
    shmget(key2, N, IPC_CREAT | 0666); // создание разделяемой памяти
    semget(key3, 2, IPC_CREAT | 0666); // создаем массив из 2 семафоров
    exit(0);
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, hndlr);
    union semun sem;
    static ushort buf[2];
    sem.array = buf;
    key_t key1 = ftok(argv[0], 'a'); // создание ресурса
    key_t key2 = ftok(argv[0], 'b');
    key_t key3 = ftok(argv[0], 'c');
    msgid = msgget(key1, IPC_CREAT | 0666); // создание очереди сообщений
    shmid = shmget(key2, N, IPC_CREAT | 0666); // создание разделяемой памяти
    semid = semget(key3, 2, IPC_CREAT | 0666); // создаем массив из 2 семафоров
    sem.array[0] = 0;
    sem.array[1] = 0;
    // установление начальных значений семафоров в 0
    semctl(semid, 0, SETALL, sem);
    if (fork() == 0) {
        proc(argv[0]);
    }
    wait(NULL);
    shmctl(shmid, IPC_RMID, NULL); // удаление разделяемой памяти
    semctl(semid, 0, IPC_RMID, (int) 0); // удаление массива семафоров
    msgctl(msgid, IPC_RMID, NULL); // удаление очереди сообщений
    return 0;
}