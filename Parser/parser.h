#include "Scanner.h"
#include  <string.h>


/* 
 * Structure declarations here, definitions below
 */

typedef struct program_node program_node;
typedef struct decl_seq_node decl_seq_node;
typedef struct stmt_seq_node stmt_seq_node;
typedef struct decl_node decl_node;
typedef struct id_list_node id_list_node;
typedef struct stmt_node stmt_node;
typedef struct assign_node assign_node;
typedef struct if_node if_node;
typedef struct loop_node loop_node;
typedef struct in_node in_node;
typedef struct out_node out_node;
typedef struct expr_node expr_node;
typedef struct term_node term_node;
typedef struct factor_node factor_node;
typedef struct cond_node cond_node;
typedef struct cmpr_node cmpr_node;



/* 
 * struct definitions below
 * Each struct represents a type of node in the parse tree
 * Each nonterminals of the grammar should have a type of node
 */

// program_node represents the root of the parse tree
struct program_node {
	decl_seq_node *dsn; // Link to the decl_seq child or root
	stmt_seq_node *ssn; // Link to the stmt_seq child of root
};

// decl_seq_node represents decl_seq nodes in the parse tree
struct decl_seq_node{
	decl_node *decl;
	decl_seq_node *declSeq;
};

// stmt_seq_node represents stmt_seq nodes in the parse tree
struct stmt_seq_node{
	stmt_node *stmt;
	stmt_seq_node *stmtSeq;
};

struct decl_node{
	id_list_node *idList;
};

struct id_list_node{
	char id[10];		// assume identifier no more than 10 characters
	int value;
	id_list_node *idList;
	id_list_node *next;
	int initialized;
};

struct stmt_node{
	int identity;
	assign_node *assignNode;
	if_node *ifNode;
	loop_node *loopNode;
	in_node *inNode;
	out_node *outNode;
};

struct assign_node{
	char id[10];
	expr_node *expr;
};

struct if_node{
	cond_node *cond;
	stmt_seq_node *stmt_seq;
	stmt_seq_node *stmt_seq_seq;
};

struct loop_node{
	cond_node *cond;
	stmt_seq_node *stmt_seq;
};

struct in_node{
	char id[10];
};

struct out_node{
	expr_node *expr;
};

struct expr_node{
	term_node *term;
	expr_node *expr;
	int op;
	int value;
};

struct term_node{
	factor_node *factor;
	term_node *term;
	int value;
};

struct factor_node{
	int constant;
	char id[10];
	expr_node *expr;
	int identity;
	int value;
};

struct cond_node{
	cmpr_node *cmpr;
	cond_node *cond;
	int value;
};

struct cmpr_node{
	expr_node *expr;
	expr_node *expr_expr;
	int identity;
	int value;
};

/*
 * Function declarations here, definitions below
 */

void parser_program(program_node *prog, char* codeFileName);
void printer_program(program_node *prog);
void executor_program(program_node *prog, char* dataFileName);
void delete_program(program_node *prog);

void parser_decl_seq(decl_seq_node *dsn);
void printer_decl_seq(decl_seq_node *prog, int tabbing);
void executor_decl_seq(decl_seq_node *prog);
void delete_decl_seq(decl_seq_node *prog);

void parser_stmt_seq(stmt_seq_node *ssn);
void printer_stmt_seq(stmt_seq_node *prog, int tabbing);
void executor_stmt_seq(stmt_seq_node *prog);
void delete_stmt_seq(stmt_seq_node *prog);

void parser_decl(decl_node *decl);
void printer_decl(decl_node *prog);
void executor_decl(decl_node *prog);
void delete_decl(decl_node *prog);

void parser_id_list(id_list_node *idlist);
void printer_id_list(id_list_node *prog);
void executor_id_list(id_list_node *prog);
void delete_id_list(id_list_node *prog);

void parser_stmt(stmt_node *prog);
void printer_stmt(stmt_node *prog, int tabbing);
void executor_stmt(stmt_node *prog);
void delete_stmt(stmt_node *prog);

void parser_assign(assign_node *prog);
void printer_assign(assign_node *prog);
void executor_assign(assign_node *prog);
void delete_assign(assign_node *prog);

