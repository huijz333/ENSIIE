#include "main.h"

static void registerQuestion(t_array_list * questions, char * label, t_function function) {
	t_question question;
	question.label = strdup(label);
	question.function = function;
	array_list_add(questions, &question);
}

static void maxInt() {
	int a, b;
	printf("max(a, b)\n");
	printf("a=");
	scanf("%d", &a);
	printf("b=");
	scanf("%d", &b);
	printf("maximum is: %d\n", (a > b ? a : b));
}

static void parity() {
	printf("Parite de a\n");
	printf("a=");
	int a;
	scanf("%d", &a);
	printf("a est pair: %d\n", (a + 1) % 2);
}

static int ft_pow(int a, int b) {
	int r = 1;
	int i;
	for (i = 0 ; i < b; i++) {
		r = r * a;
	}
	return (r);
}

static void power() {
	printf("pow(a, b)\n");
	int a, b;
	printf("a=");
	scanf("%d", &a);
	printf("b=");
	scanf("%d", &b);
	printf("pow(a, b) == %d\n", ft_pow(a, b));
}

static void triangle() {
	printf("nombre de ligne=");
	int n;
	scanf("%d", &n);
	int i, j;
	for (i = 0 ; i < n ; i++) {
		for (j = 0 ; j < n - i - 1 ; j++) {
			printf(" ");
		}
		for (j = 0 ; j < i * 2 + 1 ; j++) {
			printf("*");
		}
		printf("\n");
	}
}

static void triangleRev() {
	printf("nombre de ligne=");
	int n;
	scanf("%d", &n);
	int i, j;
	for (i = 0 ; i < n ; i++) {
		for (j = 0 ; j < i ; j++) {
			printf(" ");
		}
		for (j = 0 ; j < (n - i - 1) * 2 + 1 ; j++) {
			printf("*");
		}
		printf("\n");
	}
}

static int _pgcd(int a, int b) {
	while (1) {
		if (a < b) {
			int c = a;
			a = b;
			b = c;
		}
		int r = a % b;
		if (r == 0) {
			break ;
		}
		a = b;
		b = r;
	}
	return (b);
}

static void pgcd() {
	printf("pgcd(a, b)\n");
	int a, b;
	printf("a=");
	scanf("%d", &a);
	printf("b=");
	scanf("%d", &b);

	//https://fr.wikipedia.org/wiki/Algorithme_d%27Euclide
	printf("pgcd(%d, %d) == %d\n", a, b, _pgcd(a, b));
}

void _swap(int * a, int * b) {
	int c = *a;
	*a = *b;
	*b = c;
}

void puiss(int a, int b, int * r) {
	*r = ft_pow(a, b);
}

static void rationnelAdd(int * a, int * b, int * c) {
	c[0] = a[0] * b[1] + a[1] * b[0];
	c[1] = a[1] * b[1];
	int p = _pgcd(c[0], c[1]);
	c[0] /= p;
	c[1] /= p;
}

static void rationnel() {
	int a[2];
	int b[2];
	int c[2];
	printf("a=na/da\nna=");
	scanf("%d", a);
	printf("da=");
	scanf("%d", a + 1);
	
	printf("b=nb/db\nnb=");
	scanf("%d", b);
	printf("db=");
	scanf("%d", b + 1);

	rationnelAdd(a, b, c);
	printf("%d/%d + %d/%d == %d/%d\n", a[0], a[1], b[0], b[1], c[0], c[1]);
}

int main() {
	t_array_list * questions = array_list_new(16, sizeof(t_question));
	registerQuestion(questions, "Calcul le maximum des deux entiers", (t_f)maxInt);
	registerQuestion(questions, "Calcul la parité d'un entier", (t_f)parity);
	registerQuestion(questions, "Calcul puissance", (t_f)power);
	registerQuestion(questions, "Affiche un triangle", (t_f)triangle);
	registerQuestion(questions, "Affiche un triangle renverse", (t_f)triangleRev);
	registerQuestion(questions, "Calcul le pgcd de 2 fonctions", (t_f)pgcd);
	registerQuestion(questions, "Somme de 2 rationnels", (t_f)rationnel);
	while (1) {
		ARRAY_LIST_ITER_START(questions, t_question *, question, index) {
			printf("Q%lu: %s\n", index + 1, question->label);
		}
		ARRAY_LIST_ITER_END(questions, t_question *, question, index);

		unsigned int q;
		if (scanf("%d", &q) == 0) {
			break ;
		}
		if (q > questions->size) {
			printf("Mauvais argument\n");
			continue ;
		}
		t_question * question = (t_question *)array_list_get(questions, q - 1);
		printf("---------------------------------------\n");
		printf("%s\n", question->label);
		question->function();
		printf("---------------------------------------\n");
	}
	ARRAY_LIST_ITER_START(questions, t_question *, question, index) {
		free(question->label);
	}
	ARRAY_LIST_ITER_END(questions, t_question *, question, index);


	array_list_delete(questions);
	return (0);
}
