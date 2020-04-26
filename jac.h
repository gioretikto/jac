#include <stdlib.h>
#include <math.h>

#define MAX 1000

struct n{
	double value;
	char op;
	struct n *next;
	char unary;
};

void add_item(struct n **ptr, double data, char s);
void print_num(double x);
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