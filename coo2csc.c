/**
 *  Copyright The AUTH-csal/pds-codebase authors
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "coo2csc.h"


/**
 * Converts a COO matrix to CSC
 * @param row CSC row start indices
 * @param col  CSC column indices
 * @param row_coo COO row indices
 * @param col_coo COO column indices
 * @param nnz Number of nonzero elements
 * @param n Number of rows/columns
 * @param isOneBased Whether COO is 0- or 1-based
 */
void coo2csc(
        uint32_t       * const row,
        uint32_t       * const col,
        uint32_t const * const row_coo,
        uint32_t const * const col_coo,
        uint32_t const         nnz,
        uint32_t const         n,
        uint32_t const         isOneBased
        ) {
    for (uint32_t l = 0; l < n+1; l++) col[l] = 0;


    // ----- find the correct column sizes
    for (uint32_t l = 0; l < nnz; l++)
        col[col_coo[l] - isOneBased]++;

    // ----- cumulative sum
    for (uint32_t i = 0, cumsum = 0; i < n; i++) {
        uint32_t temp = col[i];
        col[i] = cumsum;
        cumsum += temp;
    }
    col[n] = nnz;
    // ----- copy the row indices to the correct place
    for (uint32_t l = 0; l < nnz; l++) {
        uint32_t col_l;
        col_l = col_coo[l] - isOneBased;

        uint32_t dst = col[col_l];
        row[dst] = row_coo[l] - isOneBased;

        col[col_l]++;
    }
    // ----- revert the column pointers
    for (uint32_t i = 0, last = 0; i < n; i++) {
        uint32_t temp = col[i];
        col[i] = last;
        last = temp;
    }

}
