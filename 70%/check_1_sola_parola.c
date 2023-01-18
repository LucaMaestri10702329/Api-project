#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _node {

    int vincolata;
    struct _node *left;
    struct _node *right;
    char key[];
} node;

long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int ritorna_posizione(char c){
    if(c=='-')
    {
        c = c - 45;
        return c;
    } else if(c > 45 && c < 65)
    {
        c = c - 47;
        return c;
    } else if(c>64 && c<91)
    {
        c = c - 54;
        return c;
    } else if(c == '_')
    {
        c = c - 58;
        return c;
    } else
        c = c - 59;
    return c;

}

node *create_node(char *input, int dim, int vincolata) {
    node *n = malloc(sizeof(node)+sizeof(char)*dim);
    n->vincolata = vincolata;
    n->left = NULL;
    n->right = NULL;
    memmove(n->key,input,dim);
    return n;
}

int strcmp_home_made(const char * s1, const char*s2){
    while(*s1 && (*s1 == *s2)){
        s1++, s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
void MorrisTraversal(struct _node * root,int dim)
{
    struct _node* current, *pre;

    if (root == NULL)
        return;

    current = root;
    while (current != NULL) {

        if (current->left == NULL) {
            if(current->vincolata == 0){
                puts(current->key);

            }

            current = current->right;
        }
        else {

            /* Find the inorder predecessor of current */
            pre = current->left;
            while (pre->right != NULL
                   && pre->right != current)
                pre = pre->right;

            /* Make current as the right child of its
               inorder predecessor */
            if (pre->right == NULL) {
                pre->right = current;
                current = current->left;
            }

                /* Revert the changes made in the 'if' part to
                   restore the original tree i.e., fix the right
                   child of predecessor */
            else {
                pre->right = NULL;
                if(current->vincolata == 0)
                    puts(current->key);
                current = current->right;
            } /* End of if condition pre->right == NULL */
        } /* End of if condition current->left == NULL*/
    } /* End of while */
}
node *BST_insert_iterative(node *root, char *input, unsigned int dim, int vincolata){
    node *temp = root;
    int condition = 1;

    if (root == NULL)
        return create_node(input,dim,vincolata);

    while (condition) {
        if (strcmp_home_made(input,temp->key)>0) {
            if (temp->right == NULL) {
                temp->right = create_node(input,dim,vincolata);
                condition = 0;
            }
            else
                temp = temp->right;
        }
        else {
            if (temp->left == NULL) {
                temp->left = create_node(input,dim,vincolata);
                condition = 0;
            }
            else
                temp = temp->left;
        }
    }
    return root;
}

node* ricerca_parola_2 (node *bt, char * info,unsigned int dim) {
    if(bt == NULL)
        return bt;
    else{
        unsigned int i = 0;
        while(i<dim){
            if(bt->key[i] == info[i]){
                i++;
            }
            else
            {
                if(bt->key[i] > info[i]) {
                    return  ricerca_parola_2(bt->left,info,dim);
                }
                else
                    return ricerca_parola_2(bt->right,info,dim);
            }
        }
        return bt;
    }
}


void riprisitna_albero(node *tree){
    if(tree!= NULL){
        riprisitna_albero(tree->right);
        tree->vincolata=0;
        riprisitna_albero(tree->left);
    }


}

node* deleteBinaryTree(node* bt) {
    if (bt == NULL) {
        return NULL;
    }
    deleteBinaryTree(bt->left);
    deleteBinaryTree(bt->right);
    free(bt);
    return NULL;
}

void word_checker_3(char *rif, char *word,unsigned int dim, char *vincoli, int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][64]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int i;

    memmove(temp,word,dim);
    memmove(temp_rif, rif,dim);

    for( i=0;i<dim-1;i++) {
        if (rif[i] == word[i]) {
            temp[i] = '+';
            output[i] = '+';
            temp_rif[i] = '+';
        } else
            output[i] = '/';
    }
    for(i=0;i<dim-1;i++) {
        if(output[i]=='/') {
            unsigned int j;
            for(j=0;j<dim-1;j++) {
                if(temp[i]==temp_rif[j]) {
                    output[i] = '|';
                    temp_rif[j] = '*';
                    break;
                }
            }
        }
    }


    output[dim-1] = '\0';
    printf("%s\n", output);
    for(i=0;i<dim-1;i++) {
        if(output[i]=='+')
            vincoli[i] = word[i];
        else {
            if(output[i]=='|') {
                unsigned int k, pres_word = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == word[k] && (output[k]!='/'))
                        pres_word ++;
                }
                unsigned int m = 0;
                while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i])
                    m++;
                presente_n[0][m] = word[i];
                presente_n[1][m] = pres_word;

                presente_pos_sbagliata[i][ritorna_posizione(word[i])] = 1;
            }
            else {
                unsigned int k, pres_rif = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == rif[k])
                        pres_rif++;
                }
                if(pres_rif == 0) {
                    non_presente[ritorna_posizione(word[i])] = 1;
                }

                else {

                    unsigned int m = 0;
                    while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i]) {
                        m++;
                    }
                    presente_n[0][m] = word[i];
                    presente_n[2][m] = pres_rif;

                    presente_pos_sbagliata[i][ritorna_posizione(word[i])] = 1;
                }
            }
        }
    }
    free(temp);
    free(temp_rif);
    free(output);
}

