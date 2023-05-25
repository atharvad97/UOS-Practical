#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 100

int main() {
    // Generate a unique key for the shared memory segment
    key_t key = ftok(".", 'x');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create a shared memory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    char* sharedMemory = (char*)shmat(shmid, NULL, 0);
    if (sharedMemory == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    // Parent process
    if (fork() != 0) {
        // Write a message to the shared memory
        std::string message = "Hello from the parent process!";
        std::strcpy(sharedMemory, message.c_str());

        // Wait for the child process to read the message
        sleep(2);

        // Print the modified message read from shared memory
        std::cout << "Parent process received: " << sharedMemory << std::endl;

        // Detach and remove the shared memory segment
        shmdt(sharedMemory);
        shmctl(shmid, IPC_RMID, NULL);
    }
    // Child process
    else {
        // Wait for the parent process to write the message
        sleep(1);

        // Print the original message read from shared memory
        std::cout << "Child process received: " << sharedMemory << std::endl;

        // Modify the message in shared memory
        std::string modifiedMessage = "Hello from the child process!";
        std::strcpy(sharedMemory, modifiedMessage.c_str());

        // Detach the shared memory segment
        shmdt(sharedMemory);
    }

    return 0;
}
