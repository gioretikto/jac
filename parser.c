#include "jac.h"
#include "lists.h"
#include <math.h>

#define TAIL_OP 'x' /* Operator of the tail node */
#define ERROR '?'

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

enum functions {COS,SIN,ASINH,ASIN,ATAN,ACOS,SQRT,SINH,COSH,TANH,TAN,LOG,LN,MULT_DIV_POW_MOD,NONE};

void reverse(struct node** head_ref);
void add_item (struct node **ptr, long double data);
void delNextNode (struct node *node_pt);
long double calculate (struct node **head);
long double evaluateFuncResult (struct control *jac, enum functions func);
long double switchFunc(enum functions *func, const long double *number);
unsigned long factorial(unsigned long f);
/*int bin_dec(long long n);
long long dec_bin(int n);
*/
long double parseConstants(struct control *jac);
bool searchBinaryFunction (struct control *jac, struct node *head);
bool searchPowFunction (struct control *jac, long double *number);

enum functions searchFunction (struct control *jac);

long double mult_div_pow_mod(long double number)
{
	return number;
}

long double my_cosine(long double number)
{
	long double diff = M_PIl/2 - fabsl(fmodl(M_PIl/2- fmodl(number, M_PIl/2), M_PIl/2));

	if (diff < 0.00000000001)
		return 0;
	else
		return cosl(number);
}

long double my_sine(long double number)
{
	long double mod_pi = fmod(number, M_PI);
	long double diff = M_PIl/2 - fabsl(fmod(M_PI/2 - mod_pi, M_PI));

	if (diff < 0.01)
		return 0;
	else
		return sinl(number);
}

long double abort_parsing(struct node *head)
{
	if (head != NULL)
		free(head);

	return -2;
}

