#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "doubleLinkedList.h"
#include "stack.h"

#ifndef calculator_h
#define calculator_h

enum DataType {
	OPERAND = 100, OPERATOR, SPACE, ENTER, DECIMAL_POINT, INVALID_DATA
};

int getCharType(char data);
int getOperatorPriority(char oper);
void input_ex(NODE** head, NODE** tail, char* file_name);
void trans_to_postfix(NODE** infix_head, NODE** postfix_head, NODE** postfix_tail);
int getNodeLength(NODE* head);
void multiply(NODE** fir_int_head, NODE** fir_dec_head, NODE** sec_int_head, NODE** sec_dec_head, STACK** stack);
void plus(NODE** fir_int_head, NODE** fir_dec_head, NODE** sec_int_head, NODE** sec_dec_head, STACK** stack);
int node_compare(NODE* fir_int_head, NODE* fir_int_tail, NODE* fir_dec_head, NODE* fir_dec_tail, NODE* sec_int_head, NODE* sec_int_tail, NODE* sec_dec_head, NODE* sec_dec_tail);
void minus(NODE** fir_int_head, NODE** fir_dec_head, NODE** sec_int_head, NODE** sec_dec_head, STACK** stack, int compare);
void cal(NODE** postfix_head, NODE** postfix_tail);

#endif
