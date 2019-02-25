/******************************************************************************
* (c) 2010-2019 AED Team
* Last modified: abl 2019-02-15
*
* NAME
*   connectivity.c
*
* DESCRIPTION
*   Algorithms for solving the connectivity problem -  QF QU WQU CWQU
*   For each method count number of entry pairs and the number of links
*
* COMMENTS
*
******************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#include "connectivity.h"



/******************************************************************************
* quick_find()
*
* Arguments: id - array with connectivity information
*            N - size of array
*            fp - file pointer to read data from
*            quietOut - to reduce output to just final count
* Returns: (void)
* Side-Effects: pairs of elements are read and the connectivity array is
*               modified
*
* Description: Quick Find algorithm
*
*****************************************************************************/

void quick_find(int *id, int N, FILE * fp, int quietOut)
{
    int i, p, q, t, j, k;
    int un_op=0, find_op=0, total_op=0, num_conj=0;
    int pairs_cnt = 0;            /* connection pairs counter */
    int links_cnt = 0;            /* number of links counter */

    /* initialize; all disconnected */
    for (i = 0; i < N; i++) {
        id[i] = i;
    }

    /* read while there is data */
    while (fscanf(fp, "%d %d", &p, &q) == 2) {

        
        /* do search first */
        pairs_cnt++;
        if (id[p] == id[q]) {
            /* already in the same set; discard */
            if (!quietOut)
                printf("\t%d %d\n", p, q);
            continue;
        }
        find_op++;

        /* pair has new info; must perform union */
        for (t = id[p], i = 0; i < N; i++) {
            un_op++;
            if (id[i] == t) {
                id[i] = id[q];
            }
        }
        links_cnt++;
        if (!quietOut)
            printf(" %d %d\n", p, q);
    }

    /*
    for(i=0;i<N;i++){
        printf("|%d|", id[i]);
    }
      */  

    for(j=0; j<N; j++){
        
        if(id[j]!=-1){
            printf("\n%d", j);
            num_conj++;
           
            for(k=j+1; k<N; k++){

                if(id[k]==id[j]){

                    printf("-%d", k); 
                    id[k]=-1;              
                }
            }
        }
    }
       
    printf("\nNumero de conjuntos: %d", num_conj);
    
    total_op = un_op + find_op;
    printf("\nThe number of elementary operations is: %d for union and %d for finding, for a total of %d operations \n", un_op, find_op, total_op); 
    
    printf("QF: The number of links performed is %d for %d input pairs.\n",
            links_cnt, pairs_cnt);
    return;
}


/******************************************************************************
* quick_union()
*
* Arguments: id - array with connectivity information
*            N - size of array
*            fp - file pointer to read data from
*            quietOut - to reduce output to just final count
* Returns: (void)
* Side-Effects: pairs of elements are read and the connectivity array is
*               modified
*
* Description: Quick Union algorithm
*
*****************************************************************************/

void quick_union(int *id, int N, FILE * fp, int quietOut)
{

    int i, j, p, q;
    int un_op=0, find_op=0, total_op=0;
    int pairs_cnt = 0;            /* connection pairs counter */
    int links_cnt = 0;            /* number of links counter */

    /* initialize; all disconnected */
    for (i = 0; i < N; i++) {
        id[i] = i;
    }

    /* read while there is data */
    while (fscanf(fp, "%d %d", &p, &q) == 2) {
        pairs_cnt++;
        i = p;
        j = q;

        /* do search first */
        find_op++; 
        while (i != id[i]) {
            find_op++; 
            i = id[i];
        }
        
        while (j != id[j]) {
            find_op++; 
            j = id[j];
        }
        if (i == j) {
            /* already in the same set; discard */
            if (!quietOut)
            printf("\t%d %d\n", p, q);
            continue;
        }

        /* pair has new info; must perform union */
        un_op++; 
        id[i] = j;
        links_cnt++;

        if (!quietOut)
            printf(" %d %d\n", p, q);
    }

    /*
    for(i=0;i<N;i++){
        printf("|%d|", id[i]);
    }
    */

    total_op = un_op + find_op;
    printf("\nThe number of elementary operations is: %d for union and %d for finding, for a total of %d operations \n", un_op, find_op, total_op); 
    
    printf("QU: The number of links performed is %d for %d input pairs.\n",
            links_cnt, pairs_cnt);
}


