CC = cc
CFLAGS := -O2 -g -Wall -Werror -std=c11

export CC CFLAGS

all:
	scan-build -o ccipher/reports make -C ccipher
	scan-build -o prof_example/reports make -C prof_example

clean:
	make -C ccipher clean
	make -C prof_example clean


.PHONY = all clean

# EOF
