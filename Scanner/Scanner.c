#include "Core.h"
// Author: Wenzhuo Wang

// global variables
FILE *readFile;
int token;
int constant;
char identifier[20];

// Initializes the scanner
void Scanner(char* filename) {
	readFile = fopen(filename, "r");
	if(readFile == NULL){
		printf("ERROR: Could not open file %s.\n", filename);
		token = EOS;
		return;
	}
}

// Find the next token
void nextToken() {
	char read;
	char nextRead;
	int stop = fscanf(readFile, "%c", &read);
	while((read == ' ' || read == '\n' || read == '\t' || read == '\r') && stop != -1){
		stop = fscanf(readFile, "%c", &read);
	}
	if(stop == -1){
		token = EOS;
	}
	else{
		
		switch(read){
			// one character special 
			case ';':
				token = SEMICOLON;
				break;
			case '(':
				token = LPAREN;
				break;
			case ')':
				token = RPAREN;
				break;
			case ',':
				token = COMMA;
				break;
			case '!':
				token = NEGATION;
				break;
			case '+':
				token = ADD;
				break;
			case '-':
				token = SUB;
				break;
			case '*':
				token = MULT;
				break;
			// two character special
			case '=':
				fscanf(readFile, "%c", &nextRead);
				if(nextRead == '='){
					token = EQUAL;
				}
				else{
					token = ASSIGN;
					ungetc(nextRead, readFile);	// put back the read non-=
				}
				break;
			case '<':
				fscanf(readFile, "%c", &nextRead);
				if(nextRead == '='){
					token = LESSEQUAL;
				}
				else{
					token = LESS;
					ungetc(nextRead, readFile);	// put back the read non-=
				}
				break;
			default: {
				char id[20] = "";
				if(read >= '0' && read <= '9'){						// starting with digit(constant/id)
					int count = 0;
					id[count] = read;
					int ifID = 0;
					while(1){
						int check = fscanf(readFile, "%c", &nextRead);
						count += 1;
						if((nextRead >= 'a' && nextRead <= 'z') || (nextRead >= 'A' && nextRead <= 'Z')){	// if alphabet appears, it is an id
							ifID = 1;
						}
						if(nextRead == ' ' || ((nextRead < 'a' || nextRead > 'z') && (nextRead < 'A' || nextRead > 'Z') && (nextRead < '0' || nextRead > '9'))){
							ungetc(nextRead, readFile);
							break;
						}
						if(check == -1){
							break;
						}
						id[count] = nextRead;
					}
					// finish reading the token
					if(ifID){
						token = ID;
						strcpy(identifier, id);
					}
					else{
						if(id[0]-'0' == 0 && id[1]-'0' > 0){	// error of leading 0
							printf("ERROR: Constant with leading zero.\n");
							token = EOS;
							return;
						}
					
						char realDigit[count+1];		// slice the unread digit among '0000'
						int i;
						for(i = 0; i < count; i++){
							realDigit[i] = id[i];
						}
						constant = atoi(realDigit);
						if(constant > 1023){	// error of exceeding limit
							printf("ERROR: Constant exceeds the limit of 1023.\n");
							token = EOS;
							return;
						}
						token = CONST;
					}
					
				}
				else if((read >= 'a' && read <= 'z') || (read >= 'A' && read <= 'Z')){			// identifier or keyword
					int count = 0;
					id[count] = read;
			
					while(1){			// read identifier until end/whitespace/non-alphabet&non-digit
						int check = fscanf(readFile, "%c", &nextRead);
						count += 1;
						if(check == -1){
							break;
						}
 						if(nextRead == ' ' || ((nextRead < 'a' || nextRead > 'z') && (nextRead < 'A' || nextRead > 'Z') && (nextRead < '0' || nextRead > '9'))){
							ungetc(nextRead, readFile);	// go back to id
							break;
						}
						id[count] = nextRead;
					}
					// check keyword	'
					if(strcmp(id, "program") == 0){
						token = PROGRAM;
					}
					else if(strcmp(id, "begin") == 0){
						token = BEGIN;
					}
					else if(strcmp(id, "end") == 0){
						token = END;
					}
					else if(strcmp(id, "int") == 0){
						token = INT;
					}
					else if(strcmp(id, "define") == 0){
						token = DEFINE;
					}
					else if(strcmp(id, "endfunc") == 0){
						token = ENDFUNC;
					}
					else if(strcmp(id, "class") == 0){
						token = CLASS;
					}
					else if(strcmp(id, "extends") == 0){
						token = EXTENDS;
					}
					else if(strcmp(id, "endclass") == 0){
						token = ENDCLASS;
					}
					else if(strcmp(id, "if") == 0){
						token = IF;
						break;
					}
					else if(strcmp(id, "then") == 0){
						token = THEN;
					}
					else if(strcmp(id, "else") == 0){
						token = ELSE;
					}
					else if(strcmp(id, "while") == 0){
						token = WHILE;
					}
					else if(strcmp(id, "endwhile") == 0){
						token = ENDWHILE;
					}
					else if(strcmp(id, "endif") == 0){
						token = ENDIF;
					}
					else if(strcmp(id, "or") == 0){
						token = OR;
					}
					else if(strcmp(id, "input") == 0){
						token = INPUT;
					}
					else if(strcmp(id, "output") == 0){
						token = OUTPUT;
					}
					else {				//id
						token = ID;
						strcpy(identifier, id);
					}
				}
				else{		// invalid char
					printf("ERROR: Invalid input %c.\n", read);
					token = EOS;
					return;
				}
				break;
			}
		}
	}
}

// Returns the current token
int currentToken() {
	return token;
}

// If the current token is ID, returns the string value of the identifier
void getID(char* value) {
	strcpy(value, identifier);
}

// If the current token is CONST, returns the value of the constant
int getCONST() {
	return constant;
}
