#pragma once

#include <iostream>
#include <cstring>

namespace cs205
{
    class matrix_error : public std::logic_error
    {
    public:
        matrix_error(const char *e) : std::logic_error(e) {}
    };

    template <typename _T1, typename _T2>
    struct __converter
    {
        _T1 operator()(const _T2 a)
        {
            return _T1(a);
        }
    };

    template <typename _T>
    class Mat
    {
    private:
        // three dimention size of data. (d_col, d_row, d_depth)
        size_t d_col, d_row;
        // three dimention size of matrix. (col, row, depth)
        size_t col, row, depth;
        // upper left bound of the ROI matrix in data. (lu_col, lu_row, 0)
        size_t lu_col, lu_row;
        // data reference count
        size_t *refcount;
        // The data
        _T *data;

        void release()
        {
            --(*(this->refcount));
            if (!(*this->refcount))
            {
                if (data)
                {
#ifdef __DEBUG__
                    std::cout << "delete[] " << data << std::endl;
#endif
                    delete[] data;
                }
            }
        }

        // No boundary check
        inline _T __getElement(const size_t dx, const size_t dy, const size_t dz = 0) const
        {
            return data[dz * this->d_col * this->d_row + (dx + this->lu_col) * this->d_col + (dy + this->lu_row)];
        }

        // No boundary check
        inline void __setElement(const _T u, const size_t dx, const size_t dy, const size_t dz = 0)
        {
            data[dz * this->d_col * this->d_row + (dx + this->lu_col) * this->d_col + (dy + this->lu_row)] = u;
        }

    public:
        /**
         * Assign to matrix together.
         */
        Mat(const Mat<_T> &a)
        {
            if (this == &a)
                return;
            this->col = a.col;
            this->row = a.row;
            this->depth = a.depth;

            this->d_col = a.d_col;
            this->d_row = a.d_row;

            this->lu_col = a.lu_col;
            this->lu_row = a.lu_row;

            this->refcount = a.refcount;
            this->data = a.data;

            if (a.refcount)
                ++(*a.refcount);
        }

        /**
         * Create a new matrix with empty data
         */
        Mat(const size_t row, const size_t col, const size_t dep = 1)
        {
            this->col = col;
            this->row = row;
            this->depth = dep;

            this->d_col = col;
            this->d_row = row;
            this->lu_col = this->lu_row = 0;

            this->refcount = new size_t;
            (*this->refcount) = 1;
            this->data = NULL;
            try
            {
                this->data = new _T[col * row * depth];
            }
            catch (std::bad_alloc &ba)
            {
                fprintf(stderr, "Cannot malloc new matrix data. size:(%ld,%ld,%ld)\n", col, row, depth);
                fprintf(stderr, "%s\n", ba.what());
            }
#ifdef __DEBUG__
            std::cout << "new _T[]: " << data << std::endl;
#endif
        }

        /**
         * Create a new matrix with initial data
         */
        Mat(const _T *data, const size_t row, const size_t col, const size_t dep = 1)
        {
            this->col = col;
            this->row = row;
            this->depth = dep;

            this->d_col = col;
            this->d_row = row;
            this->lu_col = this->lu_row = 0;

            this->refcount = new size_t;
            (*this->refcount) = 1;
            size_t sz = col * row * depth;
            this->data = NULL;
            try
            {
                this->data = new _T[sz];
            }
            catch (std::bad_alloc &ba)
            {
                fprintf(stderr, "Cannot malloc new matrix data. size:(%ld,%ld,%ld)\n", col, row, depth);
                fprintf(stderr, "%s\n", ba.what());
            }
#ifdef __DEBUG__
            std::cout << "new _T[]: " << data << std::endl;
#endif
            if (data != NULL && this->data != NULL)
            {
                for (size_t i = 0; i < sz; ++i)
                {
                    this->data[i] = data[i];
                }
            }
        }

        /**
         * Deconstructor
         */
        ~Mat()
        {
            release();
        }

        /**
         * Assign two matrix together
         */
        Mat &operator=(const Mat<_T> &a)
        {
            if (this == &a)
                return *this;
            if (this->data)
            {
                release();
            }
            this->col = a.col;
            this->row = a.row;
            this->depth = a.depth;

            this->d_col = a.d_col;
            this->d_row = a.d_row;

            this->lu_col = a.lu_col;
            this->lu_row = a.lu_row;

            this->refcount = a.refcount;
            this->data = a.data;
            if (a.refcount)
                ++(*a.refcount);
            return *this;
        }

