void main(){
	char input[25];

	interrupt(33, 0, "blackdos ~(__^>\0", 0, 0);
	interrupt(33, 1, input, 0, 0);

	interrupt(33, 0, "\r\nYou entered: \0", 0, 0);
	interrupt(33, 0, input, 0, 0);
	interrupt(33, 0, "\r\n\0", 0, 0);

	// while (1) ;
} 