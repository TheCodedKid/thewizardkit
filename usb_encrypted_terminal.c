/*
Group members:
Angel L.
sAmEuEL

Group Name:
Angel's Team

Problem Statment:

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LENGTH 1024
#define MAX_LINE_LENGTH 256
#define MAX_ARGS 64

int helper(); //prototype
void* check_file(void* filename);
char* get_last_log_line(char* filename);
int usb_exist_in_driver(char* filename);


//Gloabals
char FILENAME[MAX_LENGTH] = "/var/log/kern.log";
char USB_DRIVER_PATH[MAX_LENGTH] = "/sys/bus/usb/devices/3-2:1.0/driver/module/drivers/usb:USB_KEY_DRIVER";
int STATUS = 0;

int main(void) {

    pthread_t thread_id;
    int ret = pthread_create(&thread_id, NULL, (void*)check_file, (char*) FILENAME);
    if (ret) {
        fprintf(stderr, "Failed to create thread: %d\n", ret);
        return EXIT_FAILURE;
    }

    while (1) {
        char input[MAX_LENGTH];
        char* args[MAX_ARGS];
        int argCount = 0;

        do{
            printf("Enter a command (for help type 'help' or type 'q' to quit):");   // Ask for input
            //scanf("%s", input);
            fflush(stdout);           //clears out stdout fstream from consecutive typing                                                
            fgets(input, MAX_LENGTH, stdin);    // Read user input from stdin
            input[strcspn(input, "\n")] = '\0';     // Strip trailing newline from input *Both required
            if(strcmp(input, "help") == 0)          //If input is help then run helper
                helper();
        }while(strcmp(input, "help") == 0);     // keep getting user input

        if (strcmp(input, "q") == 0) {   //from string.h library
            printf("Exiting out of shell...\n");
            return(0);  //exits out of program
        }
        //Returns first token
        char *token = strtok(input, " ");   // uses strtok for delimiter 'spaces'
    
        // Keep printing tokens while one of the
        // delimiters present in str[].
        while (token != NULL && argCount < MAX_ARGS)
        {
            //printf("%s\n", token);
            strcat(token,"\0"); // concats terminator in case was not placed/aligned automatticly
            args[argCount++] = token;   //store each argument in string array
            token = strtok(NULL, " ");
        }
        args[argCount] = (char*)NULL;

        pid_t pid = fork(); // pid_t type is essentiall the same as "signed int"

        switch (pid) {
            case -1:
                printf("This line is printed from parent process, fork failed. PID: %d\n", getpid());
                break;
            case 0:
                char* temp;
                printf("Child process started. PID: %d\n", getpid());
                if(STATUS == 0){
                    printf("\n*****USB Licence Not Found*****\n\n");
                    return (0);
                }
                printf("\n*****USB Licence Valid, Access Granted*****\n\n");
                execvp(args[0],args);   //function takes filepath/file and the arguments in a vector 
                printf("The command did not work, try again\n");
                return(0);
                break;
            default:
                int status;
                printf("Parent process started. PID: %d\n", getpid());
                printf("Parent process is now waiting\n");
                wait(&status);
                printf("Parent process is now resuming\n");
                if (WIFEXITED(status)) {
                    printf("This line is printed from the parent process. PID: %d\n", getpid());
                } else {
                    printf("This line is printed from parent process, child did not finsh correctly. PID: %d\n", getpid());
                }
                break;
        }
    }
    return(0);
}

void *check_file(void *filename)        //pthread function
{
    while(1){
        int valid_usb = 0;    // sanity checks to see if usb exist in correct driver
        do{
            valid_usb = usb_exist_in_driver(USB_DRIVER_PATH);
            if(!valid_usb)
                STATUS = 0;
        }while(valid_usb == 0);

        char* last_line = get_last_log_line(filename);
        if (last_line == EXIT_FAILURE) {
            printf("Error getting last log line\n");
            break;;
        }

        if (strstr(last_line, "USB LICENECE GRANTED") != NULL) {
            STATUS = 1;
        } else if (strstr(last_line, "USB LICENECE REVOKED") != NULL){
            STATUS = 0;
        } else{}

        // printf("\nLast Log: %s\n", last_line);
        // previous_line = last_line;
        free(last_line);
        sleep(1);
    }   
    return EXIT_FAILURE;    //returns 0 but shouldn't
}

char* get_last_log_line(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return EXIT_FAILURE;
    }

    char* last_line = malloc(MAX_LINE_LENGTH * sizeof(char));
    if (last_line == NULL) {
        fprintf(stderr, "Error allocating memory for last log line\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        // save the current line as the last line
        strcpy(last_line, buffer);
    }

    fclose(file);
    return last_line;
}

int usb_exist_in_driver(char* filename){
    FILE *fp = fopen(filename, "r");
    int is_exist = 0;
    if (fp != NULL)
    {
        is_exist = 1;
        fclose(fp); // close the file
    }
    return is_exist;
}

int helper(){
    char help_display[] = 
    "\nTo use commands, type in the command file/path along with any optional arguments:\n"
    "Example Format: 'Command' 'Path' 'Optional Arguments'...\n"
    "\nCommands:\t*Required:\n"
    "Examples: ls, pwd, cp, mkdir... \n"
    "\nPath:\t*Dependant on Command:\n"
    "Examples: /, ./, /home/User ...\n"
    "\nOptional Arguments:\t*Optional:\n"
    "Examples: -all, -r, -lh...\n"
    "\n\nFull Example: ls / -lh,\ttouch ./ test_file.c\n\n"
    "For Futher explanations of commands type: 'Command' --help\n";
    printf("%s",help_display);
    return (0);
};