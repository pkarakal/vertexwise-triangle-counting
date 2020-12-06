import numpy as np
import random as rd

from numpy.testing import assert_array_equal

adj_mat = np.zeros((5, 5))

for i in range(0, len(adj_mat)):
    for j in range(i, len(adj_mat[i])):
        if i != j:
            adj_mat[i][j] = rd.randint(0, 1)
            adj_mat[j][i] = adj_mat[i][j]

c3_loop = np.array([0] * 5)

for i in range(0, len(adj_mat)):
    for j in range(i, len(adj_mat)):
        for k in range(j, len(adj_mat)):
            if adj_mat[i][j] == 1 and adj_mat[j][k] == 1 and adj_mat[k][i] == 1:
                c3_loop[i] += 1
                c3_loop[j] += 1
                c3_loop[k] += 1

e = np.array([1] * 5)

arr = (adj_mat * (adj_mat.dot(adj_mat))).dot(e)

c3 = arr / 2

print(c3.transpose())
print(c3_loop.transpose())

assert_array_equal(c3, c3_loop)
