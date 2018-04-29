#include <bits/sigset.h>
#include "defs.h"
#include "readline.h"
#include "runcmd.h"

char prompt[PRMTLEN] = {0};
char preprompt[BUFLEN] = {0};
char last_back[BUFLEN] = {0};
pid_t last_back_pid = 0;

// runs a shell command
static void run_shell() {

    char *cmd;

    while ((cmd = read_line(prompt)) != NULL)
        if (run_cmd(cmd) == EXIT_SHELL)
            return;
}

// initialize the shell
// with the "HOME" directory
static void init_shell() {

    char buf[BUFLEN] = {0};
    char *home = getenv("HOME");

    if (chdir(home) < 0) {
        snprintf(buf, sizeof buf, "cannot cd to %s ", home);
        perror(buf);
    } else {
        snprintf(prompt, sizeof prompt, "(%s)", home);
    }
}

void listen_to_your_dying_child(int signal, siginfo_t *siginfo, void *context) {
    UNUSED(signal);
    UNUSED(context);
    if (siginfo->si_pid == last_back_pid) {
        snprintf(preprompt, PRMTLEN,
                 "==> terminado: PID=%d (%s)", siginfo->si_pid, last_back);
        preprompt[PRMTLEN - 1] = 0;
    }
}

void pay_attention_to_your_children() {
    struct sigaction action;
    sigset_t set;
    sigemptyset(&set);
    action.sa_mask = set;
    action.sa_flags = SA_SIGINFO | SA_RESTART;
    action.sa_sigaction = &listen_to_your_dying_child;
    sigaction(SIGCHLD, &action, NULL);
}

int main(void) {
    pay_attention_to_your_children();
    init_shell();
    run_shell();
    return 0;
}
