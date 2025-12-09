#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
typedef char string[50];

struct termios stare_ustawienia , nowe_ustawienia;
char makeachoise(){

    printf("If you want to exit type q \n");
    printf("If you want to read your file type r\n");
    printf("If you want to append somethingto your file  type a\n");
    char userChoice;
    scanf("%c" , &userChoice);
    switch(userChoice){
        case 'q':
            return 'q';
            break;
        case 'r':
            return 'r';
            break;
        case 'a':
            return 'a';
            break;
        default:
            return 'q';
    }

    

}
void die(const char *s ){
    perror(s);
    exit(1);
}
char editorReadKey(){
    int nread;
    char c;
    while((nread == read(STDERR_FILENO , &c,1)) != 1){
        if(nread == -1){ die("read");}
    }
    return c;
}
void editorRefreshscreen(){
    write(STDOUT_FILENO ,"\x1b[2J",4);
}
void editorProccesKey(){
    char c = editorReadKey();
    switch(c){
        case 'q':
            exit(0);
            break;
    }
}

void disableRawMode(){
    tcsetattr(STDIN_FILENO ,TCSANOW , &stare_ustawienia);
}
void enableRawMode(){
    if(tcgetattr(STDIN_FILENO , &stare_ustawienia) != 0){   // pobiera aktuale ustawienia terminala
        
        printf("Bład w terminalu");
    }
    atexit(disableRawMode);
    nowe_ustawienia = stare_ustawienia;
    nowe_ustawienia.c_lflag &= ~ECHO;
    nowe_ustawienia.c_lflag &= ~ICANON;
    nowe_ustawienia.c_lflag &= ~(IXON);
    nowe_ustawienia.c_lflag &= ~(IEXTEN);
    tcsetattr(STDIN_FILENO, TCSANOW , &nowe_ustawienia);
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
    
    enableRawMode();
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
    FILE *our_file_write = fopen(fileName, "a");
    int counter = 0;
    char buffer[50] = {0};
    int startSize = 2;
    // char *tekst = malloc(startSize *  sizeof(char));

    while(1){
        editorRefreshscreen();
        editorProccesKey();
    }   
    fprintf(our_file_write , "%s" , buffer);
    fclose(our_file_write);
    tcsetattr(STDIN_FILENO , TCSANOW, &stare_ustawienia);

    return 0;

}