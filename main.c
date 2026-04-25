#include <_time.h>
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
typedef struct {
    matrix a;
    matrix* b_cols;
    int cols;
} arg_t;

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
            c.tab[i][j] = rand()%10;
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

void* worker(void* arg) {
    arg_t argument = *(arg_t*)(arg);
    matrix* mat = malloc(sizeof(matrix)*argument.cols);
    for(int i = 0;i<argument.cols;i++) {
         mat[i] = matmul(argument.a, argument.b_cols[i]);
    }
    pthread_exit(mat);
    return NULL;
}

// ./tp ROWS COLS THREADS
// THREADS must be a divisor of ROWS and of COLS
// THREADS unused right now
int main(int argc, char** argv) {
    srand(time(NULL));
    assert(argc == 4);
    int ROWS, COLS, THREADS;
    sscanf(argv[1], "%d", &ROWS);
    sscanf(argv[2], "%d", &COLS);
    sscanf(argv[3], "%d", &THREADS);
    printf("Multiplying a %d x %d matrix over %d threads\n", ROWS, COLS, THREADS);
    assert(COLS % THREADS == 0 && ROWS % THREADS == 0);
    matrix a = random_matrix(ROWS, COLS);
    matrix b = random_matrix(ROWS, COLS);
    struct timespec start_multi,start_solo,end_solo,end_multi;
    clock_gettime(CLOCK_MONOTONIC,&start_multi);
    pthread_t* threads = malloc(sizeof(pthread_t)*THREADS);
    matrix* cols_b = matrix_split_cols(b);
    arg_t* args = malloc(sizeof(arg_t)*COLS);
    for(int i = 0;i<THREADS;i++) {
        args[i] = (arg_t){a,&cols_b[i*COLS/THREADS],COLS/THREADS};
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }
    matrix* cols_c = malloc(sizeof(matrix)*COLS);
    for(int i = 0;i<THREADS;i++) {
        void* tmp;
        pthread_join(threads[i], &tmp);
        for(int j = 0;j<COLS/THREADS;j++) {
            cols_c[i*COLS/THREADS+j] = ((matrix*)tmp)[j];
        }
    }
    matrix c_multi = matrix_join_cols(cols_c, COLS);
    clock_gettime(CLOCK_MONOTONIC, &end_multi);
    clock_gettime(CLOCK_MONOTONIC, &start_solo);
    matrix c_solo = matmul(a,b);
    clock_gettime(CLOCK_MONOTONIC, &end_solo);
    assert(matrix_eq(c_solo, c_multi));
    printf("Took %f s for multi and %f s for solo", (double)(end_multi.tv_sec - start_multi.tv_sec) + (end_multi.tv_nsec - start_multi.tv_nsec) / 1e9, (double)(end_solo.tv_sec - start_solo.tv_sec) + (end_solo.tv_nsec - start_solo.tv_nsec) / 1e9);
    return 0;
}
