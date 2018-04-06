int strCmp(char*, char*, int);

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

	// interrupt(33, 0, "\r\nYou entered: \0", 0, 0);
	// interrupt(33, 0, input, 0, 0);
	// interrupt(33, 0, "\r\n\0", 0, 0);

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