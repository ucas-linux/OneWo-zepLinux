/* SPDX-License-Identifier: Apache-2.0 */
/*
 * AS32X601 TCP Echo Server
 * 监听 0.0.0.0:4242，接受连接并将收到的数据原样回送。
 * PC 测试命令: echo "hello" | nc 192.168.1.100 4242
 */
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <errno.h>

LOG_MODULE_REGISTER(tcp_echo, LOG_LEVEL_INF);

#define ECHO_PORT   4242
#define BUF_SIZE    1500
#define BACKLOG     1

int main(void)
{
    int srv, cli;
    struct sockaddr_in addr;
    static uint8_t buf[BUF_SIZE];
    ssize_t len;
    int ret;

    srv = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srv < 0) {
        LOG_ERR("socket() failed: %d", errno);
        return -1;
    }

    (void)memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(ECHO_PORT);

    int opt = 1;
    zsock_setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ret = zsock_bind(srv, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        LOG_ERR("bind() failed: %d", errno);
        zsock_close(srv);
        return -1;
    }

    ret = zsock_listen(srv, BACKLOG);
    if (ret < 0) {
        LOG_ERR("listen() failed: %d", errno);
        zsock_close(srv);
        return -1;
    }

    LOG_INF("TCP echo server listening on port %d", ECHO_PORT);

    while (1) {
        cli = zsock_accept(srv, NULL, NULL);
        if (cli < 0) {
            LOG_ERR("accept() failed: %d", errno);
            continue;
        }

        LOG_INF("TCP client connected");

        while ((len = zsock_recv(cli, buf, sizeof(buf), 0)) > 0) {
            ret = zsock_send(cli, buf, len, 0);
            if (ret < 0) {
                LOG_ERR("send() error: %d", errno);
                break;
            }
        }

        if (len < 0) {
            LOG_ERR("recv() error: %d", errno);
        }

        LOG_INF("TCP client disconnected");
        zsock_close(cli);
    }

    zsock_close(srv);
    return 0;
}
