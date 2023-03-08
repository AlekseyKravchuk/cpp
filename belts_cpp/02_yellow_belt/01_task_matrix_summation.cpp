#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Matrix {
   public:
    Matrix() {
        _num_rows = 0;
        _num_cols = 0;
        _matrix.resize(_num_rows, std::vector<int>(_num_cols, 0));
    }

    Matrix(int num_rows, int num_cols) {
        if (num_rows >= 0 && num_cols >= 0) {
            _num_rows = num_rows;
            _num_cols = num_cols;
            _matrix.resize(_num_rows, std::vector<int>(_num_cols, 0));
        } else {
            throw std::out_of_range("Negative index of matrix."s);
        }
    }

    // меняет размеры матрицы на заданные и обнуляет все её элементы
    void Reset(int num_rows, int num_cols) {
        if (num_rows >= 0 && num_cols >= 0) {
            _matrix.assign(num_rows, std::vector<int>(num_cols, 0));
            _num_rows = num_rows;
            _num_cols = num_cols;
        } else {
            throw std::out_of_range("Negative index of matrix."s);
        }
    }

    int At(int row_num, int col_num) const {
        if (row_num >= 0 && row_num < _num_rows && col_num >= 0 && col_num < _num_cols) {
            return _matrix[row_num][col_num];
        } else {
            std::ostringstream oss;
            oss << "Wrong matrix index: ["s << row_num << "]["s << col_num << "]";
            throw std::out_of_range(oss.str());
        }
    }

    int& At(int row_num, int col_num) {
        if (row_num >= 0 && row_num < _num_rows && col_num >= 0 && col_num < _num_cols) {
            return _matrix[row_num][col_num];
        } else {
            std::ostringstream oss;
            oss << "Wrong matrix index: ["s << row_num << "]["s << col_num << "]";
            throw std::out_of_range(oss.str());
        }
    }

    bool HasEqualContent(const Matrix& other) const {
        return _matrix == other._matrix;
    }

    bool HasEqualSize(const Matrix& other) const {
        return (_num_rows == other._num_rows && _num_cols == other._num_cols);
    }

    int GetNumRows() const {
        return _num_rows;
    }

    int GetNumColumns() const {
        return _num_cols;
    }

   private:
    int _num_rows{};
    int _num_cols{};
    std::vector<std::vector<int>> _matrix;
};

std::istream& operator>>(std::istream& is, Matrix& matrix) {
    int num_rows{}, num_columns{};
    is >> num_rows >> num_columns >> std::ws;
    matrix.Reset(num_rows, num_columns);

    for (int i = 0; i < num_rows; ++i) {
        std::string line{};
        std::getline(is, line);
        std::istringstream iss(line);

        for (int j = 0; j < num_columns; ++j) {
            int elm{};
            if (iss >> elm) {
                matrix.At(i, j) = elm;
            }
        }
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.GetNumRows() << ' ' << matrix.GetNumColumns() << std::endl;
    for (int i = 0; i < matrix.GetNumRows(); ++i) {
        bool isFirstElmInRow = true;

        for (int j = 0; j < matrix.GetNumColumns(); ++j) {
            if (isFirstElmInRow) {
                os << matrix.At(i, j);
                isFirstElmInRow = false;
            } else {
                os << ' ' << matrix.At(i, j);
            }
        }
        std::cout << std::endl;
    }

    return os;
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return (lhs.HasEqualSize(rhs) && lhs.HasEqualContent(rhs));
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.HasEqualSize(rhs)) {
        Matrix matrix(lhs.GetNumRows(), lhs.GetNumColumns());
        for (int i = 0; i < matrix.GetNumRows(); ++i) {
            for (int j = 0; j < matrix.GetNumColumns(); ++j) {
                matrix.At(i, j) = lhs.At(i, j) + rhs.At(i, j);
            }
        }
        return matrix;
    } else {
        throw std::invalid_argument("Summable matrices have different sizes."s);
    }
}

int main() {
    // TODO: fix the bug:
    // {3 2, 9 8-7 65 4} != {3 2, 9 8, -7 6, 5 4}: The matrix must be written properly
    // {3 2, 98-7654} != {3 2, 9 8, -7 6, 5 4}
    // Рассмотреть случай, когда матрица задается одной размерностью.

    Matrix one;
    Matrix two;

    std::cin >> one >> two;
    std::cout << one + two << std::endl;
    return 0;
}
