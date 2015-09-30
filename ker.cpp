#include "ker.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double gaussian2D (double sigma, double x, double y);

/* For documentation, cf. Doxygen or header. */
ker_t *ker_init(int height, int width ){
    ker_t *ker = (ker_t *) malloc(sizeof(ker_t));
    ker->array = (KTYPE **) calloc(height, sizeof(KTYPE *));
    *(ker->array) = (KTYPE *) calloc(height*width, sizeof(KTYPE));
    for(int i=0; i<height; i++)
        ker->array[i] = (KTYPE *) &((*(ker->array))[i*width]);
    ker->height = height;
    ker->width = width;
    return ker;
}


/* For documentation, cf. Doxygen or header. */
void ker_destroy(ker_t **ker){
    if(*ker!=NULL){
        free(*(*ker)->array);
        free((*ker)->array);
        free(*ker);
        *ker=NULL;
    }
}


/* For documentation, cf. Doxygen or header. */
void ker_print(ker_t *ker){
    printf("Kernel \n");
    for(int i = 0; i < ker->height; i++){
    printf("|");
        for(int j = 0; j < ker->width; j++){
            printf("%4.2f%c", ker->array[i][j], j != ker->width-1 ? ' ' : '|');
            //fflush(stdout);
        }
        printf(" \n");
    }
    printf(" \n");
}


/* For documentation, cf. Doxygen or header. */
ker_t *ker_gauss(int size, double sigma){
    ker_t *ker = ker_init(size, size);
    KTYPE **tab = ker->array;

    float trans = - .5 * (size-1);

    for(int i=0; i<size; i++)
    for(int j=0; j<size; j++)
        tab[i][j] = gaussian2D(sigma, i+trans, j+trans);

    ker_normalize(ker);

    return ker;
}


/* For documentation, cf. Doxygen or header. */
void ker_normalize(ker_t *ker){
    KTYPE *tab = *ker->array;

    int card = ker->height * ker->width;

    double sum=0;

    for(int i=0; i<card; i++)
        sum += tab[i];

    if(sum != 0)
    for(int i=0; i<card; i++)
        tab[i] /= sum;

}


/* 2 dimension gaussian symmetric and centered */
double gaussian2D (double sigma, double x, double y){
    printf("Gaussian(%f,%f,%f) \n", sigma, x, y);
    return exp( - (x*x+y*y)/(2*sigma*sigma) );
}

