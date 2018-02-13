# Sam Borick
## Lab 1

For this project I modified kernel.c to be able to:

1. Print output
2. Print output to screen or printer
3. Access print output functions via an interrupt

This was accomplished by modifying the printstring function, and also by uncommenting the handleInterrupt21 re-routes, and allowing the assembler to handle the interrupting.

This can be tested by running compileOS.sh.  the result should be a boch emulator showing the blackdos logo, and a new file titled printer.out should contain "hello world from sam borick"