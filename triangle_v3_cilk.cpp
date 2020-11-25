#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

extern "C" {
#include "mmio.h"
#include "coo2csc.h"
}

int main(int argc, char **argv) {
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;
    // coo matrices
    uint i, *I, *J;
    double *val, duration;
    // csc matrices
    uint32_t *csrRow, *csrColumn;
    double *csrValue;
    int *c3;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
        exit(1);
    } else {
        if ((f = fopen(argv[1], "r")) == NULL)
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0) {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }


    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode)) {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0)
        exit(1);


    /* reseve memory for matrices */

    I = (uint32_t *) malloc(nz * sizeof(uint32_t));
    J = (uint32_t *) malloc(nz * sizeof(uint32_t));
    val = (double *) malloc(nz * sizeof(double));


    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    /* Replace missing val column with 1s and change the fscanf to match patter matrices*/

    if (!mm_is_pattern(matcode)) {
        for (i = 0; i < nz; i++) {
            fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    } else {
        for (i = 0; i < nz; i++) {
            fscanf(f, "%d %d\n", &I[i], &J[i]);
            val[i] = 1;
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    }


    if (f != stdin)
        fclose(f);

    csrRow = (uint32_t *) malloc(nz * sizeof(uint32_t));
    csrColumn = (uint32_t *) malloc((N + 1) * sizeof(uint32_t));
    c3 = (int *) malloc(N * sizeof(int));
    for (i = 0; i < N; ++i) {
        c3[i] = 0;
    }

    try {
        if (I[0] < J[0]) {
            coo2csc(csrRow, csrColumn, I, J, nz, M, 0);
        } else {
            coo2csc(csrRow, csrColumn, J, I, nz, N, 0);
        }

        auto start = std::chrono::high_resolution_clock::now();


        int total{};
        cilk_for(int r = 1; r < N; ++r) {
            for (int j = 0; j < csrColumn[r + 1] - csrColumn[r]; ++j) {
                int row1 = csrRow[csrColumn[r] + j];
                int col1 = r;
                for (int k = 0; k < csrColumn[row1 + 1] - csrColumn[row1]; ++k) {
                    int row3 = csrRow[csrColumn[row1] + k];
                    int col3 = row1;
                    int max, min;
                    if (row3 > col1) {
                        max = row3;
                        min = col1;
                    } else {
                        max = col1;
                        min = row3;
                    }
                    for (int l = 0; l < csrColumn[max + 1] - csrColumn[max]; ++l) {
                        int row2 = csrRow[csrColumn[max] + l];
                        if (row2 == min) {
                            total++;
                            ++c3[min];
                            ++c3[max];
                            ++c3[col3];
                        }
                    }
                }
            }
        }

        auto stop = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = stop - start;
        std::cout<<"Took "<< elapsed.count() <<std::endl;

//        for (int col = 0; col < N; ++col) {
//            std::cout << c3[col] << std::endl;
//        }
    } catch (std::exception &e) {
        std::cout << "Error" << std::endl;
        std::cerr << e.what() << std::endl;
    }
    free(val);
    free(I);
    free(J);
    return 0;
}