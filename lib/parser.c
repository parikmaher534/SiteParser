#include <stdio.h>
#include <string.h>


void parseHTML(char* path, char* tag) {
	FILE* page = fopen(path, "r");

	//Get file size	
	fseek(page, 0L, SEEK_END);
	int fileSize = ftell(page);
	fseek(page, 0L, SEEK_SET);

	int symbol = fgetc(page);
	char out[fileSize];
	int index = 0;
	int search = 0;

	while( symbol != EOF ) {

		//If we in search mode detect is it searching tag
		if( search == 1 ) {
			if( symbol == tag[index] ) {
				out[index + 1] = symbol;
				index++;
			} else {
				search = 0;
				index = 0;
			}
			
			if( strlen(tag) == index ) search = 2;
		} 

		//If it was searching tag - get whole tag with all attributes
		else if( search == 2 ) {
			out[index + 1] = symbol;
			
			if( strrchr(">", symbol) == 0 ) {
				index++;
			} else {
				search = 0;
				index = 0;
			}
		}

		//If we find '<' enable tag search mode	
		if( strrchr("<", symbol) != NULL ) {
			out[index] = symbol;
			search = 1;
		}
		
		symbol = fgetc(page);
	}

	printf("OUT: %s", out);
}
