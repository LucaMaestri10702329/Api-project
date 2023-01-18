#include<stdio.h>
#include <stdlib.h>
#include <string.h>

enum nodeColor {
    RED,
    BLACK
};

struct rbNode {
    int color, vincolata;
    char *key;
    struct rbNode *link[2];
};

struct rbNode *bt = NULL;
;

struct lista{
    char * key;
    struct lista *next;
};



struct rbNode *createNode(char * input, int dim, int vincolata) {
    struct rbNode *newnode;
    newnode = (struct rbNode *)malloc(sizeof(struct rbNode)+sizeof(char)*dim);
    strcpy(newnode->key,input);
    newnode->vincolata = vincolata;
    newnode->color = RED;
    newnode->link[0] = newnode->link[1] = NULL;
    return newnode;
}

void insertion(char *input,int dim,int vincolata) {
    struct rbNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
    int dir[98], ht = 0, index;
    ptr = bt;
    if (!bt) {
        bt = createNode(input,dim, vincolata);
        return;
    }

    stack[ht] = bt;
    dir[ht++] = 0;
    while (ptr != NULL) {
        if (strcmp(ptr->key,input)==0) {
            printf("Duplicates Not Allowed!! \n");
            return;
        }
        index = (strcmp(input,ptr->key))  > 0 ? 1 : 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    stack[ht - 1]->link[index] = newnode = createNode(input,dim,vincolata);
    while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
        if (dir[ht - 2] == 0) {
            yPtr = stack[ht - 2]->link[1];
            if (yPtr != NULL && yPtr->color == RED) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 0) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[1];
                    xPtr->link[1] = yPtr->link[0];
                    yPtr->link[0] = xPtr;
                    stack[ht - 2]->link[0] = yPtr;
                }
                xPtr = stack[ht - 2];
                xPtr->color = RED;
                yPtr->color = BLACK;
                xPtr->link[0] = yPtr->link[1];
                yPtr->link[1] = xPtr;
                if (xPtr == bt) {
                    bt = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        } else {
            yPtr = stack[ht - 2]->link[0];
            if ((yPtr != NULL) && (yPtr->color == RED)) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            } else {
                if (dir[ht - 1] == 1) {
                    yPtr = stack[ht - 1];
                } else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[0];
                    xPtr->link[0] = yPtr->link[1];
                    yPtr->link[1] = xPtr;
                    stack[ht - 2]->link[1] = yPtr;
                }
                xPtr = stack[ht - 2];
                yPtr->color = BLACK;
                xPtr->color = RED;
                xPtr->link[1] = yPtr->link[0];
                yPtr->link[0] = xPtr;
                if (xPtr == bt) {
                    bt = yPtr;
                } else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
    }
    bt->color = BLACK;
}

void reintegra_filtrate(struct lista* n,int dim ,int vincolata)
{
    while (n != NULL) {
        insertion(n->key,dim,0);
        printf("Ho reintegrato la parola: %s\n", n->key);
        n = n->next;
    }
}

void stampa_lista(struct lista *n)
{
    if(n!=NULL)
    {
        printf("%s\n", n->key);
        stampa_lista(n->next);
    }
}

void push(struct lista** head_ref, char *input, int dim)
{
    struct lista* new_node = (struct lista*) malloc(sizeof(struct lista));

    new_node->key = malloc(sizeof(char)*dim);
    unsigned i = 0;
    while(i<dim)
    {
        new_node->key[i] = input[i];
        i++;
    }
    printf("Ho inserito nella lista: %s\n",new_node->key);

    new_node->next = (*head_ref);

    (*head_ref)    = new_node;
    printf("Stampa lista \n");
    stampa_lista(new_node);
}

struct lista * push_2(struct lista *lt, char *input, int dim){
    struct lista * new_node = (struct lista*) malloc(sizeof(struct lista));
    new_node->key = malloc(sizeof(char)*dim);
    unsigned int i = 0;
    while(i<dim)
    {
        new_node->key[i] = input[i];
        i++;
    }
    new_node->next = lt;
    return new_node;
}


