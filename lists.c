/* Library to manipulate linked-lists */

#include "lists.h"

void add_item (struct node **ptr, long double data)
{
	struct node *new_item = malloc(sizeof *new_item);

	new_item->value = data;
	new_item->next = *ptr;
	new_item->op = '?';
	*ptr = new_item;
}

void delNextNode (struct node *head)
{
	struct node *tmp = head->next;
	head->op = head->next->op;
	head->next = head->next->next;
	free(tmp);
}

/* Function to reverse the linked list */
void reverse(struct node** head_ref)
{
    struct node* prev = NULL;
    struct node* current = *head_ref;
    struct node* next = NULL;

    while (current != NULL)
	{
        /* Store next */
        next = current->next;
 
        /* Reverse current node's pointer */
        current->next = prev;
 
        /* Move pointers one position ahead */
        prev = current;
        current = next;
    }

    *head_ref = prev;
}
