/* 
 * Exercise 4-4. Add commands to print the top element of the stack witout
 * poping, to duplicate it, and to swap the top two elements. Add command to
 * clear the stack. By Faisal Saadatmand
 */

#include <stdio.h>
#include <stdlib.h>          /* for atof() */
#include <ctype.h>
#include <string.h>

#define MAXOP   100          /* max size of operand or operator */
#define NUMBER  '0'          /* signal that a number was found */
#define COMMAND '1'          /* signal that a number was found */
#define MAXVAL  100          /* maximum depth of val stack */
#define BUFSIZE 100

/* function decleration */
int    getop(char []);
void   push(double);
double pop(void);
int    getch(void);
void   ungetch(int);
void   swapTop(void);

/* external variables */
int    sp = 0;               /* next free stack position */
double val[MAXVAL];          /* value stack */
char   buf[BUFSIZE];         /* buffer from ungetch */
int    bufp = 0;             /* next free position in buf */
int    sign = 1;             /* positive or negative */
double top;
char   execCommand[10];            /* exucuted command */

/* push: push f onto value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = top = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0) {
		top = val[sp - 2];
		return val[--sp];
	} else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int i, c;

	while ((s[0] = c = getch()) == ' ' || c == '\t')
	s[1] = '\0';

	if (!isdigit(c) && !isalpha(c) && c != '.' && c != '-')
		return c;                           /* not a number */

	if (c == '-')                           /* negative numbers provision */
		if (isdigit(s[0] = c = getch()))    /* peak at the next character */
			sign = -1;
		 else {
			ungetch(c);                     /* push char back for next cycle */
			return '-';
		}
	
	if (isalpha(c)) {
		i = 0;
		execCommand[i++] = c;
		while (isalpha(execCommand[i] = c = getch()))
			++i;
		execCommand[i] = '\0';
		return COMMAND;
	}

	i = 0;
	if (isdigit(c))
		while (isdigit(s[++i] = c = getch()))
			;
	if( c == '.')            /* collect fraction part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';

	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

/* printTop: prints the top element in the stack */
void printTop(void)
{
	printf("top\t%.8g\n", top);
}

/* deleteTop: deletes the top element in the stack */
void duplicateTop(void)
{
		push(top);
		printTop();
}

/* swapTopTwo: swaps top two elements */
 void swapTop(void)
 {
	 double top1, top2;
	 top1 = pop();
	 top2 = pop();
	 push(top1);
	 push(top2);
	 printf("swaped %.8g with %.8g\n", top2, top1);
	 printTop();
 }

/* clear: clears the entire stack */
void clearStack(void)
{
	while (sp > 0)
		pop();
	printf("stack cleared\n");
	printTop();
}

int getch(void)              /* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)          /* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

/* reverse Polish Calculator */
int main(void)
{
	int type;
	double op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(sign * atof(s));
			sign = 1;
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '%':
			op2 = pop();
			if (op2 != 0.0)
				push((long) pop() % (long) op2);
			else
				printf("error: zero divisor\n");
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			break;
		case COMMAND:
			if (strcmp(execCommand, "print") == 0)
				printTop();
			else if (strcmp(execCommand, "dp") == 0)
				duplicateTop();
			else if (strcmp(execCommand, "swap") == 0)
				swapTop();
			else if (strcmp(execCommand, "clear") == 0)
				clearStack();
			else
				printf("error: unknown command %s\n", s);
			break;
		defualt:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}