/*
node * applica_vincoli_vettori(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64], int  conta_filtrate[]) {
    if(bt != NULL) {
        unsigned int i, vincolata = 0;
        for (i = 0; i < dim - 1; i++) {
            if(vincoli[i]!=0 && vincoli[i]!=bt->key[i]) {

                vincolata = 1;
                break;
            }

            if(non_presente[ritorna_posizione(bt->key[i])] == 1)
                vincolata = 1;
            if(vincolata == 1)
                break;


            if(vincolata == 1)
                break;


            if(presente_pos_sbagliata[i][ritorna_posizione(bt->key[i])] == 1) {
                vincolata = 1;
                break;
            }
        }
        unsigned int j=0;
        while(presente_n[0][j]!=-1) {
            unsigned int m = 0, occorrenze = 0;
            for(m=0;m<dim-1;m++) {
                if(presente_n[0][j]==bt->key[m])
                    occorrenze ++;
            }
            if(presente_n[2][j]!=-1 && presente_n[2][j]!=occorrenze) {
                vincolata = 1;
                break;
            }

            if(presente_n[1][j]!=-1 && presente_n[1][j]>occorrenze) {

                vincolata = 1;
                break;
            }
            if(vincolata == 1)
                break;
            j++;
        }
        if(vincolata == 1) {
            conta_filtrate[0] = conta_filtrate[0] - 1;
        }
        else
            temp = BST_insert_iterative(temp,bt->key,dim);
        if(bt->right!=NULL)
            temp = applica_vincoli_vettori(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
        if(bt->left!=NULL)
            temp = applica_vincoli_vettori(bt->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
    }

    return temp;
}
 */

