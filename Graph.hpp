#ifndef VERTEXWISE_TRIANGLE_COUNTING_GRAPH_H
#define VERTEXWISE_TRIANGLE_COUNTING_GRAPH_H

#include <iostream>
#include <vector>

class Graph {
public:

    std::vector<uint32_t> cscRow;
    std::vector<uint32_t> cscColumn;
    std::vector<uint32_t> c_values;
    int nnz;
    int start;
    int end;
    int id;
    void* adjMatMul();

    Graph( std::vector<uint32_t> cscRow,std::vector<uint32_t> cscColumn,
           std::vector<uint32_t> c_values, int start, int end,  int nnz, int id);
    Graph()= default;
    ~Graph()=default;
    static void *statAdjMatMul(void *context){
        return ((Graph *)context)->adjMatMul();
    }
};



#endif //VERTEXWISE_TRIANGLE_COUNTING_GRAPH_H