void deletion(char *key) {
    struct rbNode *stack[98], *ptr, *xPtr, *yPtr;
    struct rbNode *pPtr, *qPtr, *rPtr;
    int dir[98], ht = 0, diff, i;
    enum nodeColor color;

    if (!bt) {
        printf("Tree not available\n");
        return;
    }

    ptr = bt;
    while (ptr != NULL) {
        if (strcmp(key,ptr->key)==0)
            break;
        diff = strcmp(key,ptr->key) > 0 ? 1 : 0;
        stack[ht] = ptr;
        dir[ht++] = diff;
        ptr = ptr->link[diff];
    }

    if (ptr->link[1] == NULL) {
        if ((ptr == bt) && (ptr->link[0] == NULL)) {
            free(ptr);
            bt = NULL;
        } else if (ptr == bt) {
            bt = ptr->link[0];
            free(ptr);
        } else {
            stack[ht - 1]->link[dir[ht - 1]] = ptr->link[0];
        }
    } else {
        xPtr = ptr->link[1];
        if (xPtr->link[0] == NULL) {
            xPtr->link[0] = ptr->link[0];
            color = xPtr->color;
            xPtr->color = ptr->color;
            ptr->color = color;

            if (ptr == bt) {
                bt = xPtr;
            } else {
                stack[ht - 1]->link[dir[ht - 1]] = xPtr;
            }

            dir[ht] = 1;
            stack[ht++] = xPtr;
        } else {
            i = ht++;
            while (1) {
                dir[ht] = 0;
                stack[ht++] = xPtr;
                yPtr = xPtr->link[0];
                if (!yPtr->link[0])
                    break;
                xPtr = yPtr;
            }

            dir[i] = 1;
            stack[i] = yPtr;
            if (i > 0)
                stack[i - 1]->link[dir[i - 1]] = yPtr;

            yPtr->link[0] = ptr->link[0];

            xPtr->link[0] = yPtr->link[1];
            yPtr->link[1] = ptr->link[1];

            if (ptr == bt) {
                bt = yPtr;
            }

            color = yPtr->color;
            yPtr->color = ptr->color;
            ptr->color = color;
        }
    }

    if (ht < 1)
        return;

    if (ptr->color == BLACK) {
        while (1) {
            pPtr = stack[ht - 1]->link[dir[ht - 1]];
            if (pPtr && pPtr->color == RED) {
                pPtr->color = BLACK;
                break;
            }

            if (ht < 2)
                break;

            if (dir[ht - 2] == 0) {
                rPtr = stack[ht - 1]->link[1];

                if (!rPtr)
                    break;

                if (rPtr->color == RED) {
                    stack[ht - 1]->color = RED;
                    rPtr->color = BLACK;
                    stack[ht - 1]->link[1] = rPtr->link[0];
                    rPtr->link[0] = stack[ht - 1];

                    if (stack[ht - 1] == bt) {
                        bt = rPtr;
                    } else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    dir[ht] = 0;
                    stack[ht] = stack[ht - 1];
                    stack[ht - 1] = rPtr;
                    ht++;

                    rPtr = stack[ht - 1]->link[1];
                }

                if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
                    (!rPtr->link[1] || rPtr->link[1]->color == BLACK)) {
                    rPtr->color = RED;
                } else {
                    if (!rPtr->link[1] || rPtr->link[1]->color == BLACK) {
                        qPtr = rPtr->link[0];
                        rPtr->color = RED;
                        qPtr->color = BLACK;
                        rPtr->link[0] = qPtr->link[1];
                        qPtr->link[1] = rPtr;
                        rPtr = stack[ht - 1]->link[1] = qPtr;
                    }
                    rPtr->color = stack[ht - 1]->color;
                    stack[ht - 1]->color = BLACK;
                    rPtr->link[1]->color = BLACK;
                    stack[ht - 1]->link[1] = rPtr->link[0];
                    rPtr->link[0] = stack[ht - 1];
                    if (stack[ht - 1] == bt) {
                        bt = rPtr;
                    } else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    break;
                }
            } else {
                rPtr = stack[ht - 1]->link[0];
                if (!rPtr)
                    break;

                if (rPtr->color == RED) {
                    stack[ht - 1]->color = RED;
                    rPtr->color = BLACK;
                    stack[ht - 1]->link[0] = rPtr->link[1];
                    rPtr->link[1] = stack[ht - 1];

                    if (stack[ht - 1] == bt) {
                        bt = rPtr;
                    } else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    dir[ht] = 1;
                    stack[ht] = stack[ht - 1];
                    stack[ht - 1] = rPtr;
                    ht++;

                    rPtr = stack[ht - 1]->link[0];
                }
                if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
                    (!rPtr->link[1] || rPtr->link[1]->color == BLACK)) {
                    rPtr->color = RED;
                } else {
                    if (!rPtr->link[0] || rPtr->link[0]->color == BLACK) {
                        qPtr = rPtr->link[1];
                        rPtr->color = RED;
                        qPtr->color = BLACK;
                        rPtr->link[1] = qPtr->link[0];
                        qPtr->link[0] = rPtr;
                        rPtr = stack[ht - 1]->link[0] = qPtr;
                    }
                    rPtr->color = stack[ht - 1]->color;
                    stack[ht - 1]->color = BLACK;
                    rPtr->link[0]->color = BLACK;
                    stack[ht - 1]->link[0] = rPtr->link[1];
                    rPtr->link[1] = stack[ht - 1];
                    if (stack[ht - 1] == bt) {
                        bt = rPtr;
                    } else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    break;
                }
            }
            ht--;
        }
    }
}


