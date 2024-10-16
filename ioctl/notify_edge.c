
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/gpio.h>

#define GPIO_STR_LEN    32
#define MON_PIN         27
#define MON_DEBOUNCE    1000 //us
#define MON_DEFAULT_LEN 16

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
    line_info.offset = MON_PIN;
    if (ioctl(gpio_chip_fd, GPIO_V2_GET_LINEINFO_IOCTL, &line_info) == -1)
        EXIT_ERR;
    if (line_info.flags & GPIO_V2_LINE_FLAG_USED) {
        fprintf(stderr, "%s: "TEXT_RED"GPIO2 in use\n"TEXT_CLEAR, argv[0]);
        exit(EXIT_FAILURE);
    }

    struct gpio_v2_line_request line_req = {0};
    line_req.offsets[0] = MON_PIN;
    line_req.num_lines = 1;
    strncpy(line_req.consumer, "beckett", GPIO_STR_LEN);

    if (ioctl(gpio_chip_fd, GPIO_V2_GET_LINE_IOCTL, &line_req) == -1)
        EXIT_ERR;

    struct gpio_v2_line_attribute line_attr = {0};
    line_attr.id = GPIO_V2_LINE_ATTR_ID_DEBOUNCE;
    line_attr.debounce_period_us = MON_DEBOUNCE;

    struct gpio_v2_line_config_attribute line_cfg_attr = {0};
    line_cfg_attr.attr = line_attr;
    line_cfg_attr.mask = 1;

    struct gpio_v2_line_config line_cfg = {0};
    line_cfg.flags =    GPIO_V2_LINE_FLAG_INPUT             | \
                        GPIO_V2_LINE_FLAG_BIAS_PULL_DOWN    | \
                        GPIO_V2_LINE_FLAG_EDGE_RISING       | \
                        GPIO_V2_LINE_FLAG_EDGE_FALLING;
    line_cfg.attrs[0] = line_cfg_attr;
    line_cfg.num_attrs = 1;

    if (ioctl(line_req.fd, GPIO_V2_LINE_SET_CONFIG_IOCTL, &line_cfg) == -1)
        EXIT_ERR;

    // read into edge event ought to be blocking, so no poll()
    // -- perhaps poll() is correct?
    // -- kernel buffers 16, do we care?
    // -- read() ought to return some multiple of sizeof(line_event)
    struct gpio_v2_line_event ev;
    while (1) {
        read(line_req.fd, &ev, sizeof (ev));
        if (ev.id == GPIO_V2_LINE_EVENT_RISING_EDGE)
            printf(TEXT_GREEN"rose"TEXT_CLEAR"\n");
        else
            printf(TEXT_YELLOW"fell"TEXT_CLEAR"\n");
    }

    close(line_req.fd);
    close(gpio_chip_fd);
    exit(EXIT_SUCCESS);
}

