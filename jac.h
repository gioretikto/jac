#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000

#define SPEED_LIGHT 299792458 			/* Speed of light in vacuum m/s (exact) */
#define CHARGE 1.602176634e-19 			/* C elementary charge */
#define PLANK 6.62606896e-34  			/* Planck's constant j⋅s */
#define ELECTRON_MASS 9.1093837015e-31 	/* electron mass kg*/
#define PROTON_MASS 1.6726219236951e-27	/* proton mass kg */
#define E_0 8.854187817e-12    			/* Permittivity of free space (exact)  ^2/N⋅m^2 */
#define AVOGADRO 6.02214179e23			/* Avogadros's number particles/mol */
#define K_B 1.3806504e-23				/* Boltzmann's constant j/K */

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

#ifndef M_E
#define M_E 2.71828182845904523536028747135266249775L
#endif

enum functions {SIN, COS, TAN, ASIN, ACOS, ATAN, EXP, LOG, LN, SQRT, SINH, COSH, TANH, ASINH, ABS, MOD, CBRT, BIN_DEC, DEC_BIN};

struct node{
	long double value;
	char op;
	struct node *next;
	char unary;
};

struct control{
	char *buf;
	int len;
	bool caller;
	bool failure;
};

void add_item(struct node **head, long double data);
void print_result(long double x);
void remove_spaces(char *str);
void calculate (struct node *head);
void delNextNode (struct node *node_pt);
unsigned long factorial(unsigned long f);
long double parse_evaluate_expr(struct control *jac);
bool areParenthesisBalanced(char *exp);
bool checkSyntax(char *str);
bool checkPar (struct control *jac, struct node *head);
long double evaluateFunc (struct control *jac, enum functions func);
long double switchFunc(enum functions *func, long double *number);
void incrementBuff (struct control *jac, int n);
int bin_dec(long long n);
long long dec_bin(int n);
