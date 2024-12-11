#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t sender_pid, receiver_pid;

  // Fork and execute the sender process
  sender_pid = fork();
  if (sender_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (sender_pid == 0) {
    execl("./sender", "sender", NULL);
    perror("execl");
    exit(EXIT_FAILURE);
  }

  // Fork and execute the receiver process
  receiver_pid = fork();
  if (receiver_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (receiver_pid == 0) {
    execl("./receiver", "receiver", NULL);
    perror("execl");
    exit(EXIT_FAILURE);
  }

  // Wait for SIGINT to stop the sender process
  printf("Press Ctrl+C to stop the sender process and complete execution.\n");

  // block sigint
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  wait(NULL);  // Wait for sender to finish
  wait(NULL);  // Wait for receiver to finish

  printf("\t\tParent process completed.\n");
  return 0;
}

