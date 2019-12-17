#include <stdio.h>
#include "node.h"
#include "calculator.h"

int main(int argc, char* argv[])
{
	NODE* infix_head = NULL;
	NODE* infix_tail = NULL;
	NODE* postfix_head = NULL;
	NODE* postfix_tail = NULL;
	input_ex(&infix_head, &infix_tail, argv[1]);
	trans_to_postfix(&infix_head, &postfix_head, &postfix_tail);
	cal(&postfix_head, &postfix_tail);


	return 0;
}
