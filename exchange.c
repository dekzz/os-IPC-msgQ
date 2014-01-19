#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    setenv("KEY","1337",1);
    key_t key = atoi(getenv("KEY"));
    
    int nrProg = argc-1;
    int msqid;
    
	/*
    mq.msg_perm.uid = 1337;
    mq.msg_perm.gid = getgid();
    mq.msg_perm.mode = 600;
    mq.msg_qbytes = 5;
	*/

    if ((msqid = msgget(key,IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
	
	msgctl(msqid, IPC_SET, &mq);

    switch(fork())
        {
           case -1:
                printf("Cannot create new 'receiver' process!\n");
                break;
           case 0:
                execl("./receiver", "receiver", NULL, NULL);
                exit(0);
           default:
                wait(NULL);
        }

    int i = 1;
    while(i <= nrProg)
    {
        switch(fork())
        {
           case -1:
                printf("Cannot create new 'sender' process!\n");
                break;
           case 0:
                execl("./sender", "sender", argv[i], NULL);
                exit(1);
           default:
                wait(NULL);
        }
        i++;
    }

    nrProg++;
    while(nrProg > 0)
 {
        wait(NULL);
        nrProg--;
    }

    return 0;
}