void parser_expr(expr_node *prog);
void printer_expr(expr_node *prog);
void executor_expr(expr_node *prog);
void delete_expr(expr_node *prog);

void parser_term(term_node *prog);
void printer_term(term_node *prog);
void executor_term(term_node *prog);
void delete_term(term_node *prog);

void parser_factor(factor_node *prog);
void printer_factor(factor_node *prog);
void executor_factor(factor_node *prog);
void delete_factor(factor_node *prog);

void parser_in(in_node *prog);
void printer_in(in_node *prog);
void executor_in(in_node *prog);
void delete_in(in_node *prog);

void parser_out(out_node *prog);
void printer_out(out_node *prog);
void executor_out(out_node *prog);
void delete_out(out_node *prog);

void parser_if(if_node *prog);
void printer_if(if_node *prog, int tabbing);
void executor_if(if_node *prog);
void delete_if(if_node *prog);

void parser_loop(loop_node *prog);
void printer_loop(loop_node *prog, int tabbing);
void executor_loop(loop_node *prog);
void delete_loop(loop_node *prog);

void parser_cond(cond_node *prog);
void printer_cond(cond_node *prog);
void executor_cond(cond_node *prog);
void delete_cond(cond_node *prog);

void parser_cmpr(cmpr_node *prog);
void printer_cmpr(cmpr_node *prog);
void executor_cmpr(cmpr_node *prog);
void delete_cmpr(cmpr_node *prog);


// global variable
id_list_node *list_start = NULL;	// statr and traverse pointers for every decl
id_list_node *list_end = NULL;
id_list_node *startNode = NULL;		// pointers for the decl-seq
id_list_node *endNode = NULL;
char fileName[20];
FILE *input;

/* 
 * Function definitions below
 * Each struct should have 4 functions associated with it:
 * parser_* - this handles the recursive descent parsing, and builds the parse tree
 * printer_* - this nicely prints the program from the parse tree, to verify the tree
 * executor_* - this walks over the tree and executes the statements of the program
 * delete_* - this recursively dismantles the tree, releasing allocated memory
 */

/* program functions */

void parser_program(program_node *prog, char* codeFileName) {
	// Initialize the scanner
	Scanner(codeFileName);
	
	nextToken();
	if (currentToken() != PROGRAM) {
		printf("ERROR: Expected PROGRAM token\n");
		exit(0);
	}
	nextToken(); // Consume the PROGRAM token
	if(currentToken() != BEGIN){	// if there is declaration
		// Scanner should be at the begining of the declaration sequence
		(*prog).dsn = (decl_seq_node*)malloc(sizeof(decl_seq_node));
		parser_decl_seq((*prog).dsn);
	}
	else{
		(*prog).dsn = NULL;
	}

	// After parsing the declaration sequence, should have BEGIN token
	if (currentToken() != BEGIN) {
		printf("ERROR: Expected BEGIN token\n");
		exit(0);
	}
	nextToken(); // Consume the BEGIN token
	
	if(currentToken() != END){	// is there is statement
		// Scanner should be at the start of the statement sequence
		(*prog).ssn = (stmt_seq_node*)malloc(sizeof(stmt_seq_node));
		parser_stmt_seq((*prog).ssn);
	}
	else{
		(*prog).ssn = NULL;
	}
	
	// After parsing the declaration sequence, should have END token, then EOS
	if (currentToken() != END) {
		printf("ERROR: Expected BEGIN token\n");
		exit(0);
	}
	nextToken(); // Consume the END token
	if (currentToken() != EOS) {
		printf("ERROR: Unexpected token after END\n");
		exit(0);
	}
}
  
void printer_program(program_node *prog) {
	printf("program\n");
	printer_decl_seq((*prog).dsn, 1);
	printf("begin\n");
	printer_stmt_seq((*prog).ssn, 1);
	printf("end\n");
}

void executor_program(program_node *prog, char* dataFileName) {
	strcpy(fileName, dataFileName);
	if((*prog).dsn != NULL){
		executor_decl_seq((*prog).dsn);
	}
	if((*prog).ssn != NULL){
		executor_stmt_seq((*prog).ssn);
	}
}