/******************************************************************************
* weighted_quick_union()
*
* Arguments: id - array with connectivity information
*            N - size of array
*            fp - file pointer to read data from
*            quietOut - to reduce output to just final count
* Returns: (void)
* Side-Effects: pairs of elements are read and the connectivity array is
*               modified
*
* Description: Weighted Quick Union algorithm
*
*****************************************************************************/

void weighted_quick_union(int *id, int N, FILE * fp, int quietOut)
{

    int i, j, p, q;
    int un_op=0, find_op=0, eq_op=0, total_op=0;
    int *sz = (int *) malloc(N * sizeof(int));
    int pairs_cnt = 0;            /* connection pairs counter */
    int links_cnt = 0;            /* number of links counter */

    /* initialize; all disconnected */
    for (i = 0; i < N; i++) {
        id[i] = i;
        sz[i] = 1;
    }

    /* read while there is data */
    while (fscanf(fp, "%d %d", &p, &q) == 2) {
        pairs_cnt++;

        /* do search first */
        find_op++;
        for (i = p; i != id[i]; i = id[i])
            find_op++;
        for (j = q; j != id[j]; j = id[j])
            find_op++;

        if (i == j) {
            /* already in the same set; discard */
            if (!quietOut)
            printf("\t%d %d\n", p, q);
            continue;
        }

        /* pair has new info; must perform union; pick right direction */
        if (sz[i] < sz[j]) {
            id[i] = j;
            sz[j] += sz[i];
            eq_op++;
            un_op++;
        }
        else {
            id[j] = i;
            sz[i] += sz[j];
            eq_op++;
            un_op++;
        }
        links_cnt++;

        if (!quietOut)
        printf(" %d %d\n", p, q);
    }
    
    total_op = un_op + find_op + eq_op;
    printf("\nThe number of elementary operations is: %d for union, %d for finding and %d for balance, for a total of %d operations \n", un_op, find_op, eq_op, total_op); 

    printf("WQU: The number of links performed is %d for %d input pairs.\n",
            links_cnt, pairs_cnt);
}


/******************************************************************************
* compressed_weighted_quick_union()
*
* Arguments: id - array with connectivity information
*            N - size of array
*            fp - file pointer to read data from
*            quietOut - to reduce output to just final count
* Returns: (void)
* Side-Effects: pairs of elements are read and the connectivity array is
*               modified
*
* Description: Compressed Weighted Quick Union algorithm
*
*****************************************************************************/

void compressed_weighted_quick_union(int *id, int N, FILE * fp, int quietOut)
{
    int i, j, p, q, t, x;
    int un_op=0, find_op=0, eq_op=0, comp_op=0, total_op=0;
    int *sz = (int *) malloc(N * sizeof(int));
    int pairs_cnt = 0;            /* connection pairs counter */
    int links_cnt = 0;            /* number of links counter */

    /* initialize; all disconnected */
    for (i = 0; i < N; i++) {
        id[i] = i;
        sz[i] = 1;
    }

    /* read while there is data */
    while (fscanf(fp, "%d %d", &p, &q) == 2) {
        pairs_cnt++;

        /* do search first */
        find_op++;
        for (i = p; i != id[i]; i = id[i])
            find_op++;
        for (j = q; j != id[j]; j = id[j])
            find_op++;

        if (i == j) {
            /* already in the same set; discard */
            if (!quietOut)
            printf("\t%d %d\n", p, q);
            continue;
        }

        /* pair has new info; must perform union; pick right direction */
        if (sz[i] < sz[j]) {
            id[i] = j;
            sz[j] += sz[i];
            t = j;
            eq_op++;
            un_op++;
        }
        else {
            id[j] = i;
            sz[i] += sz[j];
            t = i;
            eq_op++;
            un_op++;
        }
        links_cnt++;

        /* retrace the path and compress to the top */
        for (i = p; i != id[i]; i = x) {
            x = id[i];
            id[i] = t;
            comp_op++;
        }
        for (j = q; j != id[j]; j = x) {
            x = id[j];
            id[j] = t;
            comp_op++;
        }
        if (!quietOut)
        printf(" %d %d\n", p, q);
    }

    total_op = un_op + find_op + comp_op + eq_op;
    printf("\nThe number of elementary operations is: %d for union, %d for finding, %d for balance and %d for compression, for a total of %d operations \n", un_op, find_op, eq_op, comp_op, total_op); 
    

    printf("CWQU: The number of links performed is %d for %d input pairs.\n",
            links_cnt, pairs_cnt);

    return;
}
