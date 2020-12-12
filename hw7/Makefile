CC = cc
CFLAGS := -O2 -g -Wall -Werror -std=c11

export CC CFLAGS

all:
	#scan-build -o ccipher/reports make -C ccipher              # MacOS with clang and additional code vetting
	#scan-build -o prof_example/reports make -C prof_example    # MacOS with clang and additional code vetting
	make -C ccipher                                             # Regular build for linux
	make -C prof_example                                        # Regular build for linux

test: all
	make -C ccipher test    # memory leak detector and test -s 2

clean:
	make -C prof_example clean

reallyclean: clean
	make -C ccipher reallyclean
	make -C prof_example reallyclean


.PHONY = all test clean reallyclean

# EOF
