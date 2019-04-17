#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

int main(int argc, char* argv[]) {
    struct __argvopts argvopts = {
        .argv0 = argv[0],
        .argv1 = argv[1] ?: "",
        .read_input = 0,
        .echo_input_back = 0,
    };
    for (int i = 0; i < argc; ++i) {
        if (!strcmp("--read", argv[i])) {
            argvopts.read_input = 1;
        }
        else if (!strcmp("--no-read", argv[i])) {
            argvopts.read_input = 0;
        }
        else if (!strcmp("--echo", argv[i])) {
            argvopts.echo_input_back = 1;
        }
    }

    return run(argvopts);
}