void delete_program(program_node *prog) {
	if((*prog).dsn != NULL){
		delete_decl_seq((*prog).dsn);}
	if((*prog).ssn != NULL){
		delete_stmt_seq((*prog).ssn); }
	free((*prog).dsn);
	free((*prog).ssn);
}


/* decl_seq functions */

void parser_decl_seq(decl_seq_node *dsn) {
	(*dsn).decl = (decl_node*)malloc(sizeof(decl_node));
	parser_decl((*dsn).decl);
	if (currentToken() == INT){
		(*dsn).declSeq = (decl_seq_node*)malloc(sizeof(decl_seq_node));
		parser_decl_seq((*dsn).declSeq);
	}
}
  
void printer_decl_seq(decl_seq_node *prog, int tabbing) {
	int i;
	if (prog != NULL){
		for (i = 0; i < tabbing; i++){
			printf("\t");
		}
		printer_decl((*prog).decl);
		printer_decl_seq((*prog).declSeq, tabbing);
	}
}

void executor_decl_seq(decl_seq_node *prog) {
	executor_decl((*prog).decl);
	if((*prog).declSeq != NULL){
		executor_decl_seq((*prog).declSeq);
	}
}

void delete_decl_seq(decl_seq_node *prog) {
	delete_decl((*prog).decl);
	if((*prog).declSeq != NULL){
		delete_decl_seq((*prog).declSeq);
	}
	free((*prog).decl);
	free((*prog).declSeq);
}


/* stmt_seq functions */

void parser_stmt_seq(stmt_seq_node *ssn) {
	(*ssn).stmt = (stmt_node*)malloc(sizeof(stmt_node));
	parser_stmt((*ssn).stmt);
	if(currentToken() != END && currentToken() != ENDIF && currentToken() != ENDWHILE && currentToken() != ELSE){
		(*ssn).stmtSeq = (stmt_seq_node*)malloc(sizeof(stmt_seq_node));
		parser_stmt_seq((*ssn).stmtSeq);
	}
}
  
void printer_stmt_seq(stmt_seq_node *prog, int tabbing) {
	if(prog != NULL){
		printer_stmt((*prog).stmt, tabbing);
		printer_stmt_seq((*prog).stmtSeq, tabbing);
	}
}

void executor_stmt_seq(stmt_seq_node *prog) {
	executor_stmt((*prog).stmt);
	if((*prog).stmtSeq != NULL){
		executor_stmt_seq((*prog).stmtSeq);
	}
}

void delete_stmt_seq(stmt_seq_node *prog) {
	delete_stmt((*prog).stmt);
	if((*prog).stmtSeq != NULL){
		delete_stmt_seq((*prog).stmtSeq);
	}
	free((*prog).stmt);
	free((*prog).stmtSeq);
}


/* decl functions */

void parser_decl(decl_node *decl) {
	if (currentToken() != INT){
		printf("ERROR: Expected INT token\n");
		exit(0);
	}
	list_start = NULL;
	list_end = NULL;
	nextToken();	// consume the INT token
	(*decl).idList = (id_list_node*)malloc(sizeof(id_list_node));
	parser_id_list((*decl).idList);
	if (currentToken() != SEMICOLON){
		printf("ERROR: Expected SEMICOLON token after the declaration\n");
		exit(0);
	}
	nextToken();	// consume the ; token
}
  
void printer_decl(decl_node *prog) {
	printf("int ");
	printer_id_list((*prog).idList);
	printf(";\n");
}

void executor_decl(decl_node *prog) {
	executor_id_list((*prog).idList);
}

void delete_decl(decl_node *prog) {
	delete_id_list((*prog).idList);
	free((*prog).idList);
}

/* id_list functions */

void parser_id_list(id_list_node *idlist) {
	if(currentToken() != ID){
		printf("ERROR: Expected identifier as variable name\n");
		exit(0);
	}
	getID((*idlist).id);
	nextToken();
	if(currentToken() == COMMA){
		nextToken();	// consume the , token
		(*idlist).idList = (id_list_node*)malloc(sizeof(id_list_node));
		parser_id_list((*idlist).idList);
	}
	
}
  
void printer_id_list(id_list_node *prog) {
	if(prog != NULL){
		printf((*prog).id);
		if((*prog).idList != NULL){
			printf(",");
			printer_id_list((*prog).idList);
		}
	}
}

