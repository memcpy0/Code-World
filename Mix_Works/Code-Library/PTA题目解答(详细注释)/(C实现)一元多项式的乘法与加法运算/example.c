/******************************************************************************************************************************
题目：一元多项式的乘法与加法运算
设计函数分别求两个一元多项式的乘积与和。
输入格式:
输入分2行，每行分别先给出多项式非零项的个数，再以指数递降方式输入一个多项式非零项系数和指数（绝对值均为不超过1000的整数）。
数字间以空格分隔。
输出格式:
输出分2行，分别以指数递降方式输出乘积多项式以及和多项式非零项的系数和指数。数字间以空格分隔，但结尾不能有多余空格。
零多项式应输出0 0。
输入样例:
4 3 4 -5 2  6 1  -2 0
3 5 20  -7 4  3 1
输出样例:
15 24 -25 22 30 21 -10 20 -21 8 35 6 -33 5 14 4 -15 3 18 2 -6 1
5 20 -4 4 -5 2 9 1 -2 0
*********************************************************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

#define ElementType int

typedef struct PNode *Polynomial;
struct PNode{
	ElementType coef;
	ElementType expon;
	Polynomial next;
};

void Attach(ElementType coef, ElementType expon, Polynomial *tail);
Polynomial Insert(Polynomial P, ElementType coef, ElementType expon);
Polynomial scanPoly(void);
Polynomial AddPoly(Polynomial P1, Polynomial P2);
Polynomial MultiPoly(Polynomial P1, Polynomial P2);
void printPoly(Polynomial P);

int main()
{
	Polynomial P1, P2, PAdd, PMulti;
	P1 = scanPoly();
	P2 = scanPoly();
	
	PAdd = AddPoly(P1, P2);
	PMulti = MultiPoly(P1, P2);
	
	printPoly(PMulti);
	printPoly(PAdd);
	
	return 0;
}

void Attach(ElementType coef, ElementType expon, Polynomial *tail)
{
	Polynomial temp = (Polynomial)(malloc(sizeof(struct PNode)));
	temp->coef = coef;
	temp->expon = expon;
	temp->next = NULL;
	(*tail)->next = temp;
	*tail = temp;	
}

Polynomial scanPoly(void)
{
	Polynomial P1, tail, head;
	ElementType coef, expon;
	int num;
	scanf("%d", &num);
	P1 = (Polynomial)(malloc(sizeof(struct PNode)));
	P1->next = NULL;
	tail = P1;
	while(num--){
		scanf("%d %d", &coef, &expon);
		Attach(coef, expon, &tail);		
	}
	head = P1;
	P1 = P1->next;
	free(head);
	
	return P1;
}

Polynomial AddPoly(Polynomial P1, Polynomial P2)
{
	Polynomial PAdd, tail, head; 
	ElementType coef, expon;
	PAdd = (Polynomial)(malloc(sizeof(struct PNode)));
	PAdd->next = NULL;
	tail = PAdd;
	while(P1 && P2){
		if( P1->expon > P2->expon) {
			coef = P1->coef;
			expon = P1->expon;
			Attach(coef, expon, &tail);
			P1 = P1->next;
		}
		else if(P1->expon == P2->expon ){
			coef = P1->coef + P2->coef;
			if(coef != 0){
				expon = P1->expon;
				Attach(coef, expon, &tail);
				P1 = P1->next;P2 = P2->next;	
			}
		}
		else {
			coef = P2->coef;
			expon = P2->expon;
			Attach(coef, expon, &tail);
			P2 = P2->next;
		}
	}
	while(P1){
		coef = P1->coef;
		expon = P1->expon;
		Attach(coef, expon, &tail);
		P1 = P1->next;
	}
	while(P2){
		coef = P2->coef;
		expon = P2->expon;
		Attach(coef, expon, &tail);
		P2 = P2->next;
	}
	head = PAdd;
	PAdd = PAdd->next;
	free(head);
	
	return PAdd;
}

Polynomial Insert(Polynomial P, ElementType coef, ElementType expon)
{
	Polynomial temp, cur, curPre;
	temp = (Polynomial)malloc(sizeof(struct PNode));
	temp->coef = coef;
	temp->expon = expon;
	temp->next = NULL;
	if(P == NULL)
		P = temp;
	else {
		for(curPre = cur = P; cur && (cur->expon > temp->expon); cur = cur->next) 
			curPre = cur;
		if(cur == NULL){
			curPre->next = temp;
		}
		else if(cur->expon < temp->expon){
//			if(cur == P){               /*因为插入的第一个项肯定为指数最高项，因此这里不存在比头结点还大的指数项 */ 
//				temp->next = P;
//				P = temp;
//			}
//			else{
				curPre->next = temp;
				temp->next = cur;
//			}
		}
		else{
			cur->coef += temp->coef;  /*因为插入的第一个项肯定为指数最高项，因此这里不存在比头结点还大的指数项 */
			if(cur->coef == 0){
				curPre->next = cur->next;
				free(cur);
			}
		}
	}
	
	return P;
}

Polynomial MultiPoly(Polynomial P1, Polynomial P2)
{
	Polynomial P3, curP1, curP2;
	ElementType coef, expon;
	P3 = NULL;
	for(curP1 = P1; curP1; curP1 = curP1->next){
		for(curP2 = P2; curP2; curP2 = curP2->next){
			coef = curP1->coef * curP2->coef;
			expon = curP1->expon + curP2->expon;
			if(coef != 0) 
				P3 = Insert(P3, coef, expon);
		}
	}
	
	return P3; 
}


void printPoly(Polynomial P)
{
	if(P == NULL)
		printf("0 0\n");
	else {
		for(; P->next; P = P->next){
			printf("%d %d", P->coef, P->expon);
			printf(" ");
		}
		printf("%d %d\n", P->coef, P->expon);
	}
}
