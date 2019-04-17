#pragma once

struct __argvopts {
    char* argv0;
    char* argv1;
    unsigned short read_input:1;
    unsigned short echo_input_back:1;
};

int run(struct __argvopts);
