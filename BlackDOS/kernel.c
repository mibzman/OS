﻿// for academic integrity agreement, see README.md
#include "blackdos.h"

void handleInterrupt21(int, int, int, int);
void printLogo();
char strnCpy(char, char, int);

void main() {
  char buffer[512];
  makeInterrupt21();
  interrupt(33,2,buffer,258,0);
  interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
  printLogo();
  // deleteFile("spc02");
  interrupt(33,4,"Shell\0",2,0);
  interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);
  while (1) ;
}

void printChar(char * c) {
  char al = * c;
  char ah = 14;
  int ax = ah * 256 + al;
  interrupt(16, ax, 0, 0, 0);
  return;
}

void printString(char * c, int d) {
  char temp;
  if (d == 1) {
    while ( * c != '\0') {
      temp = * c;
      interrupt(23, temp, 0, 0, 0);
      c++;
    }
    return;
  }
  //screen
  while ( * c != '\0') {
    printChar(c);
    c++;
  }
  return;
}

void readString(char line[80]) {
  int counter = 0;
  char * charHolder;
  char * realCharPointer;
  char realChar;
  do {
    //doesn't return a char pointer pointing to a space in memory that contains the ASCII value (i.e. a char),
    //it just returns the ASCII value typed incorrectly
    charHolder = interrupt(22, 0, 0, 0, 0);
    realCharPointer = & charHolder;
    realChar = * realCharPointer;
    // Note that we pass a pointer to charHolder (a pointer pointer),
    //which is then dereferenced and read as a char.

    if (realChar == '\0') {
      continue;
    } else if (realChar == '\r') {
      line[counter] = '\0';
      counter++;
    } else if (realChar == '\b') {
      counter--;
    } else {
      line[counter] = realChar;
      counter++;
    }

    printChar( & charHolder);

  } while (realChar != '\r');
  printString("\n\0", 0);
}

void clearScreen(int bx, int cx) {
  int val = 4096 * (bx - 1) + 256 * (cx - 1);
  printString("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\0");
  interrupt(16, 512, 0, 0, 0);
  if (bx > 0 && cx > 0) {
    interrupt(16, 1536, val, 0, 6223);
  }

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

void printInt(int input, int mode) {
  char result[80];
  int counter = 0;
  int temp;

  if (input == 0) {
    printChar('0');
    return;
  }

  if (input < 0) {
    result[0] = '-';
    input = input * -1;
    counter++;
  }

  //fill our array with null terminators so we don't have to do it later
  for (temp = input; temp > 0; temp = div(temp, 10), counter++) {
    result[counter] = '\0';
  }
  result[counter] = '\0';
  result[counter + 1] = '\0';
  //now our counter is at the 'end' of our string

  for (temp = input; temp > 0; temp = div(temp, 10)) {
    result[--counter] = mod(temp, 10) + '0';
  }

  printString(result, mode);
}

void readInt(int * output) {
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
    result = result * 10 + (input[counter] - '0'); //generating the integer according to read parsed numbers.
  }

  result = result * sign; * output = result;
}


void readSector(char* buffer, int sector)
{
   int ah = 2, /* tells the BIOS to read a sector */
   al = 1, /* number of sectors to read */
   ch = div(sector,36), /* track number, trackNo */
   cl = mod(sector,18) + 1, /* relative sector number, relSecNo */
   dh = mod(div(sector,18),2), /* head number, headNo */
   dl = 0, /* device number for the floppy disk */

   ax = ah * 256 + al,
   cx = ch * 256 + cl,
   dx = dh * 256 + dl;

   interrupt(0x13, ax, buffer, cx, dx);
   return;
}

void error(int bx) {
  switch (bx) {
  case 0:
    printString("File not found.\r\n\0", 0);
    break;
  case 1:
    printString("Bad file name.\r\n\0", 0);
    break;
  case 2:
    printString("Disk full.\r\n\0", 0);
    break;
  case 3:
    printString("Debugging Breakpoint hit.\r\n\0", 0);
    break;
  default:
    printString("General Error.\r\n\0", 0);
  }
  // while (1);
}

