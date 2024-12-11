#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define KEY_ID 1234  // Unique key ID for the message queue

// Define the structure for the message queue
typedef struct message {
  long msg_type;       // Message type (must be a positive integer)
  char msg_text[100];  // Message content
} message_t;

int main() {
  // block sigint
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  int msgid;  // Message queue identifier

  // Access the message queue created by the sender
  msgid = msgget(KEY_ID, S_IRUSR | S_IWUSR);
  if (msgid == -1) {     // Check if the message queue retrieval failed
    perror("msgget");    // Print an error message
    exit(EXIT_FAILURE);  // Exit the program with a failure status
  }

  message_t msg;  // Declare a message structure

  while (1) {  // Loop until the SIGINT signal is received
    printf("(Receiver) Waiting to receive messages...\n");

    // Receive a message from the queue of type 1
    if (msgrcv(msgid, &msg, sizeof(message_t) - sizeof(long), 1, 0) == -1) {
      perror("msgrcv");
      exit(1);
    }

    // Log the received message
    printf("(Receiver) Message received: %s\n", msg.msg_text);

    //exit if message is "STOP"
    if (strcmp(msg.msg_text, "STOP") == 0) {
      break;
    }
  }

  // Remove the message queue before exiting
  if (msgctl(msgid, IPC_RMID, NULL) == -1) {
    perror("msgctl");    // Print an error message if removal fails
    exit(EXIT_FAILURE);  // Exit the program with a failure status
  }
  printf("\t\tReceiver process terminated.\n");  // Log program termination
  return 0;                                  // Exit with a success status
}
