/*
    Team No. : 15

        Name                    Roll No.
    Joydeep Saha        -   002011001103
    Anil Munda          -   002011001110
    Shivam Das          -   002011001058
    Chiranjit Sarkar    -   002011001104

    Date : 13/08/2022
*/  

/*
ASSIGNMENT DETAILS :-
A big file (about 8GB) should be created using the ‘fallocate’ command. This big file
should be written with a single integer value (say X) at a specific offset (say F). Both
the integer value and the offset should be generated using a random function.
Please do remember this random function should generate a quantity anywhere
between 0 and 8G.
The above big file should also be mapped in the virtual address space using mmap()
call. Once it is mapped, the data should be read from the same specific offset (F).
Now, if the data read is X`; then verify that X and X` are the same. In case of
verification failure, an error message is to be printed. Note that, the offset value F
can be anywhere between 0 and 8G.
This sequence of writing and reading data to/from a specific offset and also
verification should be put in a while loop to go forever.
In another terminal execute the command ‘sar –B 1 1000’ to observe for the page
fault. This command should be started before the above program is put under
execution. So, one can observe that the page faults are increasing, once the above
program starts executing.
The output of the program and the ‘sar’ command should be pasted as a comment
at the beginning of the program file as indicated by the guidelines.

8GB file created using command -->  fallocate -l 8G file.txt

INPUT DESCRIPTION :-
file path is taken as input from the command line

OUTPUT DESCRIPTION :-
page fault is displayed

*/

/*

COMPILATION COMMAND :-
 gcc 15_4.c -o 15_4

EXECUTION SEQUENCE :-
./15_4 file.txt
*/

/*
SAMPLE INPUT(taken from the command line) :-
file.txt

SAMPLE OUTPUT :-
Entering infinite while loop !!!

Data written into file = 414428720 , Offset = 1645905170
Data read from mapped file = 414428720, Offset = 1645905170
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Data written into file = 1691809932 , Offset = 4607152981
Data read from mapped file = 1691809932, Offset = 4607152981
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Data written into file = 7510049335 , Offset = 7605042773
Data read from mapped file = 7510049335, Offset = 7605042773
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Data written into file = 4789519391 , Offset = 3779819782
Data read from mapped file = 4789519391, Offset = 3779819782
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Data written into file = 5289616302 , Offset = 7085639279
Data read from mapped file = 5289616302, Offset = 7085639279
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Data written into file = 5397263171 , Offset = 8482706088
Data read from mapped file = 5397263171, Offset = 8482706088
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Data written into file = 5943615852 , Offset = 2354743973
Data read from mapped file = 5943615852, Offset = 2354743973
Data written into the file and data read from the mapped file are identical....
Therefore, successfully verified...

Sample Output using sar -B 1 1000 command to display page fault.

16:57:25     pgpgin/s pgpgout/s   fault/s  majflt/s  pgfree/s pgscank/s pgscand/s pgsteal/s    %vmeff
	16:57:26         0.00      0.00      6.00      0.00     32.00      0.00      0.00      0.00      0.00
	16:57:27         0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
	16:57:28         0.00      0.00      0.00      0.00      1.00      0.00      0.00      0.00      0.00
	16:57:29         0.00      0.00    102.00      0.00     51.00      0.00      0.00      0.00      0.00
	16:57:30         0.00   4344.00      0.00      0.00     98.00      0.00      0.00      0.00      0.00
	16:57:31       236.00      0.00     75.00      0.00    197.00      0.00      0.00      0.00      0.00
	16:57:32       324.00      0.00     89.00      0.00    255.00      0.00      0.00      0.00      0.00
	16:57:33       356.00      0.00     98.00      0.00    284.00      0.00      0.00      0.00      0.00
	16:57:34       320.00      0.00     94.00      0.00    272.00      0.00      0.00      0.00      0.00
	16:57:35       276.00   2904.00     92.00      0.00    304.00      0.00      0.00      0.00      0.00
	16:57:36       364.00      0.00    120.00      0.00    342.00      0.00      0.00      0.00      0.00
	16:57:37       360.00      0.00    116.00      0.00    344.00      0.00      0.00      0.00      0.00
	16:57:38       356.00      0.00    116.00      0.00    317.00      0.00      0.00      0.00      0.00
	16:57:39       340.00      0.00    110.00      0.00    315.00      0.00      0.00      0.00      0.00
	16:57:40       336.00     12.00    113.00      0.00    331.00      0.00      0.00      0.00      0.00
	16:57:41       252.00      0.00     80.00      0.00    233.00      0.00      0.00      0.00      0.00
	16:57:42       332.00      0.00    107.00      0.00    301.00      0.00      0.00      0.00      0.00
	16:57:43       348.00      0.00    124.00      0.00    347.00      0.00      0.00      0.00      0.00
	16:57:44       360.00      0.00    120.00      0.00    372.00      0.00      0.00      0.00      0.00
	16:57:45       328.00     12.00    115.00      0.00    333.00      0.00      0.00      0.00      0.00
	16:57:46       328.00      0.00    125.00      0.00    377.00      0.00      0.00      0.00      0.00
	16:57:47       352.00      0.00    137.00      0.00    374.00      0.00      0.00      0.00      0.00
	16:57:48       324.00      0.00    119.00      0.00    337.00      0.00      0.00      0.00      0.00
	16:57:49       332.00      0.00    155.00      0.00    445.00      0.00      0.00      0.00      0.00
	16:57:50       292.00     12.00    117.00      0.00    343.00      0.00      0.00      0.00      0.00
	16:57:51       292.00      0.00     96.00      0.00    273.00      0.00      0.00      0.00      0.00
	^C

	Average:       261.85    280.15     93.31      0.00    264.54      0.00      0.00      0.00      0.00
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
#include<string.h>
#include<time.h>
#include<sys/random.h>

/* random_number_generator function generates random number between 0 and fileSize*/
long long int generateRandomNum(long long int fileSize) {
	long long int randomVal;

	/* Using getrandom() function to generate a random number */
	/* if getrandom() throws error ,  it returns -1 as as GRND_NONBLOCK flag is set */
	if (getrandom(&randomVal, sizeof(long long int), GRND_NONBLOCK) == -1) {
		perror("Could not generate a random number...\n");
		exit(1);
	}

	/* if getrandom() genrates negative numbers , making it positive */
	if (randomVal < 0)
		randomVal = randomVal * -1;

	/* Generating numbers in range of 0 to File size .*/
	randomVal = randomVal % fileSize;

	return randomVal;
}

