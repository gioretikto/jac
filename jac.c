#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

int main()
{
	struct n *head = NULL;
	struct n *end;
	char line[MAX];
	char *buf = line;
	int n;
	double num;
	double tmp;

	_Bool success = 1;
	_Bool operation[6] = {0};
	
	int len = 0;
	
	printf("Enter q to quit\n>>");
	
	for(;;) {
        
        if(fgets(buf = line, MAX, stdin) == NULL || *buf == 'q')
            break;
        
		remove_spaces(buf);		
		
		while (*buf && *buf != '\n' && len < MAX) {
		
			if ( isdigit(*buf) || ( (*buf == '+' || *buf == '-') && len == 0) ) {
				sscanf(buf, "%lf%n", &num, &n);
				add_item(&head, num, '?');
				buf += n;
				len += n;				
							
				if ( *buf == '!') { /* Factorial */
					buf++;
					head->value = factorial(head->value);
					len++;
				}
			
			}
			
			if ( buf[-1] == '(' && (*buf == '+' || *buf == '-') ) {
				sscanf(buf, "%lf%n", &num, &n);
				add_item(&head, num, '?');
				buf += n;
				len += n;
			}
		    			
			else if (*buf == '/' || *buf == '*' || *buf == '+' || *buf == '-') {
			
				operation[*buf - 42] = 1;
				
				if (head->op == '?') {
					head->op = *buf;
				}
				
				else { /*Syntax Error*/
					success = 0;
					break;
				}
				
				buf++;
			}
			
			else if (*buf == '^') {
				buf++;
				len++;
				sscanf(buf, "%lf%n", &num, &n);
				buf +=n;
				len +=n;
				
				if(*buf=='/'){
					buf++;
					len++;
					sscanf(buf, "%lf%n", &tmp, &n);
					buf += n;
					len += n;
					num = num/tmp;
					printf("result: %lf\n", num);
				}
				head->value = pow(head->value, num);
			}
			
		    else if (*buf == '(') {
		    
				if(head != NULL && head->op == '?') {
					head->op = '*';
					operation[0] = 1;
				}
				end = head;
				buf++;
				len++;		
			}
			
			else if (*buf == ')') {
				calculate(head, end, operation);
				end = NULL;
				buf++;
				len++;
			}
			
			else if (strncmp(buf, "sin", 3) == 0 || strncmp(buf, "cos", 3) == 0 || strncmp(buf, "tan", 3) == 0 || strncmp(buf, "exp", 3) == 0 || strncmp(buf, "log", 3) == 0  ) {
				operation[2] = 1;
				add_item(&head, 0, *buf);
		        buf += 3;
		        len += 3;
		    }
		    
		    else if (strncmp(buf, "ln", 2) == 0) {
		    	operation[2] = 1;
				add_item(&head, 0, 'n');
		        buf += 2;
		        len += 2;
		    }
		    
		    else if (strncmp(buf, "sqrt", 4) == 0 || strncmp(buf, "asin", 4) == 0  || strncmp(buf, "acos", 4) == 0 || strncmp(buf, "atan", 4) == 0) {
				operation[2] = 1;
				add_item(&head, 0, buf[2]);
		        buf += 4;
		        len += 4;
		    }
		    
		    else
		    	buf++;
		} /* end of while (*buf && *buf != '\n' && len < MAX) */
		    
		if(len == MAX) {
			printf("The limit size of the expression was reached\n");
			return 1;
		}
					
		if (success == 1 && head != NULL) {
			calculate(head, end, operation);
			print_num(head->value);
			putchar('\n');
			free(head);
		}

		else 
		    printf("Syntax Error\n");				

		head = NULL;
		success = 1;
		len = 0;
		memset(operation, 0, 6);
		printf(">>");
	}

	return 0;
}

void add_item(struct n **ptr, double data, char s)
{
        struct n *item = malloc(sizeof *item);

		item->value = data;
		item->next = *ptr;
		item->op = '?';
		item->unary = s;
		*ptr = item;
}

void print_num(double x)
{
    double i, r = modf(x, &i);
    
    if (fabs(r) <.00001)
        printf("%.f ", i);
    
    else printf("%.20lf ", x);
}

void remove_spaces(char *str)
{ 
    int count = 0;
  
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
                                    
    str[count] = '\0';
}

void calculate (struct n *head, struct n *end, _Bool *operation)
{
		if(operation[2] == 1)
			unary(head, end);
		if(operation[0] == 1)
			multiply(head, end);
		if(operation[5] == 1)
			divide(head, end);
		if(operation[1] == 1 || operation[3] == 1)
			add(head, end);
}

void unary (struct n *head, struct n *end)
{
    if (head->next == end || head->next == NULL)
        return;
   
    else {
    
    	if ((head->next)->unary != '?') {
			switch( (head->next)->unary) {
			
				case 'a':
					head->value = atan(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'c':
					head->value = cos(head->value);
					del_next(head);
					unary(head, end);
					break;
			
				case 'e':
					head->value = exp(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'i':
					head->value = asin(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'l':
					head->value = log10(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'n':
					head->value = log10(head->value);
					del_next(head);
					unary(head, end);
					break;	
					
				case 'o':
					head->value = acos(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'r':
					head->value = sqrt(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 's':
					head->value = sin(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 't':
					head->value = tan(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				default:
					break;			
			}
		}
		
		else
			unary(head->next, end);
	}
}

void multiply (struct n *head, struct n *end)
{
	double result;

    if (head->next == end || head->next == NULL)
        return;
   
    else {
		
		if ((head->next)->op == '*') {
			result = (head->next)->value * head->value;
			head->value = result;
			del_next(head);
			multiply(head, end);
		}
		
		else
			multiply(head->next, end);
	}
}

void divide (struct n *head, struct n *end)
{
	double result;

    if (head->next == end || head->next == NULL)
        return;
   
    else {
		
		if ((head->next)->op == '/') {
			result = (head->next)->value / head->value;
			head->value = result;
			del_next(head);
			divide(head, end);
		}
		
		else
			divide(head->next, end);
	}
}

void add (struct n *head, struct n *end)
{

	double result;

    if (head->next == end || head->next == NULL)
        return;
   
    else {
		
		if ((head->next)->op == '+' || (head->next)->op == '-') {
		
			if ((head->next)->op == '+')
       	    	result = (head->next)->value +1 * head->value;
       	    	
            else {
       			result = (head->next)->value -1 * head->value;
       		}

			head->value = result;
			del_next(head);
			add(head, end);
		}
		
		else
			add(head->next, end);
	}
}

void del_next (struct n *before_del_next)
{
    struct n *temp;
    temp = before_del_next->next;
    before_del_next->next = temp->next;
    free(temp);
}

unsigned long factorial(unsigned long f)
{
    if ( f == 0 ) 
        return 1;
    return(f * factorial(f - 1));
}