#include <stdio.h>
#include <string.h>
#include <errno.h>


/* Detect Out Of Memory problem log and exit program */
void __HTMLparserLogENOMEM();

/* Get tag attribute */
char* HTMLgetAttr(char* tag, char* attrName);


char** HTMLparser(char* path, char* tag) {
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
			__HTMLparserLogENOMEM();
			
			tagStr[index] = symbol;
	
			if( strrchr(">", symbol) == 0 ) {
				index++;
			} else {
				char* tag = (char*)malloc(index + 2);
				__HTMLparserLogENOMEM();
				
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

/* TODO: Check on correct params type */
char* HTMLgetAttr(char* tag, char* attrName) {
	int strSize = strlen(tag);
	int counter = 0;
	int search = 0;
	int isAttrLn = 0;

	char symbol;
	int index = 0;
	int attrValIndex = 0;
	
	char* attr = (char*)malloc(strlen(attrName));
	strcat(attr, attrName);
	char* attrVal = (char*)malloc(1);

	while(counter < strSize) {
		
		//If we end current attr value	
		if( search == 1 && strrchr("\"", symbol) != NULL ) break;	
		
		if( search == 1 ) {
			attrVal[attrValIndex] = symbol;
			attrValIndex++;
		}

		if( symbol == attr[index] ) {
			index++;
		} else {
			index = 0;
		}
		
		if( strlen(attr) == index ) isAttrLn = 1;
		
		//If we found correct attr and =" is gone - start select attr value	
		if( isAttrLn == 1 && strrchr("\"", symbol) != NULL ) search = 1;	
		
		symbol = tag[counter];
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

