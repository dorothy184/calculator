#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "doubleLinkedList.h"
#include "stack.h"
#include "calculator.h"

int getCharType(char data) {
	if (data >= '0' && data <= '9') { return OPERAND; }
	else if (data == '+' || data == '-' || data == '*' || data == '/' || data == '(' || data == ')') { return OPERATOR; }
	else if (data == ' ') { return SPACE; }
	else if (data == '\n') { return ENTER; }
	else if (data == '.') { return DECIMAL_POINT; }
	else { return INVALID_DATA; }
}

int getOperatorPriority(char oper) {
	if (oper == '*' || oper == '/')
		return 3;
	else if (oper == '+' || oper == '-')
		return 2;
	else if (oper == '(')
		return 1;
	else
		return 0;
}


void input_ex(NODE** head, NODE** tail, char* file_name) {
	char data, prev_data;
	FILE* file;
	file = fopen(file_name, "r");
	STACK* open_bracket = NULL;          //'('을 저장하는 스택
	int data_type, prev_data_type;      //현재 데이터 타입, 이전 데이터 타입
	int dotcnt, openBra_cnt, closeBra_cnt = 0;
	int continuezero, zerocheck = 0;
	while (1) {
		data = fgetc(file);
		data_type = getCharType(data);
		if (*tail != NULL) {
			prev_data_type = getCharType((*tail)->data);
			prev_data = (*tail)->data;
		}
		if (data == EOF) {
			if (prev_data_type == OPERATOR && prev_data != ')') {
				printf("식을 잘못 입력하셨습니다(식의 끝에 연산자 입력)\n");
				exit(1);
			}
			break;
		}
		else if (data_type == INVALID_DATA) {
			printf("식을 잘못 입력하셨습니다(%c를 입력함).\n", data);
			exit(1);
		}
		else if ((*head) == NULL && data_type == OPERATOR && data != '(') {
			if (data == ')') {
				printf("식을 잘못 입력하셨습니다(bracket error)\n");
				exit(1);
			}
			else if (data == '+') {
				continue;
			}
			else if (data == '-') {
				append(head, tail, newnode('0'));
				append(head, tail, newnode(data));
			}
			else if (data == '*') {
				printf("식을 잘못 입력하셨습니다(first * error)\n");
				exit(1);
			}
		}
		else if (data_type == SPACE || data_type == ENTER) { continue; }
		else if (data_type == OPERATOR || data_type == OPERAND || data_type == DECIMAL_POINT) {
			if (data_type == OPERAND) {
				if (prev_data == '(') {
					append(head, tail, newnode('0'));
					append(head, tail, newnode('+'));
				}
				if (prev_data == ')') {
					append(head, tail, newnode('*'));
				}
				if ((data != '0' && data_type == OPERAND) || prev_data_type == DECIMAL_POINT) {
					continuezero = 1;
				}
				if (continuezero == 0) {
					if (zerocheck == 0) {
						append(head, tail, newnode('0'));
						zerocheck = 1;
						continue;
					}
					else if (zerocheck == 1) {
						continue;
					}
				}
			}
			if (data_type == DECIMAL_POINT && (prev_data_type == OPERATOR || (*head) == NULL))   //소수점 앞에 숫자가 없을 경우
			{
				dotcnt++;
				append(head, tail, newnode('0'));
				append(head, tail, newnode(data));
			}
			else {
				if (data == '(') {
					push(&open_bracket, '(');
				}
				else if (data == ')' && open_bracket != NULL) {
					if (prev_data == '+' || prev_data == '-' || prev_data == '*' || prev_data == '(') {
						printf("식을 잘못 입력하셨습니다.\n");
						exit(1);
					}
					pop(&open_bracket);
				}
				else if (data == ')' && open_bracket == NULL) {
					printf("식을 잘못 입력하셨습니다(bracket error)\n");
					exit(1);
				}
				else if (data == '+' || data == '-') {
					if (prev_data == '(') {
						append(head, tail, newnode('0'));
					}
				}
				else if (data == '*') {
					if (prev_data == '(' || prev_data == '+' || prev_data == '-' || prev_data == '*') {
						printf("식을 잘못 입력하셨습니다.\n");
						exit(1);
					}
				}
				if (data_type == OPERATOR) {
					dotcnt = 0;
				}
				else if (data_type == OPERAND || data_type == DECIMAL_POINT) {
					if (data == '.') {
						dotcnt++;
					}
					if (dotcnt > 1) {
						printf("식을 잘못 입력하셨습니다.\n");
						exit(1);
					}
				}
				append(head, tail, newnode(data));
			}
		}
	}
	if (openBra_cnt != closeBra_cnt) {
		printf("식을 잘못 입력하셨습니다(bracket error)\n");
		exit(1);
	}
	fclose(file);
}

