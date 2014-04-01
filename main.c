#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>


/* Define functions */
FILE* getSiteContent(char* address);


int main() {
    FILE* content = getSiteContent("http://yandex.ru");

    

    return 0;
}

FILE* getSiteContent(char* address) {
    FILE *pipe;
    char commandName[] = "curl ";
    int addressLn = strlen(address) + strlen(commandName); 
   
    //Init empty array. But set first element 0 value to
    //replace all non specified elements by \0 
    char command[addressLn];
    command[0] = 0;

    strcat(command, commandName);
    strcat(command, address);


    if( !(pipe = popen(command, "r")) ) {
        exit(1);
    } 

    FILE *HTMLFILE;
    HTMLFILE = fopen("html", "w+");

    char chunk[256];
    while(fgets(chunk, sizeof(chunk), pipe)) {
        fputs(chunk, HTMLFILE);
    }

    return HTMLFILE;
}





