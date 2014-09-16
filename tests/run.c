#include "../lib/parser.c"
#include <stdio.h>
#include <dirent.h>


char testsPath[] = "parser";


void TEST_getTags() {
	DIR* TESTS_DIR = opendir("./parser");

	if( TESTS_DIR != NULL ) {
		
		struct dirent* pDirent;
		int dirIterator = 0;
		
		//Loop all tests dirs
		while( (pDirent = readdir(TESTS_DIR)) != NULL ){
			if( dirIterator > 1 ) {
				char testDirName[strlen(testsPath) + strlen(pDirent->d_name) + 1];
				testDirName[0] = 0;
				strcat(testDirName, testsPath);
				strcat(testDirName, "/");
				strcat(testDirName, pDirent->d_name);

				DIR* TEST = opendir(testDirName);

				//Loop test .in and .out files
				if( TEST != NULL ) {
					int filesIterator = 0;
					struct dirent* testDirent;

					while( (testDirent = readdir(TEST)) != NULL ) {
						if( filesIterator > 1 ) {
							char pathToFile[strlen(testDirName) + strlen(testDirent->d_name) + 1];
							pathToFile[0] = 0;
							strcat(pathToFile, testDirName);
							strcat(pathToFile, "/");
							strcat(pathToFile, testDirent->d_name);

							//TODO: read files
							printf("FILE: %s\n", pathToFile);
						}
						filesIterator++;
					}
				}
			}
			dirIterator++;
		}

	} else {
		printf("No directory.\n");	
	}
}



int main() {
	TEST_getTags();
	return 0;
}
