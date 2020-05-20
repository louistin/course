#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("SHELL=%s\n", getenv("SHELL"));
    return 0;
}
