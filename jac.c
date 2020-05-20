#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "jac.h"

int main() {

	struct n *head = NULL;
	struct n *end;
	
	char line[MAX];
	char *buf = line;
	int n;
	double num;
	double tmp;

	_Bool success = 1;
	_Bool operation[6] = {0}; 	/* Setting the bit describes type of the operation:
								   operation[2] = 1 if unary */	
	int len = 0;
	
	printf("Enter q to quit\n>>");
	
	for(;;) {
        
        if (fgets(buf = line, MAX, stdin) == NULL || *buf == 'q')
            break;
        
		remove_spaces(buf);		
		
		while (*buf && *buf != '\n' && len < MAX) {
		
			if ( isdigit(*buf) || ( (*buf == '+' || *buf == '-') && len == 0) ) {
			
				sscanf(buf, "%lf%n", &num, &n);
				add_item(&head, num, '?');
				buf += n;
				len += n;				
							
				if (*buf == '!') { /* Factorial */
					buf++;
					head->value = factorial(head->value);
					len++;
				}
			
			}
			
			else if (strncmp(buf, "pi", 2) == 0) {
			 
		    	add_item(&head, M_PI, '?');
		    	buf += 2;
		        len += 2;
		    
		    }			
			
			else if ( buf[-1] == '(' && (*buf == '+' || *buf == '-') ) {
			
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
				
				if (*buf=='/') {
				
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
		    
				if (head != NULL && head->op == '?') {
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
		    
		    else if (strncmp(buf, "bin_dec", 7) == 0 || strncmp(buf, "dec_bin", 7) == 0 ) {
				operation[2] = 1;
				add_item(&head, 0, *buf);
		        buf += 7;
		        len += 7;
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
		    
		if (len == MAX) {
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

void print_num(double x) {

    double i, r = modf(x, &i);
    
    if (fabs(r) <.00001)
        printf("%.f ", i);
    
    else printf("%.20lf ", x);
}

void remove_spaces(char *str) {

    int count = 0;
  	int i;
    for ( i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
                                    
    str[count] = '\0';
}

void calculate (struct n *head, struct n *end, _Bool *operation) {

		if (operation[2] == 1)
			unary(head, end);
			
		if (operation[0] == 1)
			multiply(head, end);
			
		if (operation[5] == 1)
			divide(head, end);
			
		if (operation[1] == 1 || operation[3] == 1)
			add(head, end);
}

void unary (struct n *head, struct n *end)		/* Evaluate unary operations */
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
				
				case 'b':
					head->value = bin_dec(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'c':
					head->value = cos(head->value);
					del_next(head);
					unary(head, end);
					break;
					
				case 'd':
					head->value = dec_bin(head->value);
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
