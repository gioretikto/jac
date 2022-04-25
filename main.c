/* jac v.1.6.0 - Giovanni Resta 23/09/2021 */

#include "jac.h"

#define ERROR '?'

long double parse_evaluate_expr(struct control *jac, bool inFunc);
void print_result(long double x);
void remove_spaces(char *str);
bool areParenthesisBalanced(char *exp);
bool checkSyntax(char *buf);

int main(int argc, char* argv[])
{
	char line[MAX];	

	int i;

	long double result;

	struct control jac;

	jac.par = 0;

	if (argc == 1)
		printf("Enter x to quit\n>>");

	if (argc > 1)
	{
		strcpy(line, argv[1]);

		for (i = 2; i < argc; i++)
			strcat(line, argv[i]);

		remove_spaces(line);

		if (!areParenthesisBalanced(line))
		{
			fprintf(stderr,"%s\n","Syntax error: Mismatched parenthesis");

			return -2;
		}

		else if (!checkSyntax(line))
		{
			printf("%s\n", jac.buf);
			return -2;
		}

		else
		{
			jac.buf = line;
			result = parse_evaluate_expr(&jac, false);

			if (jac.buf[0] != ERROR)
				print_result(result);

			else
				return -2;
		}

		return 0;
	}

	for (;;)
	{
		if (fgets(line, MAX, stdin) == NULL || line[0] == 'x')
			break;

		if (line[0] == '\n')
		{
			printf(">>");
			continue;
		}

		remove_spaces(line);

		if (!areParenthesisBalanced(line))
		{
			fprintf(stderr,"%s\n","Syntax error: Mismatched parenthesis");

			printf(">>");
		}

		else if (!checkSyntax(line))
			printf(">>");

		else
		{
			jac.buf = line;

			result = parse_evaluate_expr(&jac, false);

			if (jac.buf[0] != ERROR)
				print_result(result);

			else
				fprintf(stderr,"%s\n","Syntax error");

			printf(">>");
		}
	}

	return 0;
}
