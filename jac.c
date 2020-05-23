#include "jac.h"

int main(int argc, char* argv[]) {

	char line[MAX];
	char *buf = line;

	int i;
	
	if (argc == 1)
		printf("Enter q to quit\n>>");
		
	if (argc > 1) {
	
		strcpy(line, argv[1]);

		for ( i = 2; i < argc; i++)
		{
		  strcat(line, argv[i]);
		}
		
		parse_expression(line);
		
		return 0;
	}
	
	for(;;) {
	
		if (fgets(buf = line, MAX, stdin) == NULL || *buf == 'q')
			break;
        	
		remove_spaces(buf);
		
		parse_expression(buf);
		
		printf(">>");
		
	}

	return 0;
}

void add_item(struct n **ptr, long double data, char s)
{
	struct n *item = malloc(sizeof *item);

	item->value = data;
	item->next = *ptr;
	item->op = '?';
	item->unary = s;
	*ptr = item;
}

void print_num(long double x) {

    long double i, r = modfl(x, &i);
    
    if (fabs(r) <.00001)
        printf("%.Lf ", i);
    
    else printf("%.19Lf ", x);
}

void remove_spaces(char *str) {

    int count = 0;
  	int i;
    for ( i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
                                    
    str[count] = '\0';
}

void calculate (struct n *head, struct n *end, bool *operation) {

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