void trans_to_postfix(NODE** infix_head, NODE** postfix_head, NODE** postfix_tail) {
	STACK* operator = NULL;

	while (*infix_head != NULL) {
		char infix_data = dequeue(infix_head);
		int infix_type = getCharType(infix_data);

		if (infix_type == OPERAND || infix_type == DECIMAL_POINT) {
			append(postfix_head, postfix_tail, newnode(infix_data));
		}
		else if (infix_type == OPERATOR) {
			int infix_prio = getOperatorPriority(infix_data);
			int operator_prio = 0;
			if (operator != NULL) {
				operator_prio = getOperatorPriority(operator->data);
			}

			if (operator == NULL) {
				append(postfix_head, postfix_tail, newnode(' '));
				push(&operator, infix_data);
			}
			else if (infix_data == '(') {
				push(&operator, infix_data);
			}
			else if (operator->data == '(') {
				append(postfix_head, postfix_tail, newnode(' '));
				push(&operator, infix_data);
			}
			else if (infix_data == ')') {
				while (operator != NULL && operator->data != '(') {
					append(postfix_head, postfix_tail, newnode(' '));
					append(postfix_head, postfix_tail, newnode(pop(&operator)));
				}
				if (operator != NULL && operator->data == '(') {
					pop(&operator);
				}
			}
			else if (operator != NULL && infix_prio > operator_prio) {
				append(postfix_head, postfix_tail, newnode(' '));
				push(&operator, infix_data);
			}

			else if (operator != NULL && infix_prio <= operator_prio) {
				while (operator != NULL && (operator->data != '(' && infix_prio <= operator_prio)) {
					append(postfix_head, postfix_tail, newnode(' '));
					append(postfix_head, postfix_tail, newnode(pop(&operator)));
				}
				append(postfix_head, postfix_tail, newnode(' '));
				push(&operator, infix_data);

			}

		}
	}
	while (operator != NULL) {
		append(postfix_head, postfix_tail, newnode(' '));
		append(postfix_head, postfix_tail, newnode(pop(&operator)));
	}
}



int getNodeLength(NODE* head) {
	NODE* tmp = head;
	int cnt = 0;
	while (tmp != NULL) {
		cnt++;
		tmp = tmp->next;
	}

	return cnt;
}

