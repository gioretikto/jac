#include "jac.h"

long double parse_evaluate_expr(struct control *jac) {

	long double number, result;
	
	bool success = true;
	
	unsigned int n = 0;
	
	struct n *head = NULL;
	
	while (jac->len < MAX && *jac->buf != '\0') {
	
		if (*jac->buf == '/' || *jac->buf == '*' || *jac->buf == '+' || *jac->buf == '-') {
		
			if (head != NULL && head->op == '?')
				head->op = *jac->buf;		

			incrementBuff(jac,1);
		}
	
		if (1 == sscanf(jac->buf, "%Lf%n", &number, &n)) {
		
 			add_item(&head, number);
			incrementBuff(jac,n);
			
			if (*jac->buf != '+' && *jac->buf != '-' && *jac->buf != '/' && *jac->buf != '\0') /* A situation like 5( or 5ln*/
				head->op = '*';
				
			else {
				head->op = *jac->buf;
				incrementBuff(jac,1);
			}
		}
		
		if ((*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{')) {
		
			incrementBuff(jac,1);
		
			number = evaluatePar(jac);
			add_item(&head, number);
						
		}
		
		else if ((*jac->buf == ')' || *jac->buf == ']' || *jac->buf == '}')) {
		
			incrementBuff(jac,1);
			
			if (isdigit(*jac->buf))
				head->op = '*';
		
			if (jac->caller != 0) {
				jac->caller = 0;
				break;
			}
		}
		
		else if (strncmp(jac->buf, "pi", 2) == 0) {
		
			add_item(&head, M_PIl);
			incrementBuff(jac,2);
		
		}
		
		if (*jac->buf == '^') {
		
			incrementBuff(jac,1);
			number = evaluateFunc(jac, 'e');
			head->value = pow(head->value, number);
			
		}
		
		if (*jac->buf == '!') { /* Factorial */
			
			head->value = factorial(head->value);
			incrementBuff(jac,1);
				
		}	
		
		else if (strncmp(jac->buf, "cos", 3) == 0) {
		
			incrementBuff(jac,3);
			number = evaluateFunc(jac, 'c');
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "sin", 3) == 0) {
		
			incrementBuff(jac,3);
			number = evaluateFunc(jac, 's');
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "tan", 3) == 0) {
				
			incrementBuff(jac,3);
			number = evaluateFunc(jac, 't');
			add_item(&head, number);
				
		}
		 
		else if (strncmp(jac->buf, "atan", 4) == 0) {
				
			incrementBuff(jac,4);
			number = evaluateFunc(jac, 'a');
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "acos", 4) == 0) {
				
			incrementBuff(jac,4);
			number = evaluateFunc(jac, 'o');
			add_item(&head, number);
				
		}		   

		else if (strncmp(jac->buf, "log", 3) == 0) {
		
			incrementBuff(jac,3);
			number = evaluateFunc(jac, 'g');
			add_item(&head, number);
		
		}
		
		else if (strncmp(jac->buf, "sqrt", 4) == 0) {
		
			incrementBuff(jac,4);
			number = evaluateFunc(jac, 'r');
			add_item(&head, number);
		
		}
		 		 
		else if (strncmp(jac->buf, "ln", 2) == 0) {
			
			incrementBuff(jac,2);
			number = evaluateFunc(jac, 'l');
			add_item(&head, number);
			
		}
		
		else if (*jac->buf == 'e') { /* e^ increment by 2 */
		
			incrementBuff(jac,2);
			number = evaluateFunc(jac, 'x');
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "bin_dec", 7) == 0) {
		
			incrementBuff(jac,7);
			number = evaluateFunc(jac, 'b');
			add_item(&head, number);
		
		}
		
		else if (strncmp(jac->buf, "dec_bin", 7) == 0) {
		
			incrementBuff(jac,7);
			number = evaluateFunc(jac, 'd');
			add_item(&head, number);
		
		}
		
		else if (strncmp(jac->buf, "asin", 4) == 0) {
		
			incrementBuff(jac,4);
			number = evaluateFunc(jac, 'i');
			add_item(&head, number);
			
		}
		
				
		else if (strncmp(jac->buf, "cbrt", 4) == 0) {
		
			incrementBuff(jac,4);
			number = evaluateFunc(jac, 'u');
			add_item(&head, number);
			
		}
		
		else
			continue;
		
	} /* End of parsing */
		    
	if (jac->len == MAX) {
	
		fprintf(stderr,"%s\n","The limit size of the expression was reached");
		
		if (head != NULL)
			free(head);
			
		exit(0);
	}
	
	else if (head == NULL)
		return 0;
						
	else if (success == true && head != NULL) {
	
		if (head->next != NULL)
			calculate(head);
		
		result = head->value;
		
		free(head);
		
		return result;
	}

	else {
		fprintf(stderr,"%s\n","Syntax error");
		exit(0);
	}
	
	return false;
}

