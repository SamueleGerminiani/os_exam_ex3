#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define KEY_ID 1234  // Unique key ID for the message queue

// Define the structure for the message queue
typedef struct message {
  long msg_type;       // Message type (must be a positive integer)
  char msg_text[100];  // Message content
} message_t;

int keep_running = 1;  // Flag to control the loop, modified by signal handler

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
  keep_running = 0;  // Set the flag to 0 to break the loop
}

int main() {
  // Register the SIGINT signal handler
  signal(SIGINT, handle_sigint);

  int msgid;  // Message queue identifier

  // Access the message queue created by the sender
  msgid = msgget(KEY_ID, S_IRUSR | S_IWUSR);
  if (msgid == -1) {     // Check if the message queue retrieval failed
    perror("msgget");    // Print an error message
    exit(EXIT_FAILURE);  // Exit the program with a failure status
  }

  message_t msg;  // Declare a message structure

  while (keep_running) {  // Loop until the SIGINT signal is received
    printf("(Receiver) Waiting to receive messages...\n");

    // Receive a message from the queue of type 1
    if (msgrcv(msgid, &msg, sizeof(message_t) - sizeof(long), 1, 0) == -1) {
      perror("msgrcv");  // Print an error message if receiving fails
      break;             // Exit the loop on failure
    }

    // Log the received message
    printf("(Receiver) Message received: %s\n", msg.msg_text);
  }

  printf("Receiver process completed.\n");  // Log program termination
  return 0;                                 // Exit with a success status
}