void multiply(NODE** fir_int_head, NODE** fir_dec_head, NODE** sec_int_head, NODE** sec_dec_head, STACK** stack) {
	NODE* result_head = NULL, * result_tail = NULL;
	NODE* fir_head = NULL, * fir_tail = NULL, * sec_head = NULL, * sec_tail = NULL;
	NODE* sum_tmp_head = NULL, * sum_tmp_tail = NULL, * mul_tmp_head = NULL, * mul_tmp_tail = NULL, * tmp_head = NULL, * tmp_tail = NULL;
	char sec_argument;      //sec의 한자리 수
	NODE* fir_arg_pos = NULL;      //fir의 한자리 수 위치
	int fir_dec_len = 0, sec_dec_len = 0, result_dec_len;   //fir과 sec의 소수부 길이 그리고 result의 소수부 길이
	int num_tmp = 0, mul_roundup = 0, plus_roundup = 0, cnt = 0;

	fir_dec_len = getNodeLength(*fir_dec_head);
	sec_dec_len = getNodeLength(*sec_dec_head);
	result_dec_len = fir_dec_len + sec_dec_len;

	//fir에다가 소수부를 채움
	while (*fir_dec_head != NULL) {
		append(&fir_head, &fir_tail, newnode(dequeue(fir_dec_head)));
	}
	//fir에다가 정수부를 채움
	while (*fir_int_head != NULL) {
		append(&fir_head, &fir_tail, newnode(dequeue(fir_int_head)));
	}
	//sec에다가 소수부를 채움
	while (*sec_dec_head != NULL) {
		append(&sec_head, &sec_tail, newnode(dequeue(sec_dec_head)));
	}
	//fir에다가 정수부를 채움
	while (*sec_int_head != NULL) {
		append(&sec_head, &sec_tail, newnode(dequeue(sec_int_head)));
	}

	while (sec_head != NULL) {
		fir_arg_pos = fir_head;
		sec_argument = dequeue(&sec_head);
		mul_roundup = 0;
		plus_roundup = 0;


		//0을 앞에다가 추가해주어서 자리수를 맞춘다.
		for (int i = 0; i < cnt; i++) {
			append(&mul_tmp_head, &mul_tmp_tail, newnode('0'));
		}
		cnt++;

		for (int i = 0; i < getNodeLength(fir_head); i++) {
			num_tmp = (fir_arg_pos->data - '0') * (sec_argument - '0') + mul_roundup;

			if (num_tmp > 9) {
				mul_roundup = num_tmp / 10;
				num_tmp %= 10;
			}
			else {
				mul_roundup = 0;
			}

			append(&mul_tmp_head, &mul_tmp_tail, newnode(num_tmp + '0'));
			fir_arg_pos = fir_arg_pos->next;
		}


		if (mul_roundup != 0) {
			append(&mul_tmp_head, &mul_tmp_tail, newnode(mul_roundup + '0'));
		}

		if (sum_tmp_head == NULL) {
			sum_tmp_head = mul_tmp_head;
			sum_tmp_tail = mul_tmp_tail;
			mul_tmp_head = NULL;
			mul_tmp_tail = NULL;

		}
		else {
			tmp_head = sum_tmp_head;
			tmp_tail = sum_tmp_tail;
			sum_tmp_head = NULL;
			sum_tmp_tail = NULL;

			//tmp(=sum_tmp) + mul_tmp
			while (tmp_head != NULL) {
				num_tmp = (dequeue(&tmp_head) - '0') + (dequeue(&mul_tmp_head) - '0') + plus_roundup;
				if (num_tmp > 9) {
					plus_roundup = 1;
					num_tmp -= 10;
				}
				else {
					plus_roundup = 0;
				}
				append(&sum_tmp_head, &sum_tmp_tail, newnode(num_tmp + '0'));
			}
			while (mul_tmp_head != NULL) {
				num_tmp = (dequeue(&mul_tmp_head) - '0') + plus_roundup;
				if (num_tmp > 9) {
					plus_roundup = 1;
					num_tmp -= 10;
				}
				else {
					plus_roundup = 0;
				}
				append(&sum_tmp_head, &sum_tmp_tail, newnode(num_tmp + '0'));
			}
			if (plus_roundup != 0) {
				append(&sum_tmp_head, &sum_tmp_tail, newnode(plus_roundup + '0'));
			}
		}

		//소수점을 삽입하는 과정
		int a = 1;
		while (sum_tmp_head != NULL) {
			append(&result_head, &result_tail, newnode(dequeue(&sum_tmp_head)));
			if (result_dec_len == a) {
				append(&result_head, &result_tail, newnode('.'));
			}
			a++;
		}

		//소수부 필요없는 0 제거
		while (result_head != NULL && result_head->data == '0') {
			dequeue(&result_head);
			if (result_head != NULL && result_head->data == '.') {
				dequeue(&result_head);
				break;
			}
		}

		//정수부 필요없는 0 제거
		while (result_tail != NULL && result_tail->data == '0') {
			if (result_tail->prev != NULL && (result_tail->prev->data == '.')) {
				break;
			}
			reverse_dequeue(&result_tail, &result_head);
		}

		while (result_tail != NULL) {
			push(stack, reverse_dequeue(&result_tail, &result_head));

		}

		/*
		 while((*stack)->data == '0'){
		 pop(stack);
		 }
		 */
	}

}

