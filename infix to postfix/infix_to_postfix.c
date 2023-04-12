#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100
int err;

typedef char element;
typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
}StackType;

// 스택 초기화
void init_stack(StackType* s)
{
	s->top = -1;
}

// 공백 상태 검사
int is_empty(StackType* s)
{
	return (s->top == -1);
}

// 포화 상태 검사
int is_full(StackType* s)
{
	return(s->top == (MAX_STACK_SIZE - 1));
}

// 삽입함수
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "Stack full\n");
		return;
	}
	else {
		s->top++;
		s->data[s->top] = item;
		return;
	}
}

//삭제함수
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "Stack empty\n");
		return;
	}

	else return s->data[(s->top--)];
}

// 피크함수
element peek(StackType* s)
{
	return s->data[s->top];
}

// 연산자 우선순위 반환
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}
int infix_to_postfix(char exp[], char cexp[]) {
	int i, j = 0;

	StackType s;
	init_stack(&s);

	for (i = 0; i < strlen(exp); i++) {
		switch (exp[i])
		{
		case '+': case '-': case '*': case '/':
			while (!is_empty(&s) && prec(exp[i]) <= prec(peek(&s)))
				cexp[j++] = pop(&s);
			push(&s, exp[i]);
			break;
		case '(':
			push(&s, exp[i]);
			break;
		case ')':
			while (pop(&s) != '(')
				cexp[j++] = pop(&s);
			break;
		default:
			cexp[j++] = exp[i];
			break;
		}
	}
	while (!is_empty(&s)) {
		cexp[j++] = pop(&s);
		cexp[j] = NULL;
	}
}

int eval(char exp[]) {
	int op1, op2, value, i;
	int len = strlen(exp);
	char ch;
	StackType s;

	init_stack(&s);

	for (i = 0; i < len; i++) {
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			value = ch - '0';
			push(&s, value);
		}
		else {
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch) {
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	return pop(&s);
}

void check_error(char exp[]) {
	err = -1;
	int len = strlen(exp);

	// 괄호 오류
	int count = 0;
	for (int i = 0; i < len; i++) {
		if (exp[i] == '(') {
			count++;
		}
		else if (exp[i] == ')') {
			count--;
		}
	}

	if (count > 0 || count < 0) {
		printf("Error: Mismathced parentheses\n");
		err = 1;
	}

	// 문자 입력 오류
	for (int i = 0; i < len; i++) {
		if (exp[i] != '+' && exp[i] != '-' && exp[i] != '*' && exp[i] != '/') {
			if (!isdigit(exp[i])) {
				printf("Error: Invalid character\n");
				err = 1;
			}
		}
		else {
			//수식 입력 오류
			if (!isdigit(exp[i + 1]) || exp[i + 1] == NULL) {
				printf("Error: Invalid expression\n");
				err = 1;
			}
		}
	}

	// 수식 입력 오류 
	if (exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/') {
		printf("Error: Invalid expression\n");
		err = 1;
	}
		return err;
}

int main() {
	char infix[MAX_STACK_SIZE];
	char postfix[MAX_STACK_SIZE];
	int result;

	printf("Enter an infix expression: ");
	scanf("%[^\n]s\n", infix);

	check_error(infix);

	if (err == -1) {
		infix_to_postfix(infix, postfix);
		printf("infix_to_postfix: %s\n", postfix);

		result = eval(postfix);
		printf("Result: %d\n", result);
	}

	return 0;
}