node * applica_vincoli_vettori_boolean(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64], int  conta_filtrate[]) {
    if(bt != NULL) {
        if(bt->vincolata==0) {
            if(bt->left!=NULL){
                if(bt->left->vincolata==0){
                    unsigned int i, vincolata = 0, vincolata_left = 0;
                    for (i = 0; i < dim - 1; i++) {
                        if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                            vincolata = 1;
                        }
                        if(vincoli[i] != 0 && vincoli[i] != bt->left->key[i])
                            vincolata_left = 1;

                        if (non_presente[ritorna_posizione(bt->key[i])] == 1)
                            vincolata = 1;
                        if (non_presente[ritorna_posizione(bt->left->key[i])] == 1)
                            vincolata_left = 1;

                        if (presente_pos_sbagliata[i][ritorna_posizione(bt->key[i])] == 1) {
                            vincolata = 1;
                        }
                        if (presente_pos_sbagliata[i][ritorna_posizione(bt->left->key[i])] == 1) {
                            vincolata_left = 1;
                        }
                        if(vincolata_left == 1 && vincolata == 1)
                            break;
                    }
                    unsigned int j = 0;
                    while (presente_n[0][j] != -1) {
                        unsigned int m = 0, occorrenze = 0, occorrenze_left = 0;
                        for (m = 0; m < dim - 1; m++) {
                            if (presente_n[0][j] == bt->key[m])
                                occorrenze++;
                            if (presente_n[0][j] == bt->left->key[m])
                            occorrenze_left++;
                        }
                        if (presente_n[2][j] != -1 && presente_n[2][j] != occorrenze) {
                            vincolata = 1;
                        }
                        if (presente_n[2][j] != -1 && presente_n[2][j] != occorrenze_left) {
                            vincolata_left = 1;
                        }

                        if (presente_n[1][j] != -1 && presente_n[1][j] > occorrenze) {
                            vincolata = 1;
                        }
                        if (presente_n[1][j] != -1 && presente_n[1][j] > occorrenze_left) {
                            vincolata_left = 1;
                        }
                        if (vincolata == 1 && vincolata_left == 1)
                            break;
                        j++;
                    }
                    if (vincolata == 1) {
                        conta_filtrate[0] = conta_filtrate[0] - 1;
                        bt->vincolata = 1;
                    }
                    if (vincolata_left == 1) {
                        conta_filtrate[0] = conta_filtrate[0] - 1;
                        bt->left->vincolata = 1;
                    }

                    if(bt->left->left!=NULL)
                        temp = applica_vincoli_vettori_boolean(bt->left->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    if(bt->left->right!=NULL)
                        temp = applica_vincoli_vettori_boolean(bt->left->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    if(bt->right!=NULL){
                        temp = applica_vincoli_vettori_boolean(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    }

                }
                else
                {
                    unsigned int i, vincolata = 0;
                    for (i = 0; i < dim - 1; i++) {
                        if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                            vincolata = 1;
                            break;
                        }

                        if (non_presente[ritorna_posizione(bt->key[i])] == 1)
                            vincolata = 1;
                        if (vincolata == 1)
                            break;

                        if (presente_pos_sbagliata[i][ritorna_posizione(bt->key[i])] == 1) {
                            vincolata = 1;
                            break;
                        }
                    }
                    unsigned int j = 0;
                    while (presente_n[0][j] != -1) {
                        unsigned int m = 0, occorrenze = 0;
                        for (m = 0; m < dim - 1; m++) {
                            if (presente_n[0][j] == bt->key[m])
                                occorrenze++;
                        }
                        if (presente_n[2][j] != -1 && presente_n[2][j] != occorrenze) {
                            vincolata = 1;
                            break;
                        }

                        if (presente_n[1][j] != -1 && presente_n[1][j] > occorrenze) {

                            vincolata = 1;
                            break;
                        }
                        if (vincolata == 1)
                            break;
                        j++;
                    }
                    if (vincolata == 1) {
                        conta_filtrate[0] = conta_filtrate[0] - 1;
                        bt->vincolata = 1;
                    }
                    if(bt->right!=NULL){
                        if(bt->right->vincolata == 1){
                            if(bt->right->right != NULL)
                                temp = applica_vincoli_vettori_boolean(bt->right->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                            if(bt->right->left != NULL)
                                temp = applica_vincoli_vettori_boolean(bt->right->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                        }
                        else {
                            temp = applica_vincoli_vettori_boolean(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                        }
                    }

                    if(bt->left!=NULL){
                        if(bt->left->vincolata == 1){
                            if(bt->left->right!=NULL)
                                temp = applica_vincoli_vettori_boolean(bt->left->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                            if(bt->left->left!=NULL)
                                temp = applica_vincoli_vettori_boolean(bt->left->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                        }
                        else {
                            temp = applica_vincoli_vettori_boolean(bt->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                        }
                    }
                }
            } else {
                unsigned int i, vincolata = 0;
                for (i = 0; i < dim - 1; i++) {
                    if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                        vincolata = 1;
                        break;
                    }

                    if (non_presente[ritorna_posizione(bt->key[i])] == 1)
                        vincolata = 1;
                    if (vincolata == 1)
                        break;

                    if (presente_pos_sbagliata[i][ritorna_posizione(bt->key[i])] == 1) {
                        vincolata = 1;
                        break;
                    }
                }
                unsigned int j = 0;
                while (presente_n[0][j] != -1) {
                    unsigned int m = 0, occorrenze = 0;
                    for (m = 0; m < dim - 1; m++) {
                        if (presente_n[0][j] == bt->key[m])
                            occorrenze++;
                    }
                    if (presente_n[2][j] != -1 && presente_n[2][j] != occorrenze) {
                        vincolata = 1;
                        break;
                    }

                    if (presente_n[1][j] != -1 && presente_n[1][j] > occorrenze) {

                        vincolata = 1;
                        break;
                    }
                    if (vincolata == 1)
                        break;
                    j++;
                }
                if (vincolata == 1) {
                    conta_filtrate[0] = conta_filtrate[0] - 1;
                    bt->vincolata = 1;
                }
                if(bt->right!=NULL){
                    if(bt->right->vincolata == 1){
                        if(bt->right->right != NULL)
                            temp = applica_vincoli_vettori_boolean(bt->right->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                        if(bt->right->left != NULL)
                            temp = applica_vincoli_vettori_boolean(bt->right->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    }
                    else {
                        temp = applica_vincoli_vettori_boolean(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    }
                }

                if(bt->left!=NULL){
                        if(bt->left->right!=NULL)
                            temp = applica_vincoli_vettori_boolean(bt->left->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                        if(bt->left->left!=NULL)
                            temp = applica_vincoli_vettori_boolean(bt->left->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    }

            }

        } else
        {
            if(bt->right!=NULL){
                if(bt->right->vincolata == 1){
                    if(bt->right->right != NULL)
                        temp = applica_vincoli_vettori_boolean(bt->right->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    if(bt->right->left != NULL)
                        temp = applica_vincoli_vettori_boolean(bt->right->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                }
                else {
                    temp = applica_vincoli_vettori_boolean(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                }
            }

            if(bt->left!=NULL){
                if(bt->left->vincolata == 1){
                    if(bt->left->right!=NULL)
                        temp = applica_vincoli_vettori_boolean(bt->left->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                    if(bt->left->left!=NULL)
                        temp = applica_vincoli_vettori_boolean(bt->left->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                }
                else {
                    temp = applica_vincoli_vettori_boolean(bt->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                }
            }
        }
    }

    return temp;
}

unsigned int applica_vincoli_vettori_single_word(char* input, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64]) {
    unsigned int i;
    for (i = 0; i < dim - 1; i++) {
        if (vincoli[i] != 0 && vincoli[i] !=input[i]) {
            return 1;
        }
        if(non_presente[ritorna_posizione(input[i])] == 1)
            return 1;
        unsigned int j = 0;
        while (presente_n[0][j] != -1) {
            unsigned int m = 0, occorrenze = 0;
            for (m = 0; m < dim - 1; m++) {
                if (presente_n[0][j] == input[m])
                    occorrenze++;
            }
            if (presente_n[2][j] != -1 && presente_n[2][j] != occorrenze) {
                return 1;
            }

            if (presente_n[1][j] != -1 && presente_n[1][j] > occorrenze) {
                return 1;
            }
            j++;
        }
        j = 0;

        if(presente_pos_sbagliata[i][ritorna_posizione(input[i])] == 1) {
            return 1;
        }
        j++;
    }
    return 0;
}

void ripristina_vincoli_vettori( int presente_n[][64],int dim,int presente_pos_sbagliata[][64],int non_presente[]) {
    unsigned int m,n;

    for(n=0;n<64;n++){
        presente_n[0][n] = -1;
        presente_n[1][n] = -1;
        presente_n[2][n] = -1;
    }
    for(m=0;m<dim;m++){
        for(n=0;n<64;n++){
            presente_pos_sbagliata[m][n] = -1;
        }

    }
    for(m=0;m<320;m++) {
        non_presente[m] = -1;
    }

}

int main() {
    unsigned int dim=0, err, tent, prima_attraversata = 0;
    node * bt = NULL;
    int c;
    clock_t t;
    double time_taken;
    t = clock();
    long start_t, end_t;
    //long start_dict_t, end_dict_t;

    err = fscanf(stdin, "%d\n", &dim);

    // Tempo di inizio
    start_t = get_nanos();

    char * fin ;
    dim++;
    char *par_let = malloc(18*sizeof(char));

    int non_presente[320];
    int presente_n[3][64];
    int presente_pos_sbagliata[dim][64];
    int conta_dizionario = 0;
    int conta_filtrate[1];

    // Tempo di inizio dizionario
    //start_dict_t = get_nanos();
    char *p = malloc(dim*sizeof(char));
    while(1) {
        fin = fgets(p,dim,stdin);
        c = getc_unlocked(stdin);

        if(p[0] == '+') {
            //while(getc_unlocked(stdin)!='\n'){}
            if(p[1] == 'i'){
                while (1) {
                    err = fscanf(stdin, "%s\n", par_let);
                    if(par_let[0] == '+') break;
                    if(par_let[0] != '\n' || strlen(par_let)==dim){
                        bt = BST_insert_iterative(bt, par_let, dim,0);
                        conta_dizionario++;
                    }

                }
                fin = fgets(p, dim, stdin);
            }
            if(p[1] == 'n'){
                while (getc_unlocked(stdin) != '\n') {}
                break;
            }
            if(p[1] == '+'){
                MorrisTraversal(bt,dim);
            }
        }
        else {
            bt = BST_insert_iterative(bt,p,dim,0);
            conta_dizionario ++;
        }
    }

    //end_dict_t = get_nanos() - start_dict_t;

    while(1) {

        fin = fgets(p, dim, stdin);

        if(p[0] == '+') {
            if (p[1] == 'i') {
                while (getc_unlocked(stdin) != '\n');
                //fin = fgets(p, dim, stdin);
                while (1) {
                    err = fscanf(stdin, "%s\n", par_let);
                    //fin = fgets(par_let,18,stdin);
                    if(par_let[0] == '+') break;
                    bt = BST_insert_iterative(bt, par_let, dim,0);
                    conta_dizionario++;
                }
                fin = fgets(p, dim, stdin);
            }
            if (p[1] == 'n') {
                while (getc_unlocked(stdin) != '\n') {}
                fin = fgets(p, dim, stdin);
            }
        }

        c = getc_unlocked(stdin);
        if(c==EOF) {
            break;
        }
        else {
            err = fscanf(stdin, "%d\n", &tent);
            conta_filtrate[0] = conta_dizionario;
            ripristina_vincoli_vettori(presente_n,dim,presente_pos_sbagliata,non_presente);

            char * par_vincoli = calloc(dim,sizeof(char));
            char * par_read = malloc(sizeof(char)*18);
            while (tent > 0) {
                err = fscanf(stdin, "%s\n", par_let);
                if (par_let[0] == '+') {
                    if (par_let[1] == 's') {
                        MorrisTraversal(bt,dim);
                    }
                    else {
                        if (par_let[1] == 'i') {

                            while(1) {
                                err = fscanf(stdin,"%s\n",par_read);
                                if(par_read[0] == '+') break;
                                int da_vincolare = applica_vincoli_vettori_single_word(par_read,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                                if(da_vincolare==0) {
                                    conta_filtrate[0] ++;
                                }
                                bt = BST_insert_iterative(bt,par_read,dim,da_vincolare);
                                conta_dizionario ++;
                            }
                        }
                    }

                } else {
                    if (ricerca_parola_2(bt, par_let,dim) == NULL)
                    {
                        printf("not_exists\n");
                    }

                    else {
                        if (strcmp_home_made(p, par_let) == 0) {
                            printf("ok\n");
                            tent=0;
                        } else {
                            word_checker_3(p, par_let,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                            if(conta_filtrate[0]!=1){

                                bt = applica_vincoli_vettori_boolean(bt,bt,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                            }
                            printf("%d\n",conta_filtrate[0]);
                            tent--;
                            if(tent == 0) {
                                printf("ko\n");
                            }
                        }
                    }

                }
            }
            free(par_read);
            riprisitna_albero(bt);
            prima_attraversata = 0;
            free(par_vincoli);
        }


    }
    if(err == 10201)
        printf("ciao\n");
    if(fin != NULL)
        prima_attraversata++;
    deleteBinaryTree(bt);
    free(p);
    free(par_let);


    // tempo fine
    end_t = get_nanos();

    //printf("Tempo di caricamento dizionario (ms): %.2lf\n", end_dict_t / (float)1000000);
    printf("Tempo di esecuzione totale (ms): %.2lf\n", (end_t - start_t) / (float)1000000);

    return 0;
}
