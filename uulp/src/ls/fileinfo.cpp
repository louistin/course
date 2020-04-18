#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

static void mode2letters(int mode, char *str) {
    strcpy(str, "----------");

    if (S_ISDIR(mode)) {
        str[0] = 'd';
    } else if (S_ISCHR(mode)) {
        str[0] = 'c';
    } else if (S_ISBLK(mode)) {
        str[0] = 'b';
    }

    if (mode & S_IRUSR) {
        str[1] = 'r';
    }
    if (mode & S_IWUSR) {
        str[2] = 'w';
    }
    if (mode & S_IXUSR) {
        str[3] = 'x';
    }

    if (mode & S_IRGRP) {
        str[4] = 'r';
    }
    if (mode & S_IWGRP) {
        str[5] = 'w';
    }
    if (mode & S_IXGRP) {
        str[6] = 'x';
    }

    if (mode & S_IROTH) {
        str[7] = 'r';
    }
    if (mode & S_IWOTH) {
        str[8] = 'w';
    }
    if (mode & S_IXOTH) {
        str[9] = 'x';
    }
}

static char *uid2name(uid_t uid) {
    struct passwd *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        memset(numstr, 0, sizeof(numstr));
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
}

static char *gid2name(gid_t gid) {
    struct group *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        memset(numstr, 0, sizeof(numstr));
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return grp_ptr->gr_name;
    }
}

static void show_file_info(const char *filename, struct stat *infop) {
    char modestr[11];
    memset(modestr, 0, sizeof(modestr));
    mode2letters(infop->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int)infop->st_nlink);
    printf("%-8s ", uid2name(infop->st_uid));
    printf("%-8s ", gid2name(infop->st_gid));
    printf("%8ld ", (long)infop->st_size);
    printf("%.12s ", ctime((const time_t *)&infop->st_mtim) + 4);
    printf("%s\n", filename);
}

void do_stat(const char *filename) {
    struct stat info;
    if (stat(filename, &info) == -1) {
        perror(filename);
    } else {
        show_file_info(filename, &info);
    }
}