ccipher: ccipher.c ccipher.h
	@$(CC) $(CFLAGS) -o ccipher ccipher.c


test: ccipher
	sudo leaks --atExit -- ./ccipher -s 2 ../test3  # XCode memory leak detector
	#valgrind ./ccipher/ccipher -s 2 test3          # Linux memory leak detector

clean:
	@$(RM) -f ccipher


.PHONY = clean

