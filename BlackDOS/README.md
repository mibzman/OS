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
2. loads a shell

This can be tested by running compileOS.sh.  The result should run in the boch emulator.

- boot - Reboot the system 
- cls - Clear the screen 
- copy file1 file2 - Create file2 and copy all bytes of file1 to file2 without deleting file1. 
- del filename - Delete the named file 
- dir - List disk directory contents 
- echo comment - Display comment on screen followed by a new line (multiple spaces/tabs may be reduced to a single space); if no argument simply issue a new prompt.  
- help - Display the user manual lprint filename - Load filename into memory and print its contents (to the printer) using interrupt 
- setenv fg color - change foreground color 
- setenv bg color - tweet filename Create a text file. Prompt the user for a line of text (shorter than 140 characters). Store it in a buffer and write this buffer to a file called filename.type filename - - - Load filename into memory and display its contents onscreen using interrupt 33 calls. \0


## ACADEMIC INTEGRITY PLEDGE 
- I have not used source code obtained from another student nor any other unauthorized source, either modified or unmodified.
                                                                       
- All source code and documentation used in my program is either my original work or was derived by me from the source code published in the textbook for this course or presented in class.
                                                                       
- I have not discussed coding details about this project with anyone other than my instructor. I understand that I may discuss the concepts of this program with other students and that another student may help me debug my program so long as neither of us writes anything during the discussion or modifies any computer file during the discussion.
                                                                       
I have violated neither the spirit nor letter of these restrictions.

:_Sam Borick___ Date:_02/08/18___

3460:4/526 BlackDOS2020 kernel, Version 1.5, Spring 2018.