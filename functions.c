#include "jac.h"

void multiply (struct n *head, struct n *end)
{
	double result;

    if (head->next == end || head->next == NULL)
        return;
   
    else {
		
		if ((head->next)->op == '*') {
			result = (head->next)->value * head->value;
			head->value = result;
			del_next(head);
			multiply(head, end);
		}
		
		else
			multiply(head->next, end);
	}
}

void divide (struct n *head, struct n *end) {

	double result;

    if (head->next == end || head->next == NULL)
        return;
   
    else {
		
		if ((head->next)->op == '/') {
			result = (head->next)->value / head->value;
			head->value = result;
			del_next(head);
			divide(head, end);
		}
		
		else
			divide(head->next, end);
	}
}

void add (struct n *head, struct n *end) {

	double result;

    if (head->next == end || head->next == NULL)
        return;
   
    else {
		
		if ((head->next)->op == '+' || (head->next)->op == '-') {
		
			if ((head->next)->op == '+')
       	    	result = (head->next)->value +1 * head->value;
       	    	
            else {
       			result = (head->next)->value -1 * head->value;
       		}

			head->value = result;
			del_next(head);
			add(head, end);
		}
		
		else
			add(head->next, end);
	}
}

void del_next (struct n *before_del_next) {

    struct n *temp;
    temp = before_del_next->next;
    before_del_next->next = temp->next;
    free(temp);
    
}

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