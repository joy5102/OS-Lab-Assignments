/*
    Team No. : 15

        Name               Roll No.     
    Joydeep Saha      -  002011001103
    Anil Munda        -  002011001110
    Shivam Das        -  002011001058
    Chiranjit Sarkar  -  002011001104

    Date : 6/8/22

*/

/*

ASSIGNMENT DETAILS :-
Create two processes and transfer 1GB file from process1 to process2 using a FIFO. Now transfer the same file from 
process2 to process1 using another FIFO. Now compare the two files to make sure that the same file has returned back.
Print the time required to do this double transfer. 

INPUT DESCRIPTION :-
Path of the 1GB file is taken as input from the command line.

OUTPUT DESCRIPTION :-
The success status of successful double transfer is displayed.
Confirmation of the sent file is being same as the received file is displayed.
The time taken to do the double transfer of the file is displayed.

*/

/*

COMPILATION COMMAND :-
gcc 15_2B.c -o 15_2B

EXECUTION SEQUENCE :-
./15_2B testfile.txt

*/

/*
SAMPLE INPUT (taken from the command line) :-
testfile.txt

SAMPLE OUTPUT :-
Successful transfer of file from Process1 to Process2, and back from Process2 to Process1...
The received file is same as the sent file...
Time required to do the double file transfer : 300.108193 sec

*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<fcntl.h>

#define FIFO1 "myfifo1"   //naming FIFO1
#define FIFO2 "myfifo2"   //naming FIFO2

#define BUFFER_SIZE 1024  //Buffer size

#define FILE1_BUFF "buffer_process1"  // the file received by process1 is stored temporarily in buffer
#define FILE2_BUFF "buffer_process2"  // the file received by process2 is stored temporarily in buffer

char buffer1[BUFFER_SIZE + 1];
char buffer2[BUFFER_SIZE + 1];


//Comparing the contents of the original file with the contents of the received file(via file transfer).
bool compareFiles(char *file1, char * file2) {
    int fd1 = open(file1, O_RDONLY); //opening the file for reading only
    if (fd1 == -1) {
        perror("Unable to open file.");
        exit(EXIT_FAILURE);
    }

    int fd2 = open(file2, O_RDONLY); //opening the file for reading only
    if (fd2 == -1) {
        perror("Unable to open file..");
        exit(EXIT_FAILURE);
    }

    //checking buffer by buffer
    while (1) {
        int length1 = read(fd1, buffer1, BUFFER_SIZE);
        int length2 = read(fd2, buffer2, BUFFER_SIZE);

        if (length1 != length2) {
            return false;   //contents of the file has been corrupted
        }

        if (length1 == -1) {
            close(fd1);
            close(fd2);
            perror("Unable to Read the file !!");
            exit(EXIT_FAILURE);
        }

        else if(length1 == 0) {
            // reached end of the file
            return true;
        }

        else if(length1 > 0) {
            for (int i=0; i<length1; i++) {
                if(buffer1[length1] != buffer2[length2]) 
                    return false;
            }
        }
    }
    return true;   //file contents are intact and are not corrupted.
}

//Implementing the file transfer.
void transferFile(char *file1, char *file2, char *msg) {

    int fd1 = open(file1, O_RDONLY); //opening the file for reading only

    //checking if we can open the file
    if(fd1 == -1) {
        perror("Unable to open the file..");
        exit(EXIT_FAILURE);
    }

    int fd2 = open(file2, O_WRONLY | O_CREAT, 0666); //0666 --> Permission to read and write for everyone.

    //checking if we can open the file
    if(fd2 == -1) {
        perror("Unable to open the file..");
        exit(EXIT_FAILURE);
    }

    while(1) {
        //reading from FIFO
        int length = read(fd1, buffer1, BUFFER_SIZE);

        if (length == -1) {
            close(fd1);
            close(fd2);
            perror("Unable to read the file");
            exit(EXIT_FAILURE);
        } 
        else if(length == 0) {  // reached end of file
            break;
        }
        else if(length > 0) {
            write(fd2, buffer1, length); 
        }
    }
    //closing the file
    close(fd1);
    close(fd2);
}

//This function is called from process1
void process1(char *filePath) {

    struct timeval startingTime, endingTime; //Record the time taken for the file transfer.

    gettimeofday(&startingTime, NULL);  //getting the starting time
    
    // Transferring the file using FIFO1
    transferFile(filePath, FIFO1, "p1");

    
    transferFile(FIFO2, FILE1_BUFF, "p1"); // get file from process1 through FIFO2
    
    gettimeofday(&endingTime, NULL);  // getting the ending time

    printf("Successful transfer of file from Process1 to Process2, and back from Process2 to Process1...\n");
    
    double timeTaken = (endingTime.tv_sec - startingTime.tv_sec) + (endingTime.tv_usec - startingTime.tv_usec) / 1000000.0;
    //time taken for double file transfer

    //Comparing the file contents to check for any error in File Transfers
    if (compareFiles(filePath, FILE1_BUFF)) {
        printf("The received file is same as the sent file...\n");
        printf("Time required to do the double file transfer : %lf sec\n", timeTaken);
    } 
    else {
        printf("File is Corrupted !!\n");
    }
}

//This function is called from process2.
void process2(void) {    
    transferFile(FIFO1, FILE2_BUFF, "p2");  // get file from process1 through FIFO, contents stored in FILE2_BUFF    
    transferFile(FILE2_BUFF, FIFO2, "p2"); //transferring the file back to process1
}

int main(int argc, char *argv[]){

    //Checking if the file path is inputed
    if(argc != 2) {
        printf("Provide the path of the file which is to be transfered, as argument\n");
        return EXIT_FAILURE;
    }

    char *filePath = argv[1]; //Inputted file path

    // creating two FIFOs for file fileTransfer
    if(mkfifo(FIFO1, 0666) < 0){    
        perror("Unable to create first FIFO.");
    }
    
    if(mkfifo(FIFO2, 0666) < 0){
        perror("Unable to create second FIFO.");
    }

    pid_t f_id = fork(); //forking to create two processes which will later be used for file transfer.

    // Unable to fork
    if (f_id == -1) {
        perror("Unable to fork !!\n");
        return EXIT_FAILURE;
    }

    // f_id is 0 in child process
    else if (f_id == 0) {
        process2();
    }

    // f_id is greater than 0 in the parent process
    else if (f_id > 0) {
        process1(filePath);
    }

    return 0;
}


/*
Output displayed after using ls -l command :-

-rw-r--r-- 1 joy5102 joy5102       1250 Aug  6 20:36 15_2A.c
-rwxr-xr-x 1 joy5102 joy5102      17392 Aug  6 21:20 15_2B
-rw-r--r-- 1 joy5102 joy5102       6743 Aug  6 21:26 15_2B.c
-rwxr-xr-x 1 joy5102 joy5102      16816 Aug  5 23:27 a.out
-rw-r--r-- 1 joy5102 joy5102 1073741824 Aug  6 21:25 buffer_process1
-rw-r--r-- 1 joy5102 joy5102 1073741824 Aug  6 21:22 buffer_process2
prw-r--r-- 1 joy5102 joy5102          0 Aug  6 21:22 myfifo1
prw-r--r-- 1 joy5102 joy5102          0 Aug  6 21:25 myfifo2
-rw-r--r-- 1 joy5102 joy5102 1073741824 Aug  5 20:14 testfile.txt

*/