int strCmp(char a[], char b[]) {
  int i = 0;
  for (i;; i++) {
    if (a[i] != b[i]) {
      return 1;
    }

    if (a[i] == '\0') {
      return 0;
    }
  }
}

void readFile(char * fname, char * buffer, int * size) {
  char dir[512];
  int i = 0, bufferCounter, counter; * size = 0;

  readSector(dir, 257);

  for (; i < 16; i++) {
    if (!strCmp(fname, & dir[bufferCounter])) {

      counter = bufferCounter + 8;

      while (dir[counter] != 0) {
        readSector(buffer, dir[counter]);
        buffer += 512; * size += 512;
        counter++;
      }
      return;
    }
    bufferCounter += 32;
  }

  error(0);
  return;
}

void runProgram(char* name, int segment) {
  char buffer[12288];
  int temp = 0;

  printString("reading file\r\n", 0);
  readFile(name, buffer);
  segment *= 4096;

  for (temp; temp < 12288; temp++) {
    putInMemory(segment, temp, buffer[temp]);
  }

  printString("launching program\r\n", 0);
  launchProgram(segment);
}

void stop() {
  launchProgram(8192);
}

void writeSector(char * buffer, int absSecNo) {
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

  interrupt(19, 769, buffer, CX, DX);
}

void deleteFile(char * name) {
  char map[512], disk[512];
  char *current;
  int found = 0;
  int i;

  PRINTS("deleting from kernel\r\n\0");

  readSector(map, 256);
  readSector(disk, 257);
  //
  // interrupt(33, 0, name, 0, 0);
  // interrupt(33, 0, "\r\n\0", 0, 0);
  //
  // current = disk;
  //
  // for (i = 0; i < 16; i++) {
  //   // interrupt(33, 0, "looping \r\n\0", 0, 0);
  //   if (strCmp(name, current)) {
  //     // interrupt(33, 0, "looping2 \r\n\0", 0, 0);
  //     found = 1;
  //
  //     *current = 0x0;
  //     current += 8;
  //
  //     while (*current != 0x0) {
  //       map[*current] = 0x0;
  //       current++;
  //       // interrupt(33, 0, "zeroing \r\n\0", 0, 0);
  //     }
  //   }
  //
  //   current += 32;
  // }
  //
  // if (!found) {
  //   error(0);
  //   return;
  // }
  //
  // writeSector(map, 256);
  // writeSector(disk, 257);
}

void writeFile(char* name, char* buffer, int numberOfSectors) {
   char fileDir[512];
   char map[512];
   char filenameHolder[6];
   char emptyBuffer[512];
   char tempBuffer[512];
   int counter = 0;

   int diridx = 600;
   int diridxCounter;

   int tempidx = 600;

   readSector(map, 256);
   readSector(fileDir, 257);

   for (counter; counter < 512; counter = counter + 32) {
      if (strCmp(name, fileDir + counter) == 0){
         error(1);
         return;
      }
   }

   diridx = findEmptySector(map);

   if (diridx == 600) {
      error(2);
      return;
   }

   //this should take care of the padding
   strnCpy(filenameHolder, name, 6);
   //insert name
   map[diridx] = 255;
   strnCpy(fileDir + diridx, filenameHolder, 6);
   diridxCounter = diridx + 8;

   counter = 0;
   for (counter; counter < numberOfSectors; counter++) {
      strnCpy(tempBuffer, emptyBuffer, 512);

      tempidx = findEmptySector(map);

      if (tempidx == 600) {
         error(2);
         return;
      }

      map[counter] = 255;
      fileDir[diridxCounter] = counter;

      //write sector will get the length right
      strnCpy(tempBuffer, buffer + (counter * 512), 512);
      writeSector(tempBuffer, counter);

      tempidx = 600;
   }

   for (diridxCounter; diridxCounter < diridx + 32; diridxCounter++ ){
      fileDir[diridxCounter] = 0;
   }

   writeSector(map, 256);
   writeSector(fileDir, 257);
}


