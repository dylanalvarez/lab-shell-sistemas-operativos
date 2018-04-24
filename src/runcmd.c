#include "runcmd.h"

int status = 0;
struct cmd *parsed_pipe;

// runs the command in 'cmd'
int run_cmd(char *cmd) {

    pid_t p;
    // parses the command line
    struct cmd *parsed = parse_line(cmd);

    // if the "enter" key is pressed
    // just print the prompt again
    if (cmd[0] == END_STRING)
        return 0;

    if (parsed->type == EXEC) {
        // cd built-in call
        if (cd((struct execcmd *) parsed))
            return 0;

        // exit built-in call
        if (exit_shell((struct execcmd *) parsed)) {
            free_command(parsed);
            return EXIT_SHELL;
        }

        // pwd built-in call
        if (pwd((struct execcmd *) parsed))
            return 0;
    }

    // forks and run the command
    if ((p = fork()) == 0) {

        // keep a reference
        // to the parsed pipe cmd
        // so it can be freed later
        if (parsed->type == PIPE)
            parsed_pipe = parsed;

        exec_cmd(parsed);
    }

    // store the pid of the process
    parsed->pid = p;

    // background process special treatment
    if (parsed->type != BACK) {
        // waits for the process to finish
        waitpid(p, &status, 0);
        print_status_info(parsed);
    }

    free_command(parsed);

    return 0;
}

