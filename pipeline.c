
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_COMMANDS 10
#define MAX_ARGS 20
#define MAX_INPUT 1024

int main(void) {
    char input[MAX_INPUT];
    char *commands[MAX_COMMANDS];
    int count = 0;

    printf("Multi-Process Command Pipeline System\n");
    printf("Enter commands separated by | :\n> ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL)
    return 1;

input[strcspn(input, "\n")] = '\0';

/* Reject empty or whitespace-only input */
char *check = input;

while (*check == ' ' || *check == '\t') {
    check++;
}

if (*check == '\0') {
    fprintf(stderr, "Error: No commands entered.\n");
    return 1;
}
    char *saveptr;
    char *token = strtok_r(input, "|", &saveptr);

    while (token != NULL && count < MAX_COMMANDS) {
        while (*token == ' ' || *token == '\t')
            token++;

        commands[count++] = token;
        token = strtok_r(NULL, "|", &saveptr);
    }

    if (count == 0) {
        fprintf(stderr, "Error: No commands entered.\n");
        return 1;
    }

    char *argv[MAX_COMMANDS][MAX_ARGS];

    for (int i = 0; i < count; i++) {
        int argc = 0;
        char *arg_saveptr;
        char *arg = strtok_r(commands[i], " \t", &arg_saveptr);

        while (arg != NULL && argc < MAX_ARGS - 1) {
            argv[i][argc++] = arg;
            arg = strtok_r(NULL, " \t", &arg_saveptr);
        }

        argv[i][argc] = NULL;

        if (argc == 0) {
            fprintf(stderr, "Error: Empty command in pipeline.\n");
            return 1;
        }
    }

    int previous_read = -1;
    pid_t pids[MAX_COMMANDS];
    int launched = 0;
    int failed = 0;

    for (int i = 0; i < count; i++) {
        int fd[2] = {-1, -1};

        if (i < count - 1 && pipe(fd) == -1) {
            perror("pipe");
            failed = 1;
            break;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");

            if (fd[0] != -1) close(fd[0]);
            if (fd[1] != -1) close(fd[1]);

            failed = 1;
            break;
        }

        if (pid == 0) {
            if (previous_read != -1) {
                if (dup2(previous_read, STDIN_FILENO) == -1) {
                    perror("dup2 input");
                    _exit(1);
                }
            }

            if (i < count - 1) {
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("dup2 output");
                    _exit(1);
                }
            }

            if (previous_read != -1) close(previous_read);
            if (fd[0] != -1) close(fd[0]);
            if (fd[1] != -1) close(fd[1]);

            execvp(argv[i][0], argv[i]);

            fprintf(stderr, "Command failed: %s: ", argv[i][0]);
            perror("");
            _exit(127);
        }

        pids[launched++] = pid;

        if (previous_read != -1)
            close(previous_read);

        if (fd[1] != -1)
            close(fd[1]);

        previous_read = fd[0];
    }

    if (previous_read != -1)
        close(previous_read);

    int status_error = failed;

    for (int i = 0; i < launched; i++) {
        int status;

        while (waitpid(pids[i], &status, 0) == -1) {
            if (errno == EINTR)
                continue;

            perror("waitpid");
            status_error = 1;
            break;
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            status_error = 1;
    }

    if (status_error)
        fprintf(stderr, "Pipeline completed with an error.\n");

    return status_error ? 1 : 0;
}