/* Driver function */
int main(int argc, char *argv[argc + 1]) {

    //checking if the file is inputted
	if (argc != 2) {
		printf("Provide the path of the file which is to be transfered, as argument...\n");
		exit(1);
	}

	char *filePath = argv[1];

	/* Defining a stat structure variable */
	struct stat sb;

	if (stat(argv[1], &sb) == -1) {
		perror("Could not get file information.");
		exit(1);
	}

	/* Gettting File Size using sb variable from stat stucture */
	long long int fileSize = sb.st_size;

	/* Warning about infinite loop execution */
	printf("Entering infinite while loop !!!\n\n");
	sleep(2);

	/* infinite while loop */
	while (1) {
		/* file descriptor */
		int fd;

		/*opening file*/
		fd = open(filePath, O_RDWR);

		/* file open error */
		if (fd == -1) {
			perror("Could not open file....\n");
			exit(1);
		}

		/* generating random number for offset */
		long long int offset = generateRandomNum(fileSize);

		/*generating random data to be written at specific offset */
		long long int X = generateRandomNum(fileSize);

		/*setting cursor to offset position using lseek function */
		long long int set_offset = lseek(fd, offset, SEEK_SET);

		/* if lseek() throws error it returns -1 */
		if (set_offset < 0) {
			perror("lseek:");
			close(fd);
			exit(1);
		}

		char str[20];

		/*Converting long long int data to string */
		sprintf(str, "%lld", X);

		/* In case offset + length of data to be written causes overflow condition */
		if ((fileSize - offset) < (strlen(str) + 1)) {
			offset = offset - (strlen(str) + 1);
		}

		/*Writing random generated data at specific offset */
		set_offset = write(fd, str, sizeof(str));

		/* writing in file error */
		if (set_offset < 0) {
			perror("Unable to write in file");
			close(fd);
			exit(1);
		}
		else {
			printf("Data written into file = %lld , Offset = %lld ", X, offset);
		}

		/* Mapping file in the virtual address using mmap() */
		char *fileInMemory = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

		/* In case mapping fails MAP_FAILED is returned */
		if (fileInMemory == MAP_FAILED) {
			perror("Mapping Error !!!");
			exit(1);
		}

		/* closing file , after mapping successfully file can be closed */
		close(fd);

		/*verifying data written in file earlier and value read from file now */
		printf("\nData read from mapped file = ");
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (fileInMemory[offset + i] == str[i]) {
				printf("%c", fileInMemory[offset + i]);
			}
			else {
				perror(" Verfication Failure...\n");
			}
		}
		printf(", Offset = %lld \n", offset);
		printf("Data written into the file and data read from the mapped file are identical....\n");
        printf("Therefore, successfully verified...\n\n");

		/* Unmapping file */
		int unmapFile = munmap(fileInMemory, sb.st_size);
		if (unmapFile < 0) {
			perror("Unmapping error !!!");
			exit(1);
		}
	}
	return 0;
}
