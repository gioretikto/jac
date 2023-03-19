#include "jac.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long long binomial(unsigned long long n, 
				   unsigned long long k)
{
	if(k > n)
		return 0;

	/* symmetry */
	if(k > n - k)
		k = n - k;

	if(k > n / 2)
		k = n - k;

	unsigned long long c = 1;
	unsigned long long result = 1;

	for(;c <= k; c++) {
		result *= n--;
		result /= c;
	}

	return result;
}

unsigned long factorial(unsigned long f)
{
	if (f == 0) 
		return 1;
        
	return(f * factorial(f - 1));
}

int quadr_eq()	/* Solve a quadratic equation */
{
	double  a, b, c, x1, disc;
    
	printf("Please enter the value of a:\n");
	printf(">");
	scanf("%lf", &a);

	printf("Please enter the value of b:\n");
	printf(">");
	scanf("%lf", &b);

	printf("Please enter the value of c:\n");
	printf(">");
	scanf("%lf", &c);

	if (a == 0)
	{
		x1 = -(c/b);
		printf("\nThis is not a quadratic equation.\n");
		printf("x = %.3lf\n", x1);
	}

	else
	{
		disc = (b*b) - 4*a*c;
		double den = 2*a;

		if (disc == 0)
		{
			x1 = -b / den;
			printf("x1 = %lf\nx2 = %lf\n", x1, x1);
        }

        else if (disc > 0)
		{
			double sq = sqrt(disc);
			x1 = (-b + sq) / den;
			double x2 = (-b - sq) / den;
			printf("x1 = %.1lf\nx2 = %.1lf\n", x1, x2);
		}

		else
		{
			double xr = - b / den;
			double num = -(b*b - 4*a*c);
			printf("x1 = %lf + i sqrt(%lf)/%lf\nx2 = %lf -i sqrt(%lf)/%lf\n", xr, num,den, xr, num,den);
		}
	}

	return 0;
}

long long bin_dec(long long n)
{
	int dec = 0, i = 0, rem;

	while (n != 0)
	{
		rem = n % 10;
		n /= 10;
		dec += rem * pow(2, i);
		++i;
	}

	return dec;
}

long long dec_bin(long long n)
{
	long long bin = 0;
	int rem, i;

	rem = i = 1;

	while (n != 0)
	{
		rem = n % 2;
		n /= 2;
		bin += rem * i;
		i *= 10;
	}

	return bin;
}

int input_number(unsigned long long *num)
{
	*num = 0;
	char c;
	while(1) {
	c = getchar();
	if (!isspace(c))
		break;
	}

	while(1) {
		if (c == '\n')
			return 0;

		if (!isdigit(c))
			return -2;

		*num *= 10;
		*num += c - '0';
		c = getchar();
	}
}

int special_functions() /* Functions seldomly used */
{
	long long (*fp[2])(long long n);

	fp[0] = dec_bin;
	fp[1] = bin_dec;

	unsigned int ans;

	long long number;

	printf("Enter 1 for decimal to binary conversion\n");
	printf("Enter 2 for binary to decimal conversion\n");
	printf("Enter 3 to solve ax^2+bx+c=0\n");
	printf("Enter 4 to calculate the binomial coefficient C_{n,k}\n");
	printf("Enter 0 to quit\n>>");

	scanf("%d", &ans);

	if(ans != 0 && ans < 5)
	{
		if (ans == 3)
			quadr_eq();

		if (ans == 4)
		{
			unsigned long long n,k;

			printf("input n:\n>");

			int ret = input_number(&n);

			if (ret < 0) {
                printf("Error reading n\n");
                return 1;
			}

			printf("input k:\n>");
			ret = input_number(&k);

			if (ret < 0) {
				printf("Error reading k\n");
				return 1;
			}

			printf("%lld\n>>", binomial(n,k));
		}

		else
		{
			printf(">");
			scanf("%lld", &number);
			printf("%lld\n", fp[ans-1](number));
		}
	}

	return 0;
}

void print_result(long double x)
{
	long double tmp = ceill(x);

	if (fabsl(x-tmp) < 0.00000000001)
	{
		x = tmp;
	}	
	
	if (x == -0)
		x = fabsl(x);
	
	printf("%.19Lg\n", x);
}

void remove_spaces(char *str)
{
	unsigned int count = 0;
	unsigned int i;

	for (i = 0; str[i]; i++)
		if (str[i] != ' ' && str[i] != '\n')
			str[count++] = str[i];
                                    
	str[count] = '\0';
}
