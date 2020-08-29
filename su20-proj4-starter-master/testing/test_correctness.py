"""
Feel free to add more test classes and/or tests that are not provided by the skeleton code!
Make sure you follow these naming conventions: https://docs.pytest.org/en/reorganize-docs/goodpractices.html#test-discovery
for your new tests/classes/python files or else they might be skipped.
"""
#from utils import *
from pytest import approx
import numc as nc
"""
For each operation, you should write tests to test correctness on matrices of different sizes.
Hint: use rand_dp_nc_matrix to generate dumbpy and numc matrices with the same data and use
      cmp_dp_nc_matrix to compare the results
"""
class TestAddCorrectness:
    def test_small_add(self):
        # TODO: YOUR CODE HERE
        size = 3
        mat1 = nc.Matrix(size, size, 1.1)
        mat2 = nc.Matrix(size, size, 1.1)
        mat3 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(2.2)
        mat1 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(2.2)
        mat1 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(3.3)



    def test_medium_add(self):
        # TODO: YOUR CODE HERE
        size = 300
        mat1 = nc.Matrix(size, size, 1.1)
        mat2 = nc.Matrix(size, size, 1.1)
        mat3 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(2.2)
        mat1 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(2.2)
        mat1 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(3.3)


    def test_large_add(self):
        # TODO: YOUR CODE HERE
        size = 3000
        mat1 = nc.Matrix(size, size, 1.1)
        mat2 = nc.Matrix(size, size, 1.1)
        mat3 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(2.2)
        mat1 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(2.2)
        mat1 = mat1 + mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(3.3)


class TestSubCorrectness:
    def test_small_sub(self):
        # TODO: YOUR CODE HERE
        size = 3
        mat1 = nc.Matrix(size, size, 10.2)
        mat2 = nc.Matrix(size, size, 1.1)
        mat3 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(9.1)
        mat1 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(9.1)
        mat1 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(8.0)


    def test_medium_sub(self):
        # TODO: YOUR CODE HERE
        size = 3000
        mat1 = nc.Matrix(size, size, 10.2)
        mat2 = nc.Matrix(size, size, 1.1)
        mat3 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(9.1)
        mat1 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(9.1)
        mat1 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(8.0)


    def test_large_sub(self):
        # TODO: YOUR CODE HERE
        size = 3000
        mat1 = nc.Matrix(size, size, 10.2)
        mat2 = nc.Matrix(size, size, 1.1)
        mat3 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(9.1)
        mat1 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(9.1)
        mat1 = mat1 - mat2
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(8.0)


class TestAbsCorrectness:
    def test_small_abs(self):
        # TODO: YOUR CODE HERE
        size = 3
        mat1 = nc.Matrix(size, size, -10.1)
        mat2 = abs(mat1)
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(10.1)
        mat3 = abs(mat2)
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(10.1)
        mat1 = abs(mat1)
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(10.1)

    def test_medium_abs(self):
        # TODO: YOUR CODE HERE
        size = 300
        mat1 = nc.Matrix(size, size, -10.1)
        mat2 = abs(mat1)
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(10.1)
        mat3 = abs(mat2)
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(10.1)
        mat1 = abs(mat1)
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(10.1)

    def test_large_abs(self):
        # TODO: YOUR CODE HERE
        size = 3000
        mat1 = nc.Matrix(size, size, -10.1)
        mat2 = abs(mat1)
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(10.1)
        mat3 = abs(mat2)
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(10.1)
        mat1 = abs(mat1)
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(10.1)

