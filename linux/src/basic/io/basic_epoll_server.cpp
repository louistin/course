#include "basic_epoll_server.h"


// 设置信号忽略
static void basic_epoll_server_sigaction() {
    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sig, NULL);
}

static int basic_epoll_server_set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) < 0) {
        LOG_ERROR("fcntl error.");
        return -1;
    }

    return 0;
}

static int basic_epoll_server_set_reusable(int fd) {
    int on = 0;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    return 0;
}

static int basic_epoll_server_create() {
    struct epoll_event ev;
    struct epoll_event events[EPOLL_EVENTS];

    int epfd = epoll_create(EPOLL_MAXFDS);
    LOG_INFO("create epoll fd.");

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        LOG_ERROR("create socket error. %s", errno);
        return -1;
    }

    LOG_INFO("create server fd.");

    basic_epoll_server_set_nonblock(fd);
    basic_epoll_server_set_reusable(fd);

    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    // 注册 epoll 事件
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        LOG_ERROR("epoll add fd error.");
        close(fd);
        close(epfd);
        return -1;
    }

    LOG_INFO("add fd EOPLLIN event to epoll.");

    struct sockaddr_in saddr, caddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons((short)EPOLL_SERVER_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        LOG_ERROR("bind error.");
        close(fd);
        close(epfd);
        return -1;
    }

    LOG_INFO("bind server fd port{%u} success.", saddr.sin_port);

    if (listen(fd, 32) < 0) {
        LOG_ERROR("listen error.");
        close(fd);
        close(epfd);
        return -1;
    }

    LOG_INFO("server fd start to listen...");

    int cfd;
    int nfds;
    int ret;
    char buffer[1024];
    for (;;) {
        LOG_INFO("epoll wait ...");
        nfds = epoll_wait(epfd, events, EPOLL_MAXFDS, 500);
        LOG_INFO("epoll return...");

        for (int i = 0; i < nfds; i++) {
            if (fd == events[i].data.fd) {
                memset(&caddr, 0, sizeof(caddr));
                size_t len = sizeof(caddr);
                cfd = accept(fd, (struct sockaddr *)&caddr, (socklen_t *)&len);
                if (cfd < 0) {
                    LOG_ERROR("accept error.");
                    break;
                }

                LOG_INFO("accept client fd, [%s:%u].", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

                basic_epoll_server_set_nonblock(cfd);

                ev.data.fd = cfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                LOG_INFO("add client fd EOPLLIN event to epoll.");

            } else if (events[i].events & EPOLLIN) {
                memset(buffer, 0, sizeof(buffer));

                LOG_INFO("read client message...");
                ret = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (ret < 0) {
                    LOG_ERROR("recv error.");
                    close(fd);
                    close(epfd);
                    return -1;
                }

                LOG_INFO("recv client message: %s", buffer);

                ev.data.fd = events[i].data.fd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);

                LOG_INFO("add fd EOPLLOUT event to epoll.");

            } else if (events[i].events & EPOLLOUT) {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%s", "louis.tianlu@gmail.com");

                ret = send(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (ret < 0) {
                    LOG_ERROR("send error.");
                    close(fd);
                    close(epfd);
                    return -1;
                }

                LOG_INFO("send message to client: %s", buffer);

                ev.data.fd = events[i].data.fd;
                epoll_ctl(epfd, EPOLL_CTL_DEL, ev.data.fd, &ev);

                LOG_INFO("del fd from epoll.");
            }
        }
    }

    if (fd > 0) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }

    if (epfd > 0) {
        close(epfd);
    }

    return 0;
}


int basic_epoll_server_init() {
    basic_epoll_server_sigaction();
    basic_epoll_server_create();

    return 0;
}