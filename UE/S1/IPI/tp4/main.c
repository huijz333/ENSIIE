#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

/* PILE Q1 */

typedef struct {
    int count;
    int values[MAX_SIZE];
} stack;

/*  @require: ps non null.
    @assign: ps
    @ensure: initialise ou vide ps. */
void stack_init(stack* ps) {
    ps->count = 0;
}

/*  @require: ps non null.
    @ensure: ... */
int stack_isEmpty(stack* ps) {
    return ps->count == 0;
}

/*  @require: ps non null.
    @assign: ps
    @ensure: ... retourne 0 si pile pleine, 1 sinon. */
int stack_push(stack* ps, int value) {
    if (ps->count == MAX_SIZE) return 0;
    
    ps->values[ps->count] = value;
    ps->count++;
    return 1;
}

/*  @require: ps non null. ps non vide.
    @assign: ps
    @ensure: ... */
int stack_pop(stack* ps) {
    ps->count--;
    return ps->values[ps->count];
}

/*  @require: ps non null. pvalue non null.
    @assign: ps, pvalue
    @ensure: ... retourne 0 si pile vide, 1 sinon. */
int stack_tryPop(stack* ps, int* pvalue) {
    if (ps->count == 0) return 0;
    
    ps->count--;
    *pvalue = ps->values[ps->count];
    return 1;
}

/*  @require: ps non null.
    @ensure: ... */
void stack_print(stack* ps) {
    int i;
    for (i = ps->count-1; i >= 0; i--) {
        printf("%4d ", ps->values[i]);
    }
    
    printf("\n");
}

/* PILE Q2 */
/*  @require: ps non null.
    @ensure: ... retourne 0 si pile vide ou pleine, 1 sinon. */
int stack_duplicateTop(stack* ps) {
    int top;
    if (!stack_tryPop(ps, &top))    return 0;
    if (!stack_push(ps, top))       return 0;
    if (!stack_push(ps, top))       return 0;
    
    return 1;
}

/* PILE Q3.1 */
/*  @require: ps non null.
    @ensure: ... retourne 0 si pile vide ou pleine, 1 sinon. */
int stack_pushTopMinusOne(stack* ps) {
    int top;
    if (!stack_tryPop(ps, &top))    return 0;
    if (!stack_push(ps, top))       return 0;
    if (!stack_push(ps, top-1))     return 0;
    
    return 1;
}

/* PILE Q3.2 */
/* Non précisé dans l'énoncé mais on s'autorise ici à ce que la pile soit vidée
à la sortie de la fonction. */
    
/*  @require: ps non null.
    @assign: ps
    @ensure: ... */
int stack_product(stack* ps) {
    int top, ret = 1;
    while (stack_tryPop(ps, &top)){
        ret = ret * top;
    }
    
    return ret;
}

/* PILE Q3.3 */

int factorial1(int n) {
    int i;
    stack s;
    stack_init(&s);
    stack_push(&s, n);
    stack_print(&s);
    
    for (i = 0; i < n-1; i++) {
        stack_pushTopMinusOne(&s);
        stack_print(&s);
    }
    
    return stack_product(&s);
}

/* PILE Q4.1 */

/* Nom de fonction imprécise. Répéter le détail de l'énoncé dans @ensure.*/
/*  @require: ps non null.
    @assign: ps
    @ensure: empile le produit des deux premiers éléments accessibles d'une pile
    (éléments de type int) puis l'ancien sommet moins 1. Par exemple si une pile
    contient k éléments, que son sommet est n et l'étage en dessous du sommet m,
    au sortir de la fonction la pile aura k éléments, son sommet sera n-1 et
    l'étage en dessous du sommet contiendra n*m.
    Retourne 0 si la pile ne contient pas au moins 2 éléments. */
int stack_productTop(stack* ps) {
    int n, m;
    if (!stack_tryPop(ps, &n)) return 0;
    if (!stack_tryPop(ps, &m)) return 0;
    
    stack_push(ps, n*m);
    stack_push(ps, n-1);
    
    return 1;
}

/* PILE Q4.2 */

int factorial2(int n) {
    int i;
    stack s;
    stack_init(&s);
    stack_push(&s, n);
    stack_push(&s, n-1);
    stack_print(&s);
    
    for (i = 0; i < n-1; i++) {
        stack_productTop(&s);
        stack_print(&s);
    }
    
    stack_pop(&s);
    return stack_pop(&s);
}

int main()
{
    printf("factorial1(5) = %d\n", factorial1(5));
    printf("factorial2(5) = %d\n", factorial2(5));

    return EXIT_SUCCESS;
}

/* PILE Q5 */
/*  -> factorial1 necessite une pile de taille n, tandis que factorial2
necessite une pile de taille 2.
    -> factorial1 simule la pile mémoire lors de l'execution d'une fonction
factorielle récursive tandis que factorial2 correspond à une factorielle
itérative.
*/
