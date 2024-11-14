#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/utsname.h>

// Process related system calls
void fork_example();
void exit_example();
void wait_example();
void kill_example();
void exec_example();

// File related system calls
void open_read_write_example();
void link_unlink_example();
void stat_example();

// Communication related system calls
void pipe_example();
void fifo_example();

// Information related system calls
void getpid_example();
void getppid_example();
void uname_example();

int main() {
    int choice;
    while(1) {
        printf("\nMenu Driven System Call Demonstration\n");
        printf("1. Process related system calls\n");
        printf("2. File related system calls\n");
        printf("3. Communication related system calls\n");
        printf("4. Information related system calls\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("1. Fork\n2. Exit\n3. Wait\n4. Kill\n5. Exec\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if(choice == 1) fork_example();
                else if(choice == 2) exit_example();
                else if(choice == 3) wait_example();
                else if(choice == 4) kill_example();
                else if(choice == 5) exec_example();
                break;
            case 2:
                printf("1. Open, Read, Write, Close\n2. Link, Unlink\n3. Stat\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if(choice == 1) open_read_write_example();
                else if(choice == 2) link_unlink_example();
                else if(choice == 3) stat_example();
                break;
            case 3:
                printf("1. Pipe\n2. FIFO\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if(choice == 1) pipe_example();
                else if(choice == 2) fifo_example();
                break;
            case 4:
                printf("1. Get PID\n2. Get PPID\n3. Uname\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if(choice == 1) getpid_example();
                else if(choice == 2) getppid_example();
                else if(choice == 3) uname_example();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Process related system calls
void fork_example() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process: PID = %d\n", getpid());
    } else {
        printf("Parent process: PID = %d\n", getpid());
        wait(NULL); // Wait for child to finish
    }
}

void exit_example() {
    printf("This process will terminate using exit().\n");
    exit(0);
}

void wait_example() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process running. PID = %d\n", getpid());
        sleep(2); // Simulate some work in child
        exit(0);
    } else {
        printf("Parent waiting for child to terminate.\n");
        wait(NULL);
        printf("Child terminated.\n");
    }
}

void kill_example() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process running. PID = %d\n", getpid());
        while(1); // Infinite loop
    } else {
        sleep(1);
        printf("Killing child process.\n");
        kill(pid, SIGKILL); // Terminate the child process
        wait(NULL);
    }
}

void exec_example() {
    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/ls", "ls", NULL); // Replace child process with `ls` command
    } else {
        wait(NULL);
    }
}

// File related system calls
void open_read_write_example() {
    int fd;
    char buffer[100];
    fd = open("demo_output.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    write(fd, "Hello, World!\n", 14);
    lseek(fd, 0, SEEK_SET); // Move to beginning of the file
    read(fd, buffer, sizeof(buffer));
    printf("File content: %s\n", buffer);
    close(fd);
}

void link_unlink_example() {
    link("demo_output.txt", "testfile_link.txt");
    printf("Link created.\n");
    unlink("testfile_link.txt");
    printf("Link removed.\n");
}

void stat_example() {
    struct stat fileStat;
    if(stat("demo_output.txt",&fileStat) < 0) {
        perror("Error getting file stats");
        return;
    }
    printf("Information for demo_output.txt:\n");
    printf("File Size: \t\t%ld bytes\n", fileStat.st_size);
    printf("Number of Links: \t%ld\n", fileStat.st_nlink);
    printf("File inode: \t\t%ld\n", fileStat.st_ino);
    printf("File Permissions: \t");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf("\n");
}

// Communication related system calls
void pipe_example() {
    int fd[2];
    pid_t pid;
    char buffer[100];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return;
    }
    pid = fork();
    if (pid == 0) {
        // Child process
        close(fd[0]); // Close read end
        write(fd[1], "Hello from child!\n", 18);
        close(fd[1]);
    } else {
        // Parent process
        close(fd[1]); // Close write end
        read(fd[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);
        close(fd[0]);
        wait(NULL);
    }
}

void fifo_example() {
    char *fifo = "/tmp/myfifo";
    mkfifo(fifo, 0666);
    if(fork() == 0) {
        int fd = open(fifo, O_WRONLY);
        write(fd, "Hello via FIFO!\n", 16);
        close(fd);
    } else {
        char buffer[100];
        int fd = open(fifo, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);
        close(fd);
        wait(NULL);
    }
    unlink(fifo);
}

// Information related system calls
void getpid_example() {
    printf("Process ID: %d\n", getpid());
}

void getppid_example() {
    printf("Parent Process ID: %d\n", getppid());
}

void uname_example() {
    struct utsname buffer;
    if (uname(&buffer) < 0) {
        perror("Uname failed");
        return;
    }
    printf("Group ID: %d\n", getgid());
    printf("System page size: %ld bytes\n", sysconf(_SC_PAGESIZE));
    printf("System name: %s\n", buffer.sysname);
    printf("Node name: %s\n", buffer.nodename);
    printf("Release: %s\n", buffer.release);
    printf("Version: %s\n", buffer.version);
    printf("Machine: %s\n", buffer.machine);
}

