#include "basic_epoll_client.h"


static int basic_epoll_client_create() {
    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        LOG_ERROR("socket create error.");
        return -1;
    } else {
        LOG_INFO("socket create.");
    }

    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));

    saddr.sin_port = htons((short)EPOLL_SERVER_PORT);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        LOG_ERROR("connect error");
        return -1;
    } else {
        LOG_INFO("connect success.");
    }

    char buff[1024];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%s", "louis.tianlu@foxmail.com");

    send(fd, buff, strlen(buff), 0);
    LOG_INFO("client send: %s", buff);

    memset(buff, 0, sizeof(buff));
    recv(fd, buff, sizeof(buff), 0);

    LOG_INFO("client recv: %s", buff);

    send(fd, buff, strlen(buff), 0);
    LOG_INFO("client send: %s", buff);

    memset(buff, 0, sizeof(buff));
    recv(fd, buff, sizeof(buff), 0);

    LOG_INFO("client recv: %s", buff);

    close(fd);
    LOG_INFO("close socket fd");

    return 0;
}

int basic_epoll_client_init() {
    basic_epoll_client_create();

    return 0;
}