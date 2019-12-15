#include "doubleLinkedList.h"

NODE* newnode(char data) {
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->data = data;
	temp->prev = NULL;
	temp->next = NULL;
	return temp;
}

void append(NODE** head, NODE** tail, NODE* newnode)
{
	if (*head == NULL) {
		*head = newnode;
		*tail = newnode;
	}
	else {
		NODE* tmp = *tail;
		(*tail)->next = newnode;
		*tail = newnode;
		(*tail)->prev = tmp;
	}

	return;
}

char dequeue(NODE** head) {
	NODE* tmp = *head;
	char data = tmp->data;
	(*head) = (*head)->next;
	if (*head != NULL) {
		(*head)->prev = NULL;
	}
	free(tmp);
	return data;

}

char reverse_dequeue(NODE** tail, NODE** head) {
	NODE* tmp = *tail;
	char data = tmp->data;
	(*tail) = (*tail)->prev;
	if (*tail != NULL) {
		(*tail)->next = NULL;
	}
	if (*tail == NULL) {
		*head = NULL;
	}
	free(tmp);
	return data;
}

