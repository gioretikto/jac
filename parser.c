#include "jac.h"
#include "lists.h"
#include <math.h>

#define TAIL_OP 'x' /* Operator of the tail node */
#define ERROR '?'

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

void reverse(struct node** head_ref);
void add_item (struct node **ptr, long double data);
void delNextNode (struct node *node_pt);

long double calculate (struct node **head);
long double evaluateFuncResult (struct control *jac, enum functions func);
long double switchFunc(enum functions *func, const long double *number);
void incrementBuff (struct control *jac, const int n);

unsigned long factorial(unsigned long f);
int bin_dec(long long n);
long long dec_bin(int n);
long double parseConstants(struct control *jac);
bool searchBinaryFunction (struct control *jac, struct node *head);
bool searchPowFunction (struct control *jac, long double *number);

enum functions searchFunction (struct control *jac);

long double abort_parsing(struct node *head)
{
	free(head);

	return -2;
}

long double parse_evaluate_expr(struct control *jac, bool inFunc)
{
	long double number;

	unsigned int n = 0;

	struct node *head = NULL;

	enum functions func = NONE;

	while (jac->len < MAX && *jac->buf != '\0')
	{
		if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
		{
			incrementBuff(jac,n);

		 	add_item(&head, number);
 
			if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{' || isalpha(*jac->buf))	/* For situations like 5(3+2) */
				head->op = '*';			
		}

		else if (*jac->buf == '-' || *jac->buf == '+')			/* Important for situation like -(5+3) which jac translates into -1*(5+3) */
		{
			if (*jac->buf == '-')
			 	add_item(&head, -1);
			else
				add_item(&head, 1);

			incrementBuff(jac,1);
			head->op = '*';
		}

		else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{')
		{
			incrementBuff(jac,1);
			add_item(&head, parse_evaluate_expr(jac, inFunc));
		}

		else if (*jac->buf == ')' || *jac->buf == ']' || *jac->buf == '}')
		{
			incrementBuff(jac,1);

			if (*jac->buf != '\0')										/* For situations like (3+2)5 */
				if (isalpha(*jac->buf) || isdigit(*jac->buf))
					head->op = '*';

			number = calculate(&head);

			if (inFunc == true)
			{
				free(head);
				return number;
			}
		}

		else if (searchBinaryFunction(jac, head))
			;

		else if (isalpha(*jac->buf))
		{
			/* Parsing for functions like sin, cos, etc */

			if ((func = searchFunction(jac)) != NONE)
				add_item(&head, evaluateFuncResult(jac, func));

			/* Parse for constants */
			else if ((number = parseConstants(jac)) != 0)
			{
				if (inFunc == true) {
					searchPowFunction(jac, &number);
					return number;
				}

				else
				{
					add_item(&head, number);

					if (*jac->buf == '(')		/* For situations like 5(3+2) */
						head->op = '*';
				}
			}

			else if (*jac->buf == 'E')
			{
				add_item(&head, 10);
				incrementBuff(jac,1);

				if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
				{
					incrementBuff(jac,n);
					head->value = pow(head->value, number);
				}

				else {
					jac->buf[0] = ERROR;
					return abort_parsing(head);
				}
			}
			
			else 				/* Syntax error */
			{
				fprintf(stderr,"Illegal character %s\n", jac->buf);
				jac->buf[0] = ERROR;
				return abort_parsing(head);
			}
		}

		else
			continue;

	} /* End of parsing */

	/* Once parsed, jac evaluates the expression */

	if (head == NULL)
	{
		jac->buf[0] = ERROR;
		return -2;
	}

	else if (jac->len >= MAX)
	{
		fprintf(stderr,"%s\n","The limit size of the expression was reached");
		jac->buf[0] = ERROR;
		return abort_parsing(head);
	}

	else
	{
		number = calculate(&head);
		free(head);
		return number;
	}
}

bool searchBinaryFunction (struct control *jac, struct node *head)
{
		bool result = true;

		if (*jac->buf == '/')
		{
			incrementBuff(jac,1);
			head->value = head->value / (evaluateFuncResult(jac, DIV));
		}

		else if (*jac->buf == '*')
		{
			incrementBuff(jac,1);
			head->value = head->value * (evaluateFuncResult(jac, MULT));
			head->op = TAIL_OP;	 /* we don't want head->op to contain '*' and cause interference in calculate() */
		}

		else if (*jac->buf == '^')
		{
			incrementBuff(jac,1);
			head->value = pow(head->value, evaluateFuncResult(jac, POW));
		}

		else if (*jac->buf == '!') 	/* Factorial */
		{
			incrementBuff(jac,1);
			head->value = factorial(head->value);
		}

		else if (*jac->buf == '%')
		{
			incrementBuff(jac,1);
			head->value = fmodl(head->value, evaluateFuncResult(jac, MOD));
		}

		else
			return false;

		return result;
}

long double parseConstants(struct control *jac)
{
	unsigned int incrBuff = 0;
	long double data = 0;

	if (strncmp(jac->buf, "pi", 2) == 0)
	{
		data = M_PIl;
		incrBuff = 2;
	}

	else if (strncmp(jac->buf, "m_p", 3) == 0) 		/* Proton Mass */
	{
		data = PROTON_MASS;
		incrBuff = 3;
	}

	else if (*jac->buf == 'e')
	{
		data = M_E;
		incrBuff = 1;
	}

	else if (strncmp(jac->buf, "m_e", 3) == 0) 		/* Electron mass */
	{
		data = ELECTRON_MASS;
		incrBuff = 3;
	}

	else if (strncmp(jac->buf, "c_0", 3) == 0)  	/* Speed of light in vacuum m/s (exact) */
	{
		data = SPEED_LIGHT;
		incrBuff = 3;
	}

	else if (*jac->buf == 'q')						/* elementary charge*/
	{
		data = CHARGE;
		incrBuff = 1;
	}

	else if (*jac->buf == 'h')						/* Plank's constant*/
	{
		data = PLANK;
		incrBuff = 1;
	}

	else if (strncmp(jac->buf, "e_0", 3) == 0) 		/* Permittivity of free space */
	{
		data = E_0;
		incrBuff = 3;
	}

	else if (strncmp(jac->buf, "n_a", 3) == 0)  /* Avogadros's number */
	{
		data = AVOGADRO;
		incrBuff = 3;
	}	

	else if (*jac->buf == 'k')  	/* Boltzmann's constant */
	{
		data = K_B;
		incrBuff = 1;
	}

	else
		return data;

	incrementBuff(jac,incrBuff);

	return data;
}

long double evaluateFuncResult (struct control *jac, enum functions func)
{
	long double number;
	unsigned int n;

	if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
	{
		incrementBuff(jac,n);

		while(searchPowFunction(jac, &number))		/* For situations like 5^2^3^.. */
			{;}
	
		return switchFunc(&func, &number);
	}

	/* Parse for constants */
	else if ((number = parseConstants(jac)) != 0)
		{
			while(searchPowFunction(jac, &number))		/* For situations like pi^2^3^.. */
				{;}
		}

	else
		number = parse_evaluate_expr(jac, true);

	return switchFunc(&func, &number);
}

unsigned int searchFunction (struct control *jac)
{
	enum functions func = NONE;
	unsigned int incrBuff = 0;

	if (strncmp(jac->buf, "cos", 3) == 0)
	{
		incrBuff = 3;
		func = COS;
	}

	else if (strncmp(jac->buf, "sin", 3) == 0)
	{
		incrBuff = 3;
		func = SIN;
	}

	else if (strncmp(jac->buf, "asinhl", 6) == 0)
	{
		incrBuff = 6;
		func = ASINH;
	}

	else if (strncmp(jac->buf, "asin", 4) == 0)
	{
		incrBuff = 4;
		func = ASIN;
	}

	else if (strncmp(jac->buf, "atan", 4) == 0)
	{
		incrBuff = 4;
		func = ATAN;
	}

	else if (strncmp(jac->buf, "acos", 4) == 0)
	{
		incrBuff = 4;
		func = ACOS;
	}

	else if (strncmp(jac->buf, "sqrt", 4) == 0)
	{
		incrBuff = 4;
		func = SQRT;
	}

	else if (strncmp(jac->buf, "sinh", 4) == 0)
	{
		incrBuff = 4;
		func = SINH;
	}

	else if (strncmp(jac->buf, "cosh", 4) == 0)
	{
		incrBuff = 4;
		func = COSH;
	}

	else if (strncmp(jac->buf, "tanh", 4) == 0)
	{
		incrBuff = 4;
		func = TANH;
	}

	else if (strncmp(jac->buf, "tan", 3) == 0)
	{
		incrBuff = 3;
		func = TAN;
	}

	else if (strncmp(jac->buf, "log", 3) == 0)
	{
		incrBuff = 3;
		func = LOG;
	}

	else if (strncmp(jac->buf, "ln", 2) == 0)
	{
		incrBuff = 2;
		func = LN;
	}

	else if (strncmp(jac->buf, "bin_dec", 7) == 0)
	{
		incrBuff = 7;
		func = BIN_DEC;
	}

	else if (strncmp(jac->buf, "dec_bin", 7) == 0)
	{
		incrBuff = 7;
		func = DEC_BIN;
	}

	else if (strncmp(jac->buf, "cbrt", 4) == 0)
	{
		incrBuff = 4;
		func = CBRT;
	}

	else if (strncmp(jac->buf, "abs", 3) == 0)
	{
		incrBuff = 3;
		func = ABS;
	}

	else
		return NONE;

	incrementBuff(jac,incrBuff);

	return func;
}

long double calculate (struct node **head)
{
	reverse(head);
	
	struct node *tmp = *head;

	long double result;

	while (tmp->next != NULL)			/* this cycle is needed to evaluate situations like 5(..) */
	{
		if (tmp->op == '*')
		{
			tmp->value = tmp->value * tmp->next->value;

			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}

	tmp = *head;

	/* Add up all numbers with their signs */

	while (tmp->next != NULL)
	{
		tmp->value = tmp->value + tmp->next->value;

		delNextNode(tmp);
	}

	result = (*head)->value;

	return result;
}

long double switchFunc(enum functions *func, const long double *number)
{
	switch (*func)
	{
		case MULT:
			return *number;

		case DIV:
			return *number;

		case SIN:
			return sinl(*number);

		case COS:
	    		return cosl(*number);

		case TAN:
			return tanl(*number);

		case POW:
			return *number;

		case ATAN:
			return atanl(*number);

		case SINH:
			return sinhl(*number);

		case COSH:
			return coshl(*number);

		case TANH:
			return tanhl(*number);

		case ASINH:
			return asinhl(*number);

		case BIN_DEC:
			return bin_dec(*number);

		case DEC_BIN:
	    		return dec_bin(*number);

		case ABS:
			return fabsl(*number);

		case LOG:
			return log10l(*number);

		case ASIN:
			return asinl(*number);

		case LN:
			return logl(*number);

		case MOD:
			return *number;

		case ACOS:
			return acosl(*number);

		case SQRT:
			return sqrtl(*number);

		case CBRT:
			return cbrtl(*number);

		default:
			return -2;
	}
}

void incrementBuff (struct control *jac, const int n)
{
	jac->buf += n;
	jac->len += n;
}

bool searchPowFunction (struct control *jac, long double *number)
{
		bool result = true;

		if (*jac->buf == '^')
		{
			incrementBuff(jac,1);
			*number = pow(*number, evaluateFuncResult(jac, POW));
		}

		else if (*jac->buf == '!') 	/* Factorial */
		{
			incrementBuff(jac,1);
			*number = factorial(*number);
		}

		else if (*jac->buf == '%')
		{
			incrementBuff(jac,1);
			*number = fmodl(*number, evaluateFuncResult(jac, MOD));
		}

		else
			return false;

		return result;
}
