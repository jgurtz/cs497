# Program notes
- The assignment for ccipher is below, mine adds the ability to work with stdin via the special file name '-'. Run it with `-h` for the help screen
- Type `make` in the base directory to build the `ccipher` utility and the basic `cat` example in prof_example/
- If you have clang and have installed the scan-build utility, edit the Makefile in the base directory to enable compilation under that tool. For more information, see https://clang-analyzer.llvm.org/. Additional information on the scan-build utility is at https://clang-analyzer.llvm.org/
- On linux, you can install `valgrind` and run `make test` to check for memory leaks. Unfortunately, valgrind does not work properly (yet) on MacOS "Catalina" or presumably "Big Sur"
- You may notice the lookup table is implemented on the heap with dynamic memory allocation. This is not neccesary and was originally on the stack. But there was a weird bug that I thought was some kind of stack corruption and so I put it on the heap. When the bug persisted, I found it was actually an issue with using -O0 as the compiler optimization level. Switching to -O2 fixed it and I decided to leave the heap allocation version ¯\_(ツ)_/¯
- This is not really useful, but maybe insteresting as an example


# CS 497 - Assignment 7 - Due 12/7/2020
```
Name
    ccipher
Synopsis
    ccipher [OPTION] ... [FILE]...
Description
    Caesar cipher the contents of FILE, then concatenate to standard output.
OPTIONS:
    -s NUM  NUM represent the number of letters to shift -r

    -r      should reverse or 'undo'.
            EX: -r -s 5: This will undo a message shifted 5.

    -n      number all output lines
```

Use the open(), read(), and close() system calls to create a Caesar cipher application in C.

Take command line arguments for the number of letters to shift and for the name of the file to open, read, and close.
Read each letter in from the file, shift the requested number of places using the Caesar’s cipher method, then redirect the output to stdout.
Use the cat application example as a guide. See lecture slides for more information.

## Example usage:
Encipher contents of input_file.txt with 5 shifts
`ccipher -s 5 input_file.txt`

Undo a Caesar’s cipher of 5 shifts from file.txt, redirect the output to a file:
`ccipher -r -s 5 input_file.txt >> output_file.txt`

Provide a report document to demonstrate your application with screenshots of your results. All screenshots should be nicely resized and annotated. Your document should show what you did, how you did it, display the results, and explain what happened. Use the `strace` utility to monitor your application and include that output in your report along with some discussion.

## Deliverables:
- C source file
- Document demonstrating your application.
