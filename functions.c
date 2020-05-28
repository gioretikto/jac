#include "jac.h"

unsigned long factorial(unsigned long f) {

    if ( f == 0 ) 
        return 1;
    return(f * factorial(f - 1));
}

int bin_dec(long long n) {

    int dec = 0, i = 0, rem;
    
    while (n != 0) {
        rem = n % 10;
        n /= 10;
        dec += rem * pow(2, i);
        ++i;
    }
    return dec;
}

long long dec_bin(int n) {

    long long bin = 0;
    int rem, i;
    
    rem = i = 1;
    
    while (n != 0) {
        rem = n % 2;
        n /= 2;
        bin += rem * i;
        i *= 10;
    }
    
    return bin;
}

void print_result(long double x) {

 	long double tmp = ceil(x);

    if (fabs(x-tmp) < 0.00000000001) {
        x = tmp;
    }    

	printf("%.19Lg\n", x);

}

void remove_spaces(char *str) {

    int count = 0;
  	int i;
    for (i = 0; str[i]; i++)
        if (str[i] != ' ' && str[i] != '\n')
            str[count++] = str[i];
                                    
    str[count] = '\0';
}
