#include <stdio.h>
#include <string.h>


char** parseHTML(char* path, char* tag) {
	FILE* page = fopen(path, "r");

	//Get file size	
	fseek(page, 0L, SEEK_END);
	int fileSize = ftell(page);
	fseek(page, 0L, SEEK_SET);

	char** tags = malloc(fileSize);
	int tagsIndex = 0;

	char* tagStr = (char*)malloc(1);
	int symbol = fgetc(page);
	int index = 0;
	int search = 0;
	
	while( symbol != EOF ) {

		//If we in search mode detect is it searching tag
		if( search == 1 ) {
			if( symbol == tag[index] ) {
				tagStr = (char*)realloc(tagStr, index + 1);
				tagStr[index + 1] = symbol;

				index++;
			} else {
				search = 0;
				index = 0;
			}
			
			if( strlen(tag) == index ) {
				search = 2;
				index++;
			}
		} 

		//If it was searching tag - get whole tag with all attributes
		else if( search == 2 ) {
			tagStr = (char*)realloc(tagStr, index + 2);
			tagStr[index] = symbol;
	
			if( strrchr(">", symbol) == 0 ) {
				index++;
			} else {
				char* tag = (char*)malloc(index + 2);
				strcpy(tag, tagStr);

				tags[tagsIndex] = tag;
				search = 0;
				index = 0;
				tagsIndex++;
			}
		}

		//If we find '<' enable tag search mode	
		if( strrchr("<", symbol) != NULL ) {
			tagStr[index] = symbol;
			search = 1;
		}
		
		symbol = fgetc(page);
	}

	free(tagStr);

	return tags;
}
