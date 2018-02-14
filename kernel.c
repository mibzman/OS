/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:_Sam Borick___ Date:_02/08/18____        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2018.             */

void handleInterrupt21(int,int,int,int);
void printString(char*,int);
void printChar(char*);
void readString(char[80]);
int readInt();
void printLogo();

void main()
{
   char input[80];
   int resultNumber;

   makeInterrupt21();
   printLogo();
   // interrupt(33,0,"Hello world from Sam Borick\r\n\0",1,0);
   printString("Please enter a command: \r\n\0" , 0);
   readString(input);
   printString("You wrote: \0" , 0);
   printString(input, 0);
   printString("\r\n\0", 0);


   printString("Please enter the number 4: \r\n\0" , 0);
   resultNumber = readInt();
   if (resultNumber == 4){
      printString("You wrote 4! good! \r\n\0" , 0);   
   } else {
      printString("You did not write 4! bad! \r\n\0" , 0);   
   }
   

   while(1);
}

void printString(char* c, int d)
{
   if (d == 1) {
      while(*c != '\0'){ 
         interrupt(23, *c, 0, 0, 0);
         c++;
      }
      return;
   }
   while(*c != '\0'){
      printChar(c);
      c++;
   }
   return;
}

void printChar(char* c)
{
   char al = *c;
   char ah = 14;
   int ax = ah * 256 + al;
   interrupt(16, ax, 0, 0, 0);
   return;
}

void printLogo()
{
   printString("       ___   `._   ____  _            _    _____   ____   _____ \r\n\0",0);
   printString("      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0",0);
   printString("     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0",0);
   printString("    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0",0);
   printString("   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0",0);
   printString("._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0",0);
   printString(" BlackNOS2020 v. 1.01, c. 2018. Based on a project by M. Black. \r\n\0",0);
   printString(" Author: Sam Borick.\r\n\r\n\0",0);
}

void readString(char line[80]) {
   int counter = 0;
   char* charHolder;
   char* realCharPointer;
   char realChar;
   do{
      charHolder = interrupt(22,0,0,0,0);
      realCharPointer = &charHolder;
      realChar = *realCharPointer;

      if (realChar == '\0'){
         continue;
      }
      else if (realChar == '\r'){
         line[counter] = '\0';
         counter++;
      } else if (realChar == '\b'){
         counter--;
      } else {
         line[counter] = realChar;
         counter++;
      }
      
      printChar(&charHolder);

   }while(realChar != '\r');
   printString("\n\0",0);
}

int readInt() {
   char input[80];
   int sign = 1;
   int counter = -1;
   int output = 0;

   readString(input);

   if (input[0] == '-') {
     sign = -1;
     counter = 0;
   }

   while (input[++counter] != '\0') { //iterate until the array end
     output = output*10 + (input[counter] - '0'); //generating the integer according to read parsed numbers.
   }

   output = output*sign;
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   switch(ax) {  
      case 0: printString(bx,cx); break;
      default: printString("General BlackDOS error.\r\n\0"); 
   }  
   return;
}
