#include "exec.h"

// sets the "key" argument with the key part of
// the "arg" argument and null-terminates it
static void get_environ_key(char *arg, char *key) {

    int i;
    for (i = 0; arg[i] != '='; i++)
        key[i] = arg[i];

    key[i] = END_STRING;
}

// sets the "value" argument with the value part of
// the "arg" argument and null-terminates it
static void get_environ_value(char *arg, char *value, int idx) {

    int i, j;
    for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
        value[j] = arg[i];

    value[j] = END_STRING;
}

// sets the environment variables passed
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void set_environ_vars(char **eargv, int eargc) {

    // Your code here
}

// opens the file in which the stdin/stdout or
// stderr flow will be redirected, and returns
// the file descriptor
// 
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int open_redir_fd(char *file) {

    // Your code here
    return -1;
}

int replace(int newfd, char *file_name, bool write) {
    int oldfd;
    if ((oldfd = open(file_name,
                      (write ? O_CREAT : 0) | (write ? O_WRONLY : O_RDONLY),
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) == -1) {
        char buf[BUFLEN] = {0};
        snprintf(buf, sizeof buf, "%s: %s", SHELL_NAME, file_name);
        perror(buf);
        return 1;
    } else {
        if (dup2(oldfd, newfd) == -1) {
            perror(SHELL_NAME);
            close(oldfd);
            return 1;
        }
    }
    return 0;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
void exec_cmd(struct cmd *cmd) {
    struct execcmd *execcmd = (struct execcmd *) cmd;
    struct backcmd *backcmd = (struct backcmd *) cmd;
    switch (cmd->type) {
        case EXEC:
            // spawns a command
            for (int i = 0; execcmd->eargv[i] != NULL; i++) {
                char *name = execcmd->eargv[i];
                char *value = NULL;
                int position = 0;
                while (value == NULL) {
                    if (name[position] == '=') {
                        name[position] = 0;
                        value = name + position + 1;
                    } else {
                        position++;
                    }
                }
                if (setenv(name, value, true) == -1) {
                    char buf[BUFLEN] = {0};
                    snprintf(buf, sizeof buf,
                             "cannot define environment variable %s", name);
                    perror(buf);
                }
                name[position] = '=';
            }
            if (execvp(
                    execcmd->argv[0],
                    execcmd->argv
            ) == -1) {
                perror(SHELL_NAME);
            }
            break;
        case BACK: {
            // runs a command in background
            exec_cmd(backcmd->c);
            break;
        }
        case REDIR: {
            // changes the input/output/stderr flow
            int errors = 0;
            if (execcmd->in_file[0] != 0)
                errors += replace(STDIN_FILENO, execcmd->in_file, false);
            if (execcmd->out_file[0] != 0)
                errors += replace(STDOUT_FILENO, execcmd->out_file, true);
            if (execcmd->err_file[0] != 0)
                errors += replace(STDERR_FILENO, execcmd->err_file, true);
            if (errors > 0) return;
            cmd->type = EXEC;
            exec_cmd(cmd);
            break;
        }
        case PIPE: {
            // pipes two commands
            //
            // Your code here
            printf("Pipes are not yet implemented\n");

            // free the memory allocated
            // for the pipe tree structure
            free_command(parsed_pipe);

            break;
        }
        default:
            break;
    }
}

