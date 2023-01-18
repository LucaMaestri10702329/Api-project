#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node {
    // data
    int c; // 1-red, 0-black
    struct _node *p; // parent
    struct _node *right; // right-child
    struct _node *left; // left child
    char key[];
} node;
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
struct _node * create_node(char *input,int dim){
    struct _node * new = malloc(sizeof(struct _node)+sizeof(char)*dim);
    memcpy(new->key,input,dim);
    new->p = NULL;
    new->left = NULL;
    new->right = NULL;
    new-> c = 1;
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



struct _node *BST_insert_iterative(struct _node *root, char *input, int dim){
    struct _node *temp = root;
    int condition = 1;
    struct _node * new = create_node(input,dim);
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

struct _node* ricerca_parola_2 (struct _node *bt, char * info,int dim) {
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


void stampa_albero(struct _node* tree) {
    if(tree != NULL) {
        stampa_albero(tree->left);
        printf("%s\n", tree->key);
        stampa_albero(tree->right);
    }

}

struct _node* deleteBinaryTree(struct _node* bt) {
    if (bt == NULL) {
        return NULL;
    }
    deleteBinaryTree(bt->left);
    deleteBinaryTree(bt->right);
    free(bt);
    return NULL;
}


void word_checker_3(char *rif, char *word,unsigned int dim, char vincoli[], int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][320]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int check;
    unsigned int i;

    memcpy(temp,word,dim);
    memcpy(temp_rif, rif,dim);
    memcpy(output,word,dim);

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

                    non_presente[ritorna_posizione(word[i])] = 1;
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


struct _node * applica_vincoli_vettori(struct _node * bt, struct _node * temp, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][320], int  conta_filtrate[]) {
    if(bt != NULL) {
        unsigned int i, vincolata = 0;
        for (i = 0; i < dim - 1; i++) {
            if(vincoli[i]!='*' && vincoli[i]!=bt->key[i]) {

                vincolata = 1;
                break;
            }
            if(non_presente[ritorna_posizione(bt->key[i])] == 1)
                vincolata = 1;
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
        else {
            temp = BST_insert_iterative(temp,bt->key,dim);
        }
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
        if (vincoli[i] != '*' && vincoli[i] !=input[i]) {
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
        par_vincoli[m] = '*';
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

    struct _node * bt = NULL;
    struct _node *filtrate;
    struct _node * temp;
    char c;
    err=fscanf(stdin, "%d\n", &dim);

    char * fin ;
    dim++;

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
    char *par_confronto = malloc((dim)*sizeof(char));
    char *par_let;
    par_let = malloc(18*sizeof(char));

    while(1) {

        fin = fgets(p, dim, stdin);


        if(p[0] == '+' && p[1]=='i') {
            while(getc_unlocked(stdin)!='\n');
            fin = fgets(p, dim, stdin);
            while(p[0] != '+') {

                bt = BST_insert_iterative(bt,p,dim);
                conta_dizionario ++;
                err = fscanf(stdin,"%s\n",par_let);
                if(par_let[0] != '+')
                    memcpy(p,par_let,dim);
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
                            char * par_read;

                            par_read = malloc(sizeof(char)*18);


                            fin = fgets(par_let,dim,stdin);
                            while(par_let[0] != '+') {

                                if(applica_vincoli_vettori_single_word(par_let,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata)==0) {

                                    if(prima_attraversata != 0){
                                        filtrate = BST_insert_iterative(filtrate,par_let,dim);
                                    }

                                    conta_filtrate[0] ++;
                                }
                                bt = BST_insert_iterative(bt,par_let,dim);
                                conta_dizionario ++;

                                err = fscanf(stdin,"%s\n",par_read);
                                if(par_read[0]!='+')
                                    memcpy(par_let,par_read,dim);
                                else {
                                    par_let[0] = '+';
                                }
                            }
                            free(par_read);
                        }
                    }

                } else {
                    memcpy(par_confronto,par_let,dim);
                    if (ricerca_parola_2(bt, par_confronto,dim) == NULL)
                    {
                        printf("not_exists\n");
                    }

                    else {
                        if (strcmp_home_made(p, par_confronto) == 0) {
                            printf("ok\n");
                            tent=0;
                        } else {
                            word_checker_3(p, par_confronto,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
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
