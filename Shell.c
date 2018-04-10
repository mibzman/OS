#define Print(x)     interrupt(33,0,x,0,0)
#define Read(x,y)    interrupt(33,2,x,y,0)

int strCmp(char*, char*, int);
void dir();
int isCapital(char* ch);
int getSpaceIndex(char* string);
int getLength(char* string);
void copyFile(char*, char* );

void main(){
	char buffer[13312];
	char tweet[280];
	char input[600];
	char arg1[600];
	char arg2[600];
	int i, index, length;

	interrupt(33, 0, "blackdos ~(__^>\0", 0, 0);
	interrupt(33, 1, input, 0, 0);

	Print("\r\n\0");
	if(strCmp(input, "boot", 4))
	{
		interrupt(33, 0, "\r\nbooting\0", 0, 0);
		interrupt(33, 11, 0, 0, 0);
	}
	else if(strCmp(input, "cls", 3))
	{
		interrupt(33,12,5,16,0);
	}
	else if(strCmp(input, "dir", 3))
	{
		dir();
	}
	else if(strCmp(input, "del", 3))
	{
		Print("deleting\r\n\0");
		if (isCapital(input[4]))
		{
			// interrupt(33, 15, 1, 0, 0);
			Print("cannot delete, protected\r\n\0");
			return;
		}
		interrupt(33, 7, input + 4, 0, 0);
		// Print("delete not implemented\r\n\0");
	}
	else if(strCmp(input, "echo", 4))
	{
		interrupt(33, 0, "\r\n\0", 0, 0);
		interrupt(33, 0, input + 5, 0, 0);
		interrupt(33, 0, "\r\n\0", 0, 0);
	}
	else if(strCmp(input, "copy", 4))
	{
		index = getSpaceIndex(input) + 1;
	    length = getLength(input);

	    if (isCapital(input[5] || isCapital(input[index])))
	    {
           interrupt(33, 15, 1, 0, 0);
		   return;
	    }
	    for (i = 5; i < index - 1; i++)
	    {
		   arg1[i - 5] = input[i];
	    }
	    for (i = index; i < length + 1; i++)
	    {
           arg2[i - index] = input[i];
	    }
	    arg1[6] = '\0';
	    arg2[6] = '\0';

       Print("arg1");
       Print(arg1);
       Print("arg2");
       Print(arg2);
        copyFile(arg1, arg2);
	}
	else if(strCmp(input, "help", 4))
	{
		interrupt(33, 0, "\r\nboot - \r\n Reboot the system \r\ncls - \r\n Clear the screen \r\ncopy file1 file2 - \r\n Create file2 and copy all bytes of file1 to file2 without deleting file1. \r\ndel filename - \r\n Delete the named file \r\ndir - \r\n List disk directory contents \r\necho comment - \r\n Display comment on screen followed by a new line (multiple spaces/tabs may be reduced to a single space); if no argument simply issue a new prompt.  \r\nhelp - \r\nDisplay the user manual \r\nlprint filename - \r\n Load filename into memory and print its contents (to the printer) using interrupt \r\nsetenv fg color - \r\n change foreground color \r\nsetenv bg color - \r\ntweet filename \r\nCreate a text file. Prompt the user for a line of text (shorter than 140 characters). Store it in a buffer and write this buffer to a file called filename.type filename - \r\nLoad filename into memory and display its contents onscreen using interrupt 33 calls. \r\n\0", 0, 0);
	}
	else if (strCmp(input, "type", 4))
	{
		interrupt(33, 3, input + 5, buffer, 0);
		Print(buffer);
		Print("\r\n\0");
	}
	else if (strCmp(input, "lprint", 5))
	{
		interrupt(33, 3, input + 5, buffer, 1);
		Print(buffer);
		Print("\r\n\0");
	}
	else if (strCmp(input, "tweet", 5))
	{
		if (isCapital(input[6]))
		{
			interrupt(33, 15, 1, 0, 0);
			return;
		}
		Print("Enter your tweet: \0");
		interrupt(33,1,tweet,0,0);
		interrupt(33,8, input + 6, tweet, 1);
		Print("\r\n\0");
	}
	else if (strCmp(input, "setenv fg", 9))
	{
		// Print(input +10);
		Print("\r\n\0");
		interrupt(33,12,input +10,0,0);
	}
	else if (strCmp(input, "setenv bg", 9))
	{
		// Print(input +10);
		Print("\r\n\0");
		interrupt(33,12,0,input +10,0);
	}
	else if (strCmp(input, "run", 3))
	{
		interrupt(33,4,input+4,2,0);
	}
	else if(strCmp(input, "", 1))
	{
		return;
	}
	else {
		interrupt(33, 0, "\r\nbad command or file name\r\n\0", 0, 0);
	}
}


int strCmp(char* a, char* b, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (a[i] != b[i])
		{
			return 0;
		}
	}
	return 1;
}

void dir()
{
   char buffer[512];
   char fname[512];
   int index = 0;
   int i;
   int runs = 0;
   int size = 0;
   int total = 0;

   Read(buffer, 257);
   Print("\r\n\0");
   while(buffer[index] != 0x0)
   {
      for (i = 0; i < 8; ++i)
      {
         fname[i] = buffer[index + i];
      }
      for (i = 8; i<32; ++i)
      {
         if (buffer[index + i] == 0x0)
         {
            break;
         }
         else
         {
            ++size;
         }
      }
      total = total + size;
      if(isCapital(fname[0]))
	  {
	  }
	  else
      {
         Print(fname);
         Print(" \0");
      }
      size = 0;
      runs = runs + 8;
      index = index + 32;
   }
   Print("\r\n\0");
}

int isCapital(char* ch)
{
	if (ch >= 'A' && ch <= 'Z')
	{
		return 1;
	}
	else return 0;
}

int getSpaceIndex(char* string)
{
	int i = 5;
	while (string[i] != ' ')
	{
		i++;
	}
	return i;
}

int getLength(char* string)
{
	int i = 0;
	while (string[i] != 0)
	{
		i++;
	}
	return i;
}

void copyFile(char* fileName1, char* fileName2)
{
	char buffer[13312];
	char directory[512];
	int i, j, k, filestart, fileend, numOfSectors = 0;
	char currentFile[6];

	interrupt(33, 2, directory, 2, 0);

	for (i = 0; i < 16; i++)
	{
		filestart = i * 32;
		fileend = filestart + 6;
		for (j = 0; j < 6; j++)
		{
			currentFile[j] = directory[filestart + j];
		}
		if (strCmp(currentFile, fileName1, 6))
		{
			for (k = fileend; directory[k] != 0x00; k++)
			{
				numOfSectors++;
			}
		}
	}
	interrupt(33, 3, fileName1, buffer);
	interrupt(33, 8, fileName2, buffer, numOfSectors);
}
