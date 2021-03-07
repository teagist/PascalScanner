/*	 
*	 CS 421: Automata Theory and Compiler Construction
*	 Spring Semester, 2020
*	 
*	 Scanner for Simplified Pascal Programming Language
*
*	     This program will open a file for reading.  Once the file is open
*    each line will be stored into a structure for scanning.  The line will
*	 then be scanned and each token will be stored in another structure and 
*	 checked for valididty.  Finally, the line numbers will be displayed 
*	 along with the token numbers associated with reserved words, symbols, and 
*	 identifiers.
* 
*	 Programmed by: Houston Brown
*	 Date Last Updated: 4/5/2020
*
*	Now on GitHub 
*/

#include <stdio.h>
#include <string.h>

#define Line_Max 81
#define Token_Max 13
#define File_Max 100


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


/*---------------------------------------------------------------------------*/
/* Function Declarations */

FILE* openFile(FILE * filePtr_p);

struct Line * initLine(struct Line prgLine);

struct Token * initToken(struct Token prgToken);

void getToken(FILE * filePtr_p, struct Line * prgLine_p);

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

void alphaLiteral(struct Line * prgLine_p, struct Token * tknPtr_p);

void printTokenNumber(struct Line * prgLine_p, struct Token * tknPtr_p);

void commentMode(FILE * filePtr, struct Line * prgLine_p);


/*---------------------------------------------------------------------------*/
/*  Driver  */

int main(void)
{
	FILE        * filePtr_p = openFile(filePtr_p);
	struct Line   progLine;			
	struct Line * linePtr_p = initLine(progLine);

	printf("Line\t\t Token Type\t\t Token Specifier\n");    /* Header */
	
	while (!feof(filePtr_p))								// Need to output the token numbers here
	{
		getToken(filePtr_p, linePtr_p);
		memset(linePtr_p->line, ' ', sizeof(linePtr_p->line));
	}

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
	printf(" by the extension\n");
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


struct Line * initLine(struct Line prgLine)
{
	return &prgLine;
}


struct Token * initToken(struct Token prgToken)
{
	prgToken.tknIdx = 0;  
	memset(prgToken.token, ' ', sizeof(prgToken.token) + 1);
	return &prgToken;
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

void getToken(FILE * filePtr_p, struct Line * prgLine_p)
{
	fgets(prgLine_p->line, Line_Max, filePtr_p);
	prgLine_p->lineNum = prgLine_p->lineNum + 1;
	prgLine_p->lnIdx = 0;
	printf("%d", prgLine_p->lineNum);		

	struct Token   newToken;
	struct Token * tokenPtr_p = initToken(newToken);

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
				alphaLiteral(prgLine_p, tokenPtr_p);    
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

void alphaLiteral(struct Line * prgLine_p, struct Token * tknPtr_p)
{
	tknPtr_p->token[tknPtr_p->tknIdx] = prgLine_p->line[prgLine_p->lnIdx];
	prgLine_p->lnIdx = prgLine_p->lnIdx + 1;
	tknPtr_p->tknIdx = tknPtr_p->tknIdx + 1;
	
	if (prgLine_p->line[prgLine_p->lnIdx] == ' '|| 
	    prgLine_p->line[prgLine_p->lnIdx] == ','|| 
		prgLine_p->line[prgLine_p->lnIdx] == ';' || 
		prgLine_p->line[prgLine_p->lnIdx] == '(' || 
		prgLine_p->line[prgLine_p->lnIdx] == ')'|| 
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
		printTokenNumber(prgLine_p, tknPtr_p);
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

void printTokenNumber(struct Line * prgLine_p, struct Token * tknPtr_p)				// Shorten if possible, may need to split
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
		int  lineIndex = 0;			// Make a function that will read them in using strtok
		int  idIndex = 0;
		char letter;
		char identifier[Token_Max] = " ";
		
		/* Checking identifier lengths */
		while (prgLine_p->line[lineIndex] != ':')
		{
			if (prgLine_p->line[lineIndex] == ' ')    /* Reached end of Id */
			{
				lineIndex = lineIndex + 1;
				idIndex = 0;
				memset(identifier, ' ', sizeof(identifier));
			}
			
			if (prgLine_p->line[lineIndex] == ',')    /* Reached end of Id */
			{
				lineIndex = lineIndex + 2;
				idIndex = 0;
				memset(identifier, ' ', sizeof(identifier));
			}
			
			else    /* Building Id */
			{
				identifier[idIndex] = prgLine_p->line[lineIndex];

				if (idIndex >= 13)    /* Found Id that is too long */
				{
					memset(identifier, ' ', sizeof(identifier));
					
					while (prgLine_p->line[lineIndex] != ' ' &&
						   prgLine_p->line[lineIndex] != ',')
					{
						lineIndex = lineIndex + 1;	   	
					}
					
				    lineIndex = lineIndex + 1;
					idIndex = 0;
					break;
				}
				
				else
				{
					lineIndex = lineIndex + 1;
					idIndex = idIndex + 1;
				}				
			}    /* closing building Id else */
		}    /* closing while loop */
		
		printf("\t\t   %d\n\n", 6); 
		return;
	}    /* closing if for INTEGER reserved word comparison */
	
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
			printf("Error: a space expected on either side of DIV operator");
			printf("\n\n");
		}
		
		else
		{
			printf("\t\t  %d\n\n", 19);
			return;	
		}
	}
		
	/*
		If the token is not a reserved word, then here is where its 
		token number is decided between a literal or an identifier.
		This will also check for identifiers that are too long, and 
		that are undeclared
		
		Make a function
	*/		
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
					printf("Error: max identifier length exceeded ");
					printf("%s\n\n", tknPtr_p->token);
					printCtr = 1;
				}
				charCtr = charCtr + 1;
			}
			
			printf("\t\t  %d\t\t\t  ^%.12s\n\n", 22, tknPtr_p->token);
		}
					
		else 
			printf("%s undeclared identifier\n\n", tknPtr_p);
			
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
