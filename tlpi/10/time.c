#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    time_t t = time(NULL);

    char *ct = ctime(&t);
    sleep(1);
    char *ct2 = ctime(&t);

    printf("%ld, %s, %s\n", t, ct, ct2);

    return 0;
}
