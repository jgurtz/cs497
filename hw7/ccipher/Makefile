ccipher: ccipher.c ccipher.h
	@$(CC) $(CFLAGS) -o ccipher ccipher.c

test: ccipher
	valgrind --leak-check=full ./ccipher -s 2 ../test3    # Linux memory leak detector

clean:
	@$(RM) -f ccipher

reallyclean: clean
	@$(RM) -rf ccipher.????
	@$(RM) -rf reports


.PHONY = test clean reallyclean

