#include <stdio.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

typedef struct
{
    long mtype;
    int data;
} Message;

void
son(int fd, char *argv)
{
    key_t key = ftok(argv[0], '1');
    int msgid = msgget(key, 0);
    Message mes;
    mes.data = 0;

    while (1) {
        int n;
        lseek(fd, -1 * sizzeof(int), SEEK_END);
        read(fd, &n, sizeof(int));
        if (n == 0) {
            mes.type = getppid();
            msgsnd(msgid, &mes, sizeof(int), 0);
            break;
        }
        --n;
        write(fd, &n, sizeof(int));
        mes.type = getppid();
        msgsnd(msgid, &mes, sizeof(int), 0);
    }

    close(fd);
    exit(0);
}

int shmid;

int
main(int argc, char *argv[])
{
    int n, k;
    sacnf("%d%d", &n, &k);

    key_t key = ftok(argv[0], '1');
    int msgid = msgget(key, 0666 | IPC_CREAT);

    char filename[] = "helpXXXXXX";
    int fd = mkstemp(filename);
    unlink(filename);
    write(fd, &k, sizeof(int));
    int array_sons = malloc(n * sizeof (int));

    int pid;
    for (int i = 0; i < n; ++i) {
        if ((pid = fork()) == 0) {
            son(fd, argv[0]);
            exit(0);
        }
        array_son[i] = pid;
    }

    Message mes;
    mes.data = 0;
    for (int i = 0; i < k + n; ++i) {
        int index = i % n;
        mes.type = array_son[index];
        msgsnd(msgid, &mes, sizeof(int), 0);
        msgrcv(msgid, &mes, sizeof(int), getpid(), 0);
    }
    while (wait(NULL) > 0) {}

    lseek(fd, 0 , SEEK_SET);
    while (read(fd, &n, sizeof(int)) == 1) {
        printf("%d\n", n);
    }
    close(fd);
    return 0;
}