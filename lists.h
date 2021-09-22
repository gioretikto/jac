#include <stdlib.h>

struct node{
	long double value;
	char op;
	struct node *next;
	char unary;
};
