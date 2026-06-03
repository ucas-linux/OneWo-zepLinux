/* SPDX-License-Identifier: Apache-2.0 */
/*
 * AS32X601 UDP Echo Server
 * 监听 0.0.0.0:4242，将收到的 UDP 数据原样回送给发送方。
 * PC 测试命令: echo "hello" | nc -u 192.168.1.100 4242
 */
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <errno.h>

LOG_MODULE_REGISTER(udp_echo, LOG_LEVEL_INF);

#define ECHO_PORT   4242
#define BUF_SIZE    1500

int main(void)
{
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t clen = sizeof(client);
    static uint8_t buf[BUF_SIZE];
    ssize_t len;
    int ret;

    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("socket() failed: %d", errno);
        return -1;
    }

    (void)memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(ECHO_PORT);

    ret = zsock_bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        LOG_ERR("bind() failed: %d", errno);
        zsock_close(sock);
        return -1;
    }

    LOG_INF("UDP echo server listening on port %d", ECHO_PORT);

    while (1) {
        clen = sizeof(client);
        len = zsock_recvfrom(sock, buf, sizeof(buf), 0,
                             (struct sockaddr *)&client, &clen);
        if (len < 0) {
            LOG_ERR("recvfrom() error: %d", errno);
            continue;
        }

        ret = zsock_sendto(sock, buf, len, 0,
                           (struct sockaddr *)&client, clen);
        if (ret < 0) {
            LOG_ERR("sendto() error: %d", errno);
        }
    }

    zsock_close(sock);
    return 0;
}
