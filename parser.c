#include "jac.h"

long double parse_evaluate_expr(struct control *jac) {

	long double number, result;
	
	bool success = true;
	
	unsigned int n = 0;
	
	struct node *head = NULL;
	
	while (jac->len < MAX && *jac->buf != '\0') {
	
		if (*jac->buf == '/' || *jac->buf == '*' || *jac->buf == '+' || *jac->buf == '-') {
		
			if (head != NULL && head->op == '?') 		/* A situation like (5+6)+5 or ln2+5 */
				head->op = *jac->buf;
			
			if (head == NULL && *jac->buf == '-')
			{
				add_item(&head, -1);
				head->op = '*';
			}

			incrementBuff(jac,1);
		}
	
		if (1 == sscanf(jac->buf, "%Lf%n", &number, &n)) {
		
			if (head != NULL && head->op == '?')
				if (jac->buf[-1] == ')' || jac->buf[-1] == ']' || jac->buf[-1] == '}')	/* A situation like (..)5*/
					head->op = '*';
		
 			add_item(&head, number);
			incrementBuff(jac,n);
			
			if (*jac->buf != '\0' && *jac->buf != '+' && *jac->buf != '-' && *jac->buf != '/') /* A situation like 5( or 5ln */
				head->op = '*';
		}
		
		if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{') {
		
			incrementBuff(jac,1);
			
			jac->caller = 1;	/* indicates recursive call */
	
			number = parse_evaluate_expr(jac);   /* Recursive call: evaluate expression inside parenthesis */
			
			add_item(&head, number);
						
		}
		
		else if (*jac->buf == ')' || *jac->buf == ']' || *jac->buf == '}') {
		
			incrementBuff(jac,1);

			if (jac->caller != 0)
			{
				jac->caller = 0;
				break;
			}
		}
		
		else if (strncmp(jac->buf, "m_p", 3) == 0) {		/* Proton Mass */
		
			add_item(&head, PROTON_MASS);
			incrementBuff(jac,3);
		
		}
		
		else if (strncmp(jac->buf, "m_e", 3) == 0) {		/* Electron mass */
		
			add_item(&head, ELECTRON_MASS);
			incrementBuff(jac,3);
		
		}
		
		else if (strncmp(jac->buf, "c_0", 3) == 0) { 		/* Speed of light in vacuum m/s (exact) */
		
			add_item(&head, SPEED_LIGHT);
			incrementBuff(jac,3);
		
		}
		
		else if (*jac->buf == 'q') { 						/* elementary charge*/
		
			add_item(&head, CHARGE);
			incrementBuff(jac,1);
		
		}
		
		else if (*jac->buf == 'h') { 						/* Plank's constant*/
		
			add_item(&head, PLANK);
			incrementBuff(jac,1);
		
		}
		
		else if (*jac->buf == '^') {
		
			jac->func = 'e';
		
			incrementBuff(jac,1);
			number = evaluateFunc(jac);
			head->value = pow(head->value, number);
			
		}
		
		else if (*jac->buf == '!') { /* Factorial */
			
			head->value = factorial(head->value);
			incrementBuff(jac,1);
				
		}	
		
		else if (strncmp(jac->buf, "cos", 3) == 0) {
		
			jac->func = 'c';
			incrementBuff(jac,3);
			number = evaluateFunc(jac);
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "sin", 3) == 0) {
		
			jac->func = 's';
			incrementBuff(jac,3);
			number = evaluateFunc(jac);
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "tan", 3) == 0) {
		
			jac->func = 't';
			incrementBuff(jac,3);
			number = evaluateFunc(jac);
			add_item(&head, number);
				
		}
		 
		else if (strncmp(jac->buf, "atan", 4) == 0) {
		
			jac->func = 'a';
			incrementBuff(jac,4);
			number = evaluateFunc(jac);
			add_item(&head, number);
				
		}
		
		else if (strncmp(jac->buf, "acos", 4) == 0) {
		
			jac->func = 'o';
			incrementBuff(jac,4);
			number = evaluateFunc(jac);
			add_item(&head, number);
				
		}		   

		else if (strncmp(jac->buf, "log", 3) == 0) {
		
			jac->func = 'g';
			incrementBuff(jac,3);
			number = evaluateFunc(jac);
			add_item(&head, number);
		
		}
		
		else if (strncmp(jac->buf, "sqrt", 4) == 0) {
		
			jac->func = 'r';
			incrementBuff(jac,4);
			number = evaluateFunc(jac);
			add_item(&head, number);
		
		}
		 		 
		else if (strncmp(jac->buf, "ln", 2) == 0) {
			jac->func = 'l';
			incrementBuff(jac,2);
			number = evaluateFunc(jac);
			add_item(&head, number);
			
		}
		
		else if (strncmp(jac->buf, "e^", 2) == 0) {
		
			jac->func = 'x';
			incrementBuff(jac,2);
			number = evaluateFunc(jac);
			add_item(&head, number);
				
		}	

		
		else if (strncmp(jac->buf, "e_0", 3) == 0) {  /* Permittivity of free space */
		
			add_item(&head, E_0);
			incrementBuff(jac,3);
				
		}
		
		else if (strncmp(jac->buf, "n_a", 3) == 0) {  /* Avogadros's number */
		
			add_item(&head, AVOGADRO);
			incrementBuff(jac,3);
				
		}	
		
		else if (strncmp(jac->buf, "bin_dec", 7) == 0) {
		
			jac->func = 'b';
			incrementBuff(jac,7);
			number = evaluateFunc(jac);
			add_item(&head, number);
		
		}
		
		else if (strncmp(jac->buf, "dec_bin", 7) == 0) {
		
			jac->func = 'd';
			incrementBuff(jac,7);
			number = evaluateFunc(jac);
			add_item(&head, number);
		
		}
		
		else if (strncmp(jac->buf, "asin", 4) == 0) {
		
			jac->func = 'i';
			incrementBuff(jac,4);
			number = evaluateFunc(jac);
			add_item(&head, number);
			
		}		
				
		else if (strncmp(jac->buf, "cbrt", 4) == 0) {
		
			jac->func = 'u';
			incrementBuff(jac,4);
			number = evaluateFunc(jac);
			add_item(&head, number);
			
		}
		
		else if (strncmp(jac->buf, "pi", 2) == 0) {
		
			add_item(&head, M_PIl);
			incrementBuff(jac,2);
		
		}
		
		else if (*jac->buf == 'e') {
		
			add_item(&head, M_E);
			incrementBuff(jac,1);
		
		}
		
		else if (*jac->buf == 'k') { 	/* Boltzmann's constant */
		
			add_item(&head, K_B);
			incrementBuff(jac,1);
		
		}
		
		else if (isalpha(*jac->buf)) {	/* Syntax error */
		
			fprintf(stderr,"Illegal character %s\n", jac->buf);
			return 0;
		
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

void add_item(struct node **ptr, long double data)
{
	struct node *new_item = malloc(sizeof *new_item);
     
	new_item->value = data;
    new_item->next = *ptr;
    new_item->op = '?';
    *ptr = new_item;
}

void calculate (struct node *head) {

	struct node *tmp = head;

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

void delNextNode (struct node *head) {

	struct node *tmp = head->next;
	head->next = head->next->next;
	free(tmp);
}

long double evaluateFunc (struct control *jac) {

	long double number;
	unsigned int n = 0;
		
    if (1 == sscanf(jac->buf, "%Lf%n", &number, &n)) {
    	incrementBuff(jac,n);
		return switchFunc(&jac->func, &number);
	}
	
	else if (*jac->buf == '(' || *jac->buf == '[' || *jac->buf == '{') {
			
		number = parse_evaluate_expr(jac);   /* Evaluate expression inside parenthesis */		
				
		return switchFunc(&jac->func, &number);
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
