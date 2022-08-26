#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    unsigned int dim, err;
    err=fscanf(stdin, "%d", &dim);
    printf("Numero parole da leggere: %d", dim);
    return 0;
}