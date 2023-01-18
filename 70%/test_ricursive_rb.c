#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _node {
    // data
    int c; // 1-red, 0-black
    int vincolata;
    struct _node *p; // parent
    struct _node *right; // right-child
    struct _node *left; // left child
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
    } else if(c > 45 && c < 65)
    {
        c = c - 47;
    } else if(c>64 && c<91)
    {
        c = c - 54;
    } else if(c == '_')
    {
        c = c - 58;
    } else
        c = c - 59;
    return c;

}

struct _node * create_node(char *input,int dim,int vincolata){
    struct _node * new = malloc(sizeof(struct _node)+sizeof(char)*dim);
    memmove(new->key,input,dim);
    new->p = NULL;
    new->left = NULL;
    new->right = NULL;
    new-> c = 1;
    new->vincolata = vincolata;
    return new;
}
void rightrotate(struct _node * root,struct _node* temp)
{
    struct _node* left = temp->left;
    temp->left = left->right;
    if (temp->left)
        temp->left->p = temp;
    left->p = temp->p;
    if (!temp->p)
        root = left;
    else if (temp == temp->p->left)
        temp->p->left = left;
    else
        temp->p->right = left;
    left->right = temp;
    temp->p = left;
}
void leftrotate(struct _node * root,struct _node* temp)
{
    struct _node* right = temp->right;
    temp->right = right->left;
    if (temp->right)
        temp->right->p = temp;
    right->p = temp->p;
    if (!temp->p)
        root = right;
    else if (temp == temp->p->left)
        temp->p->left = right;
    else
        temp->p->right = right;
    right->left = temp;
    temp->p = right;
}
void fixup(struct _node* root, struct _node* pt)
{
    struct _node* parent_pt = NULL;
    struct _node* grand_parent_pt = NULL;

    while ((pt != root) && (pt->c != 0)
           && (pt->p->c == 1))
    {
        parent_pt = pt->p;
        grand_parent_pt = pt->p->p;

        /*  Case : A
             Parent of pt is left child
             of Grand-parent of
           pt */
        if (parent_pt == grand_parent_pt->left)
        {

            struct _node* uncle_pt = grand_parent_pt->right;

            /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->c == 1)
            {
                grand_parent_pt->c = 1;
                parent_pt->c = 0;
                uncle_pt->c = 0;
                pt = grand_parent_pt;
            }

            else {

                /* Case : 2
                     pt is right child of its parent
                     Left-rotation required */
                if (pt == parent_pt->right) {
                    leftrotate(root,parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }

                /* Case : 3
                     pt is left child of its parent
                     Right-rotation required */
                rightrotate(root,grand_parent_pt);
                int t = parent_pt->c;
                parent_pt->c = grand_parent_pt->c;
                grand_parent_pt->c = t;
                pt = parent_pt;
            }
        }

            /* Case : B
                 Parent of pt is right
                 child of Grand-parent of
               pt */
        else {
            struct _node* uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->c == 1))
            {
                grand_parent_pt->c = 1;
                parent_pt->c = 0;
                uncle_pt->c = 0;
                pt = grand_parent_pt;
            }
            else {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left) {
                    rightrotate(root,parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }

                /* Case : 3
                     pt is right child of its parent
                     Left-rotation required */
                leftrotate(root,grand_parent_pt);
                int t = parent_pt->c;
                parent_pt->c = grand_parent_pt->c;
                grand_parent_pt->c = t;
                pt = parent_pt;
            }
        }
    }

    root->c = 0;
}