void executor_id_list(id_list_node *prog) {
	if(list_start == NULL){
		list_start = prog;
		list_end = prog;
	}
	else{
		(*list_end).idList = prog;
		list_end = prog;
	}

	if(startNode == NULL){
		startNode = prog;
		endNode = startNode;
		while(endNode->idList != NULL){
			endNode->next = endNode->idList;
			endNode = endNode->next;
		}
	}
	else{
		endNode->next = prog;
		endNode = prog;
		while(endNode->idList != NULL){
			endNode->next = endNode->idList;
			endNode = endNode->next;
		}
	}

	/* check double declarations */	
	id_list_node *traverse;
	id_list_node *inner;
	traverse = prog;
	inner = startNode;
	while(traverse != NULL){
		while(inner != NULL){
			if(traverse != inner && strcmp(traverse->id,inner->id) == 0){
				printf("ERROR: Declare variable %s twice\n", traverse->id);
				exit(0);
			}
			inner = inner->next;

		}
		traverse = traverse->next;
	}
}

void delete_id_list(id_list_node *prog) {
	if((*prog).idList != NULL){
		delete_id_list((*prog).idList);
	}
	free((*prog).idList);
}


/* stmt functions */

void parser_stmt(stmt_node *prog) {
	if(currentToken() == ID){
		(*prog).assignNode = (assign_node*)malloc(sizeof(assign_node));
		parser_assign((*prog).assignNode);
		(*prog).identity = 1;
	}
	else if(currentToken() == IF){
		(*prog).ifNode = (if_node*)malloc(sizeof(if_node));
		parser_if((*prog).ifNode);
		(*prog).identity = 2;
	}
	else if(currentToken() == WHILE){
		(*prog).loopNode = (loop_node*)malloc(sizeof(loop_node));
		parser_loop((*prog).loopNode);
		(*prog).identity = 3;
	}
	else if(currentToken() == INPUT){
		(*prog).inNode = (in_node*)malloc(sizeof(in_node));
		parser_in((*prog).inNode);
		(*prog).identity = 4;
	}
	else if(currentToken() == OUTPUT){
		(*prog).outNode = (out_node*)malloc(sizeof(out_node));
		parser_out((*prog).outNode);
		(*prog).identity = 5;
	}
	else{
		printf("ERROR: Expected ID/IF/WHILE/INPUT/OUTPUT token\n");
		exit(0);
	}
}
  
void printer_stmt(stmt_node *prog, int tabbing) {
	int i;
	if((*prog).identity == 1){
		for (i = 0; i < tabbing; i++){
			printf("\t");
		}
		printer_assign((*prog).assignNode);
	}
	else if((*prog).identity == 2){
		printer_if((*prog).ifNode, tabbing);
	}
	else if((*prog).identity == 3){
		printer_loop((*prog).loopNode, tabbing);
	}
	else if((*prog).identity == 4){
		for (i = 0; i < tabbing; i++){
			printf("\t");
		}
		printer_in((*prog).inNode);
	}
	else if((*prog).identity == 5){
		for (i = 0; i < tabbing; i++){
			printf("\t");
		}
		printer_out((*prog).outNode);
	}
}

void executor_stmt(stmt_node *prog) {
	if((*prog).identity == 1){
		executor_assign((*prog).assignNode);
	}
	else if((*prog).identity == 2){
		executor_if((*prog).ifNode);
	}
	else if((*prog).identity == 3){
		executor_loop((*prog).loopNode);
	}
	else if((*prog).identity == 4){
		executor_in((*prog).inNode);
	}
	else if((*prog).identity == 5){
		executor_out((*prog).outNode);
	}
}

void delete_stmt(stmt_node *prog) {
	if((*prog).identity == 1){
		delete_assign((*prog).assignNode);
		free(prog->assignNode);
	}
	else if((*prog).identity == 2){
		delete_if((*prog).ifNode);
		free(prog->ifNode);
	}
	else if((*prog).identity == 3){
		delete_loop((*prog).loopNode);
		free(prog->loopNode);
	}
	else if((*prog).identity == 4){
		delete_in((*prog).inNode);
		free(prog->inNode);
	}
	else if((*prog).identity == 5){
		delete_out((*prog).outNode);
		free(prog->outNode);
	}
}


