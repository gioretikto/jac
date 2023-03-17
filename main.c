/* jac v.1.6.2 - Giovanni Resta 23/09/2021 */

#include "jac.h"

#define ERROR '?'

long double parse_evaluate_expr(struct control *jac, bool inFunc);
void print_result(long double x);
void remove_spaces(char *str);
bool areParenthesisBalanced(char *exp);
bool checkSyntax(char *buf);

void special_functions();

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

		if (line[0] == 'b')
		{
			special_functions();
			continue;
		}

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

long long dec_bin(long long n);
long long bin_dec(long long n);

void special_functions()
{
	long long (*fp[2])(long long n);

	fp[0] = dec_bin;
	fp[1] = bin_dec;

	unsigned int ans;

	long long number;

	printf("Enter 1 for decimal to binary conversion\n");
	printf("Enter 2 for binary to decimal conversion\n");
	printf("Enter 0 to quit\n>>");

	scanf("%d", &ans);

	if(ans != 0 && ans < 4)
	{
		printf(">");
		scanf("%lld", &number);
		printf("%lld\n", fp[ans-1](number));
	}
}
