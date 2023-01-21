#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct _node
{
    int vincolata;
    struct _node *left;
    struct _node *right;

    // red ==> true, black ==> false
    bool color;
    char key[];
} node;

//Funzione per misurare il tempo di esecuzione
long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}
//versione fatta a mano della strcmp, risulta essere molto più veloce.
int strcmp_home_made(const char * s1, const char*s2){
    while(*s1 && (*s1 == *s2)){
        s1++, s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

//Passato un carattere, ritorna l'indice del carattere stesso in un vettore di 64 bit (per esempio la lettera '-' si trova in posizione 0.
int ritorna_posizione(char c){
    if(c=='-')
    {
        return c = c - 45;
    } else if(c > 45 && c < 65)
    {
        return c = c - 47;
    } else if(c>64 && c<91)
    {
        return c= c - 54;
    } else if(c == '_')
    {
        return c = c - 58;
    } else
        return c = c - 59;
}

//Passata la stringa, la dimensione e se la parola vincolata o meno(0=non vincolata, 1 = vincolata), viene creato un nuovo nodo

node* create_node(char *input, int dim,bool color, int vincolata)
{
    node *myNode = malloc(sizeof(node)+sizeof(char)*dim);
    myNode -> vincolata = vincolata;
    myNode -> left = myNode -> right = NULL;
    memmove(myNode->key,input,dim);
    myNode -> color = true;
    return myNode;
}

node* rotateLeft(node* myNode)
{
    node *child = myNode -> right;
    node *childLeft = child -> left;
    child -> left = myNode;
    myNode -> right = childLeft;
    return child;
}

node* rotateRight(node* myNode)
{
    node *child = myNode -> left;
    node *childRight =  child -> right;
    child -> right = myNode;
    myNode -> left = childRight;
    return child;
}

int isRed(node *myNode)
{
    if (myNode == NULL)
        return 0;
    return (myNode -> color == true);
}

void swapColors(node *node1, node *node2)
{
    bool temp = node1 -> color;
    node1 -> color = node2 -> color;
    node2 -> color = temp;
}


node* BST_insert_iterative(node* myNode, char * data,int dim, int vincolata )
{
    // Normal insertion code for any Binary
    // Search tree.
    if (myNode == NULL)
        return create_node(data, dim,false,vincolata);

    if (strcmp_home_made(data,myNode -> key)<0)
        myNode -> left = BST_insert_iterative(myNode -> left, data,dim,vincolata);

    else
        myNode -> right = BST_insert_iterative(myNode -> right, data,dim,vincolata);
    // case 1.
    // when right child is Red but left child is
    // Black or doesn't exist.
    if (isRed(myNode -> right) && !isRed(myNode -> left))
    {
        // left rotate the node to make it into
        // valid structure.
        myNode = rotateLeft(myNode);

        // swap the colors as the child node
        // should always be red
        swapColors(myNode,myNode->left);

    }

    // case 2
    // when left child as well as left grand child in Red
    if (isRed(myNode -> left) && isRed(myNode -> left -> left))
    {
        // right rotate the current node to make
        // it into a valid structure.
        myNode = rotateRight(myNode);

        swapColors(myNode,myNode->right);
    }


    // case 3
    // when both left and right child are Red in color.
    if (isRed(myNode -> left) && isRed(myNode -> right))
    {
        // invert the color of node as well
        // it's left and right child.
        myNode -> color = !myNode -> color;

        // change the color to black.
        myNode -> left -> color = false;
        myNode -> right -> color = false;
    }

    return myNode;
}

//funzione ricorsiva per la stampa di un albero, per velocizzare l'esecuzione, si controlla che il prossimo nodo da richiamare sia diverso da NULL.
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

//Versione velocizzata della ricerca di una parola, al posto di controllare tutta la parola con la strcmp, si confronta carattere per carattere, quando sono diversi, si decide se proseguire a sinistra o a destra dell'albero.
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

//Ripristina l'albero ponendo tutte le parole vincolate a 0.(anche qui si richiama la ricorsione solo se il prossimo nodo è diverso da NULL).
void ripristina_albero(node *tree){
    if(tree!= NULL){
        if(tree->right!=NULL)
            ripristina_albero(tree->right);
        tree->vincolata=0;
        if(tree->left!=NULL)
            ripristina_albero(tree->left);
    }
}
//Elimina l'albero ponendo tutte le parole vincolate a 0.(anche qui si richiama la ricorsione solo se il prossimo nodo è diverso da NULL).
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

//Funzione che crea l'output atteso e crea i vincoli
void word_checker_3(char *rif, char *word,unsigned int dim, char *vincoli, int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][124]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int i;
    unsigned int j;

    memmove(temp,word,dim);
    memmove(temp_rif, rif,dim);

    //Nel primo giro, l'output diventa banalmente una stringa di + e / (si guarda solo se i caratteri sono uguali o meno)
    for( i=0;i<dim-1;i++) {
        if (rif[i] == word[i]) {
            temp[i] = '+';
            output[i] = '+';
            temp_rif[i] = '+';
        } else
            output[i] = '/';
    }
    //Nel secondo giro, si conta quante volte il carattere compare nella parola, in modo da definire se i carattere '/' dell'output siano '/' o '|'
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
    puts(output);
    unsigned int k, pres_word = 0,pres_rif=0;
    unsigned int m = 0;

    //Si prosegue i vincoli in base all'output ottenuto e alla modifica delle stringhe appena usate.
    for(i=0;i<dim-1;i++) {
        //Se uguale, si inserisce il carattere nella stringa vincoli nella posizione in cui è presente.
        if(output[i]=='+')
            vincoli[i] = word[i];
        else {
            if(output[i]=='|') {
                //Se il carattere è presente in altre posizioni, si conta quante volte il carattere è presente nella parola e si aggiunge questo vincolo in una matrice chiamata presente_n
                //presente_n[0][pos] = carattere
                //presente_n[1][pos] = vincolo 4, ovvero quante volte il carattere è presente min
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

                //Nel vincolo 4 si apprense anche che il carattere non può trovarsi nella posizione i (siccome molti caratteri possono trovarsi sbagliati in tale posizione,anche in questo caso si ha una matrice).
                presente_pos_sbagliata[i][word[i]] = 1;
            }
            else {
                //Se il carattere non è presente in altre posizioni, si conta quante volte il carattere è presente nella parola e si aggiunge questo vincolo in una matrice chiamata presente_n
                //presente_n[0][pos] = carattere
                //presente_n[2][pos] = vincolo 5, ovvero quante volte il carattere è presente esattamente.
                pres_rif = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == rif[k])
                        pres_rif++;
                }
                if(pres_rif == 0) {
                    non_presente[word[i]] = 1;
                }

                else {
                    m = 0;
                    while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i]) {
                        m++;
                    }
                    presente_n[0][m] = word[i];
                    presente_n[2][m] = pres_rif;
                    //Nel vincolo 4 si apprense anche che il carattere non può trovarsi nella posizione i (siccome molti caratteri possono trovarsi sbagliati in tale posizione,anche in questo caso si ha una matrice).
                    presente_pos_sbagliata[i][word[i]] = 1;
                }
            }
        }
    }
    free(temp);
    free(temp_rif);
    free(output);
}


