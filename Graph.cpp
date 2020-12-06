#include "Graph.hpp"

void *Graph::adjMatMul() {
    for (int i = this->start; i < this->end; ++i) {
        for(int j = this->cscColumn.at(i); j < this->cscColumn.at(i+1); j++) {
            std::vector<uint32_t> k = std::vector<uint32_t>(this->cscColumn.at(this->cscRow.at(j)+1) - this->cscColumn.at(this->cscRow.at(j)));
            std::vector<uint32_t> l = std::vector<uint32_t>(this->cscColumn.at(i+1) - this->cscColumn.at(i));

            int s;
            for(s = 0; s < k.size(); ++s) {
                k[s] = this->cscRow.at(this->cscColumn.at(this->cscRow.at(j)) + s);
            }
            for(s = 0; s < l.size(); ++s) {
                l[s] = this->cscRow.at(this->cscColumn.at(i) + s);
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
                c_values.at(j) = mul_value;
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


