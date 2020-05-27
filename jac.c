#include "jac.h"

int main(int argc, char* argv[]) {

	char line[MAX];	
	
	char *buf = line;

	int i;
	
	struct control jac;
	
	jac.len = 0;
	jac.caller = 0;
	
	if (argc == 1)
		printf("Enter q to quit\n>>");
		
	if (argc > 1) {
	
		strcpy(line, argv[1]);

		for (i = 2; i < argc; i++)
			strcat(line, argv[i]);
		
		if (!areParenthesisBalanced(line)) {
		
			fprintf(stderr,"%s\n","Syntax error: Mismatched parenthesis");
			
			return 0;
		}
		
		else if (!checkSyntax(line)) {
			printf("%s\n", line);
			return 0;
		}
		
		else {
		
			jac.buf = line;
							
			remove_spaces(jac.buf);
			
			print_result(parse_evaluate_expr(&jac));
		}
		
		return 0;
	}
	
	for (;;) {
	
		if (fgets(jac.buf = line, MAX, stdin) == NULL || *buf == 'q')
			break;
			
		if (!areParenthesisBalanced(line)) {
		
			fprintf(stderr,"%s\n","Syntax error: Mismatched parenthesis");
			
			printf(">>");
		}
		
		else if (!checkSyntax(line))
			printf(">>");
		
		else {
        	
			remove_spaces(jac.buf);
			
			print_result(parse_evaluate_expr(&jac));
			
			putchar('\n');
					
			printf(">>");
		}
	}

	return 0;
}
