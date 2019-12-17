#include <stdio.h>
#include <stdlib.h>
#include "node.h"

#ifndef doubleLinkedList_h
#define doubleLinkedList_h

NODE* newnode(char data);
void append(NODE** head, NODE** tail, NODE* newnode);
char dequeue(NODE** head);
char reverse_dequeue(NODE** tail, NODE** head);

#endif

