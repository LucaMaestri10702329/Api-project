#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _node {

    struct _node *left;
    struct _node *right;
    char key[];
} node;


node *create_node(char *input, int dim) {
    node *n = malloc(sizeof(node)+sizeof(char)*dim);
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

node *BST_insert_iterative(node *root, char *input, int dim){
    node *temp = root;
    int condition = 1;

    if (root == NULL)
        return create_node(input,dim);

    while (condition) {
        if (strcmp_home_made(input,temp->key)>0) {
            if (temp->right == NULL) {
                temp->right = create_node(input,dim);
                condition = 0;
            }
            else
                temp = temp->right;
        }
        else {
            if (temp->left == NULL) {
                temp->left = create_node(input,dim);
                condition = 0;
            }
            else
                temp = temp->left;
        }
    }
    return root;
}

node* ricerca_parola_2 (node *bt, char * info,int dim) {
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

void stampa_albero(node* tree) {
    if(tree != NULL) {
        stampa_albero(tree->left);
        printf("%s\n", tree->key);
        stampa_albero(tree->right);
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

void word_checker_3(char *rif, char *word,node * bt,unsigned int dim, int stampa, char vincoli[], int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][320]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int check;
    unsigned int i;

    memmove(temp,word,dim);
    memmove(temp_rif, rif,dim);
    memmove(output,word,dim);

    for( i=0;i<dim-1;i++) {
        if (rif[i] == word[i]) {
            temp[i] = '+';
            output[i] = '+';
            temp_rif[i] = '+';
        } else
            output[i] = '/';
    }
    for(i=0;i<dim-1;i++) {
        if(rif[i] != word[i]) {
            output[i] = '/';
            check = 0;
            unsigned j;
            for(j=0;j<dim-1;j++) {
                if(temp[i]==temp_rif[j]) {
                    check = 1;
                    temp_rif[j] = '*';
                    break;
                }
            }
            if(check == 1) {
                output[i] = '|';
            }
        }
        else {
            output[i] = '+';
            temp[i] = '+';
            temp_rif[i] = '+';
        }
    }
    if(stampa)
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
                m = 0;
                while(presente_pos_sbagliata[0][m]!=-1)
                    m++;
                presente_pos_sbagliata[0][m] = word[i];
                presente_pos_sbagliata[1][m] = i;
            }
            else {
                unsigned int k, pres_rif = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == rif[k])
                        pres_rif++;
                }
                if(pres_rif == 0) {
                    unsigned int u=0;
                    while(non_presente[u]!=-1)
                        u++;
                    non_presente[u] = word[i];
                }

                else {

                    unsigned int m = 0;
                    while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i]) {
                        m++;
                    }
                    presente_n[0][m] = word[i];
                    presente_n[2][m] = pres_rif;

                    m = 0;
                    while(presente_pos_sbagliata[0][m]!=-1)
                        m++;
                    presente_pos_sbagliata[0][m] = word[i];
                    presente_pos_sbagliata[1][m] = i;
                }
            }
        }
    }
    free(temp);
    free(temp_rif);
    free(output);
}

node * applica_vincoli_vettori(node * bt, node * temp, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][320], int  conta_filtrate[]) {
    if(bt != NULL) {
        unsigned int i, vincolata = 0;
        for (i = 0; i < dim - 1; i++) {
            if(vincoli[i]!=0 && vincoli[i]!=bt->key[i]) {

                vincolata = 1;
                break;
            }
            unsigned int o = 0;
            while(non_presente[o]!=-1) {
                unsigned int p;
                for(p=0;p<dim-1;p++) {
                    if(bt->key[p]==non_presente[o]) {

                        vincolata = 1;
                        break;
                    }
                }
                o++;
            }
            if(vincolata == 1)
                break;

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
                j++;
            }
            if(vincolata == 1)
                break;
            j = 0;
            while(presente_pos_sbagliata[0][j]!=-1) {
                if(presente_pos_sbagliata[0][j] == bt->key[presente_pos_sbagliata[1][j]]) {

                    vincolata = 1;
                    break;
                }
                j++;
            }
            if(vincolata == 1)
                break;
        }
        if(vincolata == 1) {
            conta_filtrate[0] = conta_filtrate[0] - 1;
        }
        else
            temp = BST_insert_iterative(temp,bt->key,dim);
    }
    if(bt->right!=NULL)
        temp = applica_vincoli_vettori(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
    if(bt->left!=NULL)
        temp = applica_vincoli_vettori(bt->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);

    return temp;
}

unsigned int applica_vincoli_vettori_single_word(char* input, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][320]) {
    unsigned int i;
    for (i = 0; i < dim - 1; i++) {
        if (vincoli[i] != 0 && vincoli[i] !=input[i]) {
            return 1;
        }
        unsigned int o = 0;
        while (non_presente[o] != -1) {
            unsigned int p;
            for (p = 0; p < dim - 1; p++) {
                if (input[p] == non_presente[o]) {
                    return 1;
                }
            }
            o++;
        }
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
        while (presente_pos_sbagliata[0][j] != -1) {
            if (presente_pos_sbagliata[0][j] == input[presente_pos_sbagliata[1][j]]) {
                return 1;
            }
            j++;
        }
    }
    return 0;
}

