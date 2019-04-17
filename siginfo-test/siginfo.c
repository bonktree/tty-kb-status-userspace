#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

static char* signal_names[] = {
#   include "signalent.h"
};

#ifndef SIGINFO
#   define SIGINFO SIGPWR
#endif

static char* code_messages[] = {
    [SI_USER] = "SI_USER",
    [SI_KERNEL] = "SI_KERNEL",
    /* [SI_QUEUE] = "SI_QUEUE", */
    0
};

void sensible_siginfo_handler(int sig, siginfo_t *info, void *ucontext) {
    if (sig != SIGINFO) return;

    fprintf(stderr, "got %s from %s: %s\n",
            signal_names[sig],
            code_messages[info->si_code],
            strerror(info->si_errno));
}

int run(struct __argvopts ao) {
    unsigned int bytes_consumed = 0;
    int re;
    char character = '\0';
    struct sigaction old;
    struct sigaction cur = {
        .sa_mask = {{0}},
        .sa_flags = SA_SIGINFO,
        .sa_sigaction = sensible_siginfo_handler,
    };

    fprintf(stderr, "%s %s: my pid is %d\n", ao.argv0, ao.argv1, getpid());
    sigaction(SIGINFO, &cur, &old);
    fprintf(stderr, "%s: handler for signal %d installed\n", ao.argv0, SIGINFO);

    if (!ao.read_input) {
        while(1) pause();
        return 0;
    }
    while ((re = read(0, &character, 1)) != 0) {
        if ((re == -1) && (errno == EINTR)) continue;
        ++bytes_consumed;
        if (ao.echo_input_back)
            write(1, &character, 1);
    }
    fprintf(stderr, "%s %s: %d bytes read\n", ao.argv0, ao.argv1, bytes_consumed);
    return 0;
}

