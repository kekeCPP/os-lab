#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   int mtext;
};

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   int len;
   key_t key;
   system("touch msgq.txt");

   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }

   if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to send messages.\n");
   printf("Input anything and then press enter to send messages.\n");
   buf.mtype = 2; /* we don't really care in this case */

   // Any input by the user will trigger loop
   // This is just to make sure we have time to start the receiver
   char input[200];
   while(fgets(input, sizeof input, stdin) != NULL) {
      for (int i = 0; i < 50; i++)
      {
         int randomNumber = rand() % __INT_MAX__; // Generate random integer
         buf.mtext = randomNumber; // set the messagetext to the integer
         if (msgsnd(msqid, &buf, sizeof buf.mtext, 0) == -1) /* +1 for '\0' */
         perror("msgsnd");

         // if no there was no error sending the message
         else { printf("message %d was succesfully sent.\n", i + 1); }
      }

      buf.mtext = -1; // set the message text to -1 to signal that we are done
      if (msgsnd(msqid, &buf, sizeof(buf.mtext), 0) == -1)
         perror("msgsnd");
      else { printf("Final message was sent succesfully."); }
   }

   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
   printf("message queue: done sending messages.\n");
   return 0;
}
