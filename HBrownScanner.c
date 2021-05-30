/*	 
*	 CS 421: Automata Theory and Compiler Construction
*	 Spring Semester, 2020
*	 
*	 Scanner for Simplified Pascal Programming Language
*
*	     This program will open a file for reading that will be entered by the 
*	 user.  Once the file is open each line will be stored into a structure 
*	 for scanning.  The line will then be scanned and each token will be stored 
*	 in another structure and checked for valididty.  Program identifiers will
*	 be stored in a symbol table using hashing.  The maximum size of the table
*	 will be 32.  Finally, the line numbers will be displayed along with the 
*	 token numbers associated with reserved words, symbols, and identifiers.
* 
*	 Programmed by: Houston Brown
*	 Date Last Updated: 4/5/2020
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define Line_Max 81
#define Token_Max 13
#define File_Max 100
#define Size 32


/*---------------------------------------------------------------------------*/
/* Structures */

struct Line
{
	char line[Line_Max];
	int  lineNum;
	int  lnIdx;
};


struct Token
{
	char token[Token_Max];
	int  tknIdx;
};


struct hashTable
{
	char symTable[Size][Token_Max];
	int numEntries;
	bool afterBegin;
};
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Function Declarations */

FILE* openFile(FILE * filePtr_p);

void getToken(FILE * filePtr_p, struct Line * prgLine_p, 
			  struct hashTable * myTable);

void readSpace(struct Line * prgLine_p, struct Token * tknPtr_p);

void readLeftParen(FILE * filePtr_p, struct Line * prgLine_p, 
				   struct Token * tknPtr_p);
				   
void readRightParen(struct Line * prgLine_p, struct Token * tknPtr_p);

void readComma(struct Line * prgLine_p, struct Token * tknPtr_p);

void readSemicolon(struct Line * prgLine_p, struct Token * tknPtr_p);

void readColon(struct Line * prgLine_p, struct Token * tknPtr_p);

void readPlus(struct Line * prgLine_p, struct Token * tknPtr_p);

void readMinus(struct Line * prgLine_p, struct Token * tknPtr_p);

void readStar(struct Line * prgLine_p, struct Token * tknPtr_p);

void alphaLiteral(struct Line * prgLine_p, struct Token * tknPtr_p, 
				  struct hashTable * myTable_p);

void printTokenNumber(struct Line * prgLine_p, struct Token * tknPtr_p, 
					  struct hashTable * myTable_p);

void commentMode(FILE * filePtr, struct Line * prgLine_p);

int getHash(char id[]);

void hashInsert(char id[], struct hashTable * myTable_p);

int hashSearch(char id[], struct hashTable * myTable_p);

void printTable(struct hashTable * myTable_p);

void buildId(struct Line * progLine_p, struct hashTable * myTable_p);
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*  Driver  */

int main(void)
{
	FILE             * filePtr_p = openFile(filePtr_p);
	
	struct Line        progLine;			
	struct Line      * linePtr_p = &progLine;
	
	struct hashTable   myTable;
	myTable.afterBegin = false;
	struct hashTable * symbolTable = &myTable;

	printf("Line\t\t Token Type\t\t Token Specifier\n");    /* Header */
	
	while (!feof(filePtr_p))
	{
		fgets(linePtr_p->line, Line_Max, filePtr_p);
		getToken(filePtr_p, linePtr_p, symbolTable);
		memset(linePtr_p->line, ' ', sizeof(linePtr_p->line));
	}

	printTable(symbolTable);
	fclose(filePtr_p);
	return 0;
}


/*---------------------------------------------------------------------------
*	 Function: openFile
*	 -------------------
*
*	     This function will begin by attempting to open a text file for
*    reading.  If the file can not be found, then an appropriate error
*	 message will be displayed to the screen.
*
*	 filePtr_p: A pointer to the 
*
*	 Returning the file pointer or printing error message if file could
*	 not be found and opened.
*/

FILE* openFile(FILE * filePtr_p)
{
	char fileName[File_Max];
	memset(fileName, ' ', File_Max);
	
	printf("Please enter the name of the file that you wish to scan followed");
	printf(" by the file extension\n");
	
	gets(fileName);
	printf("\n\n");
	
	filePtr_p = fopen(fileName, "r");
	
	if (filePtr_p == NULL)
	{
		perror("File could not be found");
		exit(1);
	}
	
	return filePtr_p;
}

