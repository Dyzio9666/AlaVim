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
    // printf("%s" , fileName);
    if(tcgetattr(STDIN_FILENO , &stare_ustawienia) != 0){   // pobiera aktuale ustawienia terminala
        
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
    if(our_file_read == NULL){
        printf("file does not exist");
        return 1;
    }    
    fseek(our_file_read , 0 , SEEK_END);
    long file_size = ftell(our_file_read);
    rewind(our_file_read);
    char *myText = malloc((file_size + 1) * sizeof(char));
    if (myText == NULL) {
        fclose(our_file_read);
        return 1;
    }
    fread(myText , 1, file_size, our_file_read);
    myText[file_size] = '\0';
    fclose(our_file_read);
    printf("Tresc pliku:\n%s\n", myText);
    // char *buffer[] = malloc();
    FILE *our_file_write = fopen(fileName, "w");
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
    fprintf(our_file_write , "%s" , buffer);
    fclose(our_file_write);
    tcsetattr(STDIN_FILENO , TCSANOW, &stare_ustawienia);

    return 0;

}