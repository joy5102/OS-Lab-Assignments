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
Create two processes. Transfer a 1GB file from process1 to process2 using a socket. 
Now, transfer the file from process2 to process1 using the same socket.
Now, compare the two files to make sure that the same file has returned back.
Also, print the time required to do this double transfer.

INPUT DESCRIPTION :-
Path of the 1GB file is taken as input from the command line.

OUTPUT DESCRIPTION :-
Success status of creation of sockets, binding of server to socket,
client to server and server to client connection are displayed.
Success status of whether the sent and received files are the same is displayed.
Time required to do the double transfer of the file is displayed.

*/

/*
COMPILATION COMMAND :-
 gcc 15_3.c -o 15_3

EXECUTION SEQUENCE :-
./15_3 testfile.txt

*/

/*
SAMPLE INPUT (taken from the command line) :-
testfile.txt

SAMPLE OUTPUT :-
Successully created server side socket !!
Successfully created client side socket!!
Server bind to socket sucessfully !!
Server listening at PORT 5000
Client connected to the server successfully
Server connected to the client successfully
The received file and the sent file are the same..
Time required to do the DOUBLE TRANSFER : 117.992898 sec

*/

/*

Output (Bash command ss -a -t to check socket details) :-
Cannot open netlink socket: Protocol not supported
State        Recv-Q        Send-Q               Local Address:Port               Peer Address:Port       Process


Output (ls -l command) :-
total 3145812
-rwxr-xr-x 1 joy5102 joy5102      22064 Aug 13 20:40 15_3
-rw-r--r-- 1 joy5102 joy5102      16929 Aug 13 20:51 15_3.c
-rw-r--r-- 1 joy5102 joy5102 1073741824 Aug 13 20:42 process1_tempFile
-rw-r--r-- 1 joy5102 joy5102 1073741824 Aug 13 20:40 process2_tempFile
-rw-r--r-- 1 joy5102 joy5102 1073741824 Aug  5 20:14 testfile.txt

*/

//header files 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define LOCALHOST "127.0.0.1"   //address of the local host
#define PORT_NO 5000  // port number

#define TEMP_FILE_1 "process1_tempFile"
#define TEMP_FILE_2 "process2_tempFile"


//The file sent by process1 and the file received by the process2 are compared to check whether they are identical.
int compareFiles(char* file1, char* file2) {
    int fd1, fd2;   // file descriptors of the files

    fd1 = open(file1, O_RDONLY); //opening the file for reading
    fd2 = open(file2, O_RDONLY); //opening the file for reading
    
    // opening the initial input file. If -1 then error in opening file
    if (fd1 == -1) {
        perror("Error... Unable to open file\n");
        return -1;
    }

    // opening the received file
    if (fd2 == -1) {
        perror("Error... Unable to open file !!\n");
        if (close(fd2) == -1) {
            perror("Error... Unable to close file !!\n");
        }
        return -1;
    }

    // buffers to store contents of the files which can be later compared to check if they are identical
    char buffer1[BUFFER_SIZE + 1];
    char buffer2[BUFFER_SIZE + 1];

    while (true){
        int length1 = read(fd1, buffer1, BUFFER_SIZE);
        int length2 = read(fd2, buffer2, BUFFER_SIZE);
        
        // if length of the files are -1 then there occurs an error in reading file
        if(length1 == -1 || length2 == -1){
            perror("Error... Unable to read file !!\n");
            return -1;
        }

        // if the sent and received files are different then their lengths are not the same
        if(length1 != length2) {
            return 0;    
        }

        //reached end of file
        if(length1 == 0) {
            break;
        }

        // if length1 is greater than 0, then we continiue to read.
        for(int i=0; i<length1; i++){
            if (buffer1[i] != buffer2[i]){
                // files are different in their input, hence error has occured while transmission
                return 0;
            }
        }       
    }

    // Close files after reading and checking them
    if(close(fd1) == -1) {
        perror("Error... Unable to close file !!\n");
        return -1;
    }

    if(close(fd2) == -1) {
        perror("Error... Unable to close file !!\n");
        return -1;
    }

    return 1;
}

//getting the file size in bytes, this alows us to validate that the entire file has reached the other side
long getFileSize(char* filename) {
    FILE* file;

    //opening the file
    if((file = fopen(filename, "r")) == NULL) {
        perror("Unable to determine file size !!\n");
        return -1;
    }

    //takes the pointer to the end of the file
    if(fseek(file, 0, SEEK_END) == -1) 
    {
        perror("Unable to determine file size !!\n");
        return -1;
    }

    //gets the size of the file, by calulating the difference between the start of the file and the current position of the pointer in the file
    long size = ftell(file);

    //file size is -1 if it could not be determined
    if(size == -1) {
        perror("Unable to determine file size !!\n");
        return -1;
    }

    return size;
}

