#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
typedef char string[50];
struct termios stare_ustawienia , nowe_ustawienia;


void disableRawMode(){
    tcsetattr(STDIN_FILENO ,TCSANOW , &stare_ustawienia);
}
int main(){
    printf("Welcome to the text editor \n");
    char fileName[50];
    printf("Give me a file name: \n");
    fgets(fileName, 50 , stdin);
    size_t len = strlen(fileName);
    if(len > 0 && fileName[len-1] == '\n'){
        fileName[len- 1] = '\0';
        // fileName[len-2] = '/';
    }
    printf("%s" , fileName);
    if(tcgetattr(STDIN_FILENO , &stare_ustawienia) != 0){ // pobiera aktuale ustawienia terminala
        
        printf("Bład w terminalu");
        return 1;
    }
    atexit(disableRawMode);
    nowe_ustawienia = stare_ustawienia;
    nowe_ustawienia.c_lflag &= ~ECHO;
    nowe_ustawienia.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW , &nowe_ustawienia);

    // printf("write a file to edit: ");
    // fgets(fileName , sizeof(fileName) , stdin);
    
    
    FILE *our_file_read = fopen(fileName , "r");
    FILE *our_file_write = fopen(fileName, "w");
    if(our_file_read == NULL){
        printf("file does not exist");
        return 1;
    }
    // char *buffer[] = malloc();
    int counter = 0;
    char buffer[50] = {0};
    int startSize = 2;
    // char *tekst = malloc(startSize *  sizeof(char));
    while(1){
        char c = getchar();
        if ( c == 'q'){
            break;
        }
        printf("%c" , c);
        // counter++;
        buffer[counter] = c;
        counter++;
    }   
    printf("%s" , buffer);
    fprintf(our_file_write , "%s" , buffer);
    fclose(our_file_read);
    fclose(our_file_write);
    tcsetattr(STDIN_FILENO , TCSANOW, &stare_ustawienia);
    // atexit(disableRawModule());
    // free(tekst);
    return 0;

}