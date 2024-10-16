
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/i2c-dev.h>

#define TEXT_RED    "\x1b[31m"
#define TEXT_RST    "\x1b[0m"
#define TEXT_GRN    "\x1b[32m"
#define TEXT_YLW    "\x1b[33m"

#define I2C_DEV     "/dev/i2c-1"
#define ESP_ADDR    0x42

#define EXIT_ERR \
{ \
    fprintf(stderr, "%s: "TEXT_RED"%s (%d)\n"TEXT_RST, argv[0], strerror(errno), errno); \
    exit(EXIT_FAILURE); \
}

int dev_fd;
const char begin_sample = '1';

void close_dev_fd(void) {
    close(dev_fd);
}

int main(int argc, char *argv[]) {
    atexit(close_dev_fd);

    if ((dev_fd = open(I2C_DEV, O_RDWR)) == -1)
        EXIT_ERR;

    if (ioctl(dev_fd, I2C_SLAVE, ESP_ADDR) == -1)
        EXIT_ERR;

    char buf[1] = {0};
    buf[0] = 0x42;
    int ret = write(dev_fd, buf, sizeof (buf));
    printf("write to esp: %d\n", ret);
    
    exit(EXIT_SUCCESS);
}