        /**
         * get/setElement. With boundary check.
         */
        _T getElement(const size_t dx, const size_t dy, const size_t dz = 0) const
        {
            if (dx < this->col && dy < this->row && dz < this->depth)
                data[dz * this->d_col * this->d_row + (dx + this->lu_col) * this->d_col + (dy + this->lu_row)];
            else
                throw std::out_of_range("Out of range at Matrix::getElement");
        }
        void setElement(const _T u, const size_t dx, const size_t dy, const size_t dz = 0)
        {
            if (dx < this->col && dy < this->row && dz < this->depth)
                data[dz * this->d_col * this->d_row + (dx + this->lu_col) * this->d_col + (dy + this->lu_row)] = u;
            else
                throw std::out_of_range("Out of range at Matrix::getElement");
        }

        friend std::ostream &operator<<(std::ostream &os, const Mat<_T> &a)
        {
            if (a.depth != 1)
                os << "Mat(" << a.col << ", " << a.row << ", " << a.depth << ")" << std::endl;
            else
                os << "Mat(" << a.col << ", " << a.row << ")" << std::endl;
            os << "data:[";
            bool flg = 0;
            for (size_t k = 0; k < a.depth; ++k)
            {
                for (size_t i = 0; i < a.col; ++i)
                {
                    if (flg)
                        os << "      ";
                    flg = 1;
                    if (i == 0)
                    {
                        os << '[';
                    }
                    else
                        os << ' ';
                    for (size_t j = 0; j < a.row; ++j)
                    {
                        os << a.__getElement(i, j, k);
                        if (j != a.row - 1)
                            os << ", ";
                    }
                    if (i == a.col - 1)
                        os << ']';
                    os << std::endl;
                }
            }
            os << "     ]" << std::endl;
            return os;
        }

        /**
         * Matrix operations + - * ==
         * Can support two different type to do. (If they support.)
         */
        template <typename _T2>
        Mat<decltype(std::declval<_T>() + std::declval<_T2>())> operator+(const Mat<_T2> &a) const
        {
            if (a.col != this->col || a.row != this->row || a.depth != this->depth)
            {
                fprintf(stderr, "Invalid matrix plus. Dismatch matrix size.\n");
                throw matrix_error("Invalid matrix plus. Dismatch matrix size.");
            }
            if (a.data == NULL || this->data == NULL)
            {
                fprintf(stderr, "Invalid matrix plus. NULL data.\n");
                throw matrix_error("Invalid matrix plus. NULL data.");
            }
            Mat<decltype(std::declval<_T>() + std::declval<_T2>())> c = Mat(a.col, a.row, a.depth);
#pragma omp parallel for
            for (size_t k = 0; k < this->depth; ++k)
            {
                for (size_t i = 0; i < this->col; ++i)
                {
                    for (size_t j = 0; j < this->row; ++j)
                    {
                        c.data[k * this->col * this->row + i * this->row + j] =
                            this->data[k * this->d_col * this->d_row + (i + this->lu_col) * this->d_row + (j + this->lu_row)] +
                            a.data[k * a.d_col * a.d_row + (i + a.lu_col) * a.d_row + (j + a.lu_row)];
                    }
                }
            }
            return c;
        }

        template <typename _T2>
        Mat<decltype(std::declval<_T>() - std::declval<_T2>())> operator-(const Mat<_T2> &a) const
        {
            if (a.col != this->col || a.row != this->row || a.depth != this->depth)
            {
                fprintf(stderr, "Invalid matrix substract. Dismatch matrix size.\n");
                throw matrix_error("Invalid matrix substract. Dismatch matrix size.");
            }
            if (a.data == NULL || this->data == NULL)
            {
                fprintf(stderr, "Invalid matrix substract. NULL data.\n");
                throw matrix_error("Invalid matrix substract. NULL data.");
            }
            Mat<decltype(std::declval<_T>() - std::declval<_T2>())> c = Mat(a.col, a.row, a.depth);
#pragma omp parallel for
            for (size_t k = 0; k < this->depth; ++k)
            {
                for (size_t i = 0; i < this->col; ++i)
                {
                    for (size_t j = 0; j < this->row; ++j)
                    {
                        c.data[k * this->col * this->row + i * this->row + j] =
                            this->data[k * this->d_col * this->d_row + (i + this->lu_col) * this->d_row + (j + this->lu_row)] -
                            a.data[k * a.d_col * a.d_row + (i + a.lu_col) * a.d_row + (j + a.lu_row)];
                    }
                }
            }
            return c;
        }

