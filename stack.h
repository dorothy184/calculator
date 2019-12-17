#include <stdio.h>
#include <stdlib.h>

#ifndef stack_h
#define stack_h

typedef struct Stack {
	char data;
	struct Stack* next;
}STACK;

void push(STACK** head, char data);
char pop(STACK** head);

#endif

