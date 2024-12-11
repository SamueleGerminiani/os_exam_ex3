# Variables
CC = gcc
CFLAGS= -g -Wall -std=gnu99
TARGET = ex3
REC_EXEC = receiver
SEND_EXEC = sender
INCLUDES = inc

# Build the parent process and child exec program
all: $(TARGET) $(REC_EXEC) $(SEND_EXEC)

# Rule to compile the main program (ex1)
$(TARGET): src/ex3.c
	$(CC) -I $(INCLUDES) $(CFLAGS) -o $(TARGET) src/ex3.c

# Rule to compile the child exec program
$(REC_EXEC): src/receiver.c
	$(CC) -I $(INCLUDES) $(CFLAGS) -o $(REC_EXEC) src/receiver.c

# Rule to compile the child exec program
$(SEND_EXEC): src/sender.c
	$(CC) -I $(INCLUDES) $(CFLAGS) -o $(SEND_EXEC) src/sender.c

# Clean up the generated files
clean:
	rm -f $(TARGET) $(CHILD_EXEC) $(REC_EXEC) $(SEND_EXEC) src/*.o

# Phony targets
.PHONY: all clean run

