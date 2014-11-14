#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


struct HTML_parserStruct{
	char** tags;
	int length;
};

/* Get tag attribute */
char* HTMLgetAttr(char* tag, char* attrName);

struct HTML_parserStruct HTMLgetTags(char* path, char* _tag) {
	FILE* page = fopen(path, "r");

	char tag[strlen(_tag)];
	tag[0] = 0;
	strcat(tag, _tag);

	char** tags = malloc(0);
	int tagsIndex = 0;

	char* tagStr = malloc(1);
	tagStr[0] = 0;
	int symbol = fgetc(page);
	int index = 0;
	int search = 0;


	while( symbol != EOF ) {

		//If we in search mode detect is it searching tag
		if( search == 1 ) {
			if( symbol == tag[index] ) {
				tagStr = realloc(tagStr, index + 1);
				tagStr[index] = symbol;
	
				index++;
			} else {
				free(tagStr);
				search = 0;
				index = 0;
			}
			
			if( strlen(tag) == index ) {
				search = 2;
			}
		} 

		//If it was searching tag - get whole tag with all attributes
		else if( search == 2 ) {
			int _index = index + 1;
			tagStr = realloc(tagStr, _index);
			tagStr[_index] = 0;
			tagStr[index] = symbol;
			
			if( strrchr(">", symbol) != 0 ) {
				char* tagEl = malloc(index + 1);
				tagEl[0] = 0;

				strcat(tagEl, "<");
				strcat(tagEl, tagStr);
				tags[tagsIndex] = tagEl;

				free(tagStr);
				
				search = 0;
				index = 0;
				tagsIndex++;
			}
			index++;
		}

		//If we find '<' enable tag search mode	
		if( strrchr("<", symbol) != NULL ) {
			search = 1;
			tagStr = malloc(0);
		}
		
		symbol = fgetc(page);
	}

	fclose(page);
    
	struct HTML_parserStruct object;
	object.tags = tags;
	object.length = tagsIndex;
	return object;
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
