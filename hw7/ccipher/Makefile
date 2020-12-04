ccipher: ccipher.c ccipher.h
	@$(CC) $(CFLAGS) -o ccipher ccipher.c

test: ccipher
	leaks --atExit -- ./ccipher -s 2 ../test3                   # XCode memory leak detector
	#valgrind --leak-check=full ./ccipher/ccipher -s 2 test3    # Linux memory leak detector

clean:
	@$(RM) -f ccipher

reallyclean: clean
	@$(RM) -rf ccipher.????
	@$(RM) -rf reports


.PHONY = test clean reallyclean