/* assign functions */
void parser_assign(assign_node *prog){
	getID((*prog).id);
	nextToken();	// consume the ID token
	if(currentToken() != ASSIGN){
		printf("ERROR: Expected ASSIGN token\n");
		exit(0);
	}
	nextToken();	// consume the ASSIGN token
	(*prog).expr = (expr_node*)malloc(sizeof(expr_node));
	parser_expr((*prog).expr);
	if(currentToken() != SEMICOLON){
		printf("ERROR: Expected SEMICOLON token\n");
		exit(0);
	}
	nextToken();	// consume the ; token
}

void printer_assign(assign_node *prog){
	printf((*prog).id);
	printf("=");
	printer_expr((*prog).expr);
	printf(";\n");
}
void executor_assign(assign_node *prog){
	id_list_node *traverse = startNode;
	while(1){
		if(traverse == NULL){
			printf("ERROR: Expected declaration of %s before assigning value to it\n", prog->id);
			exit(0);
		}
		if(strcmp((*traverse).id,(*prog).id) == 0){
			executor_expr((*prog).expr);
			(*traverse).value = prog->expr->value;	// assign value
			traverse->initialized = 1;		// indicate value is assigned
			break;
		}
		traverse = (*traverse).next;
	}
}

void delete_assign(assign_node *prog){
	delete_expr((*prog).expr);
	free((*prog).expr);
}


/* expr functions */
void parser_expr(expr_node *prog){
	(*prog).term = (term_node*)malloc(sizeof(term_node));
	parser_term((*prog).term);
	if(currentToken() == ADD){
		nextToken();	// consume the + token
		(*prog).expr = (expr_node*)malloc(sizeof(expr_node));
		parser_expr((*prog).expr);
		(*prog).op = 0;
	}
	else if(currentToken() == SUB){
		nextToken();	// consume the - token
		(*prog).expr = (expr_node*)malloc(sizeof(expr_node));
		parser_expr((*prog).expr);
		(*prog).op = 1;
	}	
}

void printer_expr(expr_node *prog){
	printer_term((*prog).term);
	if((*prog).expr != NULL){
		if((*prog).op == 0){
			printf("+");
		}
		else if ((*prog).op == 1){
			printf("-");
		}
		printer_expr((*prog).expr);
	}
}
void executor_expr(expr_node *prog){
	executor_term((*prog).term);
	if((*prog).expr == NULL){
		(*prog).value = prog->term->value;
	}
	else{
		executor_expr((*prog).expr);
		if((*prog).op == 0){
			(*prog).value = prog->term->value + prog->expr->value ;
		}
		else if((*prog).op == 1){
			(*prog).value = prog->term->value - prog->expr->value ;
		}
	}
}
void delete_expr(expr_node *prog){
	delete_term((*prog).term);
	if(((*prog).expr) != NULL){
		delete_expr((*prog).expr);
	}
	free((*prog).term);
	free((*prog).expr);
}


/* term functions */
void parser_term(term_node *prog){
	(*prog).factor = (factor_node*)malloc(sizeof(factor_node));
	parser_factor((*prog).factor);
	if(currentToken() == MULT){
		nextToken();	// consume the * token
		(*prog).term = (term_node*)malloc(sizeof(term_node));
		parser_term((*prog).term);
	}
}

void printer_term(term_node *prog){
	printer_factor((*prog).factor);
	if((*prog).term != NULL){
		printf("*");
		printer_term((*prog).term);
	}
}
void executor_term(term_node *prog){
	executor_factor((*prog).factor);
	if((*prog).term == NULL){
		(*prog).value = prog->factor->value;
	}
	else{
		executor_term((*prog).term);
		(*prog).value = prog->factor->value * prog->term->value;
	}
}
void delete_term(term_node *prog){
	delete_factor((*prog).factor);
	if(((*prog).term) != NULL){
		delete_term((*prog).term);
	}
	free((*prog).factor);
	free((*prog).term);
}


