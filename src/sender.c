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
  int msgid;  // Message queue identifier

  // Register the SIGINT signal handler
  signal(SIGINT, handle_sigint);

  // Create or access a message queue with read/write permissions
  msgid = msgget(KEY_ID, S_IRUSR | S_IWUSR | IPC_CREAT);
  if (msgid == -1) {     // Check if the message queue creation failed
    perror("msgget");    // Print an error message
    exit(EXIT_FAILURE);  // Exit the program with a failure status
  }

  message_t msg;     // Declare a message structure
  msg.msg_type = 1;  // Set the message type to 1

  while (keep_running) {  // Loop until the SIGINT signal is received
    // Generate a message with a random number and format it into the message
    // text
    snprintf(msg.msg_text, 100, "This is your lucky number %d", rand() % 1000);

    // Send the message to the message queue
    if (msgsnd(msgid, &msg, sizeof(message_t) - sizeof(long), 0) == -1) {
      perror("msgsnd");    // Print an error message if sending fails
      exit(EXIT_FAILURE);  // Exit the program with a failure status
    }

    printf("(Sender) Message sent\n");  // Log the message sent status
    fflush(stdout);  // Ensure the message is printed immediately

    sleep(2);  // Wait for 2 seconds before sending the next message
  }

  // Remove the message queue before exiting
  if (msgctl(msgid, IPC_RMID, NULL) == -1) {
    perror("msgctl");    // Print an error message if removal fails
    exit(EXIT_FAILURE);  // Exit the program with a failure status
  }

  printf("Sender process terminated.\n");  // Log program termination
  return 0;                                // Exit with a success status
}