/*---------------------------------------------------------------------------
*	 Function: getToken
*	 -------------------
*
*	     This function will begin by clearing out the contents of the line
*	 c-string and reading in a line from the program file into line.  The 
*	 program line number will be incremented and the line index set to 0.
*	 The program line number will be printed and the token structure will be
*	 created.  The contents of the token c-string will be cleared out and the
*	 token index set to 0.  Next, it will loop through the contents of the 
*	 program line.  A switch statement will be set up to handle for any symbol
*	 or character that is encountered.
*
*	 filePtr_p: A pointer to the current line in the file being read
*	 progLine_p: A pointer to the program line structure being scanned
*
*	 Printing line number
*/

void getToken(FILE * filePtr_p, struct Line * prgLine_p, 
			  struct hashTable * myTable)
{
	prgLine_p->lineNum = prgLine_p->lineNum + 1;
	prgLine_p->lnIdx = 0;
	printf("%d", prgLine_p->lineNum);		

	struct Token newToken;
	newToken.tknIdx = 0;
	memset(newToken.token, ' ', sizeof(newToken.token) + 1);
	
	struct Token * tokenPtr_p = &newToken;

	/* Scan program line until end of line is reached */
	while (prgLine_p->line[prgLine_p->lnIdx] != '\0')
	{
		/* Switch statement for contents of program line */
		switch (prgLine_p->line[prgLine_p->lnIdx])
		{
			case ' ':
				readSpace(prgLine_p, tokenPtr_p);
				break;
				
			case '\t':
				readSpace(prgLine_p, tokenPtr_p);
				break;
				
			case '\n':
				readSpace(prgLine_p, tokenPtr_p);
				break;
				
			case '(':
				readLeftParen(filePtr_p, prgLine_p, tokenPtr_p);
				break;
			
			case ')' :
				readRightParen(prgLine_p, tokenPtr_p);
				break;
			
			case ',' :
				readComma(prgLine_p, tokenPtr_p);
				break;
			
			case ';' :
				readSemicolon(prgLine_p, tokenPtr_p);
				break;
			
			case ':' :
				readColon(prgLine_p, tokenPtr_p);
				break;
					
			case '+':
				readPlus(prgLine_p, tokenPtr_p);
				break;				
			
			case '-':
				readMinus(prgLine_p, tokenPtr_p);
				break;				
			
			case '*':
				readStar(prgLine_p, tokenPtr_p);
				break;				

			default:									
				alphaLiteral(prgLine_p, tokenPtr_p, myTable);    
				break;	
		}    /* closing switch statement */
	}    /* closing while loop */
}    /* closing getAToken function */

/*---------------------------------------------------------------------------
*	 Function: readSpace
*	 -------------------
*
*	     If a space is found in the program line then this function will 
*	 begin by clearing out what is stored in token and reset the token index   
*	 to 0.  Finally, the program line index will be incremented by one.
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 No output being performed
*/

void readSpace(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
}

/*---------------------------------------------------------------------------
*	 Function: readLeftParen
*	 -----------------------
*
*	     If a left parenthesis is found in the program line then this function
*	 will begin by checking the next character.  If the next character in the 
*	 line is an '*' then the commentMode function will be called.  Once the
*	 comment has ended the program line index will be incremented by one and 
*	 resetting the token index to 0.  If the next character is anything else
*	 then token will be cleared out and the token index reset to 0.  Then the
*	 left parenthesis will be stored to token from the program line and the
*	 appropriate token number will be printed.  Finally, the program line
*	 index will be incremented by one.
*
*	 filePtr_p: A pointer to the current line in the file being read
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 20 or echo printing the comment
*/

void readLeftParen(FILE * filePtr_p, struct Line * prgLine_p, 
				    struct Token * tknPtr_p)
{
	if (prgLine_p->line[prgLine_p->lnIdx + 1] == '*')
	{
		printf("\nComment:");
		commentMode(filePtr_p, prgLine_p);
		printf("\n\n");
		
		prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
		tknPtr_p->tknIdx = 0;
	}
	
	else
	{	
		memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
		tknPtr_p->tknIdx = 0;
		tknPtr_p->token[tknPtr_p->tknIdx] = 
		 prgLine_p->line[prgLine_p->lnIdx];	
		
		printf("\t\t  %d\n\n", 20);
		prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
	}
}    /* closing readLeftParen function */

