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

static int basic_epoll_server_create() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        LOG_ERROR("create socket error.");
        return -1;
    }

    basic_epoll_server_sigaction();

    int on = 1;
    int epfd = epoll_create(EPOLL_MAXFDS);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in saddr, caddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons((short)EPOLL_SERVER_PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        LOG_ERROR("bind error.");
        close(fd);
        close(epfd);
        return -1;
    }

    if (listen(fd, 32) < 0) {
        LOG_ERROR("listen error.");
        close(fd);
        close(epfd);
        return -1;
    }

    struct epoll_event ev;
    struct epoll_event events[EPOLL_EVENTS];

    ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    int cfd;
    int nfds;
    int ret;
    char buffer[1024];
    for (;;) {
        nfds = epoll_wait(epfd, events, EPOLL_MAXFDS, 0);

        for (int i = 0; i < nfds; i++) {
            if (fd == events[i].data.fd) {
                memset(&caddr, 0, sizeof(caddr));
                size_t len = sizeof(caddr);
                cfd = accept(fd, (struct sockaddr *)&caddr, (socklen_t *)&len);
                if (cfd < 0) {
                    LOG_ERROR("accept error.");
                    break;
                }

                basic_epoll_server_set_nonblock(cfd);

                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);

            } else if (events[i].data.fd & EPOLLIN) {
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

                ev.data.fd = fd;
                ev.events = EPOLLOUT;
                epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);

            } else if (events[i].data.fd & EPOLLOUT) {
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
    basic_epoll_server_create();

    return 0;
}