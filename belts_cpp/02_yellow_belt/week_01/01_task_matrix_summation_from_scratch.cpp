#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Matrix {
   public:
    Matrix() = default;

    Matrix(int num_rows, int num_cols) {
        Reset(num_rows, num_cols);
    }

    // меняет размеры матрицы на заданные и обнуляет все её элементы
    void Reset(int num_rows, int num_cols) {
        if (num_rows < 0) {
            throw std::out_of_range("num_rows must be >= 0"s);
        }

        if (num_cols < 0) {
            throw std::out_of_range("num_cols must be >= 0"s);
        }

        // матрицы с нулём строк или нулём столбцов считаются пустыми.
        if (num_rows == 0 || num_cols == 0) {
            num_rows = num_cols = 0;
        }

        _num_rows = num_rows;
        _num_cols = num_cols;
        _matrix.assign(num_rows, std::vector<int>(num_cols, 0));
    }

    int& At(int row, int column) {
        return _matrix.at(row).at(column);
    }

    int At(int row, int column) const {
        return _matrix.at(row).at(column);
    }

    int GetNumRows() const {
        return _num_rows;
    }

    int GetNumColumns() const {
        return _num_cols;
    }

    bool HaveEqualContent(const Matrix& other) const {
        return _matrix == other._matrix;
    }

   private:
    int _num_rows{0};
    int _num_cols{0};
    std::vector<std::vector<int>> _matrix;
};

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.GetNumRows() != rhs.GetNumRows()) {
        return false;
    }

    if (lhs.GetNumColumns() != rhs.GetNumColumns()) {
        return false;
    }

    return (lhs.HaveEqualContent(rhs));
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.GetNumRows() != rhs.GetNumRows()) {
        throw std::invalid_argument("Mismatched number of rows"s);
    }

    if (lhs.GetNumColumns() != rhs.GetNumColumns()) {
        throw std::invalid_argument("Mismatched number of columns"s);
    }

    Matrix matrix(lhs.GetNumRows(), lhs.GetNumColumns());
    for (int i = 0; i < matrix.GetNumRows(); ++i) {
        for (int j = 0; j < matrix.GetNumColumns(); ++j) {
            matrix.At(i, j) = lhs.At(i, j) + rhs.At(i, j);
        }
    }

    return matrix;
}

std::istream& operator>>(std::istream& is, Matrix& matrix) {
    int num_rows, num_columns;
    is >> num_rows >> num_columns;
    matrix.Reset(num_rows, num_columns);
    for (int row = 0; row < matrix.GetNumRows(); ++row) {
        for (int column = 0; column < matrix.GetNumColumns(); ++column) {
            is >> matrix.At(row, column);
        }
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.GetNumRows() << ' ' << matrix.GetNumColumns() << std::endl;
    for (int row = 0; row < matrix.GetNumRows(); ++row) {
        for (int column = 0; column < matrix.GetNumColumns(); ++column) {
            if (column > 0) {
                os << ' ';
            }
            os << matrix.At(row, column);
        }

        os << std::endl;
    }

    return os;
}

int main() {
    Matrix one;
    Matrix two;

    std::cin >> one >> two;
    std::cout << one + two << std::endl;
    return 0;
}