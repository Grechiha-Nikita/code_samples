//
//  matrix.h
//  B
//
//  Created by Nikita on 22.12.17.
//  Copyright © 2017 Grechiha_Nikita. All rights reserved.
//

#ifndef matrix_h
#define matrix_h

#include <vector>
#include <cmath>


class Matrix {
public:
    explicit Matrix(size_t n = 1, size_t m = 1) {
        if (!(n > 0 && m > 0)) {
            throw MatrixException();
        }
        data_ = std::vector<std::vector<float>> (n, std::vector<float> (m));
    }
    
    Matrix(const Matrix& other): data_(other.data_) {}
    
    Matrix(Matrix&& other) {
        Swap(other);
    }
    
    ~Matrix() = default;
    
    Matrix& operator=(Matrix rhs) {
        Swap(rhs);
        return *this;
    }
    
    Matrix& Merge(const Matrix& matrix) {
        for (int i = 0; i < Lines(); ++i) {
            for (const auto& it: matrix.data_[i]) {
                data_[i].push_back(it);
            }
        }
        return *this;
    }
    
    void Swap(Matrix& other) {
        data_.swap(other.data_);
    }
    
    size_t Lines() const {
        return data_.size();
    }
    
    size_t Columns() const {
        return data_.front().size();
    }
    
    const float& operator()(size_t i, size_t j) const {
        return data_[i][j];
    }
    
    float& operator()(size_t i, size_t j) {
        return data_[i][j];
    }
    
    Matrix& operator+=(const Matrix& rhs) {
        if (!this->IsEqualSized(rhs)) {
            throw MatrixException();
        }
        for (int i = 0; i < Lines(); ++i) {
            for (int j = 0; j < Columns(); ++j) {
                data_[i][j] += rhs(i, j);
            }
        }
        return *this;
    }
    
    Matrix& operator-=(const Matrix& rhs) {
        if (!this->IsEqualSized(rhs)) {
            throw MatrixException();
        }
        for (int i = 0; i < Lines(); ++i) {
            for (int j = 0; j < Columns(); ++j) {
                data_[i][j] -= rhs(i, j);
            }
        }
        return *this;
    }
    
    Matrix operator+(const Matrix& rhs) const {
        Matrix result(*this);
        result += rhs;
        return result;
    }
    
    Matrix operator-(const Matrix& rhs) const {
        Matrix result(*this);
        result -= rhs;
        return result;
    }
    
    Matrix& operator*=(const Matrix& rhs) {
        return *this = *this * rhs;
    }
    
    Matrix& operator*=(float scalar) {
        for (int i = 0; i < Lines(); ++i) {
            for (int j = 0; j < Columns(); ++j) {
                data_[i][j] *= scalar;
            }
        }
        return *this;
    }
    
    friend Matrix operator*(const Matrix& first, const Matrix& second) {
        if (!first.IsApropriate(second)) {
            throw MatrixException();
        }
        Matrix result(first.Lines(), second.Columns());
        for (int i = 0; i < result.Lines(); ++i) {
            for (int j = 0; j < result.Columns(); ++j) {
                for (int k = 0; k < second.Lines(); ++k) {
                    result(i, j) += first(i, k) * second(k, j);
                }
            }
        }
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
        for (int i = 0; i < matrix.Lines(); ++i) {
            for (int j = 0; j < matrix.Columns(); ++j) {
                out << matrix(i, j) << ' ';
            }
            out << '\n';
        }
        return out;
    }
    
    float InfinityNorm() const {
        float result = -1.0;
        for (int i = 0; i < Lines(); ++i) {
            float cur = 0.0;
            for (int j = 0; j < Columns(); ++j) {
                cur += fabs(data_[i][j]);
            }
            if (less(result, cur)) {
                result = cur;
            }
        }
        return result;
    }
    
    float EuclideanNorm() const {
        float sum = 0.0;
        for (int i = 0; i < Lines(); ++i) {
            for (int j = 0; j < Columns(); ++j) {
                sum += data_[i][j] * data_[i][j];
            }
        }
        return sqrtf(sum);
    }
    
    Matrix Transpose() const {
        Matrix result(Columns(), Lines());
        for (int i = 0; i < Lines(); ++i) {
            for (int j = 0; j < Columns(); ++j) {
                result(j, i) = data_[i][j];
            }
        }
        return result;
    }
    
    bool IsEqualSized(const Matrix& other) const {
        return Lines() == other.Lines() && Columns() == other.Columns();
    }
    
    bool IsApropriate(const Matrix& other) const {
        return Columns() == other.Lines();
    }
    
    bool IsVector() const {
        return Columns() == 1;
    }
    
    static Matrix Identity(int n) {
        Matrix result(n, n);
        for (int i = 0; i < result.Lines(); ++i) {
            result(i, i) = 1.0;
        }
        return result;
    }
    
private:
    constexpr static float kComparisonThreshold = 1e-10;
    
    static bool less(float first, float second) {
        return first + kComparisonThreshold < second;
    }
    
    static bool equal(float first, float second) {
        return fabs(first - second) < kComparisonThreshold;
    }
    
protected:
    std::vector<std::vector<float>> data_;
    
    class MatrixException: public std::exception {};
};





class Vector: public Matrix {
public:
    explicit Vector(size_t n = 1): Matrix(n, 1) {}
    
    Vector(const Matrix& other): Matrix(other) {
        if (!IsVector()) {
            throw MatrixException();
        }
    }
    
    Vector(Matrix&& other): Matrix(other) {
        if (!IsVector()) {
            throw MatrixException();
        }
    }
    
    ~Vector() = default;
    
    Matrix& operator=(Matrix rhs) {
        Swap(rhs);
        if (!IsVector()) {
            throw MatrixException();
        }
        return *this;
    }
    
    void Swap(Matrix& other) {
        other.Swap(*this);
        if (!IsVector()) {
            throw MatrixException();
        }
    }
    
    size_t Size() const {
        return data_.size();
    }
    
    const float& operator()(size_t i) const {
        return data_[i][0];
    }
    
    float& operator()(size_t i) {
        return data_[i][0];
    }
};


#endif /* matrix_h */
