// for academic integrity agreement, see README.md         

void handleInterrupt21(int, int, int, int);

void error(int);

//disk
void readSector(char * , int);
void writeSector(char * , int);
int findEmptySector(char * );

//file
void readFile(char * , char * , int * );
void writeFile(char * , char * , int);
void deleteFile(char * );

//screen management
void printString(char * , int);
void printChar(char * );
void printInt(int, int);

void readString(char[80]);
void readInt(int * );

void clearScreen(char * , char * );
void printLogo();

//games
void playMadLibs();

//util
char strnCpy(char,
  const char, int);
int strCmp(char, char);
int mod(int a, int b);
int div(int a, int b);

void main() {
  char buffer[12288];
  int size;
  char buffer2[12288];
  makeInterrupt21();

  /* Step 0 – config file */
  interrupt(33, 2, buffer, 258, 0);
  interrupt(33, 12, buffer[0] + 1, buffer[1] + 1, 0);
  printLogo();

  /* Step 1 – load/edit/print file */
  interrupt(33, 3, "spc02\0", buffer2, & size);
  buffer2[7] = '2';
  buffer2[8] = '0';
  buffer2[9] = '1';
  buffer2[10] = '8';
  // printString("printing first time\r\n\0",0);
  // interrupt(33,0,"printing first time\r\n\0",0,0);
  interrupt(33, 0, buffer2, 0, 0);

  /* Step 2 – write revised file */
  interrupt(33, 8, "spr18\0", buffer, size);
  /* Step 3 – delete original file */
  interrupt(33, 7, "spc02\0", 0, 0);

  while (1);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  switch (ax) {
  case 0:
    printString(bx, cx);
    break;
  case 1:
    readString(bx);
    break;
  case 2:
    readSector(bx, cx);
    break;
  case 3:
    readFile(bx, cx, dx);
    break;
  case 6:
    writeSector(bx, cx);
    break;
    // case 7: deleteFile(bx); break;
  case 8:
    writeFile(bx, cx, dx);
    break;
  case 12:
    clearScreen(bx, cx);
    break;
  case 13:
    printInt(bx, cx);
    break;
  case 14:
    readInt(bx);
    break;
  case 15:
    error(bx);
    break;
  default:
    printString("General BlackDOS error.\r\n\0", 0);
  }
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
  while (1);
}

void readSector(char * buffer, int absSecNo) {
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

int findEmptySector(char * map) {
  int counter = 0;
  for (counter; counter < 512; counter = counter + 32) {
    if (map[counter] == 0) {
      return counter;
    }
  }
  return 600;
}

void readFile(char * fname, char * buffer, int * size) {
  char dir[512];
  int i = 0, bufferCounter, counter; * size = 0;

  readSector(dir, 257);

  for (; i < 16; i++) {
    if (strCmp(fname, & dir[bufferCounter])) {

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

void writeFile(char * name, char * buffer, int numberOfSectors) {
  char map[512], disk[512];
  int freeDir = 0;
  int freeSector = 0;
  int diskByte = 0;
  int i = 0;
  int y = 8;

  readSector(map, 256);
  readSector(disk, 257);

  while (diskByte < 512) {
    if (disk[diskByte] != 0x0 && strCmp(name, & disk[diskByte])) {
      error(1);
      return;
    }

    if (freeDir == 0 && disk[diskByte] == 0x0){
      freeDir = diskByte;
    }


    diskByte = diskByte + 32;
  }

  if (freeDir == 0) {
    error(3);
    return;
  }

  //fill with zeros
  for (i = 0; i < 32; ++i) {
    disk[freeDir + i] = 0x0;
  }

  for (i = 0; i < y; ++i) {
    if ( * (name + i) == 0x0) {
      break;
    }

    disk[freeDir + i] = * (name + i);
  }

  for (i = 0; i < numberOfSectors; ++i) {
    for (y = 0; y < 256; ++y) {
      if (map[y] == 0x0) {
        map[y] = 255;
        disk[freeDir + 8 + i] = y;
        
        writeSector(buffer, y);
        buffer = buffer + 512;

        //breaks us out of inner loop
        y = 512;
      }
    }
  }

  writeSector(map, 256);
  writeSector(disk, 257);

  return;
}

void deleteFile(char * name) {
  char map[512], disk[512];
  char * current;
  int found = 0, i;

  /* load disk directory and map to 512 byte character arrays */
  readSector(map, 256);
  readSector(disk, 257);

  current = disk;

  /* search through the directory and try to find the file name */
  for (i = 0; i < 16; i++) {
    if (strCmp(name, current)) {
      found = 1;

      /* set the first byte of file to zero */
      * current = 0x0;
      current += 6;

      /* step through sector numbers belonging to file and set map byte to zero */
      while ( * current != 0x0) {
        map[ * current] = 0x0;
        current++;
      }
    }

    current += 32;
  }

  if (!found) {
    error(0);
  } else printString("DELETED! :)\r\n\0");

  /* write the char arrays holding the directory and map back to sectors */
  writeSector(map, 256);
  writeSector(disk, 257);
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
  while ( * c != '\0') {
    printChar(c);
    c++;
  }
  return;
}

void printChar(char * c) {
  char al = * c;
  char ah = 14;
  int ax = ah * 256 + al;
  interrupt(16, ax, 0, 0, 0);
  return;
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

void clearScreen(int bx, int cx) {
  int val = 4096 * (bx - 1) + 256 * (cx - 1);
  printString("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\0");
  interrupt(16, 512, 0, 0, 0);
  if (bx > 0 && cx > 0) {
    interrupt(16, 1536, val, 0, 6223);
  }

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

int strCmp(char a[], char b[]) {
  int i = 0;
  for (i;; i++) {
    if (a[i] != b[i]) {
      return a[i] < b[i] ? -1 : 1;
    }

    if (a[i] == '\0') {
      return 0;
    }
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