void handleInterrupt21(int ax, int bx, int cx, int dx) {
  switch (ax) {
  case 0: printString(bx, cx); break;
  case 1: readString(bx); break;
  case 2: readSector(bx, cx); break;
  case 3: readFile(bx, cx, dx); break;
  case 4: runProgram(bx, cx); break;
  case 5: stop(); break;
  case 6: writeSector(bx, cx); break;
  case 7: deleteFile(bx);  break;
  case 8: writeFile(bx, cx, dx); break;
  case 11: interrupt(25,0,0,0,0);
  case 12: clearScreen(bx, cx); break;
  case 13: printInt(bx, cx); break;
  case 14: readInt(bx); break;
  case 15: error(bx); break;
  default:
    printString("General BlackDOS error.\r\n\0", 0);
  }
  return;
}



int findEmptySector(char * map) {
  int counter = 0;
  for (counter; counter < 512; counter = counter + 32) {
    if (map[counter] == 0) {
      return counter;
    }
  }
  return 600;
}

void playMadLibs() {
  char food[25], adjective[25], color[25], animal[25];
  int temp;

  interrupt(33, 0, "\r\nWelcome to the Mad Libs kernel.\r\n\0", 0, 0);
  interrupt(33, 0, "Enter a food: \0", 0, 0);
  interrupt(33, 1, food, 0, 0);
  temp = 0;
  while ((temp < 100) || (temp > 120)) {
    interrupt(33, 0, "Enter a number between 100 and 120: \0", 0, 0);
    interrupt(33, 14, & temp, 0, 0);
  }
  interrupt(33, 0, "Enter an adjective: \0", 0, 0);
  interrupt(33, 1, adjective, 0, 0);
  interrupt(33, 0, "Enter a color: \0", 0, 0);
  interrupt(33, 1, color, 0, 0);
  interrupt(33, 0, "Enter an animal: \0", 0, 0);
  interrupt(33, 1, animal, 0, 0);
  interrupt(33, 0, "Your note is on the printer, go get it.\r\n\0", 0, 0);
  interrupt(33, 0, "Dear Professor O\'Neil,\r\n\0", 1, 0);
  interrupt(33, 0, "\r\nI am so sorry that I am unable to turn in my program at this time.\r\n\0", 1, 0);
  interrupt(33, 0, "First, I ate a rotten \0", 1, 0);
  interrupt(33, 0, food, 1, 0);
  interrupt(33, 0, ", which made me turn \0", 1, 0);
  interrupt(33, 0, color, 1, 0);
  interrupt(33, 0, " and extremely ill.\r\n\0", 1, 0);
  interrupt(33, 0, "I came down with a fever of \0", 1, 0);
  interrupt(33, 13, temp, 1, 0);
  interrupt(33, 0, ". Next my \0", 1, 0);
  interrupt(33, 0, adjective, 1, 0);
  interrupt(33, 0, " pet \0", 1, 0);
  interrupt(33, 0, animal, 1, 0);
  interrupt(33, 0, " must have\r\nsmelled the remains of the \0", 1, 0);
  interrupt(33, 0, food, 1, 0);
  interrupt(33, 0, " on my computer, because he ate it. I am\r\n\0", 1, 0);
  interrupt(33, 0, "currently rewriting the program and hope you will accept it late.\r\n\0", 1, 0);
  interrupt(33, 0, "\r\nSincerely,\r\n\0", 1, 0);
  interrupt(33, 0, "Sam Borick\r\n\0", 1, 0);
}

char strnCpy(char * dst, char * src, int n) {
  int i;
  char * temp;
  temp = dst;
  for (i = 0; i < n; i++)
    * dst++ = * src++;
  return temp;
}

void printLogo() {
  printString("       ___   `._   ____  _            _    _____   ____   _____ \r\n\0", 0);
  printString("      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0", 0);
  printString("     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0", 0);
  printString("    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0", 0);
  printString("   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0", 0);
  printString("._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0", 0);
  printString(" BlackNOS2020 v. 1.01, c. 2018. Based on a project by M. Black. \r\n\0", 0);
  printString(" Author: Sam Borick.\r\n\r\n\0", 0);
}
