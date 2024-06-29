/******************************************************************************************************************************
��Ŀ��һԪ����ʽ�ĳ˷���ӷ�����
��ƺ����ֱ�������һԪ����ʽ�ĳ˻���͡�
�����ʽ:
�����2�У�ÿ�зֱ��ȸ�������ʽ������ĸ���������ָ���ݽ���ʽ����һ������ʽ������ϵ����ָ��������ֵ��Ϊ������1000����������
���ּ��Կո�ָ���
�����ʽ:
�����2�У��ֱ���ָ���ݽ���ʽ����˻�����ʽ�Լ��Ͷ���ʽ�������ϵ����ָ�������ּ��Կո�ָ�������β�����ж���ո�
�����ʽӦ���0 0��
��������:
4 3 4 -5 2  6 1  -2 0
3 5 20  -7 4  3 1
�������:
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
//			if(cur == P){               /*��Ϊ����ĵ�һ����϶�Ϊָ������������ﲻ���ڱ�ͷ��㻹���ָ���� */ 
//				temp->next = P;
//				P = temp;
//			}
//			else{
				curPre->next = temp;
				temp->next = cur;
//			}
		}
		else{
			cur->coef += temp->coef;  /*��Ϊ����ĵ�һ����϶�Ϊָ������������ﲻ���ڱ�ͷ��㻹���ָ���� */
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