int strcmp_home_made(const char * s1, const char*s2){
    while(*s1 && (*s1 == *s2)){
        s1++, s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void stampa_albero(struct _node *root){
    if(root!=NULL){
        if(root->left!=NULL){
            stampa_albero(root->left);
        }
        if(root->vincolata == 0)
        {
            puts(root->key);
        }
        if(root->right!=NULL){
            stampa_albero(root->right);
        }

    }
}

struct _node *BST_insert_iterative(struct _node *root, char *input, int dim, int vincolata){
    struct _node *temp = root;
    int condition = 1;
    struct _node * new = create_node(input,dim,vincolata);
    if (root == NULL)
        return new;

    while (condition) {
        if (strcmp_home_made(input,temp->key)>0) {
            if (temp->right == NULL) {
                temp->right = new;
                temp->right->p = temp;
                condition = 0;
            }
            else
                temp = temp->right;
        }
        else {
            if (temp->left == NULL) {
                temp->left = new;
                temp->left->p = temp;
                condition = 0;
            }
            else
                temp = temp->left;
        }
    }
    if(root->p!=NULL)
        if(root->p->p!=NULL)
            fixup(root,new);
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

void ripristina_albero(node *tree){
    if(tree!= NULL){
        if(tree->right!=NULL)
            ripristina_albero(tree->right);
        tree->vincolata=0;
        if(tree->left!=NULL)
            ripristina_albero(tree->left);
    }
}

node* deleteBinaryTree(node* bt) {
    if (bt == NULL) {
        return NULL;
    }
    if(bt->left!=NULL)
        deleteBinaryTree(bt->left);
    if(bt->right!=NULL)
        deleteBinaryTree(bt->right);
    free(bt);
    return NULL;
}

void word_checker_3(char *rif, char *word,unsigned int dim, char *vincoli, int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][64]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int i;
    unsigned int j;

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
    unsigned int k, pres_word = 0,pres_rif=0;
    unsigned int m = 0;
    for(i=0;i<dim-1;i++) {
        if(output[i]=='+')
            vincoli[i] = word[i];
        else {
            if(output[i]=='|') {
                pres_word = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == word[k] && (output[k]!='/'))
                        pres_word ++;
                }
                m = 0;
                while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i])
                    m++;
                presente_n[0][m] = word[i];
                presente_n[1][m] = pres_word;

                presente_pos_sbagliata[i][ritorna_posizione(word[i])] = 1;
            }
            else {
                pres_rif = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == rif[k])
                        pres_rif++;
                }
                if(pres_rif == 0) {
                    non_presente[ritorna_posizione(word[i])] = 1;
                }

                else {
                    m = 0;
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

node * applica_vincoli_vettori(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64], int  conta_filtrate[]) {
    if(bt != NULL) {
        if(bt->vincolata==0) {
            unsigned int i, vincolata = 0;
            unsigned int pos;
            for (i = 0; i < dim - 1; i++) {
                pos = ritorna_posizione(bt->key[i]);
                if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                    vincolata = 1;
                    break;
                }
                if (non_presente[pos] == 1)
                    vincolata = 1;
                if (vincolata == 1)
                    break;
                if (presente_pos_sbagliata[i][pos] == 1) {
                    vincolata = 1;
                    break;
                }
                if (vincolata == 1)
                    break;
            }
            unsigned int j = 0;
            unsigned int m = 0, occorrenze = 0;
            while (presente_n[0][j] != -1) {
                occorrenze = 0;
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
                j++;
            }
            if (vincolata == 1) {
                conta_filtrate[0]--;
            } else
                temp = BST_insert_iterative(temp, bt->key, dim, 0);
        }

        if (bt->right != NULL){
            if(bt->right->vincolata==0)
                temp = applica_vincoli_vettori(bt->right, temp, dim, vincoli, non_presente, presente_n, presente_pos_sbagliata, conta_filtrate);
            else{
                if(bt->right->right!=NULL)
                    temp = applica_vincoli_vettori(bt->right->right, temp, dim, vincoli, non_presente, presente_n, presente_pos_sbagliata, conta_filtrate);
                if(bt->right->left!=NULL)
                    temp = applica_vincoli_vettori(bt->right->left, temp, dim, vincoli, non_presente, presente_n, presente_pos_sbagliata, conta_filtrate);
            }
        }
        if (bt->left != NULL){
            if(bt->left->vincolata==0)
                temp = applica_vincoli_vettori(bt->left, temp, dim, vincoli, non_presente, presente_n,
                                               presente_pos_sbagliata, conta_filtrate);
            else{
                if(bt->left->right!=NULL)
                    temp = applica_vincoli_vettori(bt->left->right, temp, dim, vincoli, non_presente, presente_n, presente_pos_sbagliata, conta_filtrate);
                if(bt->left->left!=NULL)
                    temp = applica_vincoli_vettori(bt->left->left, temp, dim, vincoli, non_presente, presente_n, presente_pos_sbagliata, conta_filtrate);
            }
        }

    }
    return temp;
}