void plus(NODE** fir_int_head, NODE** fir_dec_head, NODE** sec_int_head, NODE** sec_dec_head, STACK** stack) {

	NODE* rev_int_head = NULL, * rev_int_tail = NULL, * rev_dec_head = NULL, * rev_dec_tail = NULL;      //reverse int, dec => plus의 연산 결과
	int fir_dec_len, sec_dec_len, fir_int_len, sec_int_len, int_long_len = 0, int_short_len = 0, dec_long_len = 0, dec_short_len = 0;
	int num_tmp = 0, roundup = 0;

	fir_dec_len = getNodeLength(*fir_dec_head);
	sec_dec_len = getNodeLength(*sec_dec_head);
	dec_long_len = fir_dec_len >= sec_dec_len ? fir_dec_len : sec_dec_len;
	dec_short_len = fir_dec_len <= sec_dec_len ? fir_dec_len : sec_dec_len;



	if (fir_dec_len > sec_dec_len) {
		for (int i = 0; i < fir_dec_len - sec_dec_len; i++) {
			append(&rev_dec_head, &rev_dec_tail, newnode(dequeue(fir_dec_head)));
		}
	}
	else if (fir_dec_len < sec_dec_len) {
		for (int i = 0; i < sec_dec_len - fir_dec_len; i++) {
			append(&rev_dec_head, &rev_dec_tail, newnode(dequeue(sec_dec_head)));
		}
	}
	for (int i = 0; i < dec_short_len; i++) {
		num_tmp = (dequeue(fir_dec_head) - '0') + (dequeue(sec_dec_head) - '0') + roundup;
		if (num_tmp > 9) {
			roundup = 1;
			num_tmp -= 10;
		}
		else {
			roundup = 0;
		}
		append(&rev_dec_head, &rev_dec_tail, newnode(num_tmp + '0'));
	}

	fir_int_len = getNodeLength(*fir_int_head);
	sec_int_len = getNodeLength(*sec_int_head);
	int_short_len = fir_int_len <= sec_int_len ? fir_int_len : sec_int_len;
	int_long_len = fir_int_len >= sec_int_len ? fir_int_len : sec_int_len;

	for (int i = 0; i < int_short_len; i++) {
		num_tmp = (dequeue(fir_int_head) - '0') + (dequeue(sec_int_head) - '0') + roundup;
		if (num_tmp > 9) {
			roundup = 1;
			num_tmp -= 10;
		}
		else {
			roundup = 0;
		}
		append(&rev_int_head, &rev_int_tail, newnode(num_tmp + '0'));
	}

	if (fir_int_len >= sec_int_len) {
		for (int i = 0; i < fir_int_len - sec_int_len; i++) {
			num_tmp = (dequeue(fir_int_head) - '0') + roundup;
			if (num_tmp > 9) {
				roundup = 1;
				num_tmp -= 10;
			}
			else {
				roundup = 0;
			}
			append(&rev_int_head, &rev_int_tail, newnode(num_tmp + '0'));
		}
		if (roundup != 0) {
			append(&rev_int_head, &rev_int_tail, newnode(roundup + '0'));
			roundup = 0;
		}
	}
	else if (fir_int_len < sec_int_len) {
		for (int i = 0; i < sec_int_len - fir_int_len; i++) {
			num_tmp = (dequeue(sec_int_head) - '0') + roundup;
			if (num_tmp > 9) {
				roundup = 1;
				num_tmp -= 10;
			}
			else {
				roundup = 0;
			}
			append(&rev_int_head, &rev_int_tail, newnode(num_tmp + '0'));
		}
		if (roundup != 0) {
			append(&rev_int_head, &rev_int_tail, newnode(roundup + '0'));

		}
	}

	//정수부 앞에 필요 없는 '0' 제거
	while (rev_int_tail->prev != NULL && rev_int_tail->data == '0') {
		reverse_dequeue(&rev_int_tail, &rev_int_head);
	}
	//소수부 뒤에 필요 없는 '0' 제거
	while (rev_dec_head != NULL && rev_dec_head->data == '0') {
		dequeue(&rev_dec_head);
	}

	if (rev_dec_head == NULL || rev_dec_tail == NULL) {
		rev_dec_tail = NULL;
		rev_dec_head = NULL;
	}
	if (rev_int_head == NULL || rev_int_tail == NULL) {
		rev_int_tail = NULL;
		rev_int_head = NULL;
	}

	while (rev_int_tail != NULL) {
		char temp = reverse_dequeue(&rev_int_tail, &rev_int_head);
		push(stack, temp);
	}

	if (rev_dec_head != NULL) {
		push(stack, '.');
	}
	else {
		return;
	}
	while (rev_dec_tail != NULL) {
		char temp = reverse_dequeue(&rev_dec_tail, &rev_dec_head);
		push(stack, temp);
	}
}


