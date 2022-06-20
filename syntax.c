#include <math.h>
#include "jac.h"
#include "lists.h"

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

struct charNode
{
	char data;

	struct charNode *next;

};

void push(struct charNode** top_ref, int new_data);

int pop(struct charNode** top_ref);

bool isMatchingPar(char character1, char character2)
{
	if (character1 == '(' && character2 == ')')
		return 1;

	else if (character1 == '{' && character2 == '}')
		return 1;

	else if (character1 == '[' && character2 == ']')
		return 1;

	else
		return 0;
}

bool areParenthesisBalanced(char *buf)
{
	int i = 0;

	struct charNode *stack = NULL;

	while (buf[i])
	{
		if (buf[i] == '{' || buf[i] == '(' || buf[i] == '[')
			push(&stack, buf[i]);

		if (buf[i] == '}' || buf[i] == ')' || buf[i] == ']')
		{
			if (stack == NULL)
				return 0;

			else if (!isMatchingPar(pop(&stack), buf[i]))
			return 0;
		}

		i++;
	}

	if (stack == NULL)
		return 1;

	else
		return 0;
} 


void push(struct charNode** top_ref, int new_data)
{
	struct charNode* new_node = (struct charNode*)malloc(sizeof(struct charNode));

	if (new_node == NULL)  
	{
		printf("Stack overflow n");

		getchar();

		exit(0);

	}

	new_node->data  = new_data;

	new_node->next = (*top_ref);

	(*top_ref) = new_node;
}

int pop (struct charNode** top_ref)
{
	char res;

	struct charNode *top;

	if (*top_ref == NULL)
	{
		printf("Stack overflow n");

		getchar();

		exit(0);
	}

	else
	{
		top = *top_ref;

		res = top->data;

		*top_ref = top->next;

		free(top);

		return res;
	}
}

enum functions {COS,SIN,ASINH,ASIN,ATAN,ACOS,SQRT,SINH,COSH,TANH,TAN,LOG,LN,MULT_DIV_POW_MOD,NONE};

#define MAX 1000

long double syntaxParseConstants(char **buf, unsigned int *length)
{
	unsigned int incrBuff = 0;
	long double data = 0;

	if (strncmp(*buf, "pi", 2) == 0)
	{
		data = M_PIl;
		incrBuff = 2;
	}

	else if (strncmp(*buf, "m_p", 3) == 0) 		/* Proton Mass */
	{
		data = PROTON_MASS;
		incrBuff = 3;
	}

	else if (**buf == 'e')
	{
		data = M_E;
		incrBuff = 1;
	}

	else if (strncmp(*buf, "m_e", 3) == 0) 		/* Electron mass */
	{
		data = ELECTRON_MASS;
		incrBuff = 3;
	}

	else if (strncmp(*buf, "c_0", 3) == 0)  	/* Speed of light in vacuum m/s (exact) */
	{
		data = SPEED_LIGHT;
		incrBuff = 3;
	}

	else if (**buf == 'q')						/* elementary charge*/
	{
		data = CHARGE;
		incrBuff = 1;
	}

	else if (**buf == 'h')						/* Plank's constant*/
	{
		data = PLANK;
		incrBuff = 1;
	}

	else if (strncmp(*buf, "e_0", 3) == 0) 		/* Permittivity of free space */
	{
		data = E_0;
		incrBuff = 3;
	}

	else if (strncmp(*buf, "n_a", 3) == 0)  /* Avogadros's number */
	{
		data = AVOGADRO;
		incrBuff = 3;
	}	

	else if (**buf == 'k')  	/* Boltzmann's constant */
	{
		data = K_B;
		incrBuff = 1;
	}

	else if (**buf == 'E')
	{
		data = 5;
		incrBuff = 1;
	}

	else
		return data;

	*buf += incrBuff;
	*length += incrBuff;

	return data;
}

enum functions syntaxSearchFunction (char **buf, unsigned int *length)
{
	enum functions func = NONE;

	unsigned int incrBuff = 0;

