#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node
{
    char *key;
    struct _node *left;
    struct _node *right;
} node;

node* ricerca_parola (node *bt, char * info)
{
    if((bt == NULL) || (strcmp(info,bt->key) == 0) )
        return bt;
    else
    {
        if(strcmp(info,bt->key)>0)
            return ricerca_parola(bt->right, info);
        else
            return ricerca_parola(bt->left,info);
    }
}
/*
node* insertRec(node *tree, char *input,int dim)
{
    node *new=(node*) malloc(sizeof(node));
    new->key = malloc(dim * sizeof(char));
    strcpy(new->key, input);

    new->left=NULL;
    new->right=NULL;

    if(tree==NULL)
    {
        return new;
    }

    if((strcmp(tree->key, input)>=0))
        tree->left=insertRec(tree->left, input,dim);
    else
        tree->right=insertRec(tree->right, input,dim);

    return tree;
}
*/

struct _node* newNode(char *input, int dim)
{
    struct _node* temp = (struct _node*)malloc(sizeof(struct _node));
    temp->key = (char*) malloc(sizeof(char)*dim);
    strcpy(temp->key,input);
    temp->left = temp->right = NULL;
    return temp;
}

struct _node* insertRec(struct _node* node, char* input, int dim)
{
    if (node == NULL)
    {
        return newNode(input,dim);
    }
    if (strcmp(input,node->key)<0)
        node->left = insertRec(node->left, input, dim);
    else if (strcmp(input,node->key)>0)
        node->right = insertRec(node->right, input,dim);
    return node;
}
void stampa_albero(node* tree)
{
    if(tree == NULL)
    {
        return;
    }


    stampa_albero(tree->left);
    printf("%s\n", tree->key);
    stampa_albero(tree->right);
}

node * binarytree_destroy(node * bt)
{
    if( bt == NULL)
    {
        return NULL;
    }
    else if((bt->left == NULL) && (bt->right == NULL))
    {
        free(bt->key);
        free(bt);
        return NULL;
    }

    else
    {
        bt->left = binarytree_destroy(bt->left);
        bt->right = binarytree_destroy(bt->right);

        free(bt->key);
        free(bt);
        return bt;
    }
}

node* deleteBinaryTree(node* bt)
{
    // Caso base: albero vuoto
    if (bt == NULL) {
        return NULL;
    }

    // elimina prima il sottoalbero sinistro e destro (Postorder)
    deleteBinaryTree(bt->left);
    deleteBinaryTree(bt->right);

    // elimina il nodo corrente dopo aver eliminato il suo sottoalbero sinistro e destro
    free(bt->key);
    free(bt);

    // imposta root come null prima di restituire
    return NULL;
}

node * binarytree_min(node * bt)
{
    if(bt == NULL)
        return NULL;
    else if(bt->left == NULL)
        return bt;
    else
    {
        node * min = binarytree_min(bt->left);
        return min;
    }
}

node* binarytree_delete(node* bt, char *info)
{
    if(bt == NULL)
        return NULL;
    else if(strcmp(bt->key,info)>1)
    {
        bt->left = binarytree_delete(bt->left, info);
        return bt;
    }
    else if(strcmp(info, bt->key)>1)
    {
        bt->right = binarytree_delete(bt->right,info);
        return bt;
    }

    else {
        node * min_right;
        if((bt->right == NULL))
        {
            free(bt->key);
            free(bt);
            return NULL;
        }
        if(bt->right == NULL)
        {
            node * alias;
            alias = bt->left;
            free(bt->key);
            free(bt);
            return alias;
        }
        if(bt->left == NULL)
        {
            node * alias;
            alias = bt->right;
            free(bt->key);
            free(bt);
            return alias;
        }

        min_right = binarytree_min(bt->right);
        strcpy(bt->key,min_right->key);

        bt->right = binarytree_delete(bt->right, min_right->key);
        return bt;
    }
}

int conta_elementi(node *bt)
{
    int count_left, count_right;
    if(bt == NULL)
        return 0;
    count_left = conta_elementi(bt->left);
    count_right = conta_elementi(bt->right);
    return 1 + count_left + count_right;
}

node* ricerca_pos_esatta(node* tree,int dim,node *test,int pos, char c)
{
    if(tree == NULL)
    {
        return test;
    }
    else
    {
        if(tree->key[pos] == c)
        {
            test = insertRec(test,tree->key,dim);
        }
        //else
        //{
        //printf("Ricerca_pos_esatta, parola eliminata: %s\n", tree->key);
        //}
        test=ricerca_pos_esatta(tree->left, dim,test,pos,c);
        test = ricerca_pos_esatta(tree->right,dim,test,pos,c);

    }
    free(tree->key);
    free(tree);
    return test;

}

node* ricerca_non_pos_esatta(node* tree,int dim,node *test,int pos, char c)
{
    if(tree == NULL)
    {
        return test;
    }
    else
    {
        if(tree->key[pos] != c)
        {
            test = insertRec(test,tree->key,dim);
        }
        //else
        //{
        //printf("Ricerca_non_pos_esatta, parola eliminata: %s\n", tree->key);
        //}
        test=ricerca_non_pos_esatta(tree->left, dim,test,pos,c);
        test = ricerca_non_pos_esatta(tree->right,dim,test,pos,c);
    }
    free(tree->key);
    free(tree);
    return test;
}

