#include <stdio.h>
#include <stdlib.h>

typedef struct insegnamento {
    char nome[40];
} corso_t;

typedef struct voti {
    int *voto;
    int *cfu;
    int size;
    int target;
    corso_t *corso;
} voti_t;

voti_t *create (voti_t *v) {
    v = (voti_t*)malloc(sizeof(voti_t));
    v->voto = (int*)malloc(sizeof (int));
    v->cfu = (int*)malloc(sizeof (int));
    v->corso = (corso_t*)malloc(sizeof(corso_t));
    v->size = 0;
    v->target = 0;
    return v;
}

int check_op(char operation) {
    if (operation == 'a' || operation == 'A') return 1;
    else if (operation == 'p' || operation == 'P') return 2;
    else if (operation == 'd' || operation == 'D') return 3;
    else if (operation == 's' || operation == 'S') return 4;
    else return 0;
}

int media_print(voti_t *v, const *votame, const *cfu, int size) {
    if (size == 0)
        return 0;
    else {
/*
        printf("voto %d, cfu %d\n", votame[0], cfu[0]);
*/
        return (votame[0] * cfu[0]) + media_print(v, votame + 1, cfu + 1, size - 1);
    }
}

void print(voti_t *v) {
    int i , tot_crediti = 0, somma_parziale = 0;
    float voto_laurea, media;
    printf("INSEGNAMENTO                                 VOTO  CFU   \n");
    for (i = 0; i < v->size; i++) {
        printf("+-----------------------------------------------------+\n");
        printf("|%d. %s ", i+1, v->corso[i].nome);
        printf("| %d |", v->voto[i]);
        if (v->cfu[i] < 10) {
            printf("  %d |", v->cfu[i]);
        } else {
            printf(" %d |", v->cfu[i]);
        }
        printf("\n");
        tot_crediti += v->cfu[i];
    }
    printf("+-----------------------------------------------------+\n");
    printf("\n\n");
    somma_parziale = media_print(v, v->voto, v->cfu, v->size);
/*
    printf("sommapar %d\n", somma_parziale);
*/
    media = (float)somma_parziale;
    media /= (float)tot_crediti;
    if (v->size > 0) {
        printf("La media ponderata e': %.2f\n", media);
        voto_laurea = (media * 110) / 30;
        printf("Il voto di partenza e': %.0f/110\n\n", voto_laurea);
        printf("Ti mancano %d crediti alla laurea!\n", 180-tot_crediti);
        /*printf("Per il tuo target %d dovresti avere la media del %d\n", v->target, (v->target * 30)/110);*/
    }
}

void delete(voti_t *v) {
    if (v->size > 0) {
        corso_t temp[v->size - 1];
        int arr[v->size-1];
        int c_arr[v->size-1];
        int n, i, j, count = 0;
        printf("Cosa vuoi cancellare?");
        scanf("%d", &n);
        printf("\n");
        v->voto[n-1] = 0;
        v->cfu[n-1] = 0;
        v->corso[n-1].nome[0] = '{';
        for (i = 0; i < v->size; i++) {
            if (v->voto[i] != 0 && v->corso[i].nome[0] != '{') {
                arr[i-count] = v->voto[i];
                c_arr[i-count] = v->cfu[i];
                for (j = 0; j < 40; j++) {
                    temp[i - count].nome[j] = v->corso[i].nome[j];
                }
            } else {
                count++;
            }
        }
        v->size -= 1;
        v->voto = (int*)malloc(v->size*sizeof (int));
        v->cfu = (int*)malloc(v->size*sizeof (int));
        v->corso = (corso_t*)malloc(v->size* sizeof(corso_t));
        for (i = 0; i < v->size; i++) {
            v->voto[i] = arr[i];
            v->cfu[i] = c_arr[i];
            for (j = 0; j < 40; j++) {
                v->corso[i].nome[j] = temp[i].nome[j];
            }
        }
        print(v);
    }
}

