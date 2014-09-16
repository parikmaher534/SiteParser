#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/* Detect Out Of Memory problem log and exit program */
void __HTMLparserLogENOMEM();

/* Get tag attribute */
char* HTMLgetAttr(char* tag, char* attrName);


char** HTMLgetTags(char* path, char* tag) {
	FILE* page = fopen(path, "r");


	//Get file size	
	fseek(page, 0L, SEEK_END);
	int fileSize = ftell(page);
	fseek(page, 0L, SEEK_SET);

	char** tags = (char**)malloc(fileSize);
	int tagsIndex = 0;

	char* tagStr = (char*)malloc(1);
	int symbol = fgetc(page);
	int index = 0;
	int search = 0;

	__HTMLparserLogENOMEM();

	while( symbol != EOF ) {

		//If we in search mode detect is it searching tag
		if( search == 1 ) {
			if( symbol == tag[index] ) {
				tagStr = (char*)realloc(tagStr, index + 1);
				__HTMLparserLogENOMEM();
				
				tagStr[index + 1] = symbol;
	
				index++;
			} else {
				tagStr = (char*)realloc(tagStr, 1);
				search = 0;
				index = 0;
			}
			
			if( strlen(tag) == index ) search = 2;
		} 

		//If it was searching tag - get whole tag with all attributes
		else if( search == 2 ) {
			tagStr = (char*)realloc(tagStr, index + 2);
			__HTMLparserLogENOMEM();
			tagStr[index + 1] = symbol;
			
			if( strrchr(">", symbol) == 0 ) {
				index++;
			} else {
				char* tagEl = (char*)malloc(index + 2);
				__HTMLparserLogENOMEM();

				strcpy(tagEl, tagStr);

				tagStr = (char*)realloc(tagStr, 0);
				tags[tagsIndex] = tagEl;
				search = 0;
				index = 0;
				tagsIndex++;
			}
		}

		//If we find '<' enable tag search mode	
		if( strrchr("<", symbol) != NULL ) {
			tagStr[0] = symbol;
			search = 1;
		}
		
		symbol = fgetc(page);
	}

	free(tagStr);

	return tags;
}

char* HTMLgetAttr(char* _tag, char* attrName) {
	int strSize = strlen(_tag);
	int counter = 0;
	int search = 0;
	int isAttrLn = 0;

	char symbol;
	int index = 0;
	int attrValIndex = 0;
	char* attrVal = (char*)malloc(1);

	char* tag = (char*)malloc(strSize);
	strcat(tag, _tag);

	while(counter < strSize) {
		
		symbol = tag[counter];
		
		//If we end current attr value	
		if( search == 1 ) {
			if( strrchr("\"", symbol) != NULL ) {
				break;
			} else {
				attrVal = (char*)realloc(attrVal, attrValIndex);
				strcat(attrVal, &symbol);
				attrValIndex++;
			}
		}
	
		if( search == 0 ) {	
			if( symbol == attrName[index] ) {
				index++;
			} else {
				index = 0;
			}
			
			if( strlen(attrName) == index ) {
				isAttrLn = 1;
			}
			
			//If we found correct attr and =" is gone - start select attr value	
			if( isAttrLn == 1 && strrchr("\"", symbol) != NULL ) {
				search = 1;
				isAttrLn = 0;	
			}
		}
		
		counter++;
	}

	return attrValIndex == 0 ? NULL : attrVal;
}


void __HTMLparserLogENOMEM() {
	if( errno == ENOENT ) {
		perror("Out of memory.");
		exit(EXIT_FAILURE); 
	}
}