//sending the file via the socket 
int sendFile(char *sourceFile, int destination) {

    //getting the size of the file
    long fileSize = getFileSize(sourceFile);

    //error in getting the size of the file
    if(fileSize == -1){
        return -1;
    }
    
    //buffer to be sent
    char buffer[BUFFER_SIZE + 1];

    //first message sent is the size of the file
    *((long *)buffer) = fileSize;

    //sending the file size to the other side so that the other side can determine of the entire file has been received or not
    if(send(destination, buffer, sizeof(long), 0) < 0){
        perror("Unable to send File!!\n");
        return -1;
    }

    int fd = open(sourceFile, O_RDONLY); //opening the file which is to be sent

    //Checking if the file to be sent can be opened
    if(fd == -1){
        perror("Error... Unable to Open File !!\n");
        return -1;
    }

    //infinite loop till the entire file is sent
    while(true){
        
        //reading from the file into the buffer
        long buffRead = read(fd, buffer, BUFFER_SIZE);

        //-1 if error has occured
        if(buffRead < 0) {
            perror("Error...Unable to Read File !!\n");
            return -1;
        }

        if(buffRead == 0) {
            break;  //reached end of file
        }

        //sending the buffer/packet to the other side of the socket
        if (send(destination, buffer, buffRead, 0) == -1) {
            return -1;
        }
    }

    //closing the file
    if (close(fd) == -1) {
        perror("Error...Unable to close the file\n");
        return -1;
    }

    return 1; //return 1 if the file has been recieved successfully
}

//receiving the file via the socket
int receiveFile(char* destinationFile, int source){

    //to store the size of the file
    char buffer[BUFFER_SIZE + 1];

    //receives the file size
    if(recv(source, buffer, sizeof(long),0) < 0){
        perror("Error... Unable to receive the file !!\n");
        return -1;
    }

    //size of the file
    long file_size = *((long *)buffer);

    //opening the destination file, where the contents received via socket will be stored
    int destinationId = open(destinationFile,O_WRONLY | O_CREAT , 0666); //0666 --> permission to read and write for everyone

    if(destinationId < 0) {
        perror("Error... Unable to open the file!!!\n");
        return -1;
    }

    long toReceive = file_size;

    while(toReceive > 0) {

        //receiving the contents in form of packets
        int bytesRead = recv(source, buffer, BUFFER_SIZE,0);

        //is negative is there is an error while recieving the contents of the file
        if(bytesRead < 0) {
            perror("Error... Cannot read !!\n");
            return -1;
        }

        //end of the file has been reached
        if(bytesRead == 0){
            break;
        }

        //writing into the destination file
        if(write(destinationId,buffer,bytesRead) < 0){
            perror("Error... Cannot write\n");
            return -1;
        }

        toReceive -= bytesRead;
    }
    
    // error closing the file
    if(close(destinationId) < 0){
        perror("Error closing\n");
        return -1;
    }
    
    return 1;  //return 1 if the file has been recieved successfully
}

