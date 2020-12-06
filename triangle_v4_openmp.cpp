#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <omp.h>

extern "C" {
#include "mmio.h"
#include "coo2csc.h"
}

int main(int argc, char** argv){
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nnz;
    uint32_t i;
    std::vector<uint32_t> I, J;
    std::vector<double> val;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [martix-market-filename] [0 for binary or 1 for non binary]\n", argv[0]);
        exit(1);
    }
    else
    {
        if ((f = fopen(argv[1], "r")) == nullptr)
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    int threads{};
    if(argc == 3){
        threads= atoi(argv[2]);
    } else {
        threads = 4;
    }

    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nnz)) != 0)
        exit(1);


    I = std::vector<uint32_t>(nnz);
    J = std::vector<uint32_t>(nnz);
    val = std::vector<double>(nnz);

    std::vector<uint32_t> cscRow = std::vector<uint32_t>(2*nnz);
    std::vector<uint32_t> cscColumn = std::vector<uint32_t>(N+1);
    std::vector<uint32_t> c_values = std::vector<uint32_t>(0);

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    /* Replace missing val column with 1s and change the fscanf to match pattern matrices*/

    if (!mm_is_pattern(matcode)) {
        for (i = 0; i < nnz; i++) {
            fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    } else {
        for (i = 0; i < nnz; i++) {
            fscanf(f, "%d %d\n", &I[i], &J[i]);
            val[i] = 1;
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    }

    if (f !=stdin) fclose(f);

    if(M != N) {
        printf("COO matrix' columns and rows are not the same");
    }

    // create symmetric values
    std::vector<uint32_t> temp1 = std::vector<uint32_t>(I.begin(), I.end());
    I.insert(std::end(I), std::begin(J), std::end(J));
    J.insert(std::end(J), std::begin(temp1), std::end(temp1));
    temp1.clear();

    if (I[0] < J[0]) {
        coo2csc(&cscRow[0], &cscColumn[0], &I[0], &J[0], 2 * nnz, M, 0);
    } else {
        coo2csc(&cscRow[0], &cscColumn[0], &J[0], &I[0], 2 * nnz, N, 0);
    }

    std::vector<int>c3 = std::vector<int>(0);
    std::vector<int>ones = std::vector<int>(N, 1);
    std::vector<int>result_vector = std::vector<int>(N, 0);
    c_values = std::vector<uint32_t>(2*nnz);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<uint32_t> l;
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(threads); // Use num_of_threads threads for all consecutive parallel regions

    #pragma omp parallel for private(l)
    for(i = 0; i < N; i++) {
        for(int j = 0; j < cscColumn.at(i+1) - cscColumn.at(i); j++) {
            int a_row = cscRow.at(cscColumn.at(i) + j);
            int a_col = i;

            std::vector<uint32_t> k = std::vector<uint32_t>(cscColumn.at(a_row+1) - cscColumn.at(a_row));
            l = std::vector<uint32_t>(cscColumn.at(a_col+1) - cscColumn.at(a_col));

            int s;
            for(s = 0; s < k.size(); ++s) {
                k[s] = cscRow.at(cscColumn.at(a_row) + s);
            }
            for(s = 0; s < l.size(); ++s) {
                l[s] = cscRow.at(cscColumn.at(a_col) + s);
            }

            int m = 0;
            int n = 0;
            int mul_value = 0;

            while(m != k.size() && n != l.size()) {
                if(k[m] == l[n]) {
                    ++mul_value;
                    ++m;
                    ++n;
                } else if(k[m] > l[n]) {
                    ++n;
                } else{
                    ++m;
                }
            }

            if(mul_value) {
                #pragma omp critical
                c_values.at(cscColumn.at(i) + j) = mul_value;
            }
        }
    }

    for(i = 0; i < N; i++) {
        for(int j = 0; j < cscColumn.at(i+1) - cscColumn.at(i); j++) {
            int row = cscRow.at(cscColumn.at(i) + j);
            int col = i;
            int value = c_values.at(cscColumn.at(i) + j);
            #pragma omp critical
            result_vector.at(row) += value * ones.at(col);
        }
    }

    for(int res: result_vector){
        c3.push_back(res/2);
    }

    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = stop - start;
    std::cout<<"Took "<< elapsed.count() <<std::endl;

//    for(int item: c3){
//        std::cout<< item << " ";
//    }
    std::cout<<std::endl;

    return 0;
}