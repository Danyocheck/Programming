#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct
{
    long mtype;
    int data;
} Message;

void
son(int fd, char *filename)
{
    key_t key_msg = ftok(filename, '1');
    int msgid = msgget(key_msg, 0);

    Message msg;
    msg.data = 0;
    int num;

    while(1) {
        msgrcv(msgid, &msg, sizeof(int), getpid(), 0);
        lseek(fd, -1 * sizeof (int), SEEK_CUR);
        if (read(fd, &num, sizeof (int))) {};
        if (num == 0) {
            msg.mtype = getppid();
            msgsnd(msgid, &msg, sizeof(int), 0);
            break;
        }
        --num;
        if (write(fd, &num, sizeof (int))) {};
        msg.mtype = getppid();
        msgsnd(msgid, &msg, sizeof(int), 0);
    }

    close(fd);
    exit(0);
}


int msgid, fd;
int *array_sons;

void
sig_handler(int s)
{
    msgctl(msgid, IPC_RMID, 0);
    free(array_sons);
    close(fd);
    exit(0);
}

int
main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
    int N, K;
    if (scanf("%d%d", &N, &K)) {};

    char filename[] = "123XXXXXX";
    fd = mkstemp(filename);
    unlink(filename);

    key_t key_msg = ftok(argv[0], '1');
    msgid = msgget(key_msg, 0666 | IPC_CREAT);

    array_sons = malloc(N * sizeof (int));

    if (write(fd, &K, sizeof K)) {};
    int pid = 0;
    for (int i = 0; i < N; ++i) {
        if ((pid = fork()) == 0) {
            son(fd, argv[0]);
        }
        array_sons[i] = pid;
    }

    Message msg_from_father;
    msg_from_father.data = 0;
    for (int i = 0; i < (K + N); ++i) {
        int index = i % N;
        msg_from_father.mtype = array_sons[index];
        msgsnd(msgid, &msg_from_father, sizeof(int), 0);
        msgrcv(msgid, &msg_from_father, sizeof(int), getpid(), 0);
    }
    while (wait(NULL) > 0) {}
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &K, sizeof(K)) > 0) {
        printf("%d\n", K);
    }
    msgctl(msgid, IPC_RMID, 0);
    free(array_sons);
    close(fd);
    return 0;
}