//Funzione ricorsiva che richiama che applica i vincoli trovati con word_checker, poi inserisce le parole non vincolate in un nuovo albero (che avrà dimensione ridotta).
node * applica_vincoli_vettori(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][124], int  conta_filtrate[]) {
    if(bt != NULL) {
        if(bt->vincolata==0) {
            unsigned int i, vincolata = 0;
            for (i = 0; i < dim - 1; i++) {

                if (vincoli[i] != 0 && vincoli[i] != bt->key[i]) {
                    vincolata = 1;
                    break;
                }
                if (non_presente[bt->key[i]] == 1)
                    vincolata = 1;
                if (vincolata == 1)
                    break;
                if (presente_pos_sbagliata[i][bt->key[i]] == 1) {
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

        //Questa serie di if permette di velocizzare la visita dell'albero.
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

//Applica i vincoli ad una parola inserita durante una partita, in modo da sapere se si inserice in un albero, se va messa già vincolata, ecc.
unsigned int applica_vincoli_vettori_single_word(char* input, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][124]) {
    unsigned int i;
    unsigned int j = 0;
    unsigned int m = 0, occorrenze = 0;
    for (i = 0; i < dim - 1; i++) {

        if (vincoli[i] != 0 && vincoli[i] !=input[i]) {
            return 1;
        }
        if(non_presente[input[i]] == 1)
            return 1;
        if(presente_pos_sbagliata[i][input[i]] == 1) {
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

//funzione che ripristina i vincoli appena appresi.
void ripristina_vincoli_vettori( int presente_n[][64],int dim,int presente_pos_sbagliata[][124],int non_presente[]) {
    unsigned int m,n;
    for(n=0;n<64;n++){
        presente_n[0][n] = -1;
        presente_n[1][n] = -1;
        presente_n[2][n] = -1;
    }
    for(m=0;m<dim;m++){
        for(n=0;n<124;n++){
            presente_pos_sbagliata[m][n] = -1;
        }

    }
    for(m=0;m<320;m++) {
        non_presente[m] = -1;
    }

}

//Simile alle altre funzioni che applicano i vincoli, ma in questo caso l'albero è sempre lo stesso, la differenza è che si segna il valore vincolata di ogni nodo.
//Si è tentato di osservare due parole contemporaneamente, per questo ci sono così tanti if.
node * applica_vincoli_vettori_boolean(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][124], int  conta_filtrate[]) {
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

                        if (non_presente[bt->key[i]] == 1)
                            vincolata = 1;
                        if (non_presente[bt->left->key[i]] == 1)
                            vincolata_left = 1;

                        if (presente_pos_sbagliata[i][bt->key[i]] == 1) {
                            vincolata = 1;
                        }
                        if (presente_pos_sbagliata[i][bt->left->key[i]] == 1) {
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
                        if (non_presente[ bt->key[i]] == 1)
                            vincolata = 1;
                        if (vincolata == 1)
                            break;

                        if (presente_pos_sbagliata[i][ bt->key[i]] == 1) {
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
                    if (non_presente[bt->key[i]] == 1)
                        vincolata = 1;
                    if (presente_pos_sbagliata[i][bt->key[i]] == 1) {
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

node * bt = NULL;
node * filtrate;
node * temp;

int main() {
    unsigned int dim=0, err, tent, prima_attraversata = 0;

    int c;
    size_t size;
    clock_t t;
    double time_taken;
    t = clock();
    long start_t, end_t;
    //long start_dict_t, end_dict_t;


    //Ottengo la dimensione delle parole.
    err = fscanf(stdin, "%d\n", &dim);

    // Tempo di inizio
    start_t = get_nanos();

    char * fin ;
    dim++;
    char *par_let = malloc(19*sizeof(char));

    int non_presente[320];
    int presente_n[3][64];
    int presente_pos_sbagliata[dim][124];
    int conta_dizionario = 0;
    int conta_filtrate[1];
    int da_vincolare;

    // Tempo di inizio dizionario
    //start_dict_t = get_nanos();
    char *p = malloc(19*sizeof(char));

    //Comincio a leggere le parole da inserire nell'albero, controllo ogni volta se siano comandi o se vadano inserite parole all'interno del primo inserimento (può succdere).
    while(1) {
        getdelim(&p,&size,'\n',stdin);
        if(p[0] == '+') {
            if(p[1] == 'i'){
                while (1) {
                    getdelim(&par_let,&size,'\n',stdin);
                    if(par_let[0] == '+') break;
                        bt = BST_insert_iterative(bt, par_let, dim,0);
                        conta_dizionario++;

                }
                getdelim(&p,&size,'\n',stdin);
            }
            if(p[1] == 'n'){
                break;
            }
        }
        else {
            p[dim-1] = '\0';
            bt = BST_insert_iterative(bt,p,dim,0);
            conta_dizionario ++;
        }
    }

    //Comincia la partita, leggo la parola di riferimento p, anche qui verifico che non ci siano comandi.
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
            //partita iniziata davvero, leggo il numero di tentativi.
            err = fscanf(stdin, "%d\n", &tent);
            conta_filtrate[0] = conta_dizionario;
            filtrate = NULL;
            ripristina_vincoli_vettori(presente_n,dim,presente_pos_sbagliata,non_presente);

            char * par_vincoli = calloc(dim,sizeof(char));

            while (tent > 0) {
                temp = NULL;
                getdelim(&par_let,&size,'\n',stdin); //ottengo la parola del tentativo.
                par_let[dim-1] = '\0';

                //verifico che non siano comandi
                if (par_let[0] == '+') {
                    if (par_let[1] == 's') {
                        if(prima_attraversata == 0) //se prima_attraversata==1, significa che l'albero originale è stato sostituito.
                            //MorrisTraversal(bt,dim);
                            stampa_albero(bt);
                        else
                            //MorrisTraversal(filtrate,dim);
                            stampa_albero(filtrate);
                    }
                    else {
                        if (par_let[1] == 'i') { //Inserisce le parole a partita in corso.
                            while(1) {
                                getdelim(&par_let,&size,'\n',stdin);
                                par_let[dim-1] = '\0';
                                if(par_let[0] == '+') break;
                                da_vincolare = applica_vincoli_vettori_single_word(par_let,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                                if(da_vincolare==0) {
                                    conta_filtrate[0] ++;
                                    if(prima_attraversata!=0)
                                        filtrate = BST_insert_iterative(filtrate,par_let,dim,da_vincolare);
                                }
                                bt = BST_insert_iterative(bt,par_let,dim,da_vincolare);

                                conta_dizionario ++;
                            }
                        }
                    }

                } else { //Controllo che la parola sia valida.
                    if (ricerca_parola_2(bt, par_let,dim) == NULL)
                    {
                        printf("not_exists\n");
                    }

                    else {
                        //Cerco che non sia già la parola di riferimento.
                        if (strcmp_home_made(p, par_let) == 0) {
                            printf("ok\n");
                            tent=0;
                        } else {
                            //Produco l'output.
                            word_checker_3(p, par_let,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata);
                            //Ovviamente se ho solo una parola non vincolata, non ho bisogno di fare tutti i check successivi.
                            if(conta_filtrate[0]!=1){
                                //Con un buon numero di parole vincolate, porta vantaggi proseguire con i boolean.
                                if(conta_filtrate[0]>10000)
                                    bt = applica_vincoli_vettori_boolean(bt,bt,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                else {
                                    //dopo di che conviene sostituire l0albero.
                                    if(prima_attraversata == 0){
                                        filtrate = applica_vincoli_vettori(bt,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                        prima_attraversata++;
                                    }else{
                                        if(conta_filtrate[0]>2000){
                                            filtrate = applica_vincoli_vettori_boolean(filtrate,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);

                                        } else {
                                            if(conta_filtrate[0]>50){
                                                temp = applica_vincoli_vettori(filtrate,NULL,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                                deleteBinaryTree(filtrate);
                                                filtrate = temp;
                                            }
                                            else{
                                                filtrate = applica_vincoli_vettori_boolean(filtrate,filtrate,dim,par_vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
                                            }

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
    //Servono solo perchè il verificatore rompe sul fatto che non vengano usati i valori di ritorno delle fgets e delle scanf.
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
