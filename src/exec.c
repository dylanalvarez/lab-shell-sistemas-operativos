#include "exec.h"

int replace_fd(int newfd, int oldfd) {
    if (dup2(oldfd, newfd) == -1) {
        perror(SHELL_NAME);
        close(oldfd);
        return 1;
    }
    return 0;
}

int replace(int newfd, char *file_name, int flags) {
    int oldfd;
    if (file_name[0] == '&') {
        oldfd = (int) strtol(file_name + 1, NULL, 10);
        if (oldfd == 0) {
            return replace(STDOUT_FILENO, file_name + 1, flags)
                   + replace_fd(STDERR_FILENO, STDOUT_FILENO);
        }
    } else {
        if (file_name[0] == '>') {
            file_name += 1;
            flags |= O_APPEND;
        }
        oldfd = open(file_name, flags,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if (oldfd == -1) {
            char buf[BUFLEN] = {0};
            snprintf(buf, sizeof buf, "%s: %s", SHELL_NAME, file_name);
            perror(buf);
            return 1;
        }
    }
    return replace_fd(newfd, oldfd);
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
void exec_cmd(struct cmd *cmd) {
    struct execcmd *execcmd = (struct execcmd *) cmd;
    struct backcmd *backcmd = (struct backcmd *) cmd;
    struct pipecmd *pipecmd = (struct pipecmd *) cmd;
    pid_t p;
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
            if (execcmd->in_file[0] != 0) {
                errors += replace(STDIN_FILENO, execcmd->in_file,
                                  O_RDONLY);
            }
            if (execcmd->out_file[0] != 0)
                errors += replace(STDOUT_FILENO, execcmd->out_file,
                                  O_CREAT | O_WRONLY);
            if (execcmd->err_file[0] != 0)
                errors += replace(STDERR_FILENO, execcmd->err_file,
                                  O_CREAT | O_WRONLY);
            if (errors > 0) return;
            cmd->type = EXEC;
            exec_cmd(cmd);
            break;
        }
        case PIPE: {
            // pipes two commands
            int pipefd[2];
            if (pipe(pipefd) == -1) { perror(SHELL_NAME); }

            if ((p = fork()) == 0) {
                close(pipefd[1]); // Close unused write end
                replace_fd(STDIN_FILENO, pipefd[0]);
                exec_cmd(pipecmd->rightcmd);
            } else if (p > 0) {
                close(pipefd[0]); // Close unused read end
                replace_fd(STDOUT_FILENO, pipefd[1]);
                exec_cmd(pipecmd->leftcmd);
                waitpid(p, NULL, 0);
            } else {
                perror(SHELL_NAME);
            }
            break;
        }
        default:
            break;
    }
}