node* ricerca_carattere_non_presente(node* tree,int dim,node *test,char c)
{
    if(tree == NULL)
    {
        return test;
    }
    else
    {
        int found = 0;
        int i;
        for(i=0;i<dim-1;i++)
        {
            if(tree->key[i]==c)
                found = 1;
        }

        if(found != 1)
        {
            test = insertRec(test,tree->key,dim);
        }
        //else
        //{
        //printf("ricerca_carattere_non_presente, parola eliminata: %s\n", tree->key);

        //}
        test=ricerca_carattere_non_presente(tree->left, dim,test,c);
        test = ricerca_carattere_non_presente(tree->right,dim,test,c);

    }
    free(tree->key);
    free(tree);
    return test;

}

node* ricerca_carattere_presente_n_volte(node* tree,int dim,node *test,char c, int n_volte)
{
    if(tree == NULL)
    {
        return test;
    }
    else
    {
        int i;
        int pres = 0;
        for(i = 0; i < dim-1; i++)
        {
            if(tree->key[i] == c)
                pres++;
        }
        if(pres == n_volte)
            test = insertRec(test,tree->key,dim);
        //else
        //printf("ricerca_carattere_presente_n_volte, parola eliminata: %s\n", tree->key);
        test=ricerca_carattere_presente_n_volte(tree->left, dim,test,c,n_volte);
        test = ricerca_carattere_presente_n_volte(tree->right,dim,test,c,n_volte);

    }
    free(tree->key);
    free(tree);
    return test;
}
node* ricerca_carattere_presente_min_n_volte(node* tree,int dim,node *test,char c, int n_volte)
{
    if(tree == NULL)
    {
        return test;
    }
    else
    {
        int i;
        int pres = 0;
        for(i = 0; i < dim-1; i++)
        {
            if(tree->key[i] == c)
                pres++;
        }
        if(pres >= n_volte)
            test = insertRec(test,tree->key,dim);

        test = ricerca_carattere_presente_min_n_volte(tree->left, dim,test,c,n_volte);
        test = ricerca_carattere_presente_min_n_volte(tree->right,dim,test,c,n_volte);

    }
    free(tree->key);
    free(tree);
    return test;
}


node* word_checker_2(char *rif, char *word, int dim, node *filtrate, node *temp_bin)
{
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int check;
    unsigned int i;

    strcpy(temp,word);
    strcpy(temp_rif, rif);
    strcpy(output,word);

    for(i = 0; i<dim-1;i++)
    {
        temp[i] = word[i];
        temp_rif[i] = rif[i];
    }
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
        if(temp[i]!='+')
        {
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
    }
    printf("%s\n", output);
    temp_bin = NULL;
    for(i=0;i<dim-1;i++)
    {
        if(output[i]=='+')
            filtrate = ricerca_pos_esatta(filtrate,dim,temp_bin,i,word[i]);
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
                filtrate = ricerca_non_pos_esatta(filtrate,dim,temp_bin,i,word[i]);
                filtrate = ricerca_carattere_presente_min_n_volte(filtrate,dim,temp_bin,word[i],pres_word);
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
                    filtrate = ricerca_carattere_non_presente(filtrate,dim,temp_bin,word[i]);
                else
                {
                    filtrate = ricerca_non_pos_esatta(filtrate,dim,temp_bin,i,word[i]);
                    filtrate = ricerca_carattere_presente_n_volte(filtrate,dim,temp_bin,word[i],pres_rif);
                }
            }
        }
    }
    free(temp);
    free(temp_rif);
    free(output);
    return filtrate;
}
node * riempi_filtrate(node * bt, node *filtrate, int dim)
{
    if(bt==NULL)
        return filtrate;
    else
    {
        filtrate = insertRec(filtrate,bt->key,dim);
        filtrate = riempi_filtrate(bt->right,filtrate,dim);
        filtrate = riempi_filtrate(bt->left,filtrate,dim);
    }
    return filtrate;
}


node* applica_vincoli_parole_nuove(char *rif, char *word, int dim, node *filtrate, node *temp_bin)
{
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int check;
    unsigned int i;

    strcpy(temp,word);
    strcpy(temp_rif, rif);
    strcpy(output,word);

    for(i = 0; i<dim-1;i++)
    {
        temp[i] = word[i];
        temp_rif[i] = rif[i];
    }
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
        if(temp[i]!='+')
        {
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
    }
    //printf("%s\n", output);
    temp_bin = NULL;
    for(i=0;i<dim-1;i++)
    {
        if(output[i]=='+')
            filtrate = ricerca_pos_esatta(filtrate,dim,temp_bin,i,word[i]);
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
                filtrate = ricerca_non_pos_esatta(filtrate,dim,temp_bin,i,word[i]);
                filtrate = ricerca_carattere_presente_min_n_volte(filtrate,dim,temp_bin,word[i],pres_word);
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
                    filtrate = ricerca_carattere_non_presente(filtrate,dim,temp_bin,word[i]);
                else
                {
                    filtrate = ricerca_non_pos_esatta(filtrate,dim,temp_bin,i,word[i]);
                    filtrate = ricerca_carattere_presente_n_volte(filtrate,dim,temp_bin,word[i],pres_rif);
                }
            }
        }
    }
    free(temp);
    free(temp_rif);
    free(output);
    return filtrate;
}


