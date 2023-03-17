#include "jac.h"
#include <math.h>

unsigned long factorial(unsigned long f)
{
	if (f == 0) 
		return 1;
        
	return(f * factorial(f - 1));
}

int quadr_eq()
{
	double  a, b, c, x1, x2, disc;
    
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

		if (disc == 0)
		{
			x1 = -b / (2 * a);
			printf("x1 = %lf, x2 = %lf", x1, x1);
        }

        else if (disc > 0)
		{
			x1 = (-b + sqrt(disc)) / (2 * a);
			x2 = (-b - sqrt(disc)) / (2 * a);
			printf("x1 = %.1lf, x2 = %.1lf", x1, x2);
        }

		else
		{
			double xr = - b / ( 2 * a );
			int num = -(b*b - 4*a*c);
			int den = 2*a;
			printf("x1 = %lf + i sqrt(%d)/%d\nx2 = %lf -i sqrt(%d)/%d\n", xr, num,den, xr, num,den);
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

void special_functions() /* Functions seldomly used */
{
	long long (*fp[2])(long long n);

	fp[0] = dec_bin;
	fp[1] = bin_dec;

	unsigned int ans;

	long long number;

	printf("Enter 1 for decimal to binary conversion\n");
	printf("Enter 2 for binary to decimal conversion\n");
	printf("Enter 3 to solve ax^2+bx+c=0\n");
	printf("Enter 0 to quit\n>>");

	scanf("%d", &ans);

	if(ans != 0 && ans < 4)
	{
		if(ans ==3)
			quadr_eq();

		else
		{
			printf(">");
			scanf("%lld", &number);
			printf("%lld\n", fp[ans-1](number));
		}
	}
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
