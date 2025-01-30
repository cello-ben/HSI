CFLAGS = -Wall -Wpedantic -Wextra  -Wno-unused-parameter -Wno-unused-function -Werror -std=gnu99

all:
	gcc -o hsi driver.c $(CFLAGS) -O3
	rm -f *.o

debug:
	gcc -g -o hsi driver.c $(CFLAGS)
	rm -f *.o

clean:
	rm -rf hsi hsi.dSYM