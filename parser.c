#include "jac.h"
#include "lists.h"
#include <math.h>

#define TAIL_OP 'x' /* Operator of the tail node */

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

void reverse(struct node** head_ref);
void add_item (struct node **ptr, long double data);
void delNextNode (struct node *node_pt);

long double calculate (struct node *head);
long double evaluateFuncResult (struct control *jac, enum functions func);
long double switchFunc(enum functions *func, long double *number);
void incrementBuff (struct control *jac, int n);

unsigned long factorial(unsigned long f);
int bin_dec(long long n);
long long dec_bin(int n);
long double parseConstants(struct control *jac);

enum functions searchFunction (struct control *jac);

long double parse_evaluate_expr(struct control *jac)
{
	long double number, result;

	unsigned int n = 0;

	struct node *head = NULL;

	enum functions function;

	while (jac->len < MAX && *jac->buf != '\0')
	{	
		/* Read operator first; you don't want to read -5 and loose the - */

		if (*jac->buf == '/' || *jac->buf == '*' || *jac->buf == '+' || *jac->buf == '-')	/* Add new item when you encounter an operator */
		{
			if (head == NULL && *jac->buf == '-')
			{
				add_item(&head, -1);
				head->op = '*';
			}

			else if (jac->inFunc != NONE  && jac->bracketsFunc == false)
			{
				if (head != NULL)
				{
					reverse(&head);
					return calculate(head);
				}

				else
					head->op = *jac->buf;
			}

			else
				head->op = *jac->buf;

			incrementBuff(jac,1);
		}

		if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
		{
			incrementBuff(jac,n);

			if ( (jac->inFunc == POW || jac->inFunc == MOD) && jac->bracketsFunc == false)
				return number;
			else
			 	add_item(&head, number);
		}

		else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{')
		{
			incrementBuff(jac,1);

			if (jac->inFunc != NONE)
				jac->bracketsFunc = true;

			else
			{
				jac->insideBrackets = true;
				add_item(&head, parse_evaluate_expr(jac));
			}
		}

		else if (*jac->buf == ')' || *jac->buf == ']' || *jac->buf == '}')
		{
			incrementBuff(jac,1);

			if (jac->bracketsFunc != NONE)
			{
				jac->bracketsFunc = false;
				reverse(&head);
				return calculate(head);
			}

			else
			{
				jac->insideBrackets = false;
				break;
			}
		}

		else if (*jac->buf == '^')
		{
			incrementBuff(jac,1);
			jac->inFunc = POW;
			head->value = pow(head->value, evaluateFuncResult(jac, POW));
		}

		else if (*jac->buf == '%')
		{
			incrementBuff(jac,1);
			jac->inFunc = MOD;
			head->value = fmodl(head->value, evaluateFuncResult(jac, MOD));
		}

		else if (*jac->buf == '!') 	/* Factorial */
		{
			head->value = factorial(head->value);
			incrementBuff(jac,1);
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

			else
				jac->failure = true;
		}

		/* Parsing for functions like sin, cos, etc */

		else if ((function = searchFunction(jac)) != NONE)
		{
			jac->inFunc = function;
			add_item(&head, evaluateFuncResult(jac, function));
		}

		else if (isalpha(*jac->buf)) 	/* Syntax error */
		{
			/* Parse for constants */

			if ((number = parseConstants(jac)) != 0)
			{
				if ((jac->inFunc == POW || jac->inFunc == MOD) && jac->bracketsFunc == false)
					return number;

				else
					add_item(&head, number);
			}

			else
			{
				jac->failure = true;
				fprintf(stderr,"Illegal character %s\n", jac->buf);
				return 0;
			}
		}

		else
			continue;

	} /* End of parsing */

	/* Once parsed jac evaluates the expression */

	if (jac->failure == true)
		return 0;

	if (jac->len == MAX)
	{
		fprintf(stderr,"%s\n","The limit size of the expression was reached");

		if (head != NULL)
			free(head);

		jac->failure = true;

		return -2;
	}

	else if (head == NULL)
	{
		jac->failure = true;
		return -2;
	}

	else
	{
		head->op = TAIL_OP;

		if (head->next != NULL)
		{
			reverse(&head);
			calculate(head);
			result = head->value;
		}

		else
			result = head->value;

		free(head);

		return result;
	}

	return 0;
}

long double parseConstants(struct control *jac)
{
	int incrBuff = 0;
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

	number = parse_evaluate_expr(jac);

	jac->inFunc = NONE;

	return switchFunc(&func, &number);
}

long double calculate (struct node *head)
{
	struct node *tmp = head;

	/* divide and multiply first */

	while (tmp->next != NULL && tmp->op != TAIL_OP)
	{
		if (tmp->op == '/' || tmp->op == '*')
		{
			if (tmp->op  == '/')
				tmp->value = tmp->value / tmp->next->value;

			else
				tmp->value = tmp->value * tmp->next->value;

			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}

	tmp = head;

	/* Now do additions and subtraction */

	while (tmp != NULL && tmp->next != NULL && tmp->op != TAIL_OP)
	{
		if (tmp->op == '+' || tmp->op == '-')
		{
			if (tmp->op  == '+')
				tmp->value = tmp->value + tmp->next->value;

			else
				tmp->value = tmp->value - tmp->next->value;

			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}

	return head->value;
}

long double switchFunc(enum functions *func, long double *number)
{
	switch (*func)
	{
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

void incrementBuff (struct control *jac, int n)
{
	jac->buf += n;
	jac->len += n;
}

enum functions searchFunction (struct control *jac)
{
	if (strncmp(jac->buf, "cos", 3) == 0)
	{
		incrementBuff(jac,3);
		return COS;
	}

	else if (strncmp(jac->buf, "sin", 3) == 0)
	{
		incrementBuff(jac,3);
		return SIN;
	}

	else if (strncmp(jac->buf, "asinhl", 6) == 0)
	{
		incrementBuff(jac,6);
		return ASINH;
	}

	else if (strncmp(jac->buf, "asin", 4) == 0)
	{
		incrementBuff(jac,4);
		return ASIN;
	}

	else if (strncmp(jac->buf, "atan", 4) == 0)
	{
		incrementBuff(jac,4);
		return ATAN;
	}

	else if (strncmp(jac->buf, "acos", 4) == 0)
	{
		incrementBuff(jac,4);
		return ACOS;
	}

	else if (strncmp(jac->buf, "sqrt", 4) == 0)
	{
		incrementBuff(jac,4);
		return SQRT;
	}

	else if (strncmp(jac->buf, "sinh", 4) == 0)
	{
		incrementBuff(jac,4);
		return SINH;
	}

	else if (strncmp(jac->buf, "cosh", 4) == 0)
	{
		incrementBuff(jac,4);
		return COSH;
	}

	else if (strncmp(jac->buf, "tanh", 4) == 0)
	{
		incrementBuff(jac,4);
		return TANH;
	}

	else if (strncmp(jac->buf, "tan", 3) == 0)
	{
		incrementBuff(jac,3);
		return TAN;
	}

	else if (strncmp(jac->buf, "log", 3) == 0)
	{
		incrementBuff(jac,3);
		return LOG;
	}

	else if (strncmp(jac->buf, "ln", 2) == 0)
	{
		incrementBuff(jac,2);
		return LN;
	}

	else if (strncmp(jac->buf, "bin_dec", 7) == 0)
	{
		incrementBuff(jac,7);
		return BIN_DEC;
	}

	else if (strncmp(jac->buf, "dec_bin", 7) == 0)
	{
		incrementBuff(jac,7);
		return DEC_BIN;
	}

	else if (strncmp(jac->buf, "cbrt", 4) == 0)
	{
		incrementBuff(jac,4);
		return CBRT;
	}

	else if (strncmp(jac->buf, "abs", 3) == 0)
	{
		incrementBuff(jac,3);
		return ABS;
	}

	return NONE;
}