/*---------------------------------------------------------------------------
*	 Function: readRightParen
*	 ------------------------
*
*	     If a right parenthesis is found in the program line then this 
*	 function will begin by clearing out what is stored in token and reset
*	 the token index to 0.  The right parenthesis will then be stored from the
*	 line to the token and the appropriate token number will be printed.  
*	 Finally, the program line index will be incremented by one.
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 21
*/

void readRightParen(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx]; 
	
	printf("\t\t  %d\n\n", 21);
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
}

/*---------------------------------------------------------------------------
*	 Function: readComma
*	 -----------------------
*
*	     If a comma is found in the program line then this function will
*    begin by clearing out what is stored in token and reset the token index 
*	 to 0.  The comma will then be stored from the line to the token and 
*	 the appropriate token number will be printed.  Finally, the program line 
*	 index will be incremented by one. 
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 14
*/

void readComma(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;	
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx]; 
	
	printf("\t\t  %d\n\n", 14);
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
	
	if (prgLine_p->line[prgLine_p->lnIdx] == ' ')
	{
		prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
	}
}

/*---------------------------------------------------------------------------
*	 Function: readSemicolon
*	 -----------------------
*
*	     If a semicolon is found in the program line then this function will
*    begin by clearing out what is stored in token and reset the token index to
*	 0.  The semicolon will then be stored from the line to the token and 
*	 the appropriate token number will be printed.  Finally, the program line 
*	 index will be incremented by one. 
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 12
*/

void readSemicolon(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;	
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
	
	printf("\t\t  %d\n\n", 12);
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
}

/*---------------------------------------------------------------------------
*	 Function: readColon
*	 -------------------
*
*	     If a colon(:) is found in the program line then this function will 
*	 begin by checking the next character of the program line.  If the next 
*	 character is an equal sign(=) then what is stored in token wil be cleared 
*    out and the token index reset to 0.  The := sign will then be stored and 
*    the token number 15 will be printed.  In any other case, the token number
*	 13 wil be printed.
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 15 or 13 depeding on contents of program line
*/

void readColon(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	if (prgLine_p->line[prgLine_p->lnIdx + 1] == '=')
	{
		memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
		tknPtr_p->tknIdx = 0;
		tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
		tknPtr_p->token[tknPtr_p->tknIdx+ 1] = 
		 prgLine_p->line[prgLine_p->lnIdx+ 1];
		
		printf("\t\t  %d\n\n", 15);
		prgLine_p->lnIdx = prgLine_p->lnIdx + 2;
	}
	
	else
	{	
		memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
		tknPtr_p->tknIdx = 0;
		tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
		
		printf("\t\t  %d\n\n", 13);
		prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
	}	
}    /* closing readColon function */

/*---------------------------------------------------------------------------
*	 Function: readPlus
*	 ------------------
*
*	     If a '+' is found in the program line then this function will begin
*	 by clearing out what is stored in token and reset the token index to 0.
*	 The '+' will then be stored from the line to the token and the appropriate 
*    token number will be printed.  Finally, the program line index will be 
*	 incremented by one. 
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 16
*/

void readPlus(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;	
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx]; 
	
	printf("\t\t  %d\n\n", 16);
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;		
}

/*---------------------------------------------------------------------------
*	 Function: readMinus
*	 -------------------
*
*	     If a '-' is found in the program line then this function will begin
*	 by clearing out what is stored in token and reset the token index to 0.
*	 The '-' will then be stored from the line to the token and the 
*	 appropriate token number will be printed.  Finally, the program line 
*	 index will be incremented by one. 
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*	 Printing token number 17
*/

void readMinus(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;	
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
	
	printf("\t\t  %d\n\n", 17);	
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;		
}

/*---------------------------------------------------------------------------
*    Function: readStar
*    ------------------
*
*	     If an '*' is found on the program line then this function will begin
*    by clearing out what is stored in token and reset the token index to 0.
*    The '*' will then be stored from the line to the token and the 
*    appropriate token number will be printed.  Finally, the progam line index  
*    will be incremented by one.
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
* 
*	 Printing token number 18
*/

void readStar(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	memset(tknPtr_p->token, ' ', sizeof(tknPtr_p->token));
	tknPtr_p->tknIdx = 0;	
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
	
	printf("\t\t  %d\n\n", 18);
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;		
}

