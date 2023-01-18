#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node
{
    char *key;
    int vincolata;
    struct _node *left;
    struct _node *right;
} node;

typedef struct lista {
    char *key;
    struct lista *y;
}lista;


node* ricerca_parola_2 (node *bt, char * info,int dim)
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
                    return  ricerca_parola_2(bt->left,info,dim);
                }
                else
                    return ricerca_parola_2(bt->right,info,dim);
            }
        }
        return bt;
    }
}



struct _node* newNode(char *input, unsigned int dim,int vincolo)
{
    struct _node* temp = (struct _node*)malloc(sizeof(struct _node));
    temp->key = (char*) malloc(sizeof(char)*dim);
    temp->vincolata = vincolo;
    strcpy(temp->key,input);
    temp->left = temp->right = NULL;
    return temp;
}

struct _node* insertRec(struct _node* node, char* input, unsigned  dim, int vincolo)
{
    if (node == NULL)
    {
        return newNode(input,dim,vincolo);
    }
    if (strcmp(input,node->key)<0)
        node->left = insertRec(node->left, input, dim,vincolo);
    else
        node->right = insertRec(node->right, input,dim,vincolo);
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


void word_checker_3(char *rif, char *word,node * bt,unsigned int dim, int stampa, char vincoli[], int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][64])
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

node * applica_vincoli_vettori(node * bt, node * root, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64], int  conta_filtrate[])
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
            if(bt->vincolata == 1)
                conta_filtrate[0] = conta_filtrate[0] - 1;
        }

        root = applica_vincoli_vettori(bt->right,root,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
        root = applica_vincoli_vettori(bt->left,root,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);

    }
    return root;
}

unsigned int applica_vincoli_vettori_single_word(char* input, int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][64])
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


void stampa_filtri(node *bt)
{
    if(bt != NULL)
    {
        stampa_filtri(bt->left);
        if(bt->vincolata == 0)
            printf("%s\n",bt->key);
        stampa_filtri(bt->right);
    }
}

void ripristina_vincoli(node *bt)
{
    if(bt!=NULL)
    {
		if(bt->vincolata!=0)
			bt->vincolata = 0;
        ripristina_vincoli(bt->left);
        ripristina_vincoli(bt->right);
    }
}

void ripristina_vincoli_vettori(char par_vincoli[], int presente_n[][64],int dim,int presente_pos_sbagliata[][64],int non_presente[])
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
        for(n=0;n<64;n++)
            presente_pos_sbagliata[m][n] = -1;
    }
    for(m=0;m<64;m++)
    {
        non_presente[m] = -1;
    }

}

int main()
{
    unsigned int dim, err, tent;
    node * bt = NULL;
    err=fscanf(stdin, "%d\n", &dim);
    char * fin ;
    dim++;

    int non_presente[64];
    char *par_vincoli = malloc(sizeof(char)*dim); //posizione_corretta
    int presente_n[3][64];
    int presente_pos_sbagliata[2][64];
    int conta_dizionario = 0;
    int conta_filtrate[1];

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
            bt = insertRec(bt,p,dim,0);
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
            while(getchar_unlocked()!='\n');
            fin = fgets(p, dim, stdin);
            while(p[0] != '+')
            {
                if(p[0]!='\n' || p[0]!=' ')
                {
                    bt = insertRec(bt,p,dim,0);
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
            while(getchar_unlocked()!='\n'){}
            fin = fgets(p, dim, stdin);
        }

        c = getchar_unlocked();
        if(c==EOF)
        {
            break;
        }
        else
        {

            err = fscanf(stdin, "%d\n", &tent);
            conta_filtrate[0] = conta_dizionario;
            while (tent > 0) {
                //printf("Conta filtrate: %d, conta_dizionario %d\n", conta_filtrate,conta_dizionario);
                err = fscanf(stdin, "%s\n", par_let);
                if (par_let[0] == '+') {

                    if (par_let[1] == 's')
                    {
                        stampa_filtri(bt);
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
                                        bt = insertRec(bt,par_let,dim,0);
                                        conta_filtrate[0] ++;
                                    }
                                    else
                                        bt = insertRec(bt,par_let,dim,1);
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
                        if (ricerca_parola_2(bt, par_confronto,dim) == NULL)
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
                                bt = applica_vincoli_vettori(bt,bt,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                //printf("%d\n", conta_elementi(bt));
                                printf("%d\n",conta_filtrate[0]);
                                tent--;
                                if(tent == 0)
                                {
                                    printf("ko\n");
                                }
                            }
                        }

                }
            }
            ripristina_vincoli(bt);
        }

        if(err == 10201)
            printf("ciao\n");
        if(fin == NULL)
            break;

    }
    deleteBinaryTree(bt);
    free(p);
    free(par_let);
    free(par_confronto);
    free(par_vincoli);
    return 0;
}
