
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/gpio.h>

#define EXIT_ERR \
{ \
    fprintf(stderr, "%s: %s\n", argv[0], strerror(errno)); \
    exit(EXIT_FAILURE); \
}

#define GPIO_CHIP_AMT   3
#define GPIO_CHIP_PATH  "/dev/gpiochip"

void print_gpiochip_info(struct gpiochip_info info) {
    printf("%s:\n\t%s\n\t%u\n", info.name, info.label, info.lines);
}

int main(int argc, char *argv[]) {
    
    int gpio_chip_fd; 
    struct gpiochip_info chip_info;

    for (int i = 0; i < GPIO_CHIP_AMT; i++) {
        char chip_path[BUFSIZ];
        snprintf(chip_path, BUFSIZ, GPIO_CHIP_PATH"%d", i);
        if ((gpio_chip_fd = open(chip_path, O_RDONLY)) == -1)
            EXIT_ERR;

        if (ioctl(gpio_chip_fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) == -1)
            EXIT_ERR;

        print_gpiochip_info(chip_info);
        close(gpio_chip_fd);
    }

    exit(EXIT_SUCCESS);
}

