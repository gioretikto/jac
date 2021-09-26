/* jac v.1.4.0 - Giovanni Resta 23/09/2021 */

#include "jac.h"

long double parse_evaluate_expr(struct control *jac, bool inFunc);
void print_result(long double x);
void remove_spaces(char *str);
bool areParenthesisBalanced(char *exp);
bool checkSyntax(char *str);

int main(int argc, char* argv[])
{
	char line[MAX];	

	char *buf = line;

	int i;

	long double result;

	struct control jac;

	jac.len = 0;
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

			if (jac.buf[0] == '\n')
				return 0;

			result = parse_evaluate_expr(&jac, false);

			if (jac.failure == false)
				print_result(result);
		}

		return 0;
	}

	for (;;)
	{
		if (fgets(jac.buf = line, MAX, stdin) == NULL || *buf == 'x')
			break;

		if (jac.buf[0] == '\n'){
			printf(">>");
			continue;
		}

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

			result = parse_evaluate_expr(&jac, false);

			if (jac.failure == false)
				print_result(result);
			else
				fprintf(stderr,"%s\n","Syntax error");

			jac.failure = false;
			printf(">>");
		}

	}

	return 0;
}