long double parse_evaluate_expr(struct control *jac, bool inFunc)
{
	long double number;

	unsigned int n = 0;

	struct node *head = NULL;

	enum functions func = NONE;

	while (*jac->buf != '\0' && jac->buf[0] != ERROR)
	{
		if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
		{
			jac->buf += n;

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

			jac->buf++;
			head->op = '*';
		}

		else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{')
		{
			jac->buf++;
			jac->par++;

			add_item(&head, parse_evaluate_expr(jac, inFunc));

			if (inFunc == true)
			{
				number = calculate(&head);
				free(head);
				return number;
			}

			if (*jac->buf != '\0')										/* For situations like (3+2)5 */
				if (isalpha(*jac->buf) || isdigit(*jac->buf))
					head->op = '*';
		}

		else if (*jac->buf == ')' || *jac->buf == ']' || *jac->buf == '}')
		{
			jac->buf++;
			jac->par--;

			if (head != NULL && jac->par == 0)
			{
				number = calculate(&head);
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
			if ((number = parseConstants(jac)) != 0)
			{
				add_item(&head, number);

				if (*jac->buf == '(')		/* For situations like 5(3+2) */
					head->op = '*';
			}

			if (*jac->buf == 'E')
			{
				add_item(&head, 10);
				jac->buf++;

				if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
				{
					jac->buf += n;
					head->value = pow(head->value, number);
				}

				else
					jac->buf[0] = ERROR;
			}
		}

		else
			continue;

	} /* End of parsing */

	/* Once parsed, jac evaluates the expression */

	if (jac->buf[0] == ERROR)
		return abort_parsing(head);

	else if (head == NULL)
	{
		jac->buf[0] = ERROR;
		return -2;
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
	if (*jac->buf == '/')
	{
		jac->buf++;
		head->value = head->value / (evaluateFuncResult(jac, MULT_DIV_POW_MOD));
	}

	else if (*jac->buf == '*')
	{
		jac->buf++;
		head->value = head->value * (evaluateFuncResult(jac, MULT_DIV_POW_MOD));
		head->op = TAIL_OP;	 /* we don't want head->op to contain '*' and cause interference in calculate() */
	}

	else if (*jac->buf == '^')
	{
		jac->buf++;
		head->value = pow(head->value, evaluateFuncResult(jac, MULT_DIV_POW_MOD));
	}

	else if (*jac->buf == '!') 	/* Factorial */
	{
		jac->buf++;
		head->value = factorial(head->value);
	}

	else if (*jac->buf == '%')
	{
		jac->buf++;
		head->value = fmodl(head->value, evaluateFuncResult(jac, MULT_DIV_POW_MOD));
	}

	else
		return false;

	return true;
}

long double parseConstants(struct control *jac)
{
	long double data = 0;

	if (strncmp(jac->buf, "pi", 2) == 0)
	{
		data = M_PIl;
		jac->buf += 2;
	}

	else if (strncmp(jac->buf, "m_p", 3) == 0) 		/* Proton Mass */
	{
		data = PROTON_MASS;
		jac->buf += 3;
	}

	else if (*jac->buf == 'e')
	{
		data = M_E;
		jac->buf += 1;
	}

	else if (strncmp(jac->buf, "m_e", 3) == 0) 		/* Electron mass */
	{
		data = ELECTRON_MASS;
		jac->buf += 3;
	}

	else if (strncmp(jac->buf, "c_0", 3) == 0)  	/* Speed of light in vacuum m/s (exact) */
	{
		data = SPEED_LIGHT;
		jac->buf += 3;
	}

	else if (*jac->buf == 'q')						/* elementary charge*/
	{
		data = CHARGE;
		jac->buf++;
	}

	else if (*jac->buf == 'h')						/* Plank's constant*/
	{
		data = PLANK;
		jac->buf++;
	}

	else if (strncmp(jac->buf, "e_0", 3) == 0) 		/* Permittivity of free space */
	{
		data = E_0;
		jac->buf += 3;
	}

	else if (strncmp(jac->buf, "n_a", 3) == 0)  /* Avogadros's number */
	{
		data = AVOGADRO;
		jac->buf += 3;
	}	

	else if (*jac->buf == 'k')  	/* Boltzmann's constant */
	{
		data = K_B;
		jac->buf++;
	}

	else
		return data;

	return data;
}

long double evaluateFuncResult (struct control *jac, enum functions func)
{
	long double number;
	unsigned int n;

	typedef long double (*my_func)(long double);

	enum functions func2;

	static my_func array[] = {my_cosine, my_sine, asinhl, asinl, atanl, acosl, sqrtl, sinhl, coshl, tanhl, tanl, log10l, logl, mult_div_pow_mod};

	if (1 == sscanf(jac->buf, "%Lf%n", &number, &n))
	{
		jac->buf += n;

		while(searchPowFunction(jac, &number))		/* For situations like 5^2^3^.. */
			{;}

		return (array[func](number));
	}

	else if (isalpha(*jac->buf)) 	/* Parse for functions and constants */
	{
		if ((func2 = searchFunction(jac)) != NONE)
			return evaluateFuncResult(jac, func2);

		else
		{
			number = parseConstants(jac);

			if (number != 0)
				return (array[func](number));
		}
	}

	else
		number = parse_evaluate_expr(jac, true);

	return (array[func](number));
}

enum functions searchFunction (struct control *jac)
{
	enum functions func = NONE;

	if (strncmp(jac->buf, "cos", 3) == 0)
	{
		jac->buf += 3;
		func = COS;
	}

	else if (strncmp(jac->buf, "sin", 3) == 0)
	{
		jac->buf += 3;
		func = SIN;
	}

	else if (strncmp(jac->buf, "asinhl", 6) == 0)
	{
		jac->buf += 6;
		func = ASINH;
	}

	else if (strncmp(jac->buf, "asin", 4) == 0)
	{
		jac->buf += 4;
		func = ASIN;
	}

	else if (strncmp(jac->buf, "atan", 4) == 0)
	{
		jac->buf += 4;
		func = ATAN;
	}

	else if (strncmp(jac->buf, "acos", 4) == 0)
	{
		jac->buf += 4;
		func = ACOS;
	}

	else if (strncmp(jac->buf, "sqrt", 4) == 0)
	{
		jac->buf += 4;
		func = SQRT;
	}

	else if (strncmp(jac->buf, "sinh", 4) == 0)
	{
		jac->buf += 4;
		func = SINH;
	}

	else if (strncmp(jac->buf, "cosh", 4) == 0)
	{
		jac->buf += 4;
		func = COSH;
	}

	else if (strncmp(jac->buf, "tanh", 4) == 0)
	{
		jac->buf += 4;
		func = TANH;
	}

	else if (strncmp(jac->buf, "tan", 3) == 0)
	{
		jac->buf += 3;
		func = TAN;
	}

	else if (strncmp(jac->buf, "log", 3) == 0)
	{
		jac->buf += 3;
		func = LOG;
	}

	else if (strncmp(jac->buf, "ln", 2) == 0)
	{
		jac->buf += 2;
		func = LN;
	}

	/*else if (strncmp(jac->buf, "bin_dec", 7) == 0)
	{
		jac->buf += 7;
		func = BIN_DEC;
	}

	else if (strncmp(jac->buf, "dec_bin", 7) == 0)
	{
		jac->buf += 7;
		func = DEC_BIN;
	}*/

	else
		return NONE;

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

bool searchPowFunction (struct control *jac, long double *number)
{
		if (*jac->buf == '^')
		{
			jac->buf += 1;
			*number = pow(*number, evaluateFuncResult(jac, MULT_DIV_POW_MOD));
		}

		else if (*jac->buf == '!') 	/* Factorial */
		{
			jac->buf += 1;
			*number = factorial(*number);
		}

		else if (*jac->buf == '%')
		{
			jac->buf += 1;
			*number = fmodl(*number, evaluateFuncResult(jac, MULT_DIV_POW_MOD));
		}

		else
			return false;

		return true;
}
