#include "stack.h"

void push(STACK** head, char data) {
	STACK* tmp = (STACK*)malloc(sizeof(STACK));
	tmp->next = *head;
	tmp->data = data;
	*head = tmp;
}

char pop(STACK** head) {
	STACK* tmp = *head;
	char data = (*head)->data;
	*head = (*head)->next;
	free(tmp);
	return data;
}


