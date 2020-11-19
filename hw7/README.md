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

Provide a report document to demonstrate your application with screenshots of your results. All screenshots should be nicely resized and annotated. Your document should show what you did, how you did it, display the results, and explain what happened.

## Deliverables:
- C source file
- Document demonstrating your application.
