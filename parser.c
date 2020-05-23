#include "jac.h"

bool parse_expression(char *buf) {

	long double num;
	long double tmp;

	bool success = 1;
	bool operation[6] = {0}; 	/* Setting the bit describes type of the operation:
								   operation[2] = 1 if unary */	

	struct n *head = NULL;
	struct n *end = NULL;

	int len = 0;
	int n;

	while (*buf && *buf != '\n' && len < MAX) {
			
		if ( isdigit(*buf) || ( (*buf == '+' || *buf == '-') && len == 0) ) {
				
			sscanf(buf, "%Lf%n", &num, &n);
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
			 
			add_item(&head, PI, '?');
			buf += 2;
		    len += 2;
				
		}
				
		else if ( buf[-1] == '(' && (*buf == '+' || *buf == '-') ) {
				
			sscanf(buf, "%Lf%n", &num, &n);
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
			sscanf(buf, "%Lf%n", &num, &n);
			buf +=n;
			len +=n;
					
			if (*buf=='/') {
					
				buf++;
				len++;
				sscanf(buf, "%Lf%n", &tmp, &n);
				buf += n;
				len += n;
				num = num/tmp;
				printf("result: %Lf\n", num);
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
			return false;
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
				
		return true;
}