/*---------------------------------------------------------------------------
*    Function: alphaLiteral
*    ----------------------
*		
*        If the character read is an alphabetic or a literal then this 
*    function will be called.  It will begin by making the token and line 
*    c-string start at the same position.  It will then increment the 
*    index for both to check for the next character.  If it is a space,
*    comma, semicolon, left parenthesis, right parenthesis, a new line
*    character, or the terminating character, then the end of the token
*    has been reached.  If we have reached the end of constructing the 
*    token, then it's contents will be converted to upper case.  Finally,
*    the function printTokenNumber will be called to print the result.
*
*	 progLine_p: A pointer to the program line structure being scanned
*    tknPtr_p: A pointer to the token structure
*
*    if the entire token has been built, then printTokenNumber will be called
*/

void alphaLiteral(struct Line * prgLine_p, struct Token * tknPtr_p, 
				  struct hashTable * myTable_p)
{
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
	tknPtr_p->tknIdx = tknPtr_p->tknIdx + 1;
	
	if (prgLine_p->line[prgLine_p->lnIdx] == ' ' || 
	    prgLine_p->line[prgLine_p->lnIdx] == ',' || 
		prgLine_p->line[prgLine_p->lnIdx] == ';' || 
		prgLine_p->line[prgLine_p->lnIdx] == '(' || 
		prgLine_p->line[prgLine_p->lnIdx] == ')' || 
		prgLine_p->line[prgLine_p->lnIdx] == '\n' || 
		prgLine_p->line[prgLine_p->lnIdx] == '\0')
	{
		int  index = 0;
		char temp;
		
		/* If the token is alphabetical, make sure its uppercase */
		while (tknPtr_p->token[index] != '\0')
		{			
			temp = tknPtr_p->token[index];
			tknPtr_p->token[index] = toupper(temp);
			index = index + 1;
		}	
		printTokenNumber(prgLine_p, tknPtr_p, myTable_p);
	}		
}    /* closing alphaLiteral function */

/*---------------------------------------------------------------------------
*    Function: printTokenNumber
*    --------------------------
*
*        This function will print the token number associated with the 
*    contents of the token c-string.  It will begin by comparing the token
*    c-string with a list of reserved words for the language.  If a match is 
*    found, then the corresponding number will be printed.  If the token 
*    does not match any of the reserved words then it will determine if the 
*    token read is an identifier or a literal.  This will be accomplished by
*    checking the first character of the token.  If it is a literal then it  
*    will be printed with a '#' before it.  If it is an identifier then a '^' 
*    will be printed followed by the identifier.  If an identifier is too 
*	 long, then an appropriate error message will be displayed to the screen.
*
*	 tknPtr_p: A pointer to the token structure
*
*	 Prints the token number and/or the contents of the token
*    c-string.
*/

void printTokenNumber(struct Line * prgLine_p, struct Token * tknPtr_p,
					  struct hashTable * myTable_p)
{
	char reserved[Token_Max - 1][Token_Max] = {"PROGRAM ", "VAR ", "BEGIN ", 
												"END ", "END. ", "INTEGER ",
										  		"FOR ", "READ ", "WRITE ", 
												"TO ", "DO ", "DIV"};
	int  comparer = -1;
	
