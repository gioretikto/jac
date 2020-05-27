#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000

#ifndef M_PIl
#define M_PIl 3.141592653589793238462643383279502884L
#endif

struct n{
	long double value;
	char op;
	struct n *next;
	char unary;
};

struct control{
	char *buf;
	int len;
	bool caller;
};

void add_item(struct n **head, long double data);
void print_result(long double x);
void remove_spaces(char *str);
void calculate (struct n *head);
void delNextNode (struct n *node_pt);
unsigned long factorial(unsigned long f);
long double parse_evaluate_expr(struct control *jac);
bool areParenthesisBalanced(char *exp);
bool checkSyntax(char *str);
bool checkPar (struct control *jac, struct n *head);
long double evaluateFunc (struct control *jac, char op);
long double switchFunc(char *op, long double *number);
void incrementBuff (struct control *jac, int n);
int bin_dec(long long n);
long long dec_bin(int n);
