
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/gpio.h>

#define GPIO_STR_LEN    32
#define LED_PIN         17
#define BLINK_TIME      1

#define TEXT_RED    "\x1b[31m"
#define TEXT_CLEAR  "\x1b[0m"
#define TEXT_GREEN  "\x1b[32m"
#define TEXT_YELLOW "\x1b[33m"

#define EXIT_ERR \
{ \
    fprintf(stderr, "%s: "TEXT_RED"%s (%d)\n"TEXT_CLEAR, argv[0], strerror(errno), errno); \
    exit(EXIT_FAILURE); \
}

int main(int argc, char *argv[]) {
    int gpio_chip_fd; 
    struct gpiochip_info chip_info;

    if ((gpio_chip_fd = open("/dev/gpiochip0", O_RDONLY)) == -1)
        EXIT_ERR;

    if (ioctl(gpio_chip_fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) == -1)
        EXIT_ERR;

    struct gpio_v2_line_info line_info = {0};
    line_info.offset = LED_PIN;
    if (ioctl(gpio_chip_fd, GPIO_V2_GET_LINEINFO_IOCTL, &line_info) == -1)
        EXIT_ERR;
    if (line_info.flags & GPIO_V2_LINE_FLAG_USED) {
        fprintf(stderr, "%s: "TEXT_RED"GPIO2 in use\n"TEXT_CLEAR, argv[0]);
        exit(EXIT_FAILURE);
    }

    struct gpio_v2_line_request line_req = {0};
    line_req.offsets[0] = LED_PIN;
    line_req.num_lines = 1;
    strncpy(line_req.consumer, "beckett", GPIO_STR_LEN);

    if (ioctl(gpio_chip_fd, GPIO_V2_GET_LINE_IOCTL, &line_req) == -1)
        EXIT_ERR;

    struct gpio_v2_line_config line_cfg = {0};
    line_cfg.flags = GPIO_V2_LINE_FLAG_OUTPUT | GPIO_V2_LINE_FLAG_BIAS_PULL_DOWN;

    if (ioctl(line_req.fd, GPIO_V2_LINE_SET_CONFIG_IOCTL, &line_cfg) == -1)
        EXIT_ERR;

    struct gpio_v2_line_values led_on = {
        .bits = 1,
        .mask = 1
    };

    struct gpio_v2_line_values led_off = {
        .bits = 0,
        .mask = 1
    };

    while (1) {
        if (ioctl(line_req.fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &led_on) == -1)
            EXIT_ERR;
        printf(TEXT_GREEN"on"TEXT_CLEAR"\n");
        sleep(BLINK_TIME);

        if (ioctl(line_req.fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &led_off) == -1)
            EXIT_ERR;
        printf(TEXT_YELLOW"off"TEXT_CLEAR"\n");
        sleep(BLINK_TIME);
    }

    close(line_req.fd);
    close(gpio_chip_fd);
    exit(EXIT_SUCCESS);
}

