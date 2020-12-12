ccipher: ccipher.c ccipher.h
	@$(CC) $(CFLAGS) -o ccipher ccipher.c

test: ccipher
	valgrind --leak-check=full ./ccipher -s 2 ../test   # Linux memory leak detector

clean:
	@$(RM) -f ccipher

reallyclean: clean
	@$(RM) -rf ccipher.???? # scan-build working dir
	@$(RM) -rf reports      # scan-build html report


.PHONY = test clean reallyclean

