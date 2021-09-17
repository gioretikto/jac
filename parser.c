#include "jac.h"
#include <math.h>

int searchFunctions(struct control *jac, struct node *head);

long double parse_evaluate_expr(struct control *jac)
{
	long double number, result;

	unsigned int n = 0;

	struct node *head = NULL;

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

			else
				head->op = *jac->buf;

			incrementBuff(jac,1);
		}

		if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
		{
 			add_item(&head, number);
			incrementBuff(jac,n);
		}

		else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{')
		{
			incrementBuff(jac,1);

			jac->caller = true;					 /* indicates recursive call */

			number = parse_evaluate_expr(jac);   /* Recursive call: evaluate expression inside parenthesis */

			add_item(&head, number);
		}

		else if (*jac->buf == ')' || *jac->buf == ']' || *jac->buf == '}')
		{
			incrementBuff(jac,1);

			if (jac->caller != false)
			{
				jac->caller = false;
				break;
			}
		}

		/* Start parsing for functions like sin, cos, etc */

		else if(searchFunctions(jac, head));
		
		else if (isalpha(*jac->buf)) 	/* Syntax error */
		{
			jac->failure = true;
			fprintf(stderr,"Illegal character %s\n", jac->buf);
			return 0;
		}

		else
			continue;

	}

	if (jac->len == MAX)
	{
		fprintf(stderr,"%s\n","The limit size of the expression was reached");

		if (head != NULL)
			free(head);

		jac->failure = true;

		return 0;
	}

	else if (head == NULL)
	{
		jac->failure = true;
		return 0;
	}

	else
	{
		head->op = NOT_SET;

		if (head->next != NULL)
			calculate(head);

		result = head->value;

		free(head);

		return result;
	}

	return 0;
}

void add_item (struct node **ptr, long double data)
{
	struct node *new_item = malloc(sizeof *new_item);

	new_item->value = data;
	new_item->next = *ptr;
	new_item->op = '*';
	*ptr = new_item;
}