//creating the server
int createServer() {

    int serverId = socket(AF_INET,SOCK_STREAM, 0); //creating the server side interface using TCP/IP protocols

    //Server Id is -1 if there is failure in server creation
    if(serverId < 0){
        perror("Unable to create server side socket !!\n");
        return serverId;
    }
    printf("Successully created server side socket !!\n");

    //server side address information
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //IPv4 family of addresses
    server_address.sin_port = htons(PORT_NO);  //adding the port number
    server_address.sin_addr.s_addr = INADDR_ANY; 
    bzero(&server_address.sin_zero, 8);         

    //binding the server address to the socket created
    if (bind(serverId, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Unable to bind server address to socket\n");
        if (close(serverId) == -1) {
            perror("Error closing !!\n");
        }
        return -1;
    }

    printf("Server bind to socket sucessfully !!\n");

    //listening for the clients 
    if (listen(serverId,1) < 0){
        perror("Server listen error\n");
        if (close(serverId) == -1) {
            perror("Error closing!!!\n");
        }

        return -1;
    }

    printf("Server listening at PORT %d\n", PORT_NO);
    
    //returning the Server Id
    return serverId;
}

//connecting the server to client
int acceptConnection(int serverId){

    int connectionfd;                // connection socket file descriptor
    struct sockaddr_in client_address;  // a sockaddr_in for client
    unsigned int addrlen = sizeof(client_address);

    //accepting the client and connecting to the server
    connectionfd = accept(serverId, (struct sockaddr*)&client_address, &addrlen);

    // is -1 when it fails to accept the client
    if (connectionfd < 0) 
    {
        perror("Server could not accept connection!!!\n");
        return -1;
    }

    //return server side descriptor
    return connectionfd;
}

//connecting the client to server
int connectToServer(){
    
    //creating the client side of socket interface using TCP/IP protocols
    int clientId = socket(AF_INET, SOCK_STREAM, 0);

    //Client Id is negetive if failure in creation of socket
    if(clientId < 0){
        perror("Unable to create client side socket !!\n");
        return clientId;
    }
    printf("Successfully created client side socket!!\n");

    //address details
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;  //IPv4 address family
    client_address.sin_port = htons(PORT_NO);  //adding port to the address
    client_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    bzero(&client_address.sin_zero, 8);   

    //connecting the server side of the socket, is -1 when it fails to connect
    if(connect(clientId, (struct sockaddr *)&client_address, sizeof(client_address)) < 0){
        perror("Client could not connect\n");
        
        if (close(clientId) == -1) {
            perror("close error\n");
        }
        return -1;
    }

    //returning client Id
    return clientId;
}

//process1 (server side program)
void process1(char* sourceFile){

    //creating a server socket
    int server_id = createServer();

    //server id is negative if failure in creation of server socket 
    if(server_id < 0){
        perror("createServer failed\n");
        exit(EXIT_FAILURE);
    }

    //connecting to a client
    int connectionId = acceptConnection(server_id);

    //connect id is negative if the server is not connected the client
    if(connectionId < 0){
        perror("acceptConnection failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Server connected to the client successfully\n");

    //get time to calculate the time
    struct timeval startingTime, endingTime;
    gettimeofday(&startingTime, NULL); //getting the starting time

    //sending to the client
    if(sendFile(sourceFile,connectionId) < 0){
        if(close(connectionId) < 0){
            perror("Close error\n");
        }
        if(close(server_id) < 0){
            perror("Close error\n");
        }

        exit(EXIT_FAILURE);
    }

    //receiving the file from the client 
    if(receiveFile(TEMP_FILE_1,connectionId) < 0){
        if(close(connectionId) < 0){
            perror("Close error\n");
        }
        if(close(server_id) < 0){
            perror("Close error\n");
        }

        exit(EXIT_FAILURE);
    }
    gettimeofday(&endingTime, NULL); //getting the ending time

    //calculating the time taken for the double transfer
    double timeValue =  (endingTime.tv_sec - startingTime.tv_sec) + (endingTime.tv_usec - startingTime.tv_usec) / 1000000.0;    

    //authenticate the contents of the file
    if (compareFiles(sourceFile, TEMP_FILE_1) > 0) {
        printf("The received file and the sent file are the same..\n");
        printf("Time required to do the DOUBLE TRANSFER : %lf sec\n", timeValue);
    } else {
        printf("File is Corrupted !!\n");
    }

    //closing the connection to the socket
    if(close(connectionId) < 0){
        perror("Close error\n");
    }

    if(close(server_id) < 0){
        perror("Close error\n");
    }
}

//process2 (client side program)
void process2() {

    int clientId = connectToServer();  //function to connect to server

    //Client Id is -1 when there is connection failure
    if(clientId < 0) {
        perror("Failure in connection to server\n");
        exit(EXIT_FAILURE);
    }

    printf("Client connected to the server successfully\n");

    //receiving file via the socket (from the server)
    if (receiveFile(TEMP_FILE_2, clientId) == -1) {
        if (close(clientId) < 0) {
            perror("close error\n");
        }
        exit(EXIT_FAILURE);
    }

    // sending file via the socket (from the client)
    if (sendFile(TEMP_FILE_2, clientId) < 0 ) {
        if (close(clientId) == -1) {
            perror("close error\n");
        }
        exit(EXIT_FAILURE);
    }
}

//driver function
int main(int argc, char * argv[argc + 1]) {

    //cheacking the number of arguments
    if(argc != 2){
        printf("Number of arguments must be equal to 1\n");
        return 1;
    }

    //the path of the file which is to be sent and received via the sockets
    char* sourceFile = argv[1];
    
    pid_t f_id = fork(); //forking to create two processes (parent and child)
    //f_id --> fork id

    //If fork id is negative, then the program is unable to fork
    if(f_id < 0) {
        perror("Unable to fork !!\n");
        return 1;
    }
    
    //fork id is 0 in child process
    else if(f_id == 0) { 
        process2();  //child process (process2)
    }

    //fork id is greater than 0 in the parent process
    else {
        process1(sourceFile);  //parent process (process1)
    }

    return 0;
}