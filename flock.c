#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd;
    pid_t pid;

    // Open a file for writing
    fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        printf("Child process trying to lock the file...\n");

        // Lock the entire file for writing
        if (flock(fd, LOCK_EX) == -1) {
            perror("flock");
            exit(1);
        }

        printf("Child process locked the file successfully.\n");
        sleep(5);

        // Unlock the file
        if (flock(fd, LOCK_UN) == -1) {
            perror("flock");
            exit(1);
        }

        printf("Child process unlocked the file.\n");
    } else {
        // Parent process
        printf("Parent process trying to lock the file...\n");

        // Lock the entire file for writing
        if (flock(fd, LOCK_EX) == -1) {
            perror("flock");
            exit(1);
        }

        printf("Parent process locked the file successfully.\n");
        sleep(5);

        // Unlock the file
        if (flock(fd, LOCK_UN) == -1) {
            perror("flock");
            exit(1);
        }

        printf("Parent process unlocked the file.\n");
    }

    // Close the file
    close(fd);

    return 0;
}