void ripristina_vincoli_vettori(char par_vincoli[], int presente_n[][64],int dim,int presente_pos_sbagliata[][320],int non_presente[]) {
    unsigned int m,n;
    for(m=0;m<dim;m++)
        par_vincoli[m] = 0;
    for(m=0;m<3;m++) {
        for(n=0;n<64;n++)
            presente_n[m][n] = -1;
    }
    for(m=0;m<2;m++) {
        for(n=0;n<320;n++)
            presente_pos_sbagliata[m][n] = -1;
    }
    for(m=0;m<320;m++) {
        non_presente[m] = -1;
    }

}

int main() {
    unsigned int dim=0, err, tent, prima_attraversata = 0;
    node * bt = NULL;
    node *filtrate;
    node * temp;
    char c;
    clock_t t;
    double time_taken;
    t = clock();
    err=fscanf(stdin, "%d\n", &dim);

    char * fin ;
    dim++;
    char *par_confronto = malloc((dim)*sizeof(char));
    char *par_let = malloc(18*sizeof(char));

    int non_presente[320];
    char *par_vincoli = malloc(sizeof(char)*dim); //posizione_corretta
    int presente_n[3][64];
    int presente_pos_sbagliata[2][320];
    int conta_dizionario = 0;
    int conta_filtrate[1];


    char *p = (char*) malloc(dim*sizeof(char));
    while(1) {
        fin = fgets(p,dim,stdin);
        c = getc_unlocked(stdin);

        if(p[0] == '+') {
            while(getc_unlocked(stdin)!='\n'){}
            break;
        }
        else {
            bt = BST_insert_iterative(bt,p,dim);
            conta_dizionario ++;
        }
    }
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Tempo per l'inserimento %f\n",time_taken);
    while(1) {

        fin = fgets(p, dim, stdin);


        if(p[0] == '+' && p[1]=='i') {
            while(getc_unlocked(stdin)!='\n');
            fin = fgets(p, dim, stdin);
            while(1) {

                bt = BST_insert_iterative(bt,p,dim);
                conta_dizionario ++;
                err = fscanf(stdin,"%s\n",par_let);
                if(par_let[0] != '+')
                    memmove(p,par_let,dim);
                else {
                    break;
                }
            }
            fin = fgets(p, dim, stdin);
        }
        if(p[0]=='+' && p[1]=='n') {
            while(getc_unlocked(stdin)!='\n'){}
            fin = fgets(p, dim, stdin);
        }

        c = getc_unlocked(stdin);
        if(c==EOF) {
            break;
        }
        else {
            err = fscanf(stdin, "%d\n", &tent);
            conta_filtrate[0] = conta_dizionario;
            filtrate = NULL;
            ripristina_vincoli_vettori(par_vincoli,presente_n,dim,presente_pos_sbagliata,non_presente);
            t = clock() - t;
            time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
            printf("Tempo per iniziare una partita %f\n",time_taken);
            while (tent > 0) {
                temp = NULL;
                err = fscanf(stdin, "%s\n", par_let);
                if (par_let[0] == '+') {
                    if (par_let[1] == 's') {
                        if(prima_attraversata==0)
                            stampa_albero(bt);
                        else
                            stampa_albero(filtrate);
                    }
                    else {
                        if (par_let[1] == 'i') {
                            char * par_read;

                            par_read = malloc(sizeof(char)*18);


                            fin = fgets(par_let,dim,stdin);
                            while(par_let[0] != '+') {

                                if(applica_vincoli_vettori_single_word(par_let,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata)==0) {

                                    if(prima_attraversata != 0)
                                        filtrate = BST_insert_iterative(filtrate,par_let,dim);
                                    conta_filtrate[0] ++;
                                }
                                bt = BST_insert_iterative(bt,par_let,dim);
                                conta_dizionario ++;

                                err = fscanf(stdin,"%s\n",par_read);
                                if(par_read[0]!='+')
                                    memmove(par_let,par_read,dim);
                                else {
                                    par_let[0] = '+';
                                }
                            }
                            free(par_read);
                        }
                    }

                } else {
                    memmove(par_confronto,par_let,dim);
                    if (ricerca_parola_2(bt, par_confronto,dim) == NULL)
                    {
                        printf("not_exists\n");
                    }

                    else {
                        if (strcmp_home_made(p, par_confronto) == 0) {
                            printf("ok\n");
                            tent=0;
                        } else {
                            t = clock() - t;
                            time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
                            printf("Tempo per applicare i vincoli inizio %f\n",time_taken);
                            word_checker_3(p, par_confronto,bt,dim,1,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                            if(prima_attraversata == 0){
                                filtrate = applica_vincoli_vettori(bt,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                prima_attraversata++;
                            }else{
                                temp = applica_vincoli_vettori(filtrate,NULL,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                deleteBinaryTree(filtrate);
                                filtrate = temp;
                            }
                            t = clock() - t;
                            time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
                            printf("Tempo per applicare i vincoli fine %f\n",time_taken);
                            printf("%d\n",conta_filtrate[0]);
                            tent--;
                            if(tent == 0) {
                                printf("ko\n");
                            }
                        }
                    }

                }
            }
            t = clock() - t;
            time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
            printf("Tempo per finire la partita %f\n",time_taken);
            deleteBinaryTree(filtrate);
            prima_attraversata= 0;
        }


    }
    if(err == 10201)
        printf("ciao\n");
    if(fin != NULL)
        printf("BOh");
    deleteBinaryTree(bt);
    free(p);
    free(par_let);
    free(par_confronto);
    free(par_vincoli);
    return 0;
}