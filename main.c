/* jac v.1.3 - Giovanni Resta 29/05/2020 */

#include "jac.h"

int main(int argc, char* argv[])
{
	char line[MAX];	

	char *buf = line;

	int i;

	long double result;

	struct control jac;

	jac.len = 0;
	jac.caller = false;
	jac.failure = false;

	if (argc == 1)
		printf("Enter x to quit\n>>");

	if (argc > 1)
	{	
		strcpy(line, argv[1]);

		for (i = 2; i < argc; i++)
			strcat(line, argv[i]);

		if (!areParenthesisBalanced(line))
		{
			fprintf(stderr,"%s\n","Syntax error: Mismatched parenthesis");

			return 0;
		}

		else if (!checkSyntax(line))
		{
			printf("%s\n", line);
			return 0;
		}

		else
		{		
			jac.buf = line;

			remove_spaces(jac.buf);

			result = parse_evaluate_expr(&jac);

			if (jac.failure == false)
				print_result(result);		
		}

		return 0;
	}

	for (;;)
	{	
		if (fgets(jac.buf = line, MAX, stdin) == NULL || *buf == 'x')
			break;

		if (!areParenthesisBalanced(line))
		{
			fprintf(stderr,"%s\n","Syntax error: Mismatched parenthesis");

			printf(">>");
		}

		else if (!checkSyntax(line))
			printf(">>");

		else
		{
			remove_spaces(jac.buf);

			result = parse_evaluate_expr(&jac);
			
			if (jac.failure == false)
				print_result(result);
			
			jac.failure = false;
			printf(">>");
		}
	}

	return 0;
}
