#include "jac.h"
#include <math.h>

#define TAIL_OP 'x' /* Operator of the tail node */
enum functions {SIN, COS, TAN, ASIN, ACOS, ATAN, EXP, LOG, LN, SQRT, SINH, COSH, TANH, ASINH, ABS, MOD, CBRT, BIN_DEC, DEC_BIN};

void add_item (struct node **ptr, long double data);
static void reverse(struct node** head_ref);
void delNextNode (struct node *node_pt);
void calculate (struct node *head);
long double evaluateFunc (struct control *jac, enum functions func);
long double switchFunc(enum functions *func, long double *number);
int bin_dec(long long n);
long long dec_bin(int n);

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

		else if (strncmp(jac->buf, "m_p", 3) == 0) 		/* Proton Mass */
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

		else if (isalpha(*jac->buf)) 	/* Syntax error */
		{
			jac->failure = true;
			fprintf(stderr,"Illegal character %s\n", jac->buf);
			return 0;
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

		return 0;
	}

	else if (head == NULL)
	{
		jac->failure = true;
		return 0;
	}

	else
	{
		head->op = TAIL_OP;

		if (head->next != NULL)
		{
			reverse(&head);
			calculate(head);
		}

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

	while (tmp->next != NULL && tmp->op != TAIL_OP)
	{
		if (tmp->op == '/')
		{
			tmp->value = tmp->value / tmp->next->value;

			delNextNode(tmp);
		}

		else
			tmp = tmp->next;
	}

	tmp = head;

	/* then multiply */

	while (tmp->next != NULL && tmp->op != TAIL_OP)
	{
		if (tmp->op == '*')
		{

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
}

void delNextNode (struct node *head)
{
	struct node *tmp = head->next;
	head->op = head->next->op;
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
	{
		jac->failure = true;
		return false;
	}
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
			
		case EXP:
			return *number;

		case ATAN:
			return atanl(*number);

		case SINH:
			return sinhl(*number);

		case COSH:
			return coshl(*number);

		case TANH:
			return tanhl(*number);
			break;

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
			return false;
	}
}

void incrementBuff (struct control *jac, int n)
{
	jac->buf += n;
	jac->len += n;
}

/* Function to reverse the linked list */
static void reverse(struct node** head_ref)
{
    struct node* prev = NULL;
    struct node* current = *head_ref;
    struct node* next = NULL;

    while (current != NULL) {

        /* Store next */
        next = current->next;
 
        /* Reverse current node's pointer */
        current->next = prev;
 
        /* Move pointers one position ahead */
        prev = current;
        current = next;
    }
    *head_ref = prev;
}
