#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void _abort() {
    printf("abort() start.\n");
    abort();
    printf("abort() end.\n");
}

static void sig_handler(int sig) {
    printf("Oh my God.\n");
}

static void _signal() {
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("signal: ");
        return;
    }

    for (int i = 0; ; i++) {
        printf("%d\n", i);
        sleep(3);
    }
}

int main(int argc, char *argv[])
{
    //_abort();
    _signal();

    return 0;
}