/* factor funtions */
void parser_factor(factor_node *prog){
	if(currentToken() == CONST){
		(*prog).constant = getCONST();
		(*prog).identity = 1;
		nextToken();	// consume the CONST token
	}
	else if(currentToken() == ID){
		getID((*prog).id);
		(*prog).identity = 2;
		nextToken();	// consume the ID token
	}
	else if(currentToken() == LPAREN){
		nextToken();	// consume the ( token
		(*prog).identity = 3;
		(*prog).expr = (expr_node*)malloc(sizeof(expr_node));
		parser_expr((*prog).expr);
		if(currentToken() != RPAREN){
			printf("ERROR: Expected RPAREN token\n");
			exit(0);
		}
		nextToken();	// consume the ) token
	}
	else{
		printf("ERROR: Expected CONST/ID/LPAREN token\n");
		exit(0);
	}
}
void printer_factor(factor_node *prog){
	if((*prog).identity == 1){
		printf("%d", (*prog).constant);
	}
	else if((*prog).identity == 2){
		printf((*prog).id);
	}
	if((*prog).identity == 3){
		printf("(");
		printer_expr((*prog).expr);
		printf(")");
	}
}
void executor_factor(factor_node *prog){
	if((*prog).identity == 1){
		(*prog).value = (*prog).constant;
	}
	else if((*prog).identity == 2){
		id_list_node *traverse = startNode;
		while(1){
			if(traverse == NULL){
				printf("ERROR: variable %s is not initialized with a value\n", (*prog).id);
				exit(0);
			}
			if(strcmp((*traverse).id,(*prog).id) == 0){
				if(traverse->initialized != 1){
					printf("ERROR: variable %s is not initialized with a value\n", (*prog).id);
					exit(0);
				}
				(*prog).value = (*traverse).value;	// find value of variable
				break;
			}
			traverse = (*traverse).next;
		}
	}
	else if((*prog).identity == 3){
		executor_expr((*prog).expr);
		(*prog).value = prog->expr->value;
	}
}
void delete_factor(factor_node *prog){
	if(((*prog).expr) != NULL){
		delete_expr((*prog).expr);
	}

	free((*prog).expr);
}


/* in functions */
void parser_in(in_node *prog){
	nextToken();	// consume the INPUT token
	if(currentToken() != ID){
		printf("ERROR: Expected ID token after input\n");
		exit(0);
	}
	getID((*prog).id);
	nextToken();	// consume the ID token
	if(currentToken() != SEMICOLON){
		printf("ERROR: Expected SEMICOLON token after input statement\n");
		exit(0);
	}
	nextToken();	// consume the ; token
}
void printer_in(in_node *prog){
	printf("input ");
	printf((*prog).id);
	printf(";\n");
}
void executor_in(in_node *prog){
	if(input == NULL){
		input = fopen(fileName, "r");
	}
	int store;
	fscanf(input, "%d", &store);
	id_list_node *traverse = startNode;
	while(1){
		if(traverse == NULL){
			printf("ERROR: Expected declaration brefore inputting value to %s\n", prog->id);
			exit(0);
		}
		if(strcmp((*traverse).id,(*prog).id) == 0){
			(*traverse).value = store;	// assign value
			traverse->initialized = 1;
			break;
		}
		traverse = (*traverse).next;
	}	
}
void delete_in(in_node *prog){
	fclose(input);
}


/* out funtions */
void parser_out(out_node *prog){
	nextToken();	// consume the OUTPUT token
	(*prog).expr = (expr_node*)malloc(sizeof(expr_node));
	parser_expr((*prog).expr);
	if(currentToken() != SEMICOLON){
		printf("ERROR: Expected SEMICOLON token after output statement\n");
		exit(0);
	}
	nextToken();	// consume the ; token
}
void printer_out(out_node *prog){
	printf("output ");
	printer_expr((*prog).expr);
	printf(";\n");
}
void executor_out(out_node *prog){
	executor_expr((*prog).expr);
	printf("%d\n", prog->expr->value);
}
void delete_out(out_node *prog){
	delete_expr((*prog).expr);
	free((*prog).expr);
}


