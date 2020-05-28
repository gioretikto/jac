#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000

#define C 299792458 	/* Speed of light (m/s)*/
#define Q 1.602176634e-19  /* elementary charge */
#define H 6.626192e-27    /* Planck's constant*/
#define ME 9.1093837015e-31 /* electron mass */
#define MP 1.6726219236951e-27 /* proton mass */

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

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
	char func;
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
long double evaluateFunc (struct control *jac);
long double switchFunc(char *op, long double *number);
void incrementBuff (struct control *jac, int n);
int bin_dec(long long n);
long long dec_bin(int n);
