
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/gpio.h>

#define GPIO_STR_LEN 32

#define EXIT_ERR \
{ \
    fprintf(stderr, "%s: %s (%d)\n", argv[0], strerror(errno), errno); \
    exit(EXIT_FAILURE); \
}

void print_gpiochip_info(struct gpiochip_info info) {
    printf("%s (%s, %u lines)\n", \
            info.name, info.label, info.lines);
}

void print_gpio_line_info(int line, struct gpio_v2_line_info info) {
    char *user = info.flags & GPIO_V2_LINE_FLAG_USED ? info.consumer : "unused";
    char *input_mode = info.flags & GPIO_V2_LINE_FLAG_INPUT ? "input" : "output";
    char *active_mode = info.flags & GPIO_V2_LINE_FLAG_ACTIVE_LOW ? "active-low" : "active-high";
    printf("line %2d:\t%10s\t%10s\t%10s\t%10s\n", \
            line, info.name, user, input_mode, active_mode);
}

int main(int argc, char *argv[]) {
    
    int gpio_chip_fd; 
    struct gpiochip_info chip_info;

    if ((gpio_chip_fd = open("/dev/gpiochip0", O_RDONLY)) == -1)
        EXIT_ERR;

    if (ioctl(gpio_chip_fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) == -1)
        EXIT_ERR;

    print_gpiochip_info(chip_info);

    for (int i = 0; i < chip_info.lines; i++) {
        struct gpio_v2_line_info line_info;
        line_info.offset = i;
        if (ioctl(gpio_chip_fd, GPIO_V2_GET_LINEINFO_IOCTL, &line_info) == -1)
            printf("line $2d:\tERR: %s\n", strerror(errno));
        else
            print_gpio_line_info(i, line_info);
    }
    close(gpio_chip_fd);

    exit(EXIT_SUCCESS);
}