void calculate (struct node *head)
{
	struct node *tmp = head;

	/* divide first */

	while (tmp->next != NULL && tmp->next->op != NOT_SET)
	{
		if (tmp->next->op == '/')
		{
			tmp->value = tmp->next->value/tmp->value;

			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}

	tmp = head;

	/* then multiply */

	while (tmp->next != NULL && tmp->next->op != NOT_SET)
	{
		if (tmp->next->op == '*')
		{

			tmp->value = tmp->next->value * tmp->value;


			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}

	tmp = head;

	/* Now do additions and subtraction */

	while (tmp != NULL && tmp->next != NULL && tmp->next->op != NOT_SET)
	{
		if (tmp->next->op == '+' || tmp->next->op == '-')
		{
			if (tmp->next->op  == '+')
				tmp->value = tmp->next->value + tmp->value;

			else
				tmp->value = tmp->next->value - tmp->value;

			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}
}

void delNextNode (struct node *head)
{
	struct node *tmp = head->next;
	head->next = head->next->next;
	free(tmp);
}

long double evaluateFunc (struct control *jac, enum functions func)
{
	long double number;
	unsigned int n = 0;

    if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
    {
    	incrementBuff(jac,n);
		return switchFunc(&func, &number);
	}

	else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{')
	{
		jac->caller = true;					 /* indicates recursive call */
		incrementBuff(jac,1);
		number = parse_evaluate_expr(jac);   /* Evaluate expression inside parenthesis */

		return switchFunc(&func, &number);
	}

	else
		return false;
}

long double switchFunc(enum functions *func, long double *number)
{
	switch(*func)
	{
		case SIN:
			return sinl(*number);
			break;

		case COS:
	    	return cosl(*number);
	    	break;

		case TAN:
			return tanl(*number);
			break;
			
		case EXP:
			return *number;
			break;

		case ATAN:
			return atanl(*number);
			break;

		case SINH:
			return sinhl(*number);
			break;

		case COSH:
			return coshl(*number);
			break;

		case TANH:
			return tanhl(*number);
			break;

		case ASINH:
			return asinhl(*number);
			break;

		case BIN_DEC:
			return bin_dec(*number);
			break;

		case DEC_BIN:
	    	return dec_bin(*number);
	    	break;

		case ABS:
			return fabsl(*number);
			break;

	    case LOG:
	    	return log10l(*number);
	    	break;

	    case ASIN:
	    	return asinl(*number);
	    	break;

    	case LN:
	    	return logl(*number);
	    	break;

	    case MOD:
			return *number;
			break;

	   	case ACOS:
	    	return acosl(*number);
	    	break;

	    case SQRT:
			return sqrtl(*number);
			break;

		case CBRT:
			return cbrtl(*number);
			break;

	    default:
			return false;
		   	break;
	}
}

void incrementBuff (struct control *jac, int n)
{
	jac->buf += n;
	jac->len += n;
}



/* Parse for functions like sin, cos, etc */

int searchFunctions(struct control *jac, struct node *head)
{

	int found = true;

	long double number;

	unsigned int n = 0;

	if (strncmp(jac->buf, "m_p", 3) == 0) 			/* Proton Mass */
	{
			add_item(&head, PROTON_MASS);
			incrementBuff(jac,3);
	}

	else if (strncmp(jac->buf, "m_e", 3) == 0) 		/* Electron mass */
	{
			add_item(&head, ELECTRON_MASS);
			incrementBuff(jac,3);
	}

	else if (strncmp(jac->buf, "c_0", 3) == 0)  	/* Speed of light in vacuum m/s (exact) */
	{
		add_item(&head, SPEED_LIGHT);
		incrementBuff(jac,3);
	}

	else if (*jac->buf == 'q')						/* elementary charge*/
	{
		add_item(&head, CHARGE);
		incrementBuff(jac,1);		
	}

	else if (*jac->buf == 'h')						/* Plank's constant*/
	{
		add_item(&head, PLANK);
		incrementBuff(jac,1);
	}

	else if (*jac->buf == '^')
	{
		incrementBuff(jac,1);
		number = evaluateFunc(jac, EXP);
		head->value = pow(head->value, number);
	}

	else if (*jac->buf == '%')
	{
		incrementBuff(jac,1);
		number = evaluateFunc(jac, MOD);
		head->value = fmodl(head->value, number);
	}

	else if (*jac->buf == '!') 	/* Factorial */
	{
		head->value = factorial(head->value);
		incrementBuff(jac,1);
	}

	else if (strncmp(jac->buf, "asinhl", 6) == 0)
	{
		incrementBuff(jac,6);
		number = evaluateFunc(jac, ASINH);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "asin", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, ASIN);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "atan", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, ATAN);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "acos", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, ACOS);
		add_item(&head, number);			
	}

	else if (strncmp(jac->buf, "sqrt", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, SQRT);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "sinh", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, SINH);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "cosh", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, COSH);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "tanh", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, TANH);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "cos", 3) == 0)
	{
		incrementBuff(jac,3);
		number = evaluateFunc(jac, COS);
		add_item(&head, number);
	}
		
	else if (strncmp(jac->buf, "sin", 3) == 0)
	{
		incrementBuff(jac,3);
		number = evaluateFunc(jac, SIN);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "tan", 3) == 0)
	{
		incrementBuff(jac,3);
		number = evaluateFunc(jac, TAN);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "log", 3) == 0)
	{
		incrementBuff(jac,3);
		number = evaluateFunc(jac, LOG);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "ln", 2) == 0)
	{
		incrementBuff(jac,2);
		number = evaluateFunc(jac, LN);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "e_0", 3) == 0) 		/* Permittivity of free space */
	{
		add_item(&head, E_0);
		incrementBuff(jac,3);
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

	else if (*jac->buf == 'e')
	{
		add_item(&head, M_E);
		incrementBuff(jac,1);
	}

	else if (strncmp(jac->buf, "n_a", 3) == 0)  /* Avogadros's number */
	{
		add_item(&head, AVOGADRO);
		incrementBuff(jac,3);
	}

	else if (strncmp(jac->buf, "bin_dec", 7) == 0)
	{
		incrementBuff(jac,7);
		number = evaluateFunc(jac, BIN_DEC);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "dec_bin", 7) == 0)
	{
		incrementBuff(jac,7);
		number = evaluateFunc(jac, DEC_BIN);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "cbrt", 4) == 0)
	{
		incrementBuff(jac,4);
		number = evaluateFunc(jac, CBRT);
		add_item(&head, number);
	}

	else if (strncmp(jac->buf, "pi", 2) == 0)
	{
		add_item(&head, M_PIl);
		incrementBuff(jac,2);
	}

	else if (strncmp(jac->buf, "abs", 3) == 0)
	{
		incrementBuff(jac,3);
		number = evaluateFunc(jac, ABS);
		add_item(&head, number);
	}

	else if (*jac->buf == 'k')  	/* Boltzmann's constant */
	{
		add_item(&head, K_B);
		incrementBuff(jac,1);
	}

	else
		found = false;

	return found;
}
