#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/_pthread/_pthread_t.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    int rows;
    int cols;
    int** tab;
} matrix;

matrix matmul(matrix a, matrix b) {
    assert(a.cols == b.rows);
    matrix c = {c.rows=a.rows,c.cols=b.cols,c.tab=malloc(sizeof(int*)*a.rows)};
    for(int i = 0;i<c.rows;i++) {
        c.tab[i] = calloc(c.cols, sizeof(int));
    }
    for(int i = 0;i<c.rows;i++) {
        for(int j = 0;j<c.cols;j++) {
            for(int k = 0;k<a.cols;k++) {
                c.tab[i][j] += a.tab[i][k]*b.tab[k][j];
            }
        }
    }
    return c;
}

void print_matrix(matrix a) {
    printf("Printing a %d x %d matrix:\n=======\n",a.rows, a.cols);
    for(int i = 0;i<a.rows;i++) {
        for(int j = 0;j<a.cols;j++) {
            printf("%d ", a.tab[i][j]);
        }
        printf("\n");
    }
    printf("=======\n\n");
}

matrix random_matrix(int rows, int cols) {
    matrix c = {c.rows=rows,c.cols=cols,c.tab=malloc(sizeof(int*)*rows)};
    for(int i = 0;i<c.rows;i++) {
        c.tab[i] = calloc(c.cols, sizeof(int));
    }
    for(int i = 0;i<c.rows;i++) {
        for(int j = 0;j<c.cols;j++) {
            c.tab[i][j] = rand()%100;
        }
    }
    return c;
}

bool matrix_eq(matrix a, matrix b) {
    if(a.cols != b.cols || a.rows != b.rows) return false;
    for(int i = 0;i<a.rows;i++) {
        for(int j = 0;j<a.cols;j++) {
            if(a.tab[i][j] != b.tab[i][j]) return false;
        }
    }
    return true;
}

matrix* matrix_split_cols(matrix a) {
    matrix* cols = malloc(sizeof(matrix)*a.cols);
    for(int j = 0;j<a.cols;j++) {
        cols[j] = (matrix){a.rows,1,malloc(sizeof(int*)*a.rows)};
        for(int i = 0;i<a.rows;i++) {
            cols[j].tab[i] = malloc(sizeof(int)*1);
            cols[j].tab[i][0] = a.tab[i][j];
        }
    }
    return cols;
}

matrix matrix_join_cols(matrix* cols, int len) {
    assert(len > 0);
    matrix a = (matrix){cols[0].rows,len,malloc(sizeof(int*)*cols[0].rows)};
    for(int i =0;i<cols[0].rows;i++) {
        a.tab[i] = malloc(sizeof(int)*len);
        for(int j = 0;j<len;j++) {
            a.tab[i][j] = cols[j].tab[i][0];
        }
    }
    return a;
}

// ./tp ROWS COLS THREADS
int main(int argc, char** argv) {
    srand(time(NULL));
    assert(argc == 4);
    int ROWS, COLS, THREADS;
    sscanf(argv[1], "%d", &ROWS);
    sscanf(argv[2], "%d", &COLS);
    sscanf(argv[3], "%d", &THREADS);
    printf("Multiplying a %d x %d matrix over %d threads\n", ROWS, COLS, THREADS);
    assert(COLS % THREADS == 0 && ROWS % THREADS == 0);
    // matrix a = random_matrix(ROWS, COLS);
    // assert(matrix_eq(matrix_join_cols(matrix_split_cols(a), a.cols), a));
    // free(a.tab);
    pthread_t* threads = malloc(sizeof(pthread_t)*THREADS);
    // for(int i = 0;i<THREADS;i++) {
    //     pthread_create(&threads[0], NULL, worker, arg);
    // }
    // matrix a = (matrix){4,2,malloc(sizeof(int*)*4)};
    // for(int i = 0;i<a.rows;i++) {
    //     a.tab[i] = malloc(sizeof(int)*a.cols);
    // }
    // a.tab[0][0] = 0; a.tab[0][1] = 1;
    // a.tab[1][0] = 2; a.tab[1][1] = 3;
    // a.tab[2][0] = 4; a.tab[2][1] = 5;
    // a.tab[3][0] = 6; a.tab[3][1] = 7;
    // matrix b = (matrix){2,2,malloc(sizeof(int*)*2)};
    // for(int i = 0;i<b.rows;i++) {
    //     b.tab[i] = malloc(sizeof(int)*b.cols);
    // }
    // b.tab[0][0] = 10; b.tab[0][1] = 30;
    // b.tab[1][0] = 20; b.tab[1][1] = 40;
    // print_matrix(a);
    // print_matrix(b);
    // print_matrix(matmul(a, b));
    return 0;
}
