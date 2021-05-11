/**
  * @file matrix_errors.h
  *
  * exception definition for the matrix classes
  */
#ifndef MATRIX_ERRORS_H
#define MATRIX_ERRORS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <string>
#include <sstream>

#define MATRIX_ERROR_CODE_COUNT 19
#define MATRIX_ERR_UNDEFINED "1 Undefined exception!"
#define MATRIX_ERR_WRONG_ROW_INDEX "2 The row index is out of range."
#define MATRIX_ERR_MUL_ROW_AND_COL_NOT_EQUAL "3 The row number of second matrix must be equal with the column number of first matrix!"
#define MATRIX_ERR_MUL_ROW_AND_COL_BE_GREATER_THAN_ZERO "4 The number of rows and columns must be greater than zero!"
#define MATRIX_ERR_TOO_FEW_DATA "5 Too few data in matrix."
#define MATRIX_ERR_ADD_ROW_AND_COL_NOT_EQUAL "6 Matrixs don't have same dimensions."
#define MATRIX_OUT_OF_ARRAY "7 Try to access out of array bounds."
#define MATRIX_OUT_ROW "8 Try to modify row out of array bounds."
#define MATRIX_OFFSET_ERR "9 Offset is over col number."
#define MATRIX_OFFSET_OVER_MAX "10 Offset is bigger than max."
#define MATRIX_DIFF_HEIGHT "11 The height of matrixs to concat must be equal."
#define MATRIX_WRONG_CUT "12 col to cut must be bewteen 0 and width."
#define MATRIX_ISNT_SQUARE "13 Matrix to invert is non-square."
#define MATRIX_NUL_DETERMINANT "14 Nul determinant : Can't solve/invert"
#define MATRIX_NOT_A_VECTOR "15 Both elements of a scalar product must be vectors!"
#define MATRIX_HOUSEHOLDER_MUST_BE_VECTOR "16 Only can build a householder matrix from a column vector"
#define MATRIX_CANT_CONVERT_TO_VECTOR "17 The matrix isn't a 1xN matrix, can't convert to Vector"
#define MATRIX_CANT_EXP_INTEGER_MATRIX "18 can't calculate exp of an integer matrix"
#define MATRIX_DENMAN_NOT_CONVERGENT "19 Denman-Beavers algorithm don't converge with this matrix"

class MatrixException {
private:
    std::string message_;
    int errorCode_;
public:
    inline MatrixException(std::string message = MATRIX_ERR_UNDEFINED){
        errorCode_ = MATRIX_ERROR_CODE_COUNT + 1;
        std::stringstream ss(message);
        ss >> errorCode_;
        if (errorCode_ < 1) {
            errorCode_ = MATRIX_ERROR_CODE_COUNT + 1;
        }
        std::string::size_type pos = message.find(' ');
        if (errorCode_ <= MATRIX_ERROR_CODE_COUNT && pos != std::string::npos) {
            message_ = message.substr(pos + 1);
        } else {
            message_ = message + " (This an unknown and unsupported exception!)";
        }
    }

    inline std::string message() {
        return message_;
    };

    inline int errorCode() {
        return errorCode_;
    };
};

#endif