        template <typename _T2>
        Mat<decltype(std::declval<_T>() * std::declval<_T2>())> operator*(const Mat<_T2> &a) const
        {
            if (a.depth != this->depth || a.col != this->row)
            {
                fprintf(stderr, "Invalid Matrix multiply. Dismatch matrix size.\n");
                throw matrix_error("Invalid matrix multiply. Dismatch matrix size.");
            }
            if (a.data == NULL || this->data == NULL)
            {
                fprintf(stderr, "Invalid matrix multiply. NULL data.\n");
                throw matrix_error("Invalid matrix multiply. NULL data.");
            }
            Mat<decltype(std::declval<_T>() * std::declval<_T2>())> c = Mat(this->col, a.row, this->depth);
            for (size_t i = 0; i < this->col * a.row * this->depth; i++)
            {
                c.data[i] = decltype(std::declval<_T>() * std::declval<_T2>())(0);
            }
#pragma omp parallel for
            for (size_t de = 0; de < this->depth; ++de)
            {
                for (size_t i = 0; i < this->col; ++i)
                {
                    for (size_t k = 0; k < this->row; ++k)
                    {
                        for (size_t j = 0; j < a.row; ++j)
                        {
                            c.data[de * a.row * this->col + i * a.row + j] +=
                                this->data[de * this->d_col * this->d_row + i * this->d_row + k] *
                                a.data[de * a.d_col * a.d_row + k * a.d_row + j];
                        }
                    }
                }
            }
            return c;
        }

        bool operator==(const Mat &a) const
        {
            if (a.col != this->col || a.row != this->row || a.depth != this->depth)
            {
                return false;
            }
            if (a.data == NULL || this->data == NULL)
            {
                fprintf(stderr, "Invalid matrix substract. NULL data.\n");
                throw matrix_error("Invalid matrix substract. NULL data.");
            }
#pragma omp parallel for
            for (size_t k = 0; k < this->depth; ++k)
            {
                for (size_t i = 0; i < this->col; ++i)
                {
                    for (size_t j = 0; j < this->row; ++j)
                    {
                        if (!(this->__getElement(i, j, k) == a.__getElement(i, j, k)))
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        /**
         * Memory hard clone a new matrix;
         */
        Mat clone() const
        {
            // Hard copy
            Mat<_T> c = Mat(this->col, this->row, this->depth);
            // Copy the ROI
            for (size_t k = 0; k < this->depth; ++k)
            {
                for (size_t i = 0; i < this->col; ++i)
                {
                    for (size_t j = 0; j < this->row; ++j)
                    {
                        c.__setElement(this->__getElement(i, j, k), i, j, k);
                    }
                }
            }
            return c;
        }

        /**
         * (col, row, depth) size of new matrix
         * (lu_col, lu_row, 0) left upper bound of the subMatrix in this->data
         * lu_col & lu_row start from 0
         */
        Mat subMatrixAssign(const size_t col, const size_t row, const size_t depth,
                            const size_t lu_col, const size_t lu_row) const
        {
            if (lu_col + col > this->col || lu_row + row > this->row || depth > this->depth)
            {
                fprintf(stderr, "subMatrix is out of the original matrix's boundary!\n");
                throw std::out_of_range("subMatrix is out of the original matrix's boundary!");
            }
            Mat c = *this;
            c.col = col;
            c.row = row;
            c.depth = depth;
            c.lu_col = lu_col;
            c.lu_row = lu_row;
            return c;
        }

        Mat subMatrixClone(const size_t col, const size_t row, const size_t depth,
                           const size_t lu_col, const size_t lu_row) const
        {
            return this->subMatrixAssign(col, row, depth, lu_col, lu_row).clone();
        }

        /**
         * Convert one type of matrix to another type.
         */
        template <typename _T2, typename _Assign = __converter<_T2, _T>>
        Mat<_T2> convert()
        {
            _Assign assign;
            Mat<_T2> dist(this->col, this->row, this->depth);
            for (size_t i = 0; i < this->depth; ++i)
            {
                for (size_t j = 0; j < this->col; ++j)
                {
                    for (size_t k = 0; k < this->row; ++k)
                    {
                        dist.setElement(assign(this->__getElement(j, k, i)), j, k, i);
                    }
                }
            }
            return dist;
        }
    };
}