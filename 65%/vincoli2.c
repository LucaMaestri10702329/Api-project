#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _node {

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
void MorrisTraversal(struct _node * root)
{
    struct _node* current, *pre;

    if (root == NULL)
        return;

    current = root;
    while (current != NULL) {

        if (current->left == NULL) {
            printf("%s\n", current->key);
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
                printf("%s\n", current->key);
                current = current->right;
            } /* End of if condition pre->right == NULL */
        } /* End of if condition current->left == NULL*/
    } /* End of while */
}
node *BST_insert_iterative(node *root, char *input, unsigned int dim){
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
                    if(bt->left == NULL)
                        return NULL;
                    return  ricerca_parola_2(bt->left,info,dim);
                }
                else{
                    if(bt->right == NULL)
                        return NULL;
                    return ricerca_parola_2(bt->right,info,dim);
                }

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

void word_checker_3(char *rif, char *word,unsigned int dim, char *vincoli, int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][64]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int i;

    memmove(temp,word,dim);
    memmove(temp_rif, rif,dim);
    i = 0;
    while(i<dim-1){
        if (rif[i] == word[i]) {
            temp[i] = '+';
            output[i] = '+';
            temp_rif[i] = '+';
            vincoli[i] = word[i];
        } else
            output[i] = '/';
        i++;
    }
    i=0;
    while(i<dim-1) {
        if(output[i]=='/') {
            unsigned int j=0;
            while(j<dim-1){
                if(temp[i]==temp_rif[j]) {
                    output[i] = '|';
                    temp_rif[j] = '*';
                    break;
                }
                j++;
            }
        }
        i++;
    }
    output[dim-1] = '\0';
    printf("%s\n", output);

    unsigned int k, pres_word = 0, pres_rif = 0,m;
    i = 0;
    while(i<dim-1) {
        if(output[i]!='+') {
            pres_word = 0;
            pres_rif = 0;
            k = 0;
            while(k<dim-1){
                if (word[i] == word[k] && (output[k] != '/'))
                    pres_word++;
                if (word[i] == rif[k])
                    pres_rif++;
                k++;
            }
            if(pres_rif == 0){
                non_presente[ritorna_posizione(word[i])] = 1;
            } else{
                presente_pos_sbagliata[i][ritorna_posizione(word[i])] = 1;
                m = 0;
                while (presente_n[0][m] != -1 && presente_n[0][m] != word[i])
                    m++;
                presente_n[0][m] = word[i];
                if(output[i] == '|'){
                    presente_n[1][m] = pres_word;
                } else {
                    presente_n[2][m] = pres_rif;
                }
            }
        }
        i++;
    }
    free(temp);
    free(temp_rif);
    free(output);
}

