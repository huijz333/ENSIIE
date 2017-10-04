#include "main.h"

static void registerQuestion(t_array_list * questions, const char * label, t_function function) {
	t_question question;
	question.label = strdup(label);
	question.function = function;
	array_list_add(questions, &question);
}

static void sum() {
	printf("lol\n");
}

int main() {
	t_array_list * questions = array_list_new(16, sizeof(t_question));
	registerQuestion(questions, "Calcul somme de 'a' + 'b'", sum);
	while (1) {
		ARRAY_LIST_ITER_START(questions, t_question *, question, index) {
			printf("Q%lu: %s\n", index, question->label);
		}
		ARRAY_LIST_ITER_END(questions, t_question *, question, index);

		int q;
		scanf("%d", &q);
	}
	ARRAY_LIST_ITER_START(questions, t_question *, question, index) {
		free(question->label);
	}
	ARRAY_LIST_ITER_END(questions, t_question *, question, index);


	array_list_delete(questions);
	return (0);
}
