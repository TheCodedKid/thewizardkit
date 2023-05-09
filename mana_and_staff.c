// HackB
// Samuel Raumin, Colin Sianturi, Jeremy Noel
// Implement a simple (magic based) shell
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //used for parsing arguments
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>


#define maxlength 1024
#define MAX_LINE_LENGTH 256

// Prototype Functions
void* check_file(void* filename);
char* get_last_log_line(char* filename);
int usb_exist_in_driver(char* filename);

//Gloabals
char FILENAME[maxlength] = "/var/log/kern.log";
char USB_DRIVER_PATH[maxlength] = "/sys/bus/usb/devices/3-2:1.0/driver/module/drivers/usb:USB_KEY_DRIVER";
int STATUS = 0;

void manabar(int manainput)
{
    printf("Amount of Mana: ");
    if (manainput == 0)
        printf("__________: %d \n", manainput);
    else if (manainput > 0 && manainput <= 10)
        printf("█_________: %d \n", manainput);
    else if (manainput > 10 && manainput <= 20)
        printf("██_______: %d \n", manainput);
    else if (manainput > 20 && manainput <= 30)
        printf("███_______: %d \n", manainput);
    else if (manainput > 30 && manainput <= 40)
        printf("████______: %d \n", manainput);
    else if (manainput > 40 && manainput <= 50)
        printf("█████_____: %d \n", manainput);
    else if (manainput > 50 && manainput <= 60)
        printf("██████____: %d \n", manainput);
    else if (manainput > 60 && manainput <= 70)
        printf("███████___: %d \n", manainput);
    else if (manainput > 70 && manainput <= 80)
        printf("████████__: %d \n", manainput);
    else if (manainput > 80 && manainput <= 90)
        printf("█████████_: %d \n", manainput);
    else if (manainput >= 100)
        printf("██████████: %d \n", manainput);
}

int main()
{
    char *opening =
        "                              *   + 		  *   =\n"
        "                            +  =   *	•    -  +  *  *\n"
        "                               -   (\\.   \\      ,/)   -\n"
        "  _____ _____ _____ _____     _     \\(   |\\     )/\n"
        " |     |  _  |   | |  _  |___| |_   //\\  | \\   /\\\\\n"
        " | | | |     | | | |     |_ -|   | (/ /\\_#oo#_/\\ \\)\n"
        " |_|_|_|__|__|_|___|__|__|___|_|_|  \\/\\  ####  /\\/\n"
        "       Wizardry, at a cost               `##'\n";
    int runtime = 1;
    char cmd[maxlength];
    char *args[maxlength];
    pid_t pid;

    pthread_t thread_id;
    int ret = pthread_create(&thread_id, NULL, (void*)check_file, (char*) FILENAME);
    if (ret) {
        fprintf(stderr, "Failed to create thread: %d\n", ret);
        return EXIT_FAILURE;
    }

    int mana = 100;
    int timepassed = 0;
    time_t timeofstart;
    time(&timeofstart);
    puts(opening);

    // puts(opening);//prints opening
    while (runtime)
    { // placed within while loop to make it "easy" to exit
        time_t timeofexec;
        time(&timeofexec);
        timepassed = timeofexec - timeofstart;
        if (mana > 100)
        {
            mana = 100;
        }
        manabar(mana);
        printf("Time since program started: %u seconds\n", timepassed);
        printf("%s> ", ctime(&timeofexec));
        fflush(stdout);

        if (timepassed % 10 == 0 && timepassed != 0)
        {
            mana = mana + 10; // adds mana every 10 seconds
        }

        // Read command from user
        if (fgets(cmd, maxlength, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // Parse command into arguments
        int num_args = 0;
        if (mana > 1)
        {
            args[num_args] = strtok(cmd, " \n"); // using for delimiter purpose
            while (args[num_args] != NULL)
            {
                num_args++;
                args[num_args] = strtok(NULL, " \n");
            }
            args[num_args] = NULL;
        }
        else
        {
            printf("You depleted all your mana!\n");
            printf("You Died!");
            break;
        }

        if (strcmp(args[0], "iamasimpforliviu") == 0)
        {
            mana = 100;
        }

        if (strcmp(args[0], "ls") == 0)
        {
            mana = mana - 10;
        }

        // if (strcmp(args[0], "cd") == 0){
        //   mana = mana - 5;
        // }

        if (strcmp(args[0], "pwd") == 0)
        {
            mana = mana - 20;
        }

        if (strcmp(args[0], "echo") == 0)
        {
            mana = mana - 10;
        }

        if (strcmp(args[0], "docker") == 0)
        {
            mana = mana - 100;
        }

        if (strcmp(args[0], "mount") == 0)
        {
            mana = mana - 80;
        }

        if (strcmp(args[0], "/bin/bash") == 0)
        {
            args[0] = strtok(" ", " \n");
            printf("You failed to escape, heathen. TO THE DARK REALM!");
            break; // cause it is personal
        }

        // cat, echo

        if (strcmp(args[0], "mana") == 0)
        {
            printf("Mana Costs:\n");
            printf("cd - 5 Mana\nls - 10 Mana\necho - 10 Mana\npwd - 20 Mana\nmount - 80 Mana\ndocker - 100 Mana\n");
            args[0] = "\0"; // clears char array
            args[0] = strtok("echo", " \n");
            // args[0] = strtok(" ", " \n"); //cheeses the input using a very small whitespace character
        }

        // Exit if the user types "exit"
        if (strcmp(args[0], "exit") == 0)
        {
            break; // I do not like this
        }

        // Fork a child process to execute the command
        pid = fork();

        if (pid < 0)
        {
            printf("Error: fork failed\n"); // read man page on fork, as all error codes are negative values
            exit(1);
        }
        else if (pid == 0)
        {
            if(STATUS == 0){
                printf("Magic Staff NOT Detected... Equip the proper Staff. \n");
                return (0);
            }
            printf("Magic Staff Detected!!! Casting Spell !!! \n");
            // creates the child process
            execvp(args[0], args);                             // same as execv, but with pointers
            printf("Error: command not found: %s\n", args[0]); // skips if the command is correct
            exit(1);
        }
        else
        {
            wait(NULL); // blocks parent process until all child processes are "finished"
        }
    }
    return 0;
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
