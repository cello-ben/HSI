CFLAGS = -Wall -Wpedantic -Wextra  -Wno-unused-parameter -Wno-unused-function -Werror -std=gnu99
SRC_DIR = src

all:
	gcc -o hsi $(SRC_DIR)/*.c $(CFLAGS) -O3
	rm -f *.o

debug:
	gcc -g -o hsi $(SRC_DIR)/*.c $(CFLAGS)
	rm -f *.o

test:
	./hsi

clean:
	rm -rf hsi hsi.dSYM