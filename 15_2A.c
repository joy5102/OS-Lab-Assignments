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
Catch the signal 'SIGINT' and display "Ha Ha, Not Stopping". Use the signal system call.

INPUT DESCRIPTION :-
No input

OUTPUT DESCRIPTION :-
During execution, when we press ctrl+C, instead of exiting the process "Ha Ha, Not Stopping" message will be displayed.
ctrl+Z is pressed to exit the process.

*/

/*

COMPILATION COMMAND :-
gcc 15_2A.c

EXECUTION SEQUENCE :-
./a.out

*/

/*

SAMPLE INPUT :-
No input

SAMPLE OUTPUT :-
^C
Ha Ha, Not Stopping
^C
Ha Ha, Not Stopping
^Z
[1]+  Stopped                 ./a.out

*/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

//function for handling signal
void signalHandler(int signo) {
    if (signo == SIGINT)
        printf("\nHa Ha , Not Stopping\n");
}

int main(void)
{
    if (signal(SIGINT, signalHandler) == SIG_ERR) //signal system call
        printf("\ncan't catch SIGINT\n");

    //Infinite while loop
    while(1) {

    }
    
    return 0;
}