//fir이 크면 1, sec이 크면 -1, 같으면 0을 출력하는 함수
//fir, dec은 역순이라는 것을 주의한다.
int node_compare(NODE* fir_int_head, NODE* fir_int_tail, NODE* fir_dec_head, NODE* fir_dec_tail, NODE* sec_int_head, NODE* sec_int_tail, NODE* sec_dec_head, NODE* sec_dec_tail) {
	if (getNodeLength(fir_int_head) > getNodeLength(sec_int_head)) {
		return 1;
	}
	else if (getNodeLength(fir_int_head) < getNodeLength(sec_int_head)) {
		return -1;
	}
	else if (getNodeLength(fir_int_head) == getNodeLength(sec_int_head)) {
		while (fir_int_tail != NULL && sec_int_tail != NULL) {
			if (fir_int_tail->data > sec_int_tail->data) {
				return 1;
			}
			else if (fir_int_tail->data < sec_int_tail->data) {
				return -1;
			}
			fir_int_tail = fir_int_tail->prev;
			sec_int_tail = sec_int_tail->prev;
		}
		while (fir_dec_tail != NULL && sec_dec_tail != NULL) {
			if (fir_dec_tail->data > sec_dec_tail->data) {
				return 1;
			}
			else if (fir_dec_tail->data < sec_dec_tail->data) {
				return -1;
			}
			fir_dec_tail = fir_dec_tail->prev;
			sec_dec_tail = sec_dec_tail->prev;
		}

		if (fir_dec_tail != NULL && sec_dec_tail == NULL) {
			return 1;
		}
		else if (fir_dec_tail == NULL && sec_dec_tail != NULL) {
			return -1;
		}
		else {
			return 0;
		}

	}
	return 0;
}