unsigned int applica_vincoli_vettori_single_word(char* input, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64]) {
    unsigned int i;
    unsigned int j = 0;
    unsigned int m = 0, occorrenze = 0;
    unsigned int pos;
    for (i = 0; i < dim - 1; i++) {
        pos = ritorna_posizione(input[i]);
        if (vincoli[i] != 0 && vincoli[i] !=input[i]) {
            return 1;
        }
        if(non_presente[pos] == 1)
            return 1;
        if(presente_pos_sbagliata[i][pos] == 1) {
            return 1;
        }
        j = 0;
        while (presente_n[0][j] != -1) {
            occorrenze = 0;
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

node * applica_vincoli_vettori_boolean(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64], int  conta_filtrate[]) {
    if(bt != NULL) {
        if(bt->vincolata==0) {
            if(bt->left!=NULL){
                if(bt->left->vincolata==0){
                    unsigned int i, vincolata = 0, vincolata_left = 0;
                    unsigned int pos;
                    unsigned int pos_left;
                    for (i = 0; i < dim - 1; i++) {
                        pos = ritorna_posizione(bt->key[i]);
                        pos_left = ritorna_posizione(bt->left->key[i]);
                        if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                            vincolata = 1;
                        }
                        if(vincoli[i] != 0 && vincoli[i] != bt->left->key[i])
                            vincolata_left = 1;

                        if (non_presente[pos] == 1)
                            vincolata = 1;
                        if (non_presente[pos_left] == 1)
                            vincolata_left = 1;

                        if (presente_pos_sbagliata[i][pos] == 1) {
                            vincolata = 1;
                        }
                        if (presente_pos_sbagliata[i][pos_left] == 1) {
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
                    unsigned int pos;
                    for (i = 0; i < dim - 1; i++) {
                        pos = ritorna_posizione(bt->key[i]);
                        if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                            vincolata = 1;
                            break;
                        }
                        if (non_presente[pos] == 1)
                            vincolata = 1;
                        if (vincolata == 1)
                            break;

                        if (presente_pos_sbagliata[i][pos] == 1) {
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
                unsigned int pos;
                for (i = 0; i < dim - 1; i++) {
                    pos = ritorna_posizione(bt->key[i]);
                    if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                        vincolata = 1;
                        break;
                    }
                    if (non_presente[pos] == 1)
                        vincolata = 1;
                    if (presente_pos_sbagliata[i][pos] == 1) {
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

int main() {
    unsigned int dim=0, err, tent, prima_attraversata = 0;
    node * bt = NULL;
    node * filtrate;
    node * temp;
    int c;
    size_t size;
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
    char *par_let = malloc(19*sizeof(char));

    int non_presente[320];
    int presente_n[3][64];
    int presente_pos_sbagliata[dim][64];
    int conta_dizionario = 0;
    int conta_filtrate[1];
    int da_vincolare;

    // Tempo di inizio dizionario
    //start_dict_t = get_nanos();
    char *p = malloc((dim)*sizeof(char));
    char * par_read = malloc(sizeof(char)*19);
    while(1) {
        fin = fgets(p,dim,stdin);
        c = getc_unlocked(stdin);

        if(p[0] == '+') {
            if(p[1] == 'i'){
                while (1) {
                    getdelim(&par_let,&size,'\n',stdin);
                    if(par_let[0] == '+') break;
                    if(par_let[0] != '\n' || strlen(par_let)==dim){
                        bt = BST_insert_iterative(bt, par_let, dim,0);
                        conta_dizionario++;
                    }

                }
                getdelim(&p,&size,'\n',stdin);
            }
            if(p[1] == 'n'){
                while (getc_unlocked(stdin) != '\n') {}
                break;
            }
            if(p[1] == '+'){
                //MorrisTraversal(bt,dim);
                stampa_albero(bt);
            }
        }
        else {
            bt = BST_insert_iterative(bt,p,dim,0);
            conta_dizionario ++;
        }
    }


    while(1) {

        fin = fgets(p, dim, stdin);

        if(p[0] == '+') {
            if (p[1] == 'i') {
                while (getc_unlocked(stdin) != '\n');
                while (1) {
                    getdelim(&par_let,&size,'\n',stdin);
                    if(par_let[0] == '+') break;
                    par_let[dim-1] = '\0';
                    bt = BST_insert_iterative(bt, par_let, dim,0);
                    conta_dizionario++;
                }
                fin = fgets(p, dim, stdin);
            }
            if (p[1] == 'n') {
                while (getc_unlocked(stdin) != '\n') {}
                fin = fgets(p, dim, stdin);
                //getdelim(&p,&size,'\n',stdin);

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

            while (tent > 0) {
                temp = NULL;
                getdelim(&par_let,&size,'\n',stdin);
                par_let[dim-1] = '\0';


                if (par_let[0] == '+') {
                    if (par_let[1] == 's') {
                        if(prima_attraversata == 0)
                            //MorrisTraversal(bt,dim);
                            stampa_albero(bt);
                        else
                            //MorrisTraversal(filtrate,dim);
                            stampa_albero(filtrate);
                    }
                    else {
                        if (par_let[1] == 'i') {
                            while(1) {
                                err = fscanf(stdin,"%s\n",par_read);
                                //getdelim(&par_read,&size,'\n',stdin);
                                //par_read[dim-1] = '\0';
                                if(par_read[0] == '+') break;
                                da_vincolare = applica_vincoli_vettori_single_word(par_read,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                                if(da_vincolare==0) {
                                    conta_filtrate[0] ++;
                                    if(prima_attraversata!=0)
                                        filtrate = BST_insert_iterative(filtrate,par_read,dim,da_vincolare);
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
                                if(conta_filtrate[0]>2000)
                                    bt = applica_vincoli_vettori_boolean(bt,bt,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                else {
                                    if(prima_attraversata == 0){
                                        filtrate = applica_vincoli_vettori(bt,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                        prima_attraversata++;
                                    }else{
                                        if(conta_filtrate[0]>50){
                                            temp = applica_vincoli_vettori(filtrate,NULL,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                            deleteBinaryTree(filtrate);
                                            filtrate = temp;
                                        } else {
                                            filtrate = applica_vincoli_vettori_boolean(filtrate,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                        }

                                    }
                                }
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

            ripristina_albero(bt);
            prima_attraversata = 0;
            free(par_vincoli);
            deleteBinaryTree(filtrate);
        }


    }
    if(err == 10201)
        printf("ciao\n");
    if(fin != NULL)
        prima_attraversata++;
    deleteBinaryTree(bt);
    free(p);
    free(par_let);
    free(par_read);

    // tempo fine
    end_t = get_nanos();

    //printf("Tempo di caricamento dizionario (ms): %.2lf\n", end_dict_t / (float)1000000);
    printf("Tempo di esecuzione totale (ms): %.2lf\n", (end_t - start_t) / (float)1000000);

    return 0;
}