node * applica_vecchi_vincoli(node *filtrate, node * parole_lette, char * rif, int dim, node *temp)
{
    if(parole_lette == NULL)
        return filtrate;
    else
    {
        filtrate = applica_vincoli_parole_nuove(rif,parole_lette->key, dim,filtrate, temp);
        filtrate = applica_vecchi_vincoli(filtrate, parole_lette->left,rif,dim,temp);
        filtrate = applica_vecchi_vincoli(filtrate, parole_lette->right,rif,dim,temp);
    }
    return filtrate;
}





int main()
{
    unsigned int dim, err, tent, check2 = 0,check1=0;
    node * bt = NULL;

    node * temp = NULL;
    err=fscanf(stdin, "%d\n", &dim);
    char * fin ;
    dim++;


    char c ;
    char *p = (char*) malloc(dim*sizeof(char));
    while(1)
    {

        fin = fgets(p,dim,stdin);
        c = getchar_unlocked();

        if(p[0] == '+')
        {
            while(getchar_unlocked()!='\n'){}
            break;
        }
        else
        {
            bt = insertRec(bt,p,dim);
        }

    }
    char *par_confronto = malloc((dim)*sizeof(char));
    char *par_let = malloc(18*sizeof(char));

    while(check1==0) {
        check2 = 0;
        check1 = 0;
        node * filtrate = NULL;
        node * parole_lette = NULL;
        filtrate = riempi_filtrate(bt,filtrate,dim);

        fin = fgets(p, dim, stdin);
        if(p[0]=='+' && p[1]=='n')
        {
            while(getchar_unlocked()!='\n'){}
            fin = fgets(p, dim, stdin);

        }
        if(p[0] == '+' && p[1]=='i')
        {
            while(getchar_unlocked()!='\n');
            fin = fgets(p, dim, stdin);
            while(p[0] != '+' && p[1] != 'i')
            {
                if(p[0]!='\n' || p[0]!=' ')
                {
                    bt = insertRec(bt,p,dim);
                    filtrate = insertRec(filtrate,p,dim);
                }
                err = fscanf(stdin,"%s\n",par_let);
                if(par_let[0] != '+')
                    strcpy(p,par_let);
                else
                {
                    p[0] = '+';
                    p[1] = 'i';
                }
            }
            fin = fgets(p, dim, stdin);
            if(p[0]=='+' && p[1]=='n')
            {
                while(getchar_unlocked()!='\n'){}
                fin = fgets(p, dim, stdin);

            }
        }

        c = getchar_unlocked();
        if(c==EOF)
        {
            check1 = 1;
            check2 = 3;
        }
        else
        {

            err = fscanf(stdin, "%d\n", &tent);
            while (check2 != 3 && tent > 0) {
                err = fscanf(stdin, "%s\n", par_let);
                if (par_let[0] == '+') {

                    if (par_let[1] == 's')
                    {
                        stampa_albero(filtrate);
                    }

                    else {
                        if (par_let[1] == 'i') {
                            if (check2 == 1)
                            {
                                filtrate = applica_vecchi_vincoli(filtrate, parole_lette,p,dim,temp);
                                check2 = 0;
                            }

                            else
                            {
                                check2 = 1;
                            }

                        }
                    }
                    if (par_let[1] == 'n') {
                        check2 = 3;
                    }

                } else {
                    strcpy(par_confronto,par_let);
                    if (check2 == 1) {
                        bt = insertRec(bt, par_confronto, dim);
                        filtrate = insertRec(filtrate, par_confronto, dim);
                    } else {

                        if (ricerca_parola(bt, par_confronto) == NULL)
                        {
                            printf("not_exists\n");
                        }

                        else {
                            parole_lette = insertRec(parole_lette,par_confronto,dim);
                            if (strcmp(p, par_confronto) == 0) {
                                printf("ok\n");
                                check2 = 3;
                            } else {
                                filtrate = word_checker_2(p, par_confronto, dim , filtrate,temp);
                                printf("%d\n", conta_elementi(filtrate));
                                tent--;
                                if(tent == 0)
                                {
                                    printf("ko\n");
                                }
                            }
                        }
                    }

                }
            }


        }
        filtrate = deleteBinaryTree(filtrate);
        //printf("Stampa tutte le parole lette:\n");
        //stampa_albero(parole_lette);
        parole_lette = deleteBinaryTree(parole_lette);

        if(err == 10201)
            printf("ciao\n");
        if(fin == NULL)
            break;

    }
    deleteBinaryTree(bt);
    free(p);
    free(par_let);
    free(par_confronto);
    return 0;
}
