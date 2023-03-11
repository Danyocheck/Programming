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

int msgid1, msgid2;

void
hndlr(int s)
{
    msgctl(msgid1, IPC_RMID, NULL); // удаление первой очереди сообщений
    msgctl(msgid2, IPC_RMID, NULL); // удаление второй очереди сообщений
    exit(0);
}

void
proc_server(char *str)
{
    signal(SIGINT, SIG_DFL);
    struct {
        long mtype;
        int data;
    } msgb;

    int number = 0;
    key_t key1 = ftok(str, 'a');
    int msgid_serv_get = msgget(key1, IPC_CREAT | 0666);
    key_t key2 = ftok(str, 'b');
    int msgid_serv_send = msgget(key2, IPC_CREAT | 0666);

    while (1) {
        msgrcv(msgid_serv_get, &msgb, sizeof(int), 0, 0); // получение сообщения
        if (msgb.mtype == 1) {
            ++number;
        } else if (msgb.mtype == 2) {
            msgb.data = number;
            msgsnd(msgid_serv_send, &msgb, sizeof(int), 0); // отправка сообщения
        } else {
            break;
        }
    }
}

void
proc_client(char *str, int N)
{
    signal(SIGINT, SIG_DFL);
    struct {
        long mtype;
        int data;
    } msgb;

    // подключение ресурса
    key_t key1 = ftok(str, 'a');
    int msgid_cl_send = msgget(key1, IPC_CREAT | 0666);
    key_t key2 = ftok(str, 'b');
    int msgid_cl_get = msgget(key2, IPC_CREAT | 0666);

    msgb.mtype = 1;
    for (int i = 0; i < N; ++i) {
        msgsnd(msgid_cl_send, &msgb, sizeof(int), 0); // отправка сообщения
    }

    msgb.mtype = 2;
    msgsnd(msgid_cl_send, &msgb, sizeof(int), 0); // отправка сообщения
    msgrcv(msgid_cl_get, &msgb, sizeof(int), 0, 0); // получение сообщения
    printf("%d\n", msgb.data);

    msgb.mtype = 3;
    msgsnd(msgid_cl_send, &msgb, sizeof(int), 0); // отправка сообщения
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, hndlr);
    // создание ресурса
    key_t key1 = ftok(argv[0], 'a');
    msgid1 = msgget(key1, IPC_CREAT | 0666); // создание первой очереди сообщений
    key_t key2 = ftok(argv[0], 'b');
    msgid2 = msgget(key2, IPC_CREAT | 0666); // создание второй очереди сообщений
    int number;
    scanf("%d", &number);

    if (fork() == 0) {
        proc_server(argv[0]);
        exit(0);
    }
    if (fork() == 0) {
        proc_client(argv[0], number);
        exit(0);
    }
    while (wait(NULL) > 0) {}
    msgctl(msgid1, IPC_RMID, NULL); // удаление первой очереди сообщений
    msgctl(msgid2, IPC_RMID, NULL); // удаление второй очереди сообщений
    return 0;
}