void inorderTraversal(struct rbNode *node) {
    if (node) {
        inorderTraversal(node->link[0]);
        printf("%s\n", node->key);
        inorderTraversal(node->link[1]);
    }
    return;
}


struct rbNode* ricerca_parola_2 (struct rbNode *bt, char * info,int dim)
{
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
                if(bt->key[i] > info[i])
                {
                    return  ricerca_parola_2(bt->link[0],info,dim);
                }
                else
                    return ricerca_parola_2(bt->link[1],info,dim);
            }
        }
        return bt;
    }
}










void word_checker_3(char *rif, char *word,struct rbNode * bt,unsigned int dim, int stampa, char vincoli[], int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][320])
{
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int check;
    unsigned int i;

    strcpy(temp,word);
    strcpy(temp_rif, rif);
    strcpy(output,word);

    for( i=0;i<dim-1;i++) {
        if (rif[i] == word[i]) {
            temp[i] = '+';
            output[i] = '+';
            temp_rif[i] = '+';
        } else
            output[i] = '/';
    }
    for(i=0;i<dim-1;i++)
    {
        if(rif[i] != word[i])
        {
            output[i] = '/';
            check = 0;
            unsigned j;
            for(j=0;j<dim-1;j++)
            {
                if(temp[i]==temp_rif[j])
                {
                    check = 1;
                    temp_rif[j] = '*';
                    break;
                }
            }
            if(check == 1)
            {
                output[i] = '|';
            }
        }
        else
        {
            output[i] = '+';
            temp[i] = '+';
            temp_rif[i] = '+';
        }
    }
    if(stampa)
        printf("%s\n", output);
    for(i=0;i<dim-1;i++)
    {
        if(output[i]=='+')
            vincoli[i] = word[i];
        else
        {
            if(output[i]=='|')
            {
                unsigned int k, pres_word = 0;
                for(k=0;k<dim-1;k++)
                {
                    if(word[i] == word[k] && (output[k]!='/'))
                        pres_word ++;
                }
                //bt = ricerca_carattere_presente_min_n_volte_non_pos_esatta(bt,dim,bt,word[i],pres_word, i);
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
            else
            {
                unsigned int k, pres_rif = 0;
                for(k=0;k<dim-1;k++)
                {
                    if(word[i] == rif[k])
                        pres_rif++;
                }
                if(pres_rif == 0)
                {
                    unsigned int u=0;
                    while(non_presente[u]!=-1)
                        u++;
                    non_presente[u] = word[i];
                }

                else
                {
                    //bt = ricerca_non_pos_esatta(bt,dim,bt,i,word[i]);
                    //bt = ricerca_carattere_presente_n_volte(bt,dim,bt,word[i],pres_rif);
                    unsigned int m = 0;
                    while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i])
                    {
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


struct rbNode * applica_vincoli_vettori(struct rbNode * bt, struct rbNode * root, struct lista * filtrate, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][320], int  conta_filtrate[])
{
    if(bt == NULL) {
        return root;
    }
    else {
        unsigned int i;
        if (bt->vincolata != 1) {
            for (i = 0; i < dim - 1; i++) {
                if(vincoli[i]!=0 && vincoli[i]!=bt->key[i])
                {
                    bt->vincolata = 1;
                    break;
                }
                unsigned int o = 0;
                while(non_presente[o]!=-1)
                {
                    unsigned int p;
                    for(p=0;p<dim-1;p++)
                    {
                        if(bt->key[p]==non_presente[o])
                        {

                            bt->vincolata = 1;
                            break;
                        }
                    }
                    o++;
                }
                if(bt->vincolata == 1)
                    break;

                unsigned int j=0;
                while(presente_n[0][j]!=-1)
                {
                    unsigned int m = 0, occorrenze = 0;
                    for(m=0;m<dim-1;m++)
                    {
                        if(presente_n[0][j]==bt->key[m])
                            occorrenze ++;
                    }
                    if(presente_n[2][j]!=-1 && presente_n[2][j]!=occorrenze)
                    {

                        bt->vincolata = 1;
                        break;
                    }

                    if(presente_n[1][j]!=-1 && presente_n[1][j]>occorrenze)
                    {

                        bt->vincolata = 1;
                        break;
                    }
                    j++;
                }
                if(bt->vincolata == 1)
                    break;
                j = 0;
                while(presente_pos_sbagliata[0][j]!=-1)
                {
                    if(presente_pos_sbagliata[0][j] == bt->key[presente_pos_sbagliata[1][j]])
                    {

                        bt->vincolata = 1;
                        break;
                    }
                    j++;
                }
                if(bt->vincolata == 1)
                    break;
            }


        }
        root = applica_vincoli_vettori(bt->link[0],root,filtrate,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
        root = applica_vincoli_vettori(bt->link[1],root,filtrate,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
        if(bt->vincolata == 1)
        {
            conta_filtrate[0] = conta_filtrate[0] - 1;
            push(&filtrate,bt->key,dim);
            deletion(bt->key);
        }

    }
    return root;
}

unsigned int applica_vincoli_vettori_single_word(char* input, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][320])
{
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


void stampa_filtri(struct rbNode *bt)
{
    if(bt != NULL)
    {
        stampa_filtri(bt->link[0]);
        if(bt->vincolata == 0)
            printf("%s\n",bt->key);
        stampa_filtri(bt->link[1]);
    }
}

void ripristina_vincoli(struct rbNode *bt)
{
    if(bt!=NULL)
    {
        if(bt->vincolata!=0)
            bt->vincolata = 0;
        ripristina_vincoli(bt->link[0]);
        ripristina_vincoli(bt->link[1]);
    }
}

void ripristina_vincoli_vettori(char par_vincoli[], int presente_n[][64],int dim,int presente_pos_sbagliata[][320],int non_presente[])
{
    unsigned int m,n;
    for(m=0;m<dim;m++)
        par_vincoli[m] = 0;
    for(m=0;m<3;m++)
    {
        for(n=0;n<64;n++)
            presente_n[m][n] = -1;
    }
    for(m=0;m<2;m++)
    {
        for(n=0;n<320;n++)
            presente_pos_sbagliata[m][n] = -1;
    }
    for(m=0;m<64;m++)
    {
        non_presente[m] = -1;
    }

}

unsigned int ricerca_parola_lista(struct lista * n, char *input)
{
    if(n==NULL)
        return 0;
    else
    {
        if(strcmp(n->key,input)==0)
            return 1;
        ricerca_parola_lista(n->next,input);
    }
}



void libera_lista(struct lista *n){
    if(n!=NULL){
        struct lista * test;
        test = n->next;
        free(n->key);
        free(n);
        libera_lista(test);
    }
}
int main()
{
    unsigned int dim, err, tent;
    err=fscanf(stdin, "%d\n", &dim);
    char * fin ;
    dim++;

    int non_presente[64];
    char *par_vincoli = malloc(sizeof(char)*dim); //posizione_corretta
    int presente_n[3][64];
    int presente_pos_sbagliata[2][320];
    int conta_dizionario = 0;
    int conta_filtrate[1];

    char c ;
    char *p = (char*) malloc(dim*sizeof(char));
    while(1)
    {
        fin = fgets(p,dim,stdin);
        c = getc_unlocked(stdin);

        if(p[0] == '+')
        {
            while(getc_unlocked(stdin)!='\n'){}
            break;
        }
        else
        {
            insertion(p,dim,0);
            conta_dizionario ++;
        }
    }
    char *par_confronto = malloc((dim)*sizeof(char));
    char *par_let;
    if(dim<18)
        par_let = malloc(18*sizeof(char));
    else
        par_let = malloc(dim*sizeof(char));

    while(1) {

        fin = fgets(p, dim, stdin);
        ripristina_vincoli_vettori(par_vincoli,presente_n,dim,presente_pos_sbagliata,non_presente);

        if(p[0] == '+' && p[1]=='i')
        {
            while(getc_unlocked(stdin)!='\n');
            fin = fgets(p, dim, stdin);
            while(p[0] != '+')
            {
                if(p[0]!='\n' || p[0]!=' ')
                {
                    insertion(p,dim,0);
                    conta_dizionario ++;
                }
                err = fscanf(stdin,"%s\n",par_let);
                if(par_let[0] != '+')
                    strcpy(p,par_let);
                else
                {
                    break;
                }
            }
            fin = fgets(p, dim, stdin);

        }
        if(p[0]=='+' && p[1]=='n')
        {
            while(getc_unlocked(stdin)!='\n'){}
            fin = fgets(p, dim, stdin);
        }

        c = getc_unlocked(stdin);
        if(c==EOF)
        {
            break;
        }
        else
        {

            err = fscanf(stdin, "%d\n", &tent);
            conta_filtrate[0] = conta_dizionario;
            struct lista *filtrate = NULL;
            while (tent > 0) {
                //printf("Conta filtrate: %d, conta_dizionario %d\n", conta_filtrate,conta_dizionario);
                err = fscanf(stdin, "%s\n", par_let);
                if (par_let[0] == '+') {

                    if (par_let[1] == 's')
                    {
                        inorderTraversal(bt);
                    }

                    else {
                        if (par_let[1] == 'i') {
                            char * par_read;
                            if(dim < 18)
                                par_read = malloc(sizeof(char)*18);
                            else
                                par_read = malloc(sizeof(char)*dim);

                            fin = fgets(par_let,dim,stdin);
                            while(par_let[0] != '+' && par_let[1]!= 'i')
                            {
                                if(par_let[0]!='\n' || par_let[0]!=' ')
                                {
                                    if(applica_vincoli_vettori_single_word(par_let,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata)==0)
                                    {
                                        insertion(par_let,dim,0);
                                        conta_filtrate[0] ++;
                                    }
                                    else
                                        push(&filtrate,par_let,dim);
                                    conta_dizionario ++;
                                }
                                err = fscanf(stdin,"%s\n",par_read);
                                if(par_read[0]!='+')
                                    strcpy(par_let,par_read);
                                else
                                {
                                    par_let[0] = '+';
                                    par_let[1] = 'i';
                                }
                            }
                            free(par_read);
                        }

                    }
                    if (par_let[1] == 'n') {
                        tent = 0;
                    }

                } else {
                    strcpy(par_confronto,par_let);
                    if (ricerca_parola_2(bt, par_confronto,dim) == NULL && ricerca_parola_lista(filtrate,par_confronto)==0)
                    {
                        printf("not_exists\n");
                    }

                    else {
                        if (strcmp(p, par_confronto) == 0) {
                            printf("ok\n");
                            tent=0;
                        } else {
                            //bt = word_checker_2(p, par_confronto, bt,dim , 1);
                            word_checker_3(p, par_confronto,bt,dim,1,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                            bt = applica_vincoli_vettori(bt,bt,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                            //printf("%d\n", conta_elementi(bt));
                            printf("%d\n",conta_filtrate[0]);
                            printf("Stampo filtrate\n");
                            stampa_lista(filtrate);
                            printf("\n");
                            tent--;
                            if(tent == 0)
                            {
                                printf("ko\n");
                            }
                        }
                    }

                }
            }
            reintegra_filtrate(filtrate,dim,0);
            libera_lista(filtrate);

            ripristina_vincoli(bt);
        }

        if(err == 10201)
            printf("ciao\n");
        if(fin == NULL)
            break;

    }
    //deleteBinaryTree(bt);
    free(p);
    free(par_let);
    free(par_confronto);
    free(par_vincoli);
    return 0;
}
