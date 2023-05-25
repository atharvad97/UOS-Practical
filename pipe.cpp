#include <iostream>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process: reading end of the pipe
        close(pipefd[1]);  // Close the writing end

        char buffer[256];
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("read");
            exit(1);
        }

        std::cout << "Child process received: " << buffer << std::endl;

        close(pipefd[0]);  // Close the reading end
    } else {
        // Parent process: writing end of the pipe
        close(pipefd[0]);  // Close the reading end

        std::string message = "Hello, child process!";
        ssize_t bytesWritten = write(pipefd[1], message.c_str(), message.length());
        if (bytesWritten == -1) {
            perror("write");
            exit(1);
        }

        close(pipefd[1]);  // Close the writing end
    }

    return 0;
}
