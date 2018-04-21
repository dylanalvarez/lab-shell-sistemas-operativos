#include "builtin.h"
#include "freecmd.h"

// returns true if the 'exit' call
// should be performed
int exit_shell(struct execcmd *parsed) {
    return strncmp(parsed->scmd, "exit", 4) == 0;
}

// returns true if "chdir" was performed
// this means that if 'cmd' contains:
// 	$ cd directory (change to 'directory')
// 	$ cd (change to HOME)
// it has to be executed and then return true
int cd(struct execcmd *parsed) {
    bool invoked = false;
    if (parsed->argc > 0 && strcmp(parsed->argv[0], "cd") == 0) {
        if (chdir(parsed->argc == 1 ? getenv("HOME") : parsed->argv[1]) == -1) {
            perror(SHELL_NAME);
        } else {
            char *current_dir = get_current_dir_name();
            snprintf(prompt, sizeof prompt, "(%s)", current_dir);
            free(current_dir);
        }
        invoked = true;
    }
    return invoked;
}

// returns true if 'pwd' was invoked
// in the command line
int pwd(struct execcmd *parsed) {
    bool invoked = false;
    if (parsed->argc > 0 && strcmp(parsed->argv[0], "pwd") == 0) {
        char *current_dir = get_current_dir_name();
        printf("%s\n", current_dir);
        free(current_dir);
        invoked = true;
    }
    return invoked;
}
