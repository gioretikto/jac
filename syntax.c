#include "jac.h"

struct charNode
{

   char data;

   struct charNode *next;

};

void push(struct charNode** top_ref, int new_data);

int pop(struct charNode** top_ref);

bool isMatchingPair(char character1, char character2)
{

   if (character1 == '(' && character2 == ')')
     return 1;

   else if (character1 == '{' && character2 == '}')
     return 1;

   else if (character1 == '[' && character2 == ']')
     return 1;

   else
     return 0;
}

bool areParenthesisBalanced(char *buf)
{

   int i = 0;

   struct charNode *stack = NULL;

   while (buf[i])
   {

      if (buf[i] == '{' || buf[i] == '(' || buf[i] == '[')
	  	push(&stack, buf[i]);

      if (buf[i] == '}' || buf[i] == ')' || buf[i] == ']')

      {

         if (stack == NULL)
           return 0;

         else if ( !isMatchingPair(pop(&stack), buf[i]))
           return 0;
      }

      i++;

   }

   if (stack == NULL)
     return 1;

   else
     return 0;
} 


void push(struct charNode** top_ref, int new_data)
{

	struct charNode* new_node = (struct charNode*)malloc(sizeof(struct charNode));

	if (new_node == NULL)  
	{
		printf("Stack overflow n");

		getchar();

		exit(0);

	}           

	new_node->data  = new_data;

	new_node->next = (*top_ref); 

	(*top_ref) = new_node;

}

int pop(struct charNode** top_ref)
{

  char res;

  struct charNode *top;

  if (*top_ref == NULL)
  {
     printf("Stack overflow n");

     getchar();

     exit(0);
  }

  else
  {
     top = *top_ref;

     res = top->data;

     *top_ref = top->next;

     free(top);

     return res;
  }
}

bool checkSyntax(char *str) {

	if (!(isdigit(*str)) && !(isalpha(*str)) && *str !='(' && *str !='['
	  && *str != '-' && *str != '+' && *str != '{' && *str !=')' && *str !=']' && *str !='}') {
	  
		fprintf(stderr,"%s%c\n","Syntax error: Expression starting with ",*str);
		return false;
		
	}
	
	while (*str != '\0') {
	
		if (*str == str[1] && !(isdigit(*str)) && *str !='(' && *str !='[' && *str != '{' && *str !=')' && *str !=']' && *str !='}') {
		
			fprintf(stderr,"%s%c %c\n","Syntax error: Invalid sequence ",*str, *str);
			return false;
			
		}
		
		if ((*str >= 58 && *str <=64) || (*str > 33 && *str <=39) || *str==',' ) {
		
			fprintf(stderr,"%s: %c\n","Syntax error: Invalid symbol",*str);
			return false;
			
		}
		
		str++;
			
	}
	
	return true;
}
