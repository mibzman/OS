# BlackDOS Project, Spring 2018
## Sam Borick

### Currently, this OS can:

1. Accept string or integer input
2. Print strings or integers
3. Read data from the floppy device
4. Write data to the floppy device
5. Read files
6. Write files
7. Delete files
8. Run programs

#### Currently on startup the OS:

1. sets a color scheme (currently l33t HAXOR) from the config file on the floppy
3. Loads a test program (fib.c)
4. Runs the program
	The programs asks for a number input and then prints that number of fibonacci numbers

This can be tested by running compileOS.sh.  The result should run in the boch emulator.

### Notes:

- (Resolved?) Currently the OS has been observed to crash on startup. This occurs sporadically, and when run again (rarely after as many as 3 attempts) the machine boots as expected. The cause of this is currently unknown.

- compileOS.sh attempts to copy the contents of the config after bochs exits, however this command will not be run if force exited.


## ACADEMIC INTEGRITY PLEDGE 
- I have not used source code obtained from another student nor any other unauthorized source, either modified or unmodified.
                                                                       
- All source code and documentation used in my program is either my original work or was derived by me from the source code published in the textbook for this course or presented in class.
                                                                       
- I have not discussed coding details about this project with anyone other than my instructor. I understand that I may discuss the concepts of this program with other students and that another student may help me debug my program so long as neither of us writes anything during the discussion or modifies any computer file during the discussion.
                                                                       
I have violated neither the spirit nor letter of these restrictions.

:_Sam Borick___ Date:_02/08/18___

3460:4/526 BlackDOS2020 kernel, Version 1.5, Spring 2018.