/* if functions */
void parser_if(if_node *prog){
	nextToken();	// consume the IF token
	(*prog).cond = (cond_node*)malloc(sizeof(cond_node));
	parser_cond((*prog).cond);
	if(currentToken() != THEN){
		printf("ERROR: Expected THEN token after the if-condition\n");
		exit(0);
	}
	nextToken();	// consume the THEN token
	(*prog).stmt_seq = (stmt_seq_node*)malloc(sizeof(stmt_seq_node));
	parser_stmt_seq((*prog).stmt_seq);
	if(currentToken() == ELSE){
		nextToken();	// consume the ELSE token
		(*prog).stmt_seq_seq = (stmt_seq_node*)malloc(sizeof(stmt_seq_node));
		parser_stmt_seq((*prog).stmt_seq_seq);
	}
	else{
		if(currentToken() != ENDIF){
			printf("ERROR: Expected ENDIF/ELSE token after the statements in if-block\n");
			exit(0);
		}
	}
	nextToken();	// consume the ENDIF token
	if (currentToken() != SEMICOLON){
		printf("ERROR: Expected SEMICOLON token after the ENDIF token\n");
		exit(0);
	}
	nextToken();	// consume the ; token
}
void printer_if(if_node *prog, int tabbing){
	int i;
	for (i = 0; i < tabbing; i++){
		printf("\t");
	}
	printf("if ");
	printer_cond(prog->cond);
	printf(" then\n");
	printer_stmt_seq(prog->stmt_seq, tabbing+1);
	if(prog->stmt_seq_seq != NULL){
		for (i = 0; i < tabbing; i++){
			printf("\t");
		}
		printf("else\n");
		printer_stmt_seq(prog->stmt_seq_seq, tabbing+1);
	}
	for (i = 0; i < tabbing; i++){
		printf("\t");
	}
	printf("endif;\n");
}
void executor_if(if_node *prog){
	executor_cond(prog->cond);
	if(prog->cond->value){
		executor_stmt_seq(prog->stmt_seq);
	}
	else{
		if(prog->stmt_seq_seq != NULL){
			executor_stmt_seq(prog->stmt_seq_seq);
		}
	}
}
void delete_if(if_node *prog){
	delete_cond(prog->cond);
	delete_stmt_seq(prog->stmt_seq);
	if(prog->stmt_seq_seq != NULL){
		delete_stmt_seq(prog->stmt_seq_seq);
		free(prog->stmt_seq_seq);
	}
	free(prog->cond);
	free(prog->stmt_seq);
}


/* while functions */
void parser_loop(loop_node *prog){
	nextToken();	// consume the WHILE token
	(*prog).cond = (cond_node*)malloc(sizeof(cond_node));
	parser_cond((*prog).cond);
	if(currentToken() != BEGIN){
		printf("ERROR: Expected BEGIN token after the while-condition\n");
		exit(0);
	}
	nextToken();	// consume the BEGIN token
	(*prog).stmt_seq = (stmt_seq_node*)malloc(sizeof(stmt_seq_node));
	parser_stmt_seq((*prog).stmt_seq);
	if(currentToken() != ENDWHILE){
		printf("ERROR: Expected ENDWHILE token after the statements\n");
		exit(0);
	}
	nextToken();	// consume the ENDWHILE token
	if (currentToken() != SEMICOLON){
		printf("ERROR: Expected SEMICOLON token after the ENDWHILE token\n");
		exit(0);
	}
	nextToken();	// consume the ; token
}
void printer_loop(loop_node *prog, int tabbing){
	int i;
	for (i = 0; i < tabbing; i++){
		printf("\t");
	}
	printf("while ");
	printer_cond(prog->cond);
	printf(" begin\n");
	printer_stmt_seq(prog->stmt_seq, tabbing+1);
	for (i = 0; i < tabbing; i++){
		printf("\t");
	}
	printf("endwhile;\n");
}
void executor_loop(loop_node *prog){
	executor_cond(prog->cond);
	while(prog->cond->value){
		executor_stmt_seq(prog->stmt_seq);
		executor_cond(prog->cond);
	}
}
void delete_loop(loop_node *prog){
	delete_cond(prog->cond);
	delete_stmt_seq(prog->stmt_seq);
	free(prog->cond);
	free(prog->stmt_seq);
}


