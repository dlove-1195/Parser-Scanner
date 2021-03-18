#include <stdio.h>
#include <string.h>
#include "Scanner.h"

// CHANGE NOTHING IN THIS FILE

// Compile with the command "gcc main.c"
// Run with the command "a.out FILENAME" or "a.out FILENAME verbose"
int main(int argc,char* argv[]) {
  Scanner(argv[1]);
  
  // If the optional argument "verbose" is given, will print out
  // string values instead of the enumeration value (for testing)
  int verbose = (argc > 2) && (strcmp(argv[2], "verbose") == 0);

  nextToken();
  while (currentToken() != EOS) {
	// Get the current token
	int current = currentToken();
	  
    // If verbose, convert to char string 
	char str[10];
	if (verbose == 1) {
		switch (current) {
		  case PROGRAM : strcpy(str, "PROGRAM"); break;
		  case BEGIN : strcpy(str, "BEGIN"); break;
		  case END : strcpy(str, "END"); break;
		  case INT : strcpy(str, "INT"); break;
		  case DEFINE : strcpy(str, "DEFINE"); break;
		  case ENDFUNC : strcpy(str, "ENDFUNC"); break;
		  case CLASS : strcpy(str, "CLASS"); break;
		  case EXTENDS : strcpy(str, "EXTENDS"); break;
		  case ENDCLASS : strcpy(str, "ENDCLASS"); break;
		  case IF : strcpy(str, "IF"); break;
		  case THEN : strcpy(str, "THEN"); break;
		  case ELSE : strcpy(str, "ELSE"); break;
		  case WHILE : strcpy(str, "WHILE"); break;
		  case ENDWHILE : strcpy(str, "ENDWHILE"); break;
		  case ENDIF : strcpy(str, "ENDIF"); break;
		  case SEMICOLON : strcpy(str, "SEMICOLON"); break;
		  case LPAREN : strcpy(str, "LPAREN"); break;
		  case RPAREN : strcpy(str, "RPAREN"); break;
		  case COMMA : strcpy(str, "COMMA"); break;
		  case ASSIGN : strcpy(str, "ASSIGN"); break;
		  case NEGATION : strcpy(str, "NEGATION"); break;
		  case OR : strcpy(str, "OR"); break;
		  case EQUAL : strcpy(str, "EQUAL"); break;
		  case LESS : strcpy(str, "LESS"); break;
		  case LESSEQUAL : strcpy(str, "LESSEQUAL"); break;
		  case ADD : strcpy(str, "ADD"); break;
		  case SUB : strcpy(str, "SUB"); break;
		  case MULT : strcpy(str, "MULT"); break;
		  case INPUT : strcpy(str, "INPUT"); break;
		  case OUTPUT : strcpy(str, "OUTPUT"); break;
		  case CONST : strcpy(str, "CONST"); break;
		  case ID : strcpy(str, "ID"); break;
		  case EOS : strcpy(str, "EOS"); break;
		}
	}

    // Pring the current token, with any extra data needed
    (verbose==0) ? printf("%d", current) : printf("%s", str);
    if (currentToken() == ID) {
      char value[20];
      getID(value);
      printf("[%s]", value);
    } else if (currentToken() == CONST) {
      int value = getCONST();
      printf("[%d]", value);
    }
    printf("\n");

    // Advance to the next token
    nextToken();
  }
  


  return 0;
}
