#include "Graph.hpp"

void *Graph::adjMatMul() {
    for (int i = this->start; i < this->end; ++i) {
        for (int j = 0; j < this->cscColumn.at(i + 1) - this->cscColumn.at(i); ++j) {
            int a_row = this->cscRow.at(cscColumn.at(i) + j);
            int a_col = i;

            std::vector<uint32_t> k = std::vector<uint32_t>(this->cscColumn.at(a_row + 1) - this->cscColumn.at(a_row));
            std::vector<uint32_t> l = std::vector<uint32_t>(this->cscColumn.at(a_col + 1) - this->cscColumn.at(a_col));

            int s;
            for (s = 0; s < k.size(); ++s) {
                k[s] = this->cscRow.at(this->cscColumn.at(a_row) + s);
            }
            for (s = 0; s < l.size(); ++s) {
                l[s] = this->cscRow.at(this->cscColumn.at(a_col) + s);
            }

            int m = 0;
            int n = 0;
            int mul_value = 0;

            while (m != k.size() && n != l.size()) {
                if (k[m] == l[n]) {
                    ++mul_value;
                    ++m;
                    ++n;
                } else if (k[m] > l[n]) {
                    ++n;
                } else {
                    ++m;
                }
            }

            if (mul_value) {
                c_values.at(this->cscColumn.at(i) + j) = mul_value;
            }
        }
    }
}

Graph::Graph(std::vector<uint32_t> cscRow, std::vector<uint32_t> cscColumn,
             std::vector<uint32_t> c_values, int start, int end, int nnz, int id) {
    this->cscRow = cscRow;
    this->cscColumn = cscColumn;
    this->c_values = c_values;
    this->nnz = nnz;
    this->start = start;
    this->end = end;
    this->id = id;
}