/* cond functions */
void parser_cond(cond_node *prog){
	if(currentToken() == CONST || currentToken() == ID || currentToken() == LPAREN){
		(*prog).cmpr = (cmpr_node*)malloc(sizeof(cmpr_node));
		parser_cmpr((*prog).cmpr);
		if(currentToken() == OR){
			nextToken();	// conume the OR token
			(*prog).cond = (cond_node*)malloc(sizeof(cond_node));
			parser_cond((*prog).cond);
		}
	}
	else if(currentToken() == NEGATION){
		nextToken();	// consume the ! token
		if(currentToken() != LPAREN){
			printf("ERROR: Expected LPAREN token after the NEGATION token\n");
			exit(0);
		}
		nextToken();	// consume the ( token
		(*prog).cond = (cond_node*)malloc(sizeof(cond_node));
		parser_cond((*prog).cond);
		if(currentToken() != RPAREN){
			printf("ERROR: Expected RPAREN token\n");
			exit(0);
		}
		nextToken();	// consume the ) token
	}
	else{
		printf("ERROR: Expected CONST/ID/LPAREN/NEGATION token at condition\n");
		exit(0);
	}
}
void printer_cond(cond_node *prog){
	if(prog->cond == NULL){
		printer_cmpr(prog->cmpr);
	}
	else if(prog->cmpr == NULL){
		printf("!(");
		printer_cond(prog->cond);
		printf(")");
	}
	else{
		printer_cmpr(prog->cmpr);
		printf(" or ");
		printer_cond(prog->cond);
	}
}
void executor_cond(cond_node *prog){
	if(prog->cond == NULL){
		executor_cmpr(prog->cmpr);
		prog->value = prog->cmpr->value;
	}
	else if(prog->cmpr == NULL){
		executor_cond(prog->cond);
		if(prog->cond->value == 1){
			prog->value = 0;
		}
		else{
			prog->value = 1;
		}
	}
	else{
		executor_cmpr(prog->cmpr);
		executor_cond(prog->cond);
		if(prog->cmpr->value == 0 && (prog->cond->value == prog->cmpr->value)){
			prog->value = 0;
		}
		else{
			prog->value = 1;
		}
	}
}
void delete_cond(cond_node *prog){
	if(prog->cond != NULL){
		delete_cond(prog->cond);
	}
	if(prog->cmpr != NULL){
		delete_cmpr(prog->cmpr);
	}
	free(prog->cond);
	free(prog->cmpr);
}


/* cmpr functions */
void parser_cmpr(cmpr_node *prog){
	(*prog).expr = (expr_node*)malloc(sizeof(expr_node));
	parser_expr((*prog).expr);
	if(currentToken() == EQUAL){
		prog->identity = 1;
	}
	else if(currentToken() == LESS){
		prog->identity = 2;
	}
	else if(currentToken() == LESSEQUAL){
		prog->identity = 3;
	}
	else{
		printf("ERROR: Expected EQUAL/LESS/LESSEQUAL token\n");
		exit(0);
	}
	nextToken();	// consume the ==/</<= token
	(*prog).expr_expr = (expr_node*)malloc(sizeof(expr_node));
	parser_expr((*prog).expr_expr);
}
void printer_cmpr(cmpr_node *prog){
	printer_expr(prog->expr);
	if(prog->identity == 1){
		printf(" == ");
	}
	else if(prog->identity == 2){
		printf(" < ");
	}
	else if(prog->identity == 3){
		printf(" <= ");
	}
	printer_expr(prog->expr_expr);
}
void executor_cmpr(cmpr_node *prog){
	executor_expr(prog->expr);
	executor_expr(prog->expr_expr);
	if(prog->identity == 1){
		if(prog->expr->value == prog->expr_expr->value){
			prog->value = 1;
		}
		else{
			prog->value = 0;
		}
	}
	else if(prog->identity == 2){
		if(prog->expr->value < prog->expr_expr->value){
			prog->value = 1;
		}
		else{
			prog->value = 0;
		}
	}
	else if(prog->identity == 3){
		if(prog->expr->value <= prog->expr_expr->value){
			prog->value = 1;
		}
		else{
			prog->value = 0;
		}
	}
}
void delete_cmpr(cmpr_node *prog){
	delete_expr(prog->expr);
	delete_expr(prog->expr_expr);
	free(prog->expr);
	free(prog->expr_expr);
}