long double evaluatePar (struct control *jac) {

	long double number;
	
	jac->caller = 1;
		
	number = parse_evaluate_expr(jac);
	
	return number;
}

void add_item(struct n **ptr, long double data)
{
	struct n *new_item = malloc(sizeof *new_item);
     
	new_item->value = data;
    new_item->next = *ptr;
    new_item->op = '?';
    *ptr = new_item;
}

void calculate (struct n *head) {

	struct n *tmp = head;

	/* Multiply and divide first */

	while (tmp->next != NULL && tmp->next->op != '?') {

		if (tmp->next->op == '*' || tmp->next->op == '/') {
		
			if (tmp->next->op == '*')
				tmp->value = tmp->next->value * tmp->value;
			
			else
				tmp->value = tmp->next->value / tmp->value;
			
			delNextNode(tmp);
		}
		
		else
			tmp = tmp->next;
	}
	
	tmp = head;

	/* Now do addition and subtraction */

	while (tmp != NULL && tmp->next != NULL && tmp->next->op != '?') {

		if (tmp->next->op == '+' || tmp->next->op == '-') {
		
			if (tmp->next->op  == '+')
				tmp->value = tmp->next->value + tmp->value;
			
			else
				tmp->value = tmp->next->value - tmp->value;
			
			delNextNode(tmp);
		}
		
		else
			tmp = tmp->next;
	}
}

void delNextNode (struct n *head) {

	struct n *tmp = head->next;
	head->next = head->next->next;
	free(tmp);
}

long double evaluateFunc (struct control *jac, char op) {

	long double number;
	unsigned int n = 0;
		
    if (1 == sscanf(jac->buf, "%Lf%n", &number, &n)) {
    	incrementBuff(jac,n);
		return switchFunc(&op, &number);
	}
	
	else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{') {
			
		number = evaluatePar(jac);		
				
		return switchFunc(&op, &number);
	}
			
	else
		return false;
}

long double switchFunc(char *op, long double *number) {

	switch(*op) {
	
		case 'a':
			return atan(*number);
			break;
			
		case 'b':
			return bin_dec(*number);
			break;
			
		case 'c':
	    	return cos(*number);
	    	break;
	    	
	    case 'd':
	    	return dec_bin(*number);
	    	break;
	
		case 'e':
			return *number;
			break;
			
	    case 'g':
	    	return log10(*number);
	    	break;
	    	
	    case 'i':
	    	return asin(*number);
	    	break;
			
    	case 'l':
	    	return log(*number);
	    	break;
	    	
	   	case 'o':
	    	return acos(*number);
	    	break;
	    	
	    case 'r':
			return sqrt(*number);
			break;	    	
	    	
		case 's':
			return sin(*number);
			break;
			
		case 't':
			return tan(*number);
			break;
			
		case 'u':
			return cbrt(*number);
			break;
			
		case 'x':
			return exp(*number);
			break;
			 	
	    default:
			return false;
		   	break;
	}
  
}

void incrementBuff (struct control *jac, int n) {

	jac->buf += n;
	jac->len += n;

}