void add(voti_t *v) {
    int vote = 0;
    int cfu = 0;
    int n = 0;
    int i;
    printf("Quanti voti vuoi inserire consecutivamente? ");
    scanf("%d", &n);
    printf("\n");
    while (n>0) {
        getchar();
        printf("inserisci nome insegnamento: ");
        v->size += 1;
        v->corso = realloc(v->corso, v->size * sizeof(corso_t));
        fgets(&v->corso[v->size - 1].nome[0], 40, stdin);
        for (i = 0; i < 40; i++) {
            if (v->corso[v->size - 1].nome[i] == '\n') {
                for (i = i; i < 40; i++) {
                    v->corso[v->size - 1].nome[i] = ' ';
                }
            }
        }
        v->corso[v->size - 1].nome[39] = '\0';
        printf("inserisci voto: ");
        scanf("%d", &vote);
        printf("inserisci crediti: ");
        scanf("%d", &cfu);
        printf("\n");
        v->voto = realloc(v->voto, v->size * sizeof(int));
        v->cfu = realloc(v->cfu, v->size * sizeof(int));
        v->voto[v->size - 1] = vote;
        v->cfu[v->size - 1] = cfu;
        n--;
    }
    print(v);
}

int main() {
    FILE *votes_f;
    FILE *sizes_f;
    FILE *cfus_f;
    FILE *courses_f;
    voti_t *voto = NULL;
    int i, j;
    int exitt = 0;
    char operation = 'x';
    voto = create(voto);
    votes_f = fopen("voti.txt", "r+");
    if (votes_f) {
        printf("prova entrata\n");
        sizes_f = fopen("size.txt", "r+");
        cfus_f = fopen("cfus.txt", "r+");
        courses_f = fopen("courses.txt", "r+");
        fscanf(sizes_f, "%d", &voto->size);
        voto->voto = (int*)malloc(voto->size * sizeof(int));
        voto->cfu = (int*)malloc(voto->size * sizeof(int));
        voto->corso = (corso_t*)malloc(voto->size * sizeof(corso_t));
        for (i = 0; i < voto->size; i++) {
            fscanf(votes_f, "%d ", &voto->voto[i]);
            fscanf(cfus_f, "%d ", &voto->cfu[i]);
            fgets(voto->corso[i].nome,40, courses_f);
        }
        fclose(courses_f);
        fclose(sizes_f);
        fclose(cfus_f);
        fclose(votes_f);
    }
    /*printf("inserisci il tuo obiettivo in centodecimi: \n");
    scanf("%d", &voto->target);*/
    setbuf(stdout, 0);
    printf("CALCOLA LA TUA MEDIA VOTI UNIVERSITARIA!\n");
    while(!exitt) {
        printf("cosa vuoi fare? (a)dd, (d)el, (p)rint, (s)ave and quit?: ");
        scanf(" %c", &operation);
        switch (check_op(operation)) {
            case 1: {
                add(voto);
                continue;
            }
            case 2: {
                print(voto);
                continue;
            }
            case 3: {
                delete(voto);
                continue;
            }
            case 4: {
                exitt = 1;
                continue;
            }
        }
    }
    votes_f = fopen("voti.txt", "w");
    sizes_f = fopen("size.txt", "w");
    cfus_f = fopen("cfus.txt", "w");
    courses_f = fopen("courses.txt", "w");
    for (i = 0; i < voto->size; i++) {
        fprintf(votes_f, "%d ", voto->voto[i]);
        fprintf(cfus_f, "%d ", voto->cfu[i]);
        fprintf(courses_f, "%s", voto->corso[i].nome);
    }
    fprintf(sizes_f, "%d", voto->size);
    fclose(votes_f);
    fclose(cfus_f);
    fclose(sizes_f);
    fclose(courses_f);
    exit(EXIT_SUCCESS);
}