	if (strncmp(*buf, "cos", 3) == 0)
	{
		incrBuff = 3;
		func = COS;
	}

	else if (strncmp(*buf, "sin", 3) == 0)
	{
		incrBuff = 3;
		func = SIN;
	}

	else if (strncmp(*buf, "asinhl", 6) == 0)
	{
		incrBuff = 6;
		func = ASINH;
	}

	else if (strncmp(*buf, "asin", 4) == 0)
	{
		incrBuff = 4;
		func = ASIN;
	}

	else if (strncmp(*buf, "atan", 4) == 0)
	{
		incrBuff = 4;
		func = ATAN;
	}

	else if (strncmp(*buf, "acos", 4) == 0)
	{
		incrBuff = 4;
		func = ACOS;
	}

	else if (strncmp(*buf, "sqrt", 4) == 0)
	{
		incrBuff = 4;
		func = SQRT;
	}

	else if (strncmp(*buf, "sinh", 4) == 0)
	{
		incrBuff = 4;
		func = SINH;
	}

	else if (strncmp(*buf, "cosh", 4) == 0)
	{
		incrBuff = 4;
		func = COSH;
	}

	else if (strncmp(*buf, "tanh", 4) == 0)
	{
		incrBuff = 4;
		func = TANH;
	}

	else if (strncmp(*buf, "tan", 3) == 0)
	{
		incrBuff = 3;
		func = TAN;
	}

	else if (strncmp(*buf, "log", 3) == 0)
	{
		incrBuff = 3;
		func = LOG;
	}

	else if (strncmp(*buf, "ln", 2) == 0)
	{
		incrBuff = 2;
		func = LN;
	}

	/*else if (strncmp(buf, "bin_dec", 7) == 0)
	{
		incrBuff = 7;
		func = BIN_DEC;
	}

	else if (strncmp(buf, "dec_bin", 7) == 0)
	{
		incrBuff = 7;
		func = DEC_BIN;
	}*/

	else
		return NONE;

	*buf += incrBuff;
	*length += incrBuff;

	return func;
}

bool checkSyntax(char *buf)
{
	long double number;
	unsigned int length = 0;
	unsigned int n = 0;

	bool op = false;

	while (length < MAX && *buf != '\0' && *buf !='x')
	{
		if (1 == sscanf(buf, "%Lf%n", &number, &n))
		{
			length += n;
			buf += n;
			op = false;
		}
		
		else if (*buf == '*' || *buf == '-' || *buf == '+' || *buf == '/' ||*buf == '%' || *buf == '!' || *buf == '.' || *buf == '^')
		{
			if (op == true)
			{
				fprintf(stderr, "Double operator detected %c\n", *buf);
				return false;
			}

			else if (buf[1] == '\0' && buf[0] != '!') /* For situations like 5+ but not 5!*/
			{
				fprintf(stderr, "Invalid expression binary operator %c with second operand missing\n", *buf);
				return false;
			}

			else
			{
				op = true;
				length++;
				buf++;
			}
		}

		else if (isalpha(*buf))
		{
			/* Parsing for functions like sin, cos, etc */

			op = false;

			if ((syntaxSearchFunction(&buf,&length)) != NONE)
				;

			/* Parse for constants */
			else if (syntaxParseConstants(&buf, &length) != 0)
				;

			else
			{
				fprintf(stderr, "Invalid charachter %c \n", *buf);
				return false;
			}
		}

		else if (*buf == '(' || *buf == '{' || *buf == '[')
		{
			op = false;
			length++;
			buf++;
		}

		else if (*buf == ')' || *buf == '}' || *buf == ']')
		{
			if (op == true)
			{
				fprintf(stderr, "Invalid expression terminating in operator + %c\n", *buf);
				return false;
			}

			else
			{
				op = false;
				length++;
				buf++;
			}
		}

		else
		{
			fprintf(stderr, "Syntax error: Invalid character %c\n", *buf);
			return false;
		}
	}

	if (length >= MAX)
	{
		fprintf(stderr,"%s\n","The limit size of the expression was reached");
		return false;
	}

	return true;
}
