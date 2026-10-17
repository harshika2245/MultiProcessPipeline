# Multi-Process Command Pipeline System

## 1. Project Overview

The Multi-Process Command Pipeline System is a C-based application that demonstrates how Linux operating systems execute multiple commands using processes and inter-process communication.

Users can enter commands separated by the pipe symbol (`|`). The output of one command is passed as the input to the next command.

## 2. Objectives

- Understand process creation using `fork()`.
- Demonstrate inter-process communication using pipes.
- Redirect standard input and output using `dup2()`.
- Execute Linux commands using `execvp()`.
- Synchronize processes using `waitpid()`.
- Handle invalid commands and empty input.

## 3. Technologies Used

- Operating System: Ubuntu Linux
- Programming Language: C
- Compiler: GCC
- System Calls: `pipe()`, `fork()`, `dup2()`, `execvp()`, and `waitpid()`
- Version Control: Git and GitHub

## 4. Requirements

- Ubuntu Linux or a compatible Linux environment, including WSL.
- GCC compiler and standard development tools.
- A terminal.

Install the compiler if required:

```bash
sudo apt update
sudo apt install build-essential
```

## 5. Project Structure

```text
MultiProcessPipeline/
├── pipeline.c
├── students.txt
├── README.md
└── pipeline
```

The `pipeline` executable is generated during compilation and does not need to be committed to GitHub.

## 6. Compilation

Compile the source code using:

```bash
gcc -Wall -Wextra pipeline.c -o pipeline
```

## 7. Execution

Run the application:

```bash
./pipeline
```

Enter commands separated by `|`.

Example:

```text
cat students.txt | grep CSIT | sort
```

Expected output:

```text
CSIT Ananya
CSIT Harshika
```

Another example:

```text
cat students.txt | wc -l
```

Expected output:

```text
5
```

## 8. Operating System Concepts

### Process Creation

The `fork()` system call creates child processes to execute individual commands.

### Inter-Process Communication

The `pipe()` system call creates a communication channel between commands.

### Input/Output Redirection

The `dup2()` system call connects the output of one command to the input of the next command.

### Command Execution

The `execvp()` function executes commands using the Linux environment's command search path.

### Process Synchronization

The `waitpid()` system call allows the parent process to wait for child processes to finish.

## 9. Testing

The following tests have been performed:

1. Empty input: Displays an error and exits without executing a command.
2. Invalid command: Displays an error when a command cannot be found.
3. Two-command pipeline: Successfully executes `cat students.txt | wc -l`.
4. Three-command pipeline: Successfully executes `cat students.txt | grep CSIT | sort`.

## 10. Current Limitations

- Supports simple commands separated by pipes.
- Does not provide full shell functionality such as quoted arguments, command chaining with `&&`, or input/output redirection using `<` and `>`.
- Commands must fit within the application's configured input and argument limits.

## 11. Conclusion

This project demonstrates fundamental Linux operating system concepts through a practical multi-process command execution application. It provides hands-on experience with process creation, pipes, file descriptor redirection, command execution, and process synchronization.