	/* PROGRAM reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[0], 8);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 1);
		return;
	}
	
	/* VAR reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[1], 4);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 2);
		return;
	}
			
	/* BEGIN reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[2], 6);
	if (comparer == 0)
	{
		myTable_p->afterBegin = true;
		printf("\t\t   %d\n\n", 3);
		return;
	}
	
	/* END reserved word */	
	comparer = strncmp(tknPtr_p->token, reserved[3], 4);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 4);
		return;
	}
	
	/* END. reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[4], 5);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 5);
		return;
	}
	
	/* INTEGER reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[5], 8);
	if (comparer == 0)
	{
		int index = 0;
		while (index < Size)
		{
			memset(myTable_p->symTable[index], ' ', sizeof(tknPtr_p->token));
			index = index + 1;
		}
		buildId(prgLine_p, myTable_p);
		printf("\t\t   %d\n\n", 6); 
		return;
	}    
	
	/* FOR reserved word */ 
	comparer = strncmp(tknPtr_p->token, reserved[6], 4);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 7);
		return;
	}
	
	/* READ reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[7], 5);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 8);
		return;
	}
	
	 /* WRITE reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[8], 6);
	if (comparer == 0)
	{
		printf("\t\t   %d\n\n", 9);
		return;
	}
	
	/* TO reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[9], 3);
	if (comparer == 0)
	{
		printf("\t\t  %d\n\n", 10);
		return;
	}	
	
	/* DO reserved word */
	comparer = strncmp(tknPtr_p->token, reserved[10], 3);
	if (comparer == 0)
	{
		printf("\t\t  %d\n\n", 11);
		return;
	}
	
	/* DIV operator */
	comparer = strncmp(tknPtr_p->token, reserved[11], 3);
	if (comparer == 0)
	{
		if (prgLine_p->line[prgLine_p->lnIdx - 4] != ' ' &&
		    prgLine_p->line[prgLine_p->lnIdx + 1] != ' ')
		{
			printf("Error: a Space Expected on Either Side of DIV Operator");
			printf("\n\n");
		}
		
		else
		{
			printf("\t\t  %d\n\n", 19);
			return;	
		}
	}
		
		
	else 
	{
		if (tknPtr_p->token[0] > 47 && tknPtr_p->token[0] < 58)
			printf("\t\t  %d \t\t\t   #%.12s\n\n", 23, tknPtr_p->token);
		
		else if (tknPtr_p->token[0] > 64 && tknPtr_p->token[0] < 91)
		{		
			int charCtr = 0;
			int printCtr = 0;    /* counter error message */
			
			while (tknPtr_p->token[charCtr] != ' ' && 
			       tknPtr_p->token[charCtr] != '\0')
			{
				if (charCtr > 12 && printCtr == 0)    /* only prints once */
				{
					printf("\nError: Max Identifier Length Exceeded ");
					printf("'%s'\n\n", tknPtr_p->token);
					printCtr = 1;
				}
				charCtr = charCtr + 1;
			}
			
			if (hashSearch(tknPtr_p->token, myTable_p) == -1)
			{
				printf("\nError: Undeclared Identifier: %s\n\n", tknPtr_p->token);
			}
			printf("\t\t  %d\t\t\t  ^%.12s\n\n", 22, tknPtr_p->token);
		}
					
		else 
			printf("%s Invalid Symbol\n\n", tknPtr_p);
			
	}    /* closing outermost else clause */
}    /* closing printTokenNumber function */

/*---------------------------------------------------------------------------
*    Function commentMode
*    --------------------
*
*        This function will begin by taking in the program line and skipping
*    the first two characters that mark the beginning of a comment.  It will
*    then echo print whatever is read until the next two characters read are
*    an '*' and a ')' sequentially.  If an end of line marker is read and 
*    the comment close has still not been found, then another line will be
*    be read in from the file and processing will continue.  
*
*	 filePtr_p: A pointer to the current line in the file being read
*	 progLine_p: A pointer to the program line structure being scanned
*
*    Prints the contents of the line until *) is read
*/

void commentMode(FILE * filePtr_p, struct Line * prgLine_p)
{
	prgLine_p->lnIdx = prgLine_p->lnIdx + 2;

	while (prgLine_p->line[prgLine_p->lnIdx] != '*' && 
		   prgLine_p->line[prgLine_p->lnIdx + 1] != ')')
	{
	    prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
		printf("%c", prgLine_p->line[prgLine_p->lnIdx - 1]);
		
		if (prgLine_p->line[prgLine_p->lnIdx] == '\0')
		{
			fgets(prgLine_p->line, Line_Max, filePtr_p); 
			prgLine_p->lineNum = prgLine_p->lineNum + 1;
			prgLine_p->lnIdx = 0;
		}
	}
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
}

/*---------------------------------------------------------------------------
*    Function getHash
*    --------------------
*
*        If the identifier that is sent is NULL this function will return 0.
*	 Else, this function will sum the ascii value of each letter of the 
*	 identifier.  Then that hash will be computed by obtaining the 
*	 remainder of the division of sum and the size of the table.
*
*	 id: character array containing a program identifier
*
*    Returns the hash of the program identifier
*/

int getHash(char id[])
{    
    int hash = 0;
    int sum = 0;
    int letter = 0;
    int index = 0;
    
    if (id == NULL) 
        return 0;
 
    while (id[index] != '\0') 
	{
        letter = (int)id[index];
        sum = sum + letter;
        index = index + 1;
    } 
    
    return hash = sum % Size;
}

