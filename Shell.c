#define Print(x)     interrupt(33,0,x,0,0)
#define Read(x,y)    interrupt(33,2,x,y,0)

int strCmp(char*, char*, int);
void dir();

void main(){
	char input[600];

	interrupt(33, 0, "blackdos ~(__^>\0", 0, 0);
	interrupt(33, 1, input, 0, 0);

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
	else if(strCmp(input, "echo", 4))
	{
		interrupt(33, 0, "\r\n\0", 0, 0);
		interrupt(33, 0, input + 5, 0, 0);
		interrupt(33, 0, "\r\n\0", 0, 0);
	}
	else if(strCmp(input, "help", 4))
	{
		interrupt(33, 0, "\r\nboot - \r\n Reboot the system \r\ncls - \r\n Clear the screen \r\ncopy file1 file2 - \r\n Create file2 and copy all bytes of file1 to file2 without deleting file1. \r\ndel filename - \r\n Delete the named file \r\ndir - \r\n List disk directory contents \r\necho comment - \r\n Display comment on screen followed by a new line (multiple spaces/tabs may be reduced to a single space); if no argument simply issue a new prompt.  \r\nhelp - \r\nDisplay the user manual \r\nlprint filename - \r\n Load filename into memory and print its contents (to the printer) using interrupt \r\nsetenv fg color - \r\n change foreground color \r\nsetenv bg color - \r\ntweet filename \r\nCreate a text file. Prompt the user for a line of text (shorter than 140 characters). Store it in a buffer and write this buffer to a file called filename.type filename - \r\nLoad filename into memory and display its contents onscreen using interrupt 33 calls. \r\n\0", 0, 0);
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
      for (i = 0; i < 6; ++i)
      {
         fname[i] = buffer[index + i];
      }
      for (i = 6; i<32; ++i)
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
      if(fname[0] >= 'A' && fname[0] <= 'Z')
      {
      }
      else
      {
         // Print("File name: \0");
         Print(fname);
         // Print("        File size: \0");
         // PRINTN(size);
         Print(" \0");
      }
      size = 0;
      runs = runs + 6;
      index = index + 32;
   }
   Print("\r\n\0");
} 