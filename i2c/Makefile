
CC=gcc
CFLAGS=-Wall -Werror -Wpedantic
PROGS=toggle_led i2cdetect

all: ${PROGS}

toggle_led: toggle_led.c
	${CC} ${CFLAGS} -o $@ $^

i2cdetect: i2cdetect.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: sync clean printi2c

printi2c:
	i2cdetect -y 1

sync:
	cd ..; rsync -r --mkpath --exclude 'esp32_slave' beckett@sputnik:/home/beckett/code/frore/i2c .

clean:
	rm -rf ${PROGS}

