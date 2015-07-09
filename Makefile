all: barcode
barcode: barcode.c

.PHONY: clean
clean:
	-rm barcode

CFLAGS=-std=c11 -Wall -Wextra -pedantic