void minus(NODE** fir_int_head, NODE** fir_dec_head, NODE** sec_int_head, NODE** sec_dec_head, STACK** stack, int compare) {

	NODE* rev_int_head = NULL, * rev_int_tail = NULL, * rev_dec_head = NULL, * rev_dec_tail = NULL;      //reverse int, dec
	int fir_dec_len, sec_dec_len, fir_int_len, sec_int_len, long_len = 0, short_len = 0;
	int num_tmp = 0, rounddown = 0;

	switch (compare) {
	case -1:
		push(stack, '-');
		minus(sec_int_head, sec_dec_head, fir_int_head, fir_dec_head, stack, 1);
		return;
		break;
	case 0:
		push(stack, '0');
		return;
		break;
	}

	fir_dec_len = getNodeLength(*fir_dec_head);
	sec_dec_len = getNodeLength(*sec_dec_head);
	fir_int_len = getNodeLength(*fir_int_head);
	sec_int_len = getNodeLength(*sec_int_head);
	long_len = fir_dec_len >= sec_dec_len ? fir_dec_len : sec_dec_len;
	short_len = fir_dec_len <= sec_dec_len ? fir_dec_len : sec_dec_len;

	//소수부 자릿수가 더 큰 숫자의 뒷 부분을 먼저 채운다.
	if (fir_dec_len > sec_dec_len) {
		for (int i = 0; i < fir_dec_len - sec_dec_len; i++) {
			append(&rev_dec_head, &rev_dec_tail, newnode(dequeue(fir_dec_head)));
		}
	}
	else if (fir_dec_len < sec_dec_len) {
		for (int i = 0; i < sec_dec_len - fir_dec_len; i++) {
			num_tmp = 10 - (dequeue(sec_dec_head) - '0') - rounddown;
			rounddown = 1;
			append(&rev_dec_head, &rev_dec_tail, newnode(num_tmp + '0'));
		}
	}
	for (int i = 0; i < short_len; i++) {
		num_tmp = (dequeue(fir_dec_head) - '0') - (dequeue(sec_dec_head) - '0') - rounddown;
		if (num_tmp < 0) {
			num_tmp += 10;
			rounddown = 1;
		}
		else {
			rounddown = 0;
		}
		append(&rev_dec_head, &rev_dec_tail, newnode(num_tmp + '0'));
	}



	for (int i = 0; i < sec_int_len; i++) {
		num_tmp = (dequeue(fir_int_head) - '0') - (dequeue(sec_int_head) - '0') - rounddown;
		if (num_tmp < 0) {
			num_tmp += 10;
			rounddown = 1;
		}
		else {
			rounddown = 0;
		}
		append(&rev_int_head, &rev_int_tail, newnode(num_tmp + '0'));
	}


	for (int i = 0; i < fir_int_len - sec_int_len; i++) {
		num_tmp = (dequeue(fir_int_head) - '0') - rounddown;
		if (num_tmp < 0) {
			num_tmp += 10;
			rounddown = 1;
		}
		else {
			rounddown = 0;
		}
		append(&rev_int_head, &rev_int_tail, newnode(num_tmp + '0'));
	}

	//정수부 앞에 필요 없는 '0' 제거
	//rev_int_tail->prev!=NULL은 00.123과 같은 경우에 .123으로 만들지 않기 위함.
	while (rev_int_tail->data == '0' && rev_int_tail->prev != NULL) {
		reverse_dequeue(&rev_int_tail, &rev_int_head);
	}

	//소수부 뒤에 필요 없는 '0' 제거
	while (rev_dec_head != NULL && rev_dec_head->data == '0') {
		dequeue(&rev_dec_head);
	}

	if (rev_dec_head == NULL || rev_dec_tail == NULL) {
		rev_dec_tail = NULL;
		rev_dec_head = NULL;
	}
	if (rev_int_head == NULL || rev_int_tail == NULL) {
		rev_int_tail = NULL;
		rev_int_head = NULL;
	}

	//소수부 정수부 합치기
	while (rev_int_tail != NULL) {
		push(stack, reverse_dequeue(&rev_int_tail, &rev_int_head));

	}

	if (rev_dec_head != NULL) {
		push(stack, '.');
	}
	while (rev_dec_tail != NULL) {
		push(stack, reverse_dequeue(&rev_dec_tail, &rev_dec_head));
	}

}

