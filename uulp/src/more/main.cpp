#include <stdio.h>
#include <stdlib.h>

#define PAGELEN     24
#define LINELEN     512

int see_more(FILE *cmd) {
    int c;
    printf("\033[7m more? \033[m");

    while ((c = getc(cmd)) != EOF) {
        if (c == 'q') {
            return 0;
        } else if (c == ' ') {
            return PAGELEN;
        } else if (c == '\n') {
            return 1;
        }
    }

    return 0;
}

void do_more(FILE *fp) {
    char line[LINELEN];
    int num_of_lines = 0;
    int reply;

    FILE *fp_tty;
    fp_tty = fopen("/dev/tty", "r");
    if (fp_tty == NULL) {
        exit(1);
    }

    // fgets 每次读取一行数据到 line 中. 每读取一行 num_of_lines++, 该值等于 PAGELEN
    // 时, 即当前页面最多显示行数时, 输出 more? 并根据接下来的输入做进一步操作.
    while (fgets(line, LINELEN, fp)) {
        if (num_of_lines == PAGELEN) {
            // 从键盘读取下一步操作, 从标准输入中读取分页数据, 从键盘中读取用户输入
            reply = see_more(fp_tty);
            if (reply == 0) {
                break;
            }

            num_of_lines -= reply;
        }

        if (fputs(line, stdout) == EOF) {
            exit(1);
        }

        num_of_lines++;
    }
}

int main(int argc, char const *argv[])
{
    FILE *fp;

    if (argc == 1) {
        // 无参数时, 从标准输入读取数据, 方便使用管道 | 操作
        do_more(stdin);
    } else {
        while (--argc) {
            if ((fp = fopen(*++argv, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }
    }

    return 0;
}