class TestNegCorrectness:
    def test_small_neg(self):
        # TODO: YOUR CODE HERE
        size = 3
        mat1 = nc.Matrix(size, size, 10.1)
        mat2 = -mat1
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(-10.1)
        mat3 = -mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(10.1)
        mat1 = -mat1
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(-10.1)

    def test_medium_neg(self):
        # TODO: YOUR CODE HERE
        size = 300
        mat1 = nc.Matrix(size, size, 10.1)
        mat2 = -mat1
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(-10.1)
        mat3 = -mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(10.1)
        mat1 = -mat1
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(-10.1)

    def test_large_neg(self):
        # TODO: YOUR CODE HERE
        size = 3000
        mat1 = nc.Matrix(size, size, 10.1)
        mat2 = -mat1
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(-10.1)
        mat3 = -mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(10.1)
        mat1 = -mat1
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(-10.1)

class TestMulCorrectness:
    def test_small_mul(self):
        # TODO: YOUR CODE HERE
        size = 3
        mat1 = nc.Matrix(size, size, 3.1)
        mat2 = nc.Matrix(size, size, 2.1)
        mat3 = mat1 * mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(19.53)
        mat3 = mat2 * mat1
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(19.53)
    
    def test_medium_mul(self):
        # TODO: YOUR CODE HERE
        size = 300
        mat1 = nc.Matrix(size, size, 3.1)
        mat2 = nc.Matrix(size, size, 2.1)
        mat3 = mat1 * mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(1953.0)
        mat3 = mat2 * mat1
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(1953.0)
    
    def test_large_mul(self):
        # TODO: YOUR CODE HERE
        size = 3000
        mat1 = nc.Matrix(size, size, 3.1)
        mat2 = nc.Matrix(size, size, 2.1)
        mat3 = mat1 * mat2
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(19530.0)
        mat3 = mat2 * mat1
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(19530.0)
    
class TestPowCorrectness:
    def test_small_pow(self):
        # TODO: YOUR CODE HERE
        size = 3
        mat1 = nc.Matrix(size, size, 1.1)
        mat2 = mat1 ** 2
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(3.63)
        mat3 = mat1 ** 5
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(130.45131)
        mat1 = mat1 ** 10
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(51052.632842148356)

    def test_medium_pow(self):
        # TODO: YOUR CODE HERE
        size = 333
        mat1 = nc.Matrix(size, size, 1.1)
        mat2 = mat1 ** 2
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(402.93)
        mat3 = mat1 ** 5
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(19803427365.673744)
        mat1 = mat1 ** 6
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(7253995444046.312)
    
    def test_large_pow(self):
        # TODO: YOUR CODE HERE
        size = 3333
        mat1 = nc.Matrix(size, size, 1.1)
        mat2 = mat1 ** 2
        for i in range(size):
            for j in range(size):
                assert mat2[i][j] == approx(4032.93)
        mat3 = mat1 ** 4
        for i in range(size):
            for j in range(size):
                assert mat3[i][j] == approx(54209659774.872)
        mat1 = mat1 ** 5
        for i in range(size):
            for j in range(size):
                assert mat1[i][j] == approx(198748875632613.72)
    
class TestGetCorrectness:
    def test_get(self):
        # TODO: YOUR CODE HERE
        mat = nc.Matrix(3, 3)
        mat[0][1] = 5
        assert mat[0][0] == 0.0
        assert mat[0][1] == 5.0
        assert mat[0][2] == 0.0
        assert mat[1][0] == 0.0
        assert mat[1][1] == 0.0
        assert mat[1][2] == 0.0
        assert mat[2][0] == 0.0
        assert mat[2][1] == 0.0
        assert mat[2][2] == 0.0

class TestSetCorrectness:
    def test_set(self):
        # TODO: YOUR CODE HERE
        mat = nc.Matrix(3, 3)
        mat[0] = [1,2,3]
        assert mat[0][0] == 1.0
        assert mat[0][1] == 2.0
        assert mat[0][2] == 3.0
        assert mat[1][0] == 0.0
        assert mat[1][1] == 0.0
        assert mat[1][2] == 0.0
        assert mat[2][0] == 0.0
        assert mat[2][1] == 0.0
        assert mat[2][2] == 0.0