void cal(NODE** postfix_head, NODE** postfix_tail) {
	STACK* stack = NULL;
	NODE* result_head = NULL;
	NODE* result_tail = NULL;

	while (*postfix_head != NULL) {
		char postfix_data = dequeue(postfix_head);
		char data_type = getCharType(postfix_data);
		int compare = 100;
		if (data_type == OPERAND || data_type == DECIMAL_POINT) {
			push(&stack, postfix_data);
		}
		else if (data_type == SPACE) {
			push(&stack, '@');         //구분자
		}
		else if (data_type == OPERATOR) {
			//stack에서 꺼내서 fir, sec은 거꾸로 뒤집혀져 있다.
			NODE* fir_dec_head = NULL, * fir_dec_tail = NULL, * fir_int_head = NULL, * fir_int_tail = NULL;
			NODE* sec_dec_head = NULL, * sec_dec_tail = NULL, * sec_int_head = NULL, * sec_int_tail = NULL;
			pop(&stack);

			while (stack->data != '@' && stack->data != '.') {
				append(&sec_dec_head, &sec_dec_tail, newnode(pop(&stack)));
			}

			if (stack->data == '.') {
				pop(&stack);
				while (stack->data != '@') {
					append(&sec_int_head, &sec_int_tail, newnode(pop(&stack)));
				}
			}
			else if (stack->data == '@') {
				pop(&stack);
				sec_int_head = sec_dec_head;
				sec_int_tail = sec_dec_tail;
				sec_dec_head = NULL;           //free 해주는 편이 좋음
				sec_dec_tail = NULL;
			}

			if (stack->data == '@') {
				pop(&stack);
			}

			while (stack != NULL && stack->data != '.' && stack->data != '@') {
				append(&fir_dec_head, &fir_dec_tail, newnode(pop(&stack)));
			}

			if (stack != NULL && stack->data != '@') {
				if (stack->data == '.') {
					pop(&stack);
				}
				while (stack != NULL && stack->data != '@') {
					append(&fir_int_head, &fir_int_tail, newnode(pop(&stack)));
				}

			}
			else if (stack == NULL || stack->data == '@') {
				fir_int_head = fir_dec_head;
				fir_int_tail = fir_dec_tail;
				fir_dec_head = NULL;
				fir_dec_tail = NULL;
			}

			switch (postfix_data) {
			case '+':
				//계산하고자하는 수 중에 음수가 있을 때
				if (fir_int_tail->data == '-') {
					reverse_dequeue(&fir_int_tail, &fir_int_tail); //-를 빼준다.
					compare = node_compare(sec_int_head, sec_int_tail, sec_dec_head, sec_dec_tail, fir_int_head, fir_int_tail, fir_dec_head, fir_dec_tail);
					minus(&sec_int_head, &sec_dec_head, &fir_int_head, &fir_dec_head, &stack, compare);
					break;
				}
				else if (sec_int_tail->data == '-') {
					reverse_dequeue(&sec_int_tail, &sec_int_head);
					compare = node_compare(fir_int_head, fir_int_tail, fir_dec_head, fir_dec_tail, sec_int_head, sec_int_tail, sec_dec_head, sec_dec_tail);
					minus(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack, compare);
					break;
				}
				else {
					plus(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
				}
				break;
			case '-':
				//계산하고자하는 수 중에 음수가 있을 때
				if (fir_int_tail->data == '-') {
					reverse_dequeue(&fir_int_tail, &fir_int_head);
					push(&stack, '-');
					plus(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
					break;
				}
				else if (sec_int_tail->data == '-') {
					reverse_dequeue(&sec_int_tail, &sec_int_tail);
					plus(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
					break;
				}
				else {
					compare = node_compare(fir_int_head, fir_int_tail, fir_dec_head, fir_dec_tail, sec_int_head, sec_int_tail, sec_dec_head, sec_dec_tail);
					minus(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack, compare);
				}

				break;
			case '*':
				if (fir_int_tail->data == '-' && sec_int_tail->data == '-') {
					reverse_dequeue(&fir_int_tail, &fir_int_head);
					reverse_dequeue(&sec_int_tail, &sec_int_tail);
					multiply(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
				}
				else if (fir_int_tail->data == '-' && sec_int_tail->data != '-') {
					reverse_dequeue(&fir_int_tail, &fir_int_head);
					push(&stack, '-');
					multiply(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
				}
				else if (fir_int_tail->data != '-' && sec_int_tail->data == '-') {
					reverse_dequeue(&sec_int_tail, &sec_int_tail);
					push(&stack, '-');
					multiply(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
				}
				else {
					multiply(&fir_int_head, &fir_dec_head, &sec_int_head, &sec_dec_head, &stack);
				}
				break;


			}

		}
	}
	FILE* result = fopen("result.txt", "w");

	while (stack != NULL) {
		char tmp = pop(&stack);
		if (tmp == '@') {
			continue;
		}
		append(&result_head, &result_tail, newnode(tmp));
	}

	while (result_tail != NULL && result != NULL) {
		char tmp = reverse_dequeue(&result_tail, &result_head);
		fputc(tmp, result);
	}
	fclose(result);
}


