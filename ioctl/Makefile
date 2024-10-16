
CC=gcc
CFLAGS=-Wall -Werror -Wpedantic

all: gpio_cards gpioinfo blink_led notify_edge

gpio_cards: gpio_cards.c
	${CC} ${CFLAGS} -o $@ $^ 

gpioinfo: gpioinfo.c
	${CC} ${CFLAGS} -o $@ $^ 

blink_led: blink_led.c
	${CC} ${CFLAGS} -o $@ $^

notify_edge: notify_edge.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: printmod sync clean

printmod:
	lsmod | awk '$$3 != "0" { print $$1 }'

sync:
	cd ..; rsync -r --mkpath beckett@sputnik:/home/beckett/code/frore/ioctl .

clean:
	rm -rf gpio_cards gpioinfo blink_led notify_edge

