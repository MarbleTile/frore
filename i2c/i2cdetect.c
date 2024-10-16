
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/i2c-dev.h>

#define TEXT_RED    "\x1b[31m"
#define TEXT_RST    "\x1b[0m"
#define TEXT_GRN    "\x1b[32m"
#define TEXT_YLW    "\x1b[33m"

#define I2C_DEV     "/dev/i2c-1"

#define EXIT_ERR \
{ \
    fprintf(stderr, "%s: "TEXT_RED"%s (%d)\n"TEXT_RST, argv[0], strerror(errno), errno); \
    exit(EXIT_FAILURE); \
}

int main(int argc, char *argv[]) {
    int dev_fd;
    char buf[1];

    printf(TEXT_YLW"%4s 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n00: %24s"TEXT_RST, " ", " ");
    for (char i = 0x8; i < 0x78; i++) {
        if (i % 0x10 == 0)
            printf(TEXT_YLW"%02x: "TEXT_RST, i);
        if ((dev_fd = open(I2C_DEV, O_RDWR)) == -1) {
            printf(TEXT_RED"-- "TEXT_RST);
            continue;
        }
        if (ioctl(dev_fd, I2C_SLAVE, i) == -1) {
            printf(TEXT_RED"-- "TEXT_RST);
            close(dev_fd);
            continue;
        }
        if (read(dev_fd, buf, sizeof (buf)) != 1) {
            printf("-- ");
        } else {
            printf(TEXT_GRN"%02x "TEXT_RST, i);
        }
        if ((i&0x0f) == 0x0f)
            printf("\n");
        close(dev_fd);
    }
    printf("\n");
   
    exit(EXIT_SUCCESS);
}