node * applica_vincoli_vettori(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64], int  conta_filtrate[]) {
    if(bt != NULL) {
        unsigned int i, vincolata = 0, j;
        i=0;
        while(i<dim-1) {
            if (non_presente[ritorna_posizione(bt->key[i])]==1) {
                vincolata = 1;
                break;
            }
            i++;
        }
        i = 0;
        if(vincolata != 1)
        while(i<dim-1){
            if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                vincolata = 1;
                break;
            }
            i++;
        }
        i = 0;
        if(vincolata!=1)
            while(i<dim-1){
                if(presente_pos_sbagliata[i][ritorna_posizione(bt->key[i])] == 1){
                    vincolata = 1;
                    break;
                }
                i++;
            }
        if(vincolata != 1){
                j=0;
                while(presente_n[0][j]!=-1) {
                    unsigned int m = 0, occorrenze = 0;
                    for(m=0;m<dim-1;m++) {
                        if(presente_n[0][j]==bt->key[m])
                            occorrenze ++;
                    }
                    if((presente_n[2][j]!=-1 && presente_n[2][j]!=occorrenze) || (presente_n[1][j]!=-1 && presente_n[1][j]>occorrenze)) {
                        vincolata = 1;
                        break;
                    }
                    j++;
                }
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

unsigned int applica_vincoli_vettori_single_word(char* input, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64]) {
    unsigned int i, m, occorrenze,j;
    i = 0;
    while(i<dim-1) {
        if (vincoli[i] != 0 && vincoli[i] !=input[i])
            return 1;

        if(non_presente[ritorna_posizione(input[i])] == 1 || presente_pos_sbagliata[i][ritorna_posizione(input[i])] == 1)
            return 1;

        j = 0;
        while (presente_n[0][j] != -1) {
            m = 0, occorrenze = 0;
            for (m = 0; m < dim - 1; m++) {
                if (presente_n[0][j] == input[m])
                    occorrenze++;
            }
            if ((presente_n[2][j]!=-1 && presente_n[2][j]!=occorrenze) || (presente_n[1][j]!=-1 && presente_n[1][j]>occorrenze)) {
                return 1;
            }
            j++;
        }
        i++;
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
    node *filtrate;
    node * temp;
    int c;
    

    err = fscanf(stdin, "%d\n", &dim);

    // Tempo di inizio
   

    char * fin ;
    dim++;
    char *par_let = malloc(18*sizeof(char));

    int non_presente[320];
    int presente_n[3][64];
    int presente_pos_sbagliata[dim][64];
    int conta_dizionario = 0;
    int conta_filtrate[1];

    //Tempo di inizio dizionario
  
    char *p = malloc(dim*sizeof(char));
    while(1) {
        fin = fgets(p,dim,stdin);
        c = getc_unlocked(stdin);

        if(p[0] == '+') {
            while(getc_unlocked(stdin)!='\n'){}
            if(p[1] == 'i') {
                while (1) {
                    //err = fscanf(stdin, "%s\n", par_let);
                    fin = fgets(par_let,dim,stdin);
                    c = getc_unlocked(stdin);
                    if(par_let[0] == '+')
                    {
                        while(getc_unlocked(stdin)!='\n');
                        break;
                    }
                    if(par_let[0] != '\n') {
                        bt = BST_insert_iterative(bt, par_let, dim);
                        conta_dizionario++;
                    }

                }
                fin = fgets(p, dim, stdin);
                while (getc_unlocked(stdin) != '\n') {}
                if(p[0]!='+'){
                    bt = BST_insert_iterative(bt, p, dim);
                    conta_dizionario++;
                }

            }
            if(p[1] == 'n') break;
        }
        else {
            bt = BST_insert_iterative(bt,p,dim);
            conta_dizionario ++;
        }
    }
  

    while(1) {
     
        fin = fgets(p, dim, stdin);

        if(p[0] == '+') {
            if (p[1] == 'i') {
                while (getc_unlocked(stdin) != '\n');
                while (1) {
                    //err = fscanf(stdin, "%s\n", par_let);
                    fin = fgets(par_let,dim,stdin);
                    c = getc_unlocked(stdin);
                    if(par_let[0] == '+')
                    {
                        while(getc_unlocked(stdin)!='\n');
                        break;
                    }
                    bt = BST_insert_iterative(bt, par_let, dim);
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
            filtrate = NULL;
            ripristina_vincoli_vettori(presente_n,dim,presente_pos_sbagliata,non_presente);

            char * par_vincoli = calloc(dim,sizeof(char));
            char * par_read = malloc(sizeof(char)*18);
            while (tent > 0) {
                temp = NULL;
                err = fscanf(stdin, "%s\n", par_let);
                if (par_let[0] == '+') {
                    if (par_let[1] == 's') {
                       
                        if(prima_attraversata==0)
                            MorrisTraversal(bt);
                        else
                            MorrisTraversal(filtrate);
                   
                    }
                    else {
                       
                        if (par_let[1] == 'i') {

                            while(1) {
                                err = fscanf(stdin,"%s\n",par_read);
                                if(par_read[0] == '+') break;
                                if(applica_vincoli_vettori_single_word(par_read,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata)==0) {

                                    if(prima_attraversata != 0)
                                        filtrate = BST_insert_iterative(filtrate,par_read,dim);
                                    conta_filtrate[0] ++;
                                }
                                bt = BST_insert_iterative(bt,par_read,dim);
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
                            if(prima_attraversata == 0){
                                filtrate = applica_vincoli_vettori(bt,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                prima_attraversata++;
                            }else{
                                temp = applica_vincoli_vettori(filtrate,NULL,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                deleteBinaryTree(filtrate);
                                filtrate = temp;
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

            deleteBinaryTree(filtrate);
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
 
    return 0;
}
