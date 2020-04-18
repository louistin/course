#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/stat.h>
#include "fileinfo.h"

static void do_ls(const char *dirname) {
    DIR *dirp;
    struct dirent *direntp = NULL;

    if ((dirp = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
    } else {
        while ((direntp = readdir(dirp)) != NULL) {
            do_stat(direntp->d_name);
            direntp = NULL;
        }

        closedir(dirp);
        dirp = NULL;
    }
}

int main(int argc, char const *argv[])
{

#if 1
    if (argc == 1) {
        do_ls(".");
    } else {
        for (int i = 1; i < argc; i++) {
            printf("%s:\n", argv[i]);
            do_ls(argv[i]);
        }
    }
#endif

#if 0
    if (argc > 1) {
        do_stat(argv[1]);
    }
#endif

    return 0;
}