/*---------------------------------------------------------------------------
*    Function hashInsert
*    --------------------
*
*        This function will begin by searching the hashTable for the 
*	 identifier.  If the passed identifier is found, then an error will be
*	 printed and the function will return to the caller.  Else, the hash of  
*	 the identifier will be stored.  Then the location of the table at the 
*	 hashed position will be checked.  If it is not empty, then the table will 
*	 loop through the table until the next empty slot is found and insert the 
*	 identifier.  If the hashed position is empty, then the identifier will be 
*	 stored at that position in the hash table.  
*
*	 id: character array containing a program identifier
*	 myTable_p: pointer to the symbol table structure 
*
*    Printing error message if the identifier is already in the table
*	 or if the table is full	 
*/

void hashInsert(char id[], struct hashTable * myTable_p)
{
	if (myTable_p->numEntries == Size + 1)
		printf("\n\nError :Maximum Amount of Identifiers Exceeded\n\n");

	hashSearch(id, myTable_p);
	
	if (hashSearch(id, myTable_p) == -1)
	{
		printf("\nError: Duplicate Identifier %s\n\n", id);
		return;
	}
	
	int localHash = getHash(id);
	int ctr = localHash;

	if (myTable_p->symTable[localHash][0] > 64 &&
		myTable_p->symTable[localHash][0] < 91)
	{
		while ((myTable_p->symTable[ctr][0] > 64 &&
				myTable_p->symTable[ctr][0] < 91 )&&
		        myTable_p->symTable[ctr] != NULL)
		{
			ctr = ctr + 1;
			ctr = ctr % Size;
		}
		strxfrm(myTable_p->symTable[ctr], id, Token_Max);
		myTable_p->numEntries = myTable_p->numEntries + 1;
	}
	
	else 
	{
		strxfrm(myTable_p->symTable[localHash], id, Token_Max);
		myTable_p->numEntries = myTable_p->numEntries + 1;
	}
}    /* closing hashInsert function */

/*---------------------------------------------------------------------------
*    Function hashSearch
*    --------------------
*
*        This function will compute the hash of the passed identifier.  If 
*	 the identifier is already in the table at the hashed position, then 
*	 this function will return -1.  If not, then this function will loop 
*	 through the symbol table comparing the passed identifier and the contents
*	 of the table at each index.  If there is a match, then this function will
*	 return -1.  If not, then this function will return 1.
*
*	 id: character array containing a program identifier
*	 myTable_p: pointer to the symbol table structure
*
*    Returns -1 if the identifier is found
*	 Returns 1 if the identifier is not found
*/

int hashSearch(char id[], struct hashTable * myTable_p)
{
	int index = 0;
	int comp;
	int hash = getHash(id);
	
	if (myTable_p->symTable[hash] == id)
		return -1;

	while (index < Size)
	{
		comp = strcmp(id, myTable_p->symTable[index]);
		if (comp == 0)
			return -1;
		else 
			index = index + 1;
	}
	return 1;
}

/*---------------------------------------------------------------------------
*    Function printTable
*    --------------------
*
*        This function will print the contents of the symbol table 
*	 including empty slots. 
*
*	 myTable_p: A pointer to the symbol Table structure
*
*    Prints the contents of the table until the maximum size is reached
*/

void printTable(struct hashTable * myTable_p)
{
	int index = 0;
	char empty = '~';
	
	printf("\n\n\tSymbol Table \n");
	printf(" ________________________\n");
		
	while(index < Size)
	{
		if (myTable_p->symTable[index][0] > 64 && 
			myTable_p->symTable[index][0] < 91)
			printf("|%+10.12s   |%10d| \n", myTable_p->symTable[index], index);
		else 
			printf("|%+10c   |%10d| \n", empty, index);
		index = index + 1;
	}
	
	printf("|_____________|__________|\n");
}


/*---------------------------------------------------------------------------
*    Function buildId
*    -----------------
*
*        This function will begin by finding the location of the colon
*	 on the variable declaration line.  Then the line will be separated
*	 into words delimited by a space or comma, and this function will 
*	 attempt to insert the token word into the symbol table.
*
*	 progLine_p: A pointer to the program line structure
*	 myTable_p: A pointer to the symbol Table structure
*
*    Returns: N/A 
*/

void buildId(struct Line * progLine_p, struct hashTable * myTable_p)
{
	char   word[13];
	char * colon = strstr(progLine_p->line, ":");
	char * chPtr = strtok(progLine_p->line, " ,");
    int    index = 0;
    
	strcpy(word, chPtr);

	while (chPtr != colon)
	{
		hashInsert(word, myTable_p);
		chPtr = strtok(NULL, " ,.-+");
		strncpy(word, chPtr, sizeof(word));
		index = index + 1;
	}
}
