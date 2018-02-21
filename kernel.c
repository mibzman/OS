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
void printLogo();
void printInt(int input, int mode);
void readString(char[80]);
void readInt(int* n);

void readSector(char*, int);
void writeSector(char*, int);

void clearScreen(char*, char*);

void playMadLibs();

int mod(int a, int b);
int div(int a, int b);

void main()
{
   char buffer[512]; int i;
   makeInterrupt21();
   for (i = 0; i < 512; i++) buffer[i] = 0;
   buffer[0] = 0;
   buffer[1] = 2;
   interrupt(33,6,buffer,258,0);
   interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
   printLogo();
   interrupt(33,2,buffer,30,0);
   interrupt(33,0,buffer,0,0);
   while (1) ;
}

void playMadLibs(){
   char food[25], adjective[25], color[25], animal[25];
   int temp;  

   interrupt(33,0,"\r\nWelcome to the Mad Libs kernel.\r\n\0",0,0);
   interrupt(33,0,"Enter a food: \0",0,0);
   interrupt(33,1,food,0,0);
   temp = 0;
   while ((temp < 100) || (temp > 120)) {
      interrupt(33,0,"Enter a number between 100 and 120: \0",0,0);
      interrupt(33,14,&temp,0,0);
   }
   interrupt(33,0,"Enter an adjective: \0",0,0);
   interrupt(33,1,adjective,0,0);
   interrupt(33,0,"Enter a color: \0",0,0);
   interrupt(33,1,color,0,0);
   interrupt(33,0,"Enter an animal: \0",0,0);
   interrupt(33,1,animal,0,0);
   interrupt(33,0,"Your note is on the printer, go get it.\r\n\0",0,0);
   interrupt(33,0,"Dear Professor O\'Neil,\r\n\0",1,0);
   interrupt(33,0,"\r\nI am so sorry that I am unable to turn in my program at this time.\r\n\0",1,0);
   interrupt(33,0,"First, I ate a rotten \0",1,0);
   interrupt(33,0,food,1,0);
   interrupt(33,0,", which made me turn \0",1,0);
   interrupt(33,0,color,1,0);
   interrupt(33,0," and extremely ill.\r\n\0",1,0);
   interrupt(33,0,"I came down with a fever of \0",1,0);
   interrupt(33,13,temp,1,0);
   interrupt(33,0,". Next my \0",1,0);
   interrupt(33,0,adjective,1,0);
   interrupt(33,0," pet \0",1,0);
   interrupt(33,0,animal,1,0);
   interrupt(33,0," must have\r\nsmelled the remains of the \0",1,0);
   interrupt(33,0,food,1,0);
   interrupt(33,0," on my computer, because he ate it. I am\r\n\0",1,0);
   interrupt(33,0,"currently rewriting the program and hope you will accept it late.\r\n\0",1,0);
   interrupt(33,0,"\r\nSincerely,\r\n\0",1,0);
   interrupt(33,0,"Sam Borick\r\n\0",1,0);
}

void readSector(char* buffer, int absSecNo) {
   int CX;
   int DX;

   int trackNo;
   int relSecNo;
   int headNo;

   relSecNo = mod(absSecNo, 18) + 1;
   headNo = mod(div(absSecNo, 18), 2);
   trackNo = div(absSecNo, 36);

   CX = trackNo * 256 + relSecNo;
   DX = headNo * 256;

   interrupt(19, 513, buffer, CX, DX);
}

void writeSector(char* buffer, int absSecNo) {
   int CX;
   int DX;

   int trackNo;
   int relSecNo;
   int headNo;

   relSecNo = mod(absSecNo, 18) + 1;
   headNo = mod(div(absSecNo, 18), 2);
   trackNo = div(absSecNo, 36);

   CX = trackNo * 256 + relSecNo;
   DX = headNo * 256;

   interrupt(16, 769, buffer, CX, DX);
}

void clearScreen(int bx, int cx) {
   int val = 4096 * (bx - 1) + 256 * (cx - 1);
   printString("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\0");
   interrupt(16,512,0,0,0);
   if (bx > 0 && cx > 0){
      interrupt(16, 1536, val, 0, 6223);
   }

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

void printInt(int input, int mode) {
   char result[80];
   int counter = 0;
   int temp;

   if (input == 0){
         printChar('0');
         return;
   }

   if (input < 0){
      result[0] = '-';
      input = input * -1;
      counter++;
   }

   //fill our array with null terminators so we don't have to do it later
   for(temp=input; temp > 0; temp = div(temp, 10), counter++){
      result[counter] = '\0';
   }
   result[counter] = '\0';
   result[counter+1] = '\0';
   //now our counter is at the 'end' of our string

   for (temp=input; temp > 0; temp = div(temp, 10)){
      result[--counter] = mod(temp, 10) + '0';
   }

   printString(result, mode);
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
      //doesn't return a char pointer pointing to a space in memory that contains the ASCII value (i.e. a char), 
      //it just returns the ASCII value typed incorrectly 
      charHolder = interrupt(22,0,0,0,0);
      realCharPointer = &charHolder;
      realChar = *realCharPointer;
      // Note that we pass a pointer to charHolder (a pointer pointer), 
      //which is then dereferenced and read as a char.

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

void readInt(int* output) {
   char input[80];
   int sign = 1;
   int counter = -1;
   int result = 0;

   readString(input);

   if (input[0] == '-') {
     sign = -1;
     counter = 0;
   }

   while (input[++counter] != '\0') { //iterate until the array end
     result = result*10 + (input[counter] - '0'); //generating the integer according to read parsed numbers.
   }

   result = result*sign;
   *output = result;
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   switch(ax) {  
      case 0: printString(bx,cx); break;
      case 1: readString(bx); break;
      case 2: readSector(bx,cx); break;
      case 6: writeSector(bx,cx); break;
      case 12: clearScreen(bx,cx); break;
      case 13: printInt(bx,cx); break;
      case 14: readInt(bx); break;
      default: printString("General BlackDOS error.\r\n\0"); 
   }  
   return;
}



int mod(int a, int b) {
 int x = a;
 while (x >= b) x = x - b;
 return x;
}

int div(int a, int b) {
 int q = 0;
 while (q * b <= a) q++;
 return (q - 1);
}