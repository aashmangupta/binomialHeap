#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<ctype.h>
#include"integer.h"
#include"real.h"
#include"string.h"
#include"avl.h"
#include"gst.h"
#include"scanner.h"

char treeType = 'r';
char authorInfo = 0;

void Fatal(char *fmt, ...) {
	va_list ap;
	fprintf(stderr,"An error occured: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(-1);
}

void cleanStr(char* str, int strOrToken);
void processOptions(int argc, char** argv);
void processCommandsGreen(FILE* fp, GST* greenTree);
void processCommandsAVL(FILE* fp, AVL* avlTree);

int main(int argc, char** argv) {
	if (argc == 1) Fatal("%d arguments!\n", argc - 1);

	processOptions(argc, argv);

	if(authorInfo == 1) {
		printf("Author name: Aashman Gupta.\n");
		exit(0);
	}

	GST* greenTree;
	AVL* avlTree;

	if(treeType == 'g') {
		greenTree = newGST(displaySTRING, compareSTRING, freeSTRING);
	}
	else {
		avlTree = newAVL(displaySTRING, compareSTRING, freeSTRING);
	}

	char* str;

	FILE* corpus = fopen(argv[argc - 2], "r");
	int token = 0;
	int quoted = stringPending(corpus);
	if(quoted != 0) {
		str = readString(corpus);
		token = 11;
	}
	else {
		token = 22;
		str = readToken(corpus);
	}

	while(!feof(corpus)) {
		//printf("reading\n");		
		cleanStr(str, token);
		if(isalpha(str[0])) {
			//printf("Enters isAlpha\n");
			if(treeType == 'g') {
				//printf("Inserts the string.\n");
				insertGST(greenTree, newSTRING(str));
			}
			else {
				insertAVL(avlTree, newSTRING(str));
			}
		}

		quoted = stringPending(corpus);
		if(quoted != 0) {
			str = readString(corpus);
			token = 11;
		}	
		else {
			token = 22;
			str = readToken(corpus);
		}	
	}
	
	FILE* commands = fopen(argv[argc - 1], "r");
	if(treeType == 'g') {
		processCommandsGreen(commands, greenTree);
	}
	else {
		processCommandsAVL(commands, avlTree);
	}

return 0;
}

void processCommandsGreen(FILE* fp, GST* greenTree) {
	int quoted = stringPending(fp);
	char* str;
	int buffer = 0;	//1: insert,  2: delete, 3: frequency
	void* foundVal = newSTRING("hello");
	int token;

	if(quoted != 0) {
		str = readString(fp);
		token = 11;
	}
	else {
		token = 22;
		str = readToken(fp);
	}
	
	while(!feof(fp)) {
		cleanStr(str, token);
		if(buffer != 0) {
			if(isalpha(str[0])) {
				if(buffer == 1) {
					insertGST(greenTree, newSTRING(str));
				}
				
				else if(buffer == 2) {
					foundVal = findGST(greenTree, newSTRING(str));
					if(foundVal == 0) {
						printf("Value ");
						printf("%s", str);
        				printf(" not found.\n");
						foundVal = newSTRING("HI");
					}
					else {
					deleteGST(greenTree, newSTRING(str)); 
					}
				}
				
				else if(buffer == 3) {
					int x = findGSTcount(greenTree, newSTRING(str));
					printf("Frequency of %s: %d\n", str, x);
				}
			}
			buffer = 0;
		}

		else if((str[0] == 'i') && (str[1] == '\0')) {
			buffer = 1;
		}
		
		else if((str[0] == 'd') && (str[1] == '\0')) {
			buffer = 2;
		}
		
		else if((str[0] == 'f') && (str[1] == '\0')){
			buffer = 3;
		}
		
		else if((str[0] == 's') && (str[1] == '\0')) {
			displayGST(greenTree, stdout);
			buffer = 0;
		}

		else if((str[0] == 'r') && (str[1] == '\0')){
			statisticsGST(greenTree, stdout);
			buffer = 0;
		}
	
		quoted = stringPending(fp);
		if(quoted != 0) {
			str = readString(fp);
			token = 11;
		}
		else {
			token = 22;
			str = readToken(fp);
		} 
	}
}

void processCommandsAVL(FILE* fp, AVL* avlTree) {
	int quoted = stringPending(fp);
	char* str;
	int buffer = 0;	//1: insert,  2: delete, 3: frequency
	void* foundVal = newSTRING("HELLO");
	int token;

	if(quoted != 0) {
		token = 11;
		str = readString(fp);
	}
	else {
		token = 22;
		str = readToken(fp);
	}
	
	while(!feof(fp)) {
		cleanStr(str, token);
		if(buffer != 0) {
			if(isalpha(str[0])) {
				if(buffer == 1) {
					insertAVL(avlTree, newSTRING(str));
				}
				
				else if(buffer == 2) {
					foundVal = findAVL(avlTree, newSTRING(str));
					if(foundVal == 0) {
						printf("Value ");
						printf("%s", str);
						printf(" not found.\n");
						foundVal = newSTRING("HI");
					}
					else {
						deleteAVL(avlTree, newSTRING(str));
					}
				}
				
				else if(buffer == 3) {
					int y = findAVLcount(avlTree, newSTRING(str));
					printf("Frequency of %s: %d\n", str, y);
				}
			}
			buffer = 0;
		}

		else if((str[0] == 'i') && (str[1] == '\0')) {
			buffer = 1;
		}
		
		else if((str[0] == 'd') && (str[1] == '\0')) {
			buffer = 2;
		}
		
		else if((str[0] == 'f') && (str[1] == '\0')){
			buffer = 3;
		}
		
		else if((str[0] == 's') && (str[1] == '\0')) {
			displayAVL(avlTree, stdout);
			buffer = 0;
		}

		else if((str[0] == 'r') && (str[1] == '\0')){
			statisticsAVL(avlTree, stdout);
			buffer = 0;
		}
	
		quoted = stringPending(fp);
		if(quoted != 0) {
			token = 11;
			str = readString(fp);
		}
		else {
			token = 22;
			str = readToken(fp);
		} 
	}
}

void processOptions(int argc, char** argv) {
	int argIndex = 1;
	char argChar = 0;

	while(argIndex < argc) {
		if(argv[argIndex][0] == '-') {
			argChar = argv[argIndex][1];
			if(argChar == 'g') {
				treeType = 'g';
			}
			if(argChar == 'v') {
				authorInfo = 1;
			}
		}
		argIndex++;
	}
}

//credit to stack overflow for helping me with the logic here.
void cleanStr(char* str, int strOrToken) {		//11 for string, 22 for token
	int source = 0;
	int destination = 0;

	while(str[source] != '\0') {
		if((strOrToken == 11) && (source != 0) && (str[source + 1] != '\0') && (str[source] == ' ')) {
			if((isalpha(str[destination - 1])) && (destination != 0)) {
				str[destination] = str[source];
				destination++;
				source++;
			}
			/*else if(isspace(str[source - 1])) {
				str[destination] = str[source];
				destination++;
				source++;
			}*/
			while(!isalpha(str[source])) {
				if(str[source] == '\0') {
					str[destination - 1] = '\0';
					return;
				}
				source++;
			}
		}

		if(isalpha(str[source])) {
			str[destination] = tolower(str[source]);
			destination++;
		}
		source++;
	}
	str[destination] = '\0';
}

