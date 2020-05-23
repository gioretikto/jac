#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000

#ifndef PI
#define PI 3.141592653589793238462643383279502884L
#endif

struct n{
	long double value;
	char op;
	struct n *next;
	char unary;
};

void add_item(struct n **ptr, long double data, char s);
void print_num(long double x);
void remove_spaces(char *str);
void multiply (struct n *head, struct n *end);
void divide (struct n *head, struct n *end);
void add (struct n *head, struct n *end);
void calculate (struct n *head, struct n *end, _Bool *operation);
void del_next(struct n *head);
void unary (struct n *head, struct n *end);
unsigned long factorial(unsigned long f);
int bin_dec(long long n);
long long dec_bin(int n);
bool parse_expression(char *buf);
