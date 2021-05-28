/**
  * @file matrix.h
  *
  * library imported from an existing personnal project not documented but functions names are pretty straightforward
  */
#ifndef _MATRIX_H
#define	_MATRIX_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "matrix_errors.h"
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <complex>
#include <functional>
#include <cmath>
#include <thread>

namespace chaskal {

typedef unsigned int uint;

template<class T>
class factors;

template<class T>
struct eigen_pair;

template<class T>
class Vector;

#define ANY 0
#define LOWER 1
#define UPPER 2
#define DIAG 3

struct precision_set{
    double epsilon;
    unsigned int max_iter;
};

constexpr double epsilon = 1e-10;

template <class T>
class Matrix {
protected:
    std::vector<T> v; // the data of matrix
    uint h = 0;   // the number of rows
    uint w = 0;   // the number of columns
 
    virtual void clear() {
        v.clear();
        h = w = 0;
    }


public:
 
    bool plain_log = false;
    
    Matrix() {
        clear();
    }
    Matrix(uint);
    Matrix(uint, uint, T* = 0, uint = 0);
    Matrix(uint, uint, const std::vector<T>&);
    Matrix(uint row, uint col,const std::function<T(uint,uint)>&);
    //Matrix(const Matrix&);
    virtual ~Matrix() {
        clear();
    }
    
    //void operator=(const Matrix&) = delete;
    std::vector<T> operator[](uint) const;
    Matrix operator*(const Matrix&) const;
    Matrix operator+(const Matrix&) const;
    Matrix operator-(const Matrix&) const;
    Matrix operator*(const T&) const;
    operator Vector<T>() const;

    void operator*=(const T& l) {
        for (T& x : v)
            x *= l;
    }

    T ix(uint,uint) const;
    T& at(uint,uint);

    T operator()(uint i,uint j) const {
        return ix(i,j);
    }
   
    T& operator()(uint i,uint j) {
        return at(i,j);
    }
    //APPLY TO ALL ELEMENTS OF A MATRIX A FUNCTION
    template<class A,class B>
        friend Matrix<B> apply(const Matrix<A>& M,B transform(A));


    //CONCAT MATRIX
    void augment(const Matrix<T>& other);
    Matrix split(uint col);


    //ROW/COL MANIPULATION
    std::vector<T> get_row(uint row) const;
    void scale_row(uint row,const T&);
    void swap_rows(uint a,uint b);
    void sub_rows(uint a,uint b,const T& scale);
    uint get_abs_max(uint offset,uint row,uint max) const;
    Vector<T> extract_col(uint i,uint offset) const;
    std::vector<T> get_diag() const;

    T det_row() const;

    Matrix reduce(uint i,uint j) const;
   
    uint get_shape() const;
    
    //USUAL TRANSFORMATIONS
    Matrix invert() const;
    double get_col_norm2(uint col = 0) const;
    Matrix transpose() const;
    T diag_product() const;
    int inner_rref(uint M = -1,bool to_det = false);
    void gauss_jordan();
    
    Matrix<std::complex<double>> transconjugate() const;
    
    Matrix place(const Matrix& other, uint i,uint j);

    //USUAL INVARIANTS
    T det() const;
    T scalar_product(const Matrix& other) const;
    T norm() const;
    T norm2() const;
    T frob_norm() const;


    //LINEAR SYSTEMS
    Vector<T> solve(const Vector<T>& B,float eps = 1e-5) const;
    Vector<T> gauss_pivot(const Vector<T>& B) const;
    Vector<T> lower_solve(const Vector<T>& B) const;
    Vector<T> upper_solve(const Vector<T>& B) const;
    Vector<T> tri_diagonal_solve(const Vector<T>& B) const;
    Vector<T> tri_diagonal_periodic_solve(const Vector<T>& B) const;
    Matrix invert_triangular(uint shape) const;

    Vector<T> gauss_seidel(const Vector<T>& B,double eps = epsilon) const;


    //FOURIER ANALYSIS
    Matrix<std::complex<double>> dft();
    Matrix<double> idft();


    //EIGENVALUES/EIGENVECTORS
    eigen_pair<T> power_method(Vector<T> u) const;
    eigen_pair<T> power_method() const;
    std::vector<T> get_eigen_values_QR() const;
    eigen_pair<T> rayleigh_quotient(T mu) const;
    std::vector<eigen_pair<T>> lanczos(int max_nb = -1) const;


    //DECOMPOSITIONS
    std::pair<Matrix<T>,Matrix<T>> QR() const;

    template <class U>
        friend std::ostream& operator<<(std::ostream&,const Matrix<U>&);

    inline uint rowNum() const {
        return h;
    }
 
    inline uint colNum() const {
        return w;
    }
 
    inline uint size() const {
        return v.size();
    }
 
    inline void add(const T& t) {
        v.push_back(t);
    }

    inline Matrix operator-() const{
        Matrix<T> rslt(Matrix<T>::rowNum(),Matrix<T>::colNum(),v);
        for (uint j = 0;j<v.size();j++)
            rslt.v[j] *= -1;
        return rslt;
    }

    inline std::vector<T> get_values() const {	return v;	}
};

template<class T>
std::vector<Vector<T>> gram_schmidt(const std::vector<Vector<T>>& F);

template<class T>
struct eigen_pair{
    T value;
    Vector<T> vector;
};


template <class T>
T Matrix<T>::ix(uint i,uint j) const{
    if (i >= colNum() || j>=rowNum()){
        throw MatrixException(MATRIX_OUT_OF_ARRAY);
        return T();
    }
    return v[j*w + i];
}

template <class T>
Matrix<T>::Matrix(uint n) {
    clear();
    w = n; h = n;
    v = std::vector<T>(n*n,T(0));
}

template <class T>
Matrix<T>::Matrix(uint row, uint col, T* data, uint dataLength) {
    clear();
    if (row > 0 && col > 0) {
        h = row;
        w = col;
        uint hxw = h * w;
        if (dataLength && data) {
            for (uint i = 0; i < dataLength && i < hxw; i++) {
                v.push_back(data[i]);
            }
        }
        else if (!data){
            v = std::vector<T>(h*w,T());
        }
    }
}

template <class T>
Matrix<T>::Matrix(uint row, uint col, const std::vector<T>& data) {
    clear();
    if (row > 0 && col > 0) {
        h = row;
        w = col;
        uint hxw = h * w;
        if (data.size() > 0) {
            for (uint i = 0; i < hxw && i < data.size(); i++) {
                v.push_back(data[i]);
            }
        }
    }
}

template <class T>
Matrix<T>::Matrix(uint row, uint col,const std::function<T(uint,uint)>& generator){
    if (row > 0 && col > 0) {
        h = row;w = col;
        v = std::vector<T>(h*w,T(0));
        for (uint j = 0;j<rowNum();j++){
            for (uint i = 0;i<colNum();i++){
                at(i,j) = generator(i,j);
            }
        }
    }
    else {
        throw MatrixException(MATRIX_ERR_MUL_ROW_AND_COL_BE_GREATER_THAN_ZERO);
    }
}

template <class T>
Matrix<T> build(uint row, uint col,const std::function<T(uint,uint)>& generator){
    if (row > 0 && col > 0) {
        Matrix<T> M(row,col);
        for (uint j = 0;j<M.rowNum();j++){
            for (uint i = 0;i<M.colNum();i++){
                M.at(i,j) = generator(i,j);
            }
        }
        return M;
    }
    else {
        throw MatrixException(MATRIX_ERR_MUL_ROW_AND_COL_BE_GREATER_THAN_ZERO);
        return Matrix<T>(1,1);
    }
}

template<class T>
Matrix<T> Id(uint n){
    Matrix<T> I(n,n);
    for (uint i = 0;i<n;i++){
        I.at(i,i) = T(1);
    }
    return I;
}

/*
template<class T>
Matrix<T>::Matrix(const Matrix<T>& other) {
    if (v.size() == other.v.size()){
    w = other.w; h = other.h;
    for (uint i = 0;i<v.size();i++){
        v[i] = other.v[i];
    }
    return;
    }
    clear();
    if (other.h > 0 && other.w > 0) {
    h = other.h;
    w = other.w;
    uint hxw = h * w;
    for (uint i = 0; i < hxw && i < other.size(); i++) {
        v.push_back(other.v[i]);
    }
    }
    //return *this;
}
*/

template<class T>
std::vector<T> Matrix<T>::operator[](uint index) const {
    std::vector<T> result;
    if (index >= h) {
        throw MatrixException(MATRIX_ERR_WRONG_ROW_INDEX);
    } else if ((index + 1) * w > size()) {
        throw MatrixException(MATRIX_ERR_TOO_FEW_DATA);
    } else {
        uint begin = index * w;
        uint end = begin + w;
        for (uint i = begin; i < end; i++) {
            result.push_back(v[i]);
        }
    }
    return result;
}

template<class T>
void parallel_matrix_product(const Matrix<T>& A,const Matrix<T>& B,Matrix<T>& R,uint frow,uint lrow){ 
    for (uint i = frow;i<lrow;i++){
        for (uint j = 0; j < B.colNum(); j++) {
            T temp = T();
            for (uint k = 0; k < A.colNum(); k++) {
                temp += A.ix(k,i) * B.ix(j,k);
            }
            R.at(j,i) = temp;
        }
    }
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    Matrix result(h, other.w);
    if (w != other.h) {
        throw MatrixException(MATRIX_ERR_MUL_ROW_AND_COL_NOT_EQUAL);
    }
    else if (h <= 0 || w <= 0 || other.w <= 0) {
        throw MatrixException(MATRIX_ERR_MUL_ROW_AND_COL_BE_GREATER_THAN_ZERO);
    }
    else if (h * w > size() || other.h * other.w > other.size()) {
        throw MatrixException(MATRIX_ERR_TOO_FEW_DATA);
    }
    else {
        if (rowNum() < 20){
            for (uint i = 0;i<rowNum();i++){
                for (uint j = 0; j < other.w; j++) {
                    T temp = T();
                    for (uint k = 0; k < w; k++) {
                        temp += ix(k,i) * other.ix(j,k);
                    }
                    result.at(j,i) = temp;
                }
            }
        }
        else {
            const uint NTHREAD = 20,dt = rowNum()/NTHREAD,rest = rowNum() - NTHREAD*dt;
            std::vector<std::thread> threads(NTHREAD + ((rest != 0) ? 1 : 0));
            for (uint i = 0; i < NTHREAD; i++)
                threads[i] = std::thread(parallel_matrix_product<T>,*this,other,std::ref(result),i*dt,(i+1)*dt);

            if (rest){
                threads.back() = std::thread(parallel_matrix_product<T>,*this,other,std::ref(result),NTHREAD*dt,rowNum());

            }


            for (std::thread& t : threads)
                t.join();
        }
    }
    return result;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    Matrix result(h, w);
    if (w != other.w || h != other.h) {
        throw MatrixException(MATRIX_ERR_ADD_ROW_AND_COL_NOT_EQUAL);
    }
    else if (h <= 0 || w <= 0 || other.w <= 0) {
        throw MatrixException(MATRIX_ERR_MUL_ROW_AND_COL_BE_GREATER_THAN_ZERO);
    }
    else {
        for (uint i = 0; i < w; i++) {
            for (uint j = 0; j < h; j++) {
                result.at(i,j) = ix(i,j) + other.ix(i,j);
            }
        }
    }
    return result;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
    Matrix result(h, w);
    if (w != other.w || h != other.h) {
        throw MatrixException(MATRIX_ERR_ADD_ROW_AND_COL_NOT_EQUAL);
    }
    else if (h <= 0 || w <= 0 || other.w <= 0) {
        throw MatrixException(MATRIX_ERR_MUL_ROW_AND_COL_BE_GREATER_THAN_ZERO);
    }
    else {
        for (uint i = 0; i < w; i++) {
            for (uint j = 0; j < h; j++) {
                result.at(i,j) = ix(i,j) - other.ix(i,j);
            }
        }
    }
    return result;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T& scale) const {
    Matrix result(h, w);
    for (uint i = 0; i < w; i++) {
        for (uint j = 0; j < h; j++) {
            result.at(i,j) = ix(i,j)*scale;
        }
    }
    return result;
}


template<class T>
std::ostream& operator<<(std::ostream& os,const Matrix<T>& M){
    if (M.plain_log){
        for (T x : M.v)
            os << ((std::abs(x) > 0.0001) ? x : 0)  << ' ';
        os << '\n';
        return os;
    }
    for (uint j = 0;j<M.h;j++){
        for (uint i = 0;i<M.w;i++){
            if (std::abs(M.ix(i,j)) > epsilon)
                os << M.ix(i,j) << ' ';
            else
                os << "0 ";
        }
        os << '\n';
    }
    return os;
}

template<class T>
Matrix<T>::operator Vector<T>() const {
    if (colNum() != 1){
        throw MatrixException(MATRIX_CANT_CONVERT_TO_VECTOR);
        return Vector<T>(1);
    }
    Vector<T> rslt(rowNum());
    for (uint j = 0;j<rowNum();j++)
        rslt.at(j) = ix(0,j);
    return rslt;
}

template<class T>
Matrix<T> Matrix<T>::reduce(uint x, uint y) const {
    Matrix<T> R(rowNum()-1,colNum()-1);
    for (uint i = 0;i < colNum()-1;i++){
        for (uint j = 0;j < rowNum()-1;j++){
            R(i,j) = ix(i + int(i >= x), j + int(j >= y));
        }
    }
    return R;
}

template<class T>
T Matrix<T>::det_row() const {
    if (rowNum() == 2)
        return ix(0,0)*ix(1,1) - ix(1,0)*ix(0,1);
    if (rowNum() == 1)
        return ix(0,0);
    T D = 0;
    for (uint i = 0;i<colNum();i++){
        if (std::abs(ix(i,0)) > 1e-8)
            D += ix(i,0)*reduce(i,0).det_row()*((i % 2) ? -1.0 : 1.0);
    }
    return D;
}

template<class A,class B>
Matrix<B> apply(const Matrix<A>& M,B (*transform)(A)){
    Matrix<B> rslt(M.rowNum(),M.colNum());
    for (uint j = 0;j<M.rowNum();j++){
        for (uint i = 0;i<M.colNum();i++){
            rslt.at(i,j) = transform(M.ix(i,j));
        }
    }
    return rslt;
}

template<class T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> rslt(w,h);
    for (uint j = 0;j<h;j++){
        for (uint i = 0;i<w;i++){
            rslt.at(j,i)=ix(i,j);
        }
    }
    return rslt;
}

template<class T>
T& Matrix<T>::at(uint i,uint j){
    if (i >= w || j >= h){
        throw MatrixException(MATRIX_OUT_OF_ARRAY);
        return v[0];
    }
    return v[j*w+i];
}

template<class T>
void Matrix<T>::augment(const Matrix<T>& other){
    if (rowNum() != other.rowNum()){
        throw MatrixException(MATRIX_DIFF_HEIGHT);
        return;
    }

    std::vector<T> old = v;
    v = std::vector<T>(size() + other.size());
    uint new_w = w+other.w;
    for (uint j = 0;j<rowNum();j++){
        for (uint i = 0;i<new_w;i++){
            if (i < colNum())
                v[j*new_w + i] = old[j*w+i];
            else
                v[j*new_w + i] = other.ix(i-w,j);
        }
    }
    w = new_w;
}

template<class T>
Matrix<T> Matrix<T>::split(uint col){
    Matrix<T> rslt(rowNum(),colNum()-col);
    if (col>=colNum()){
        throw MatrixException(MATRIX_WRONG_CUT);
    }
    else{
        std::vector<T> old = v;
        v = std::vector<T>(rowNum()*col,T());
        uint old_w = colNum();
        w = col;
        for (uint j = 0;j<rowNum();j++){
            for (uint i = 0;i<old_w;i++){
                if (i < col)
                    at(i,j) = old[j*old_w+i];
                else
                    rslt.at(i-col,j) = old[j*old_w + i];
            }
        }
    }
    return rslt;
}


template<class T>
void Matrix<T>::scale_row(uint row,const T& f){
    if (row >= rowNum()){
        throw MatrixException(MATRIX_OUT_ROW);
        return;
    }
    for (uint i = 0;i<colNum();i++){
        at(i,row) *= f;
    }
}

template<class T>
std::vector<T> Matrix<T>::get_row(uint row) const{
    std::vector<T> rslt(colNum());
    if (row >= rowNum())
        throw MatrixException(MATRIX_OUT_ROW);
    else{
        for (uint i = 0;i<colNum();i++)
            rslt[i] = ix(i,row);
    }
    return rslt;
}

template<class T>
void Matrix<T>::swap_rows(uint a,uint b){
    if (b >=rowNum()){
        throw MatrixException(MATRIX_OUT_ROW);
        return;
    }
    else if (a == b)
        return;
    std::vector<T> tmp = get_row(a);
    for (uint i = 0;i<colNum();i++){
        at(i,a) = ix(i,b);
        at(i,b) = tmp[i];
    }
}

template<class T>
void Matrix<T>::sub_rows(uint a,uint b,const T& scale){
    if (a >=rowNum() || b >= rowNum()){
        throw MatrixException(MATRIX_OUT_ROW);
        return;
    }
    for (uint i = 0;i < colNum();i++){
        at(i,a) -= ix(i,b)*scale;
    }
}

template<class T>
uint Matrix<T>::get_abs_max(uint offset,uint col,uint max) const{
    if (offset > max)
        throw MatrixException(MATRIX_OFFSET_OVER_MAX);
    else if (offset >= colNum())
        throw MatrixException(MATRIX_OFFSET_ERR);
    else if (col >=rowNum())
        throw MatrixException(MATRIX_OUT_ROW);
    else if (offset == max)
        return ix(col,offset);
    else{
        uint id = offset;
        float m = std::abs(ix(col,offset));
        for (uint j = offset+1;j<max;j++){
            if (std::abs(ix(col,j)) > m){
                id = j;
                m = std::abs(ix(col,j));
            }
        }
        return id;
    }
    return 0;
}

template<class T>
int Matrix<T>::inner_rref(uint M,bool to_det){
    int r = -1;
    int sig = 1;
    for (uint i = 0;i<M;i++){
        uint idmax = get_abs_max(uint(r+1),i,M);

        if (std::abs(ix(i,idmax)) > epsilon){
            r++;
            if (!to_det)
                scale_row(idmax,T(1)/ix(i,idmax));
            swap_rows(idmax,r);
            sig *= pow(-1,r-idmax);
            for (uint j = (r+1)*to_det;j<rowNum();j++){
                if (int(j) != r)
                    sub_rows(j,r,(to_det) ? ix(i,j)/ix(i,idmax):ix(i,j));
            }
        }
        else {
            sig = 0;
            if (!to_det)
                return 0;
        }
    }
    return sig;
}

template<class T>
void Matrix<T>::gauss_jordan(){
    uint M = rowNum();
    int r = -1;
    for (uint i = 0;i<M;i++){
        uint idmax = get_abs_max(uint(r+1),i,M);

        if (std::abs(ix(i,idmax)) > 0.0){
            r++;
            scale_row(idmax,T(1)/ix(i,idmax));
            swap_rows(idmax,r);
            for (uint j = 0;j<rowNum();j++){
                if (j != r)
                    sub_rows(j,r,ix(i,j));
            }
        }
    }
}

template<class T>
Vector<T> Matrix<T>::gauss_pivot(const Vector<T>& b) const {
    Matrix<T> A = *this;
    Vector<T> X = b;
    uint M = rowNum();
    int r = -1;
    for (uint i = 0;i<M;i++){
        uint idmax = A.get_abs_max(uint(r+1),i,M);

        if (std::abs(A.ix(i,idmax)) > 0.0){
            r++;
            X(idmax) /= A.ix(i,idmax);
            A.scale_row(idmax,T(1)/A.ix(i,idmax));
            T tmp = X(idmax);
            X(idmax) = X(r);
            X(r) = tmp;
            A.swap_rows(idmax,r);
            for (uint j = 0;j<A.rowNum();j++){
                if (int(j) != r){
                    X(j) -= X(r)*A.ix(i,j);
                    A.sub_rows(j,r,A.ix(i,j));
                }
            }
        }
    }

    return X;
}

template<class T>
Matrix<T> Matrix<T>::invert() const{
    if (colNum() != rowNum()){
        throw MatrixException(MATRIX_ISNT_SQUARE);
        return Matrix<T>(1,1);
    }
    uint n = rowNum();
    Matrix<T> tmp(n,n,v);
    tmp.augment(Id<T>(n));
    int sig = tmp.inner_rref(n);
    if (sig == 0){
        std::cout << *this << std::endl;
        throw MatrixException(MATRIX_NUL_DETERMINANT);
        return tmp;
    }
    return tmp.split(n);
}

template<class T>
Matrix<T> Matrix<T>::invert_triangular(uint shape) const{
    Matrix<T> rslt(rowNum(),colNum());
    Matrix<T> b(rowNum(),1);

    for (uint j = 0;j<rowNum();j++){
        b.at(0,j) = 1;
        Matrix<T> tmp(rowNum(),1);
        if (shape == UPPER)
            tmp = upper_solve(b);
        else
            tmp = lower_solve(b);
        for (uint k = 0;k<rowNum();k++)
            rslt.at(j,k) = tmp.ix(0,k);
        b.at(0,j) = 0;
    }
    return rslt;
}

template <class T>
T Matrix<T>::det() const {
    uint n = rowNum();
    if (n == 2)
        return ix(0,0)*ix(1,1) - ix(0,1)*ix(1,0);
    Matrix<T> tmp(n,n,v);
    int sig = tmp.inner_rref(n,true);
    T rslt = T(1);
    for (uint i = 0;i<n;i++)
        rslt *= tmp.ix(i,i);
    return sig*rslt;
}

template<class T>
double Matrix<T>::get_col_norm2(uint col) const {
    if (col < 0 || col >= colNum()){
        throw MatrixException(MATRIX_OUT_OF_ARRAY);
        return T(0);
    }
    else {
        T rslt = T(0);
        for (uint j = 0;j<rowNum();j++){
            rslt += ix(col,j)*ix(col,j);
        }
        return rslt;
    }
}

template<class T>
Matrix<T> Matrix<T>::place(const Matrix<T> &other, uint i, uint j){
    if (i >= colNum() || j >= rowNum()){
        throw MatrixException(MATRIX_OUT_OF_ARRAY);
        return Matrix<T>(1,1);
    }
    Matrix<T> rslt = *this;
    for (uint y = j;y < rowNum();y++){
        for (uint x = i;x< colNum();x++){
            rslt.at(y,x) = other.ix(x-i,y-j);
        }
    }
    return rslt;
}


template<class T>
uint Matrix<T>::get_shape() const{
    bool upper = false;
    bool lower = false;
    for (uint j = 0;j<rowNum()-1 && !upper;j++){
        for (uint i = j+1;i<colNum() && !upper;i++){
            if (std::abs(ix(i,j)) > epsilon){
                upper = true;
            }
        }
    }
    for (uint i = 0;i<colNum()-1 && !lower;i++){
        for (uint j = i+1;j<rowNum() && !lower;j++){
            if (std::abs(ix(i,j)) > epsilon){
                lower = true;
            }
        }
    }
    if (lower && upper)
        return ANY;
    if (lower)
        return LOWER;
    if (upper)
        return UPPER;
    return DIAG;
}

template<class T>
T Matrix<T>::diag_product() const{
    T p = T(1);
    for (uint i = 0;i<rowNum();i++)
        p *= ix(i,i);
    return p;
}

template<class T>
Vector<T> Matrix<T>::solve(const Vector<T>& B,float eps) const{
    uint shape = get_shape();
    if (shape == LOWER || shape == UPPER){

        if (std::abs(diag_product()) < epsilon){
            throw MatrixException(MATRIX_NUL_DETERMINANT);
            return Vector<T>(1);
        }

        if (shape == LOWER)
            return lower_solve(B);
        if (shape == UPPER){
            return upper_solve(B);
        }
    }
    if (rowNum() < 10)
        return gauss_pivot(B);
    return gauss_seidel(B,eps);
}

template<class T>
Vector<T> Matrix<T>::lower_solve(const Vector<T>& B) const {
    Vector<T> rslt(rowNum());
    if (std::abs(ix(0,0)) < 1e-9)
        rslt.at(0) = B.ix(0);
    else
        rslt.at(0) = B.ix(0)/ix(0,0);
    for (int j = 1;uint(j)<rowNum();j++){
        T tmp = B.ix(j);
        for (int i = j-1;i>=0;i--){
            tmp -= rslt.ix(i)*ix(i,j);
        }
        tmp /= ix(j,j);
        rslt.at(j) = tmp;
    }
    return rslt;
}

template<class T>
Vector<T> Matrix<T>::upper_solve(const Vector<T>& B) const {
    Vector<T> rslt(rowNum());
    uint n = rowNum()-1;
    if (std::abs(ix(n,n)) < 1e-9)
        rslt.at(n) = 1.0;
    else
        rslt.at(n) = B.ix(n)/ix(n,n);
    for (int j = n-1;j>=0;j--){
        T tmp = B.ix(j);
        for (int i = j+1;i<int(colNum());i++){
            tmp -= rslt.ix(i)*ix(i,j);
        }
        tmp /= ix(j,j);
        rslt.at(j) = tmp;
    }
    return rslt;
}

template<class T>
Vector<T> Matrix<T>::tri_diagonal_solve(const Vector<T>& B) const {
    //THOMAS ALGORITHM
    uint N = B.rowNum();
    Vector<T> X(N),gamma(N),beta(N);
    beta(0) = ix(0,0);
    gamma(0) = B(0)/beta(0);
    for (uint k = 1;k<N;k++){
        beta(k) = ix(k,k) - ix(k,k-1)*ix(k-1,k)/beta(k-1);
        gamma(k) = (B(k)-ix(k-1,k)*gamma(k-1))/beta(k);
    }
    X(N-1) = gamma(N-1);
    for (int k = int(N)-2;k>=0;k--){
        X(k) = gamma(k) - ix(k+1,k)*X(k+1)/beta(k);
    }

    return X;
}

template<class T>
Vector<T> Matrix<T>::tri_diagonal_periodic_solve(const Vector<T>& B) const{
    //THOMAS PERIODIC ALGORITHM
    uint n = B.rowNum();
    Matrix<T> M = (*this);
    Vector<T> N(n);
    N(0) = ix(n-1,0);	N(n-1) = ix(0,n-1);
    M(0,0) -= ix(n-1,0); M(n-1,n-1) -= ix(0,n-1);
    M(n-1,0) = 0.f;    M(0,n-1) = 0.f;
    Vector<T> X1 = M.tri_diagonal_solve(B),
            X2 = M.tri_diagonal_solve(N);
    T Xstar = (X1(0) + X1(n-1))/(1+X2(0) + X2(n-1));
    return X1 - X2*Xstar;
}

template<class T>
T ones(uint,uint){
    return T(1);
}

template<class T>
Vector<T> Matrix<T>::gauss_seidel(const Vector<T> &B,double eps) const {
    uint n = rowNum();
    Vector<T> X(n);
    double error = 2;
    uint iter = 0;
    T diag[n];
    for (uint i = 0;i<n;i++){
        diag[i] = ix(i,i);
        if (std::abs(diag[i]) < 0.000001){
            throw MatrixException(MATRIX_NUL_DETERMINANT);
            return X;
        }
    }
    while (error > eps){
        for (int j = 0;j<int(n);j++){
            T buffer = B.ix(j);
            for (int i = 0;i<int(n);i++){
                if (i != j)
                    buffer -= ix(i,j)*X.ix(i);
            }
            X.at(j) = buffer/diag[j];
        }
        iter++;

        error = ((*this)*X - B).norm();
    }
    return X;
}

template<class T>
Matrix<std::complex<double>> Matrix<T>::dft() {
    Matrix<std::complex<double>> rslt(rowNum(),colNum());

    using namespace std;

    const double TAU = 2*M_PI;

    const uint M = rowNum(),N = colNum();
    const double im = 1.0/M,in = 1.0/N;

    std::complex<double> units[M][N];

    for (uint J = 0;J<M;J++){
        for (uint I = 0;I<N;I++){
            for (int j = 0;j<M;j++){
                for (int i = 0;i<N;i++){
                    units[j][i] = std::polar(1.0,-TAU*(i*I*in + J*j*im));
                }
            }
            for (uint j = 0;j<M;j++){
                for (uint i = 0;i<N;i++){
                    rslt.at(I,J) += ix(i,j)*units[j][i];
                }
            }
        }
    }

    return rslt;
}


template<class T>
Matrix<double> Matrix<T>::idft() {
    Matrix<std::complex<double>> tmp(rowNum(),colNum());
    Matrix<double> rslt(rowNum(),colNum());

    for (uint j = 0;j<rowNum();j++){
        for (uint i = 0;i<colNum();i++){
            tmp.at(i,j) = std::conj(ix(i,j));
        }
    }

    tmp = tmp.dft();

    const double scale_factor = 1.0/(double(rowNum()*colNum()));
    for (uint j = 0;j<rowNum();j++){
        for (uint i = 0;i<colNum();i++){
            rslt.at(i,j) = std::real(tmp.ix(i,j)) * scale_factor ;
        }
    }

    return rslt;
}

template<class T>
T Matrix<T>::scalar_product(const Matrix<T>& other) const {
    if (colNum() > 1 || other.colNum() > 1){
        throw MatrixException(MATRIX_NOT_A_VECTOR);
        return T(0);
    }
    else if (rowNum() != other.rowNum()){
        throw MatrixException(MATRIX_DIFF_HEIGHT);
        return T(0);
    }
    T rslt = T(0);
    for (uint j = 0;j<rowNum();j++){
        rslt += ix(0,j)*other.ix(0,j);
    }
    return rslt;
}

template<class T>
T Matrix<T>::norm2() const {
    return scalar_product(*this);
}

template<class T>
T Matrix<T>::norm() const{
    return sqrt(norm2());
}

template<class T>
T rfill(uint ,uint ){
    const int D = 200;
    return (rand()%D);
}

template<class T>
Vector<T> rvec(uint n){
    Vector<T> r(n);
    int N = 1e5;
    for (uint i = 0;i<n;i++)
        r.at(i) = (rand()%N - N/2.0)/N;
    return r.normalize();
}

template<class T>
eigen_pair<T> Matrix<T>::power_method() const{
    return power_method(rvec<T>(rowNum()));
}

template<class T>
eigen_pair<T> Matrix<T>::power_method(Vector<T> u) const{
    u = u.normalize();
    Vector<T> x;

    T error = 1,beta;
    uint iter = 0;
    while (error > 1e-10 && iter < 40000){
        iter++;
        x = (*this)*u;
        beta = x.norm();//*((u.ix(0)*x.ix(0) < 0.0) ? -1.0 : 1.0 );
        if (beta < epsilon){
            return {0.0,u};//in kernel
        }
        x = x.normalize();
        error = 1 - std::abs(u.scalar_product(x));
        u = x;
    }
    if (iter == 1){
        printf("probable error\n");
        return power_method();
    }
    //printf("error %f in %i \n",error,iter);
    return {beta,u};
}

template<class T>
Matrix<T> projector_to_span(const Vector<T>& u){
    Matrix<T> P(u.rowNum());
    for (uint j = 0;j<u.rowNum();j++){
        for (uint i = 0;i<u.rowNum();i++)
            P.at(i,j) = u.ix(i)*u.ix(j);
    }
    return P;
}

template<class T>
Matrix<T> householder(const Vector<T>& u){
    if (u.colNum() != 1){
        throw MatrixException(MATRIX_HOUSEHOLDER_MUST_BE_VECTOR);
        return Matrix<T>(1,1);
    }
    T unorm = T(2)/u.scalar_product(u);
    return Id<T>(u.rowNum()) - projector_to_span(u)*unorm;
}

template<class T>
Vector<T> basis_vector(uint n,uint k);

template <class T>
Matrix<T> householder_aim_e1(const Vector<T> &v){
    const Vector<T> e1 = basis_vector<T>(v.rowNum(),0);
    if (std::abs(1-e1.colinearity(v)) < epsilon){
        return Id<T>(v.rowNum());
    }
    return householder(v.normalize()-e1);
}

template<class T>
Vector<T> Matrix<T>::extract_col(uint i,uint offset) const {
    Vector<T> col(rowNum()-offset);
    for (uint j = offset;j<rowNum();j++)
        col.at(j-offset) = ix(i,j);
    return col;
}

template<class T>
std::pair<Matrix<T>,Matrix<T>> Matrix<T>::QR() const {
    uint n = rowNum();
    Matrix<T> Q = Id<T>(n),R = *this,H(n,n),M(n,n),I = Id<T>(n);
    for (uint k = 0;k<n-1;k++){
        Vector<T> C = R.extract_col(k,k);
        M = I.place(householder_aim_e1(C),k,k);
        Q = Q*M; R = M*R;
    }

    return {Q,R};
}

template<class T>
std::vector<T> Matrix<T>::get_diag() const {
    std::vector<T> diag(colNum());
    for (uint i = 0;i<colNum();i++)
        diag[i] = ix(i,i);
    return diag;
}

template<class T>
std::vector<T> Matrix<T>::get_eigen_values_QR() const {
    Matrix<T> A = *this;
    uint iter = 0;
    using std::cout;    using std::endl;
    while (true){
        std::pair<Matrix<T>,Matrix<T>> decomp = A.QR();
        A = decomp.second*decomp.first;
        bool done = true;
        for (uint j = 0;j<rowNum() && done;j++){
            for (uint i = 0;i<colNum() && done;i++){
                if (i != j){
                    if (std::abs(A.ix(i,j)) > 1e-4){
                        done = false;
                    }
                }
            }
        }
        if (done){
            std::cout << "done in " << iter << std::endl;
            return A.get_diag();
        }
        iter++;
        cout << iter << endl;
    }
    return std::vector<T>();
}

template<class T>
Matrix<std::complex<double>> Matrix<T>::transconjugate() const {
    Matrix<std::complex<double>> rslt;
    for (uint j = 0;j<rowNum();j++){
        for (uint i = 0;i<colNum();i++){
            rslt.at(j,i) = std::conj(rslt.ix(i,j));
        }
    }
    return rslt;
}

template<class T>
Matrix<T> exp(const Matrix<T>& X,const T t = T(1)) {
    Matrix<T> A = X*t, rslt = Id<T>(A.rowNum()) + A,powtmp = A;
    if (sizeof(rslt.ix(0,0)) == sizeof(int)){
        throw MatrixException(MATRIX_CANT_EXP_INTEGER_MATRIX);
        return rslt;
    }
    uint k = 1;
    T ifac = T(1);
    while (ifac*20 > epsilon){
        k++;
        powtmp = powtmp*A;
        ifac /= k;
        rslt = rslt + powtmp*ifac;
    }
    return rslt;
}

template<class T>
T Matrix<T>::frob_norm() const {
    T sum = T(0);
    for (uint j = 0;j<rowNum();j++){
        for (uint i = 0;i<colNum();i++){
            sum += std::abs(ix(i,j))*std::abs(ix(i,j));
        }
    }
    return std::sqrt(sum);
}

template<class T>
Matrix<T> denman_beavers(const Matrix<T>& A){
    if (abs(A.det()) < epsilon){
        throw MatrixException(MATRIX_NUL_DETERMINANT);
        return Matrix<T>();
    }
    uint n = A.rowNum();
    Matrix<T> Y = A,Z = Id<T>(n),tmp(n);
    T error = 2,nerror;
    uint iter = 0;
    while (error > epsilon){iter++;
        tmp = Y.invert();
        Y = (Y + Z.invert())*T(0.5);
        Z = (Z + tmp)*T(0.5);
        nerror = (Y*Y - A).frob_norm();
        if (nerror > error && iter > 2){
            throw MatrixException(MATRIX_DENMAN_NOT_CONVERGENT);
            return Y;
        }
        error = nerror;
    }
    return Y;
}

template<class T>
void compute_density(const Matrix<T>& A){
    uint non_zero_entries = 0;
    for (uint j = 0;j<A.rowNum();j++){
        for (uint i = 0;i<A.colNum();i++){
            if (std::abs(A.ix(i,j)) > epsilon)
                non_zero_entries++;

        }
    }
    printf("density of matrix is %f\n",double(non_zero_entries)/(A.rowNum()*A.colNum()));
}

template <class T>
eigen_pair<T> Matrix<T>::rayleigh_quotient(T mu) const {
    Matrix<T> I = Id<T>(rowNum());

    Vector<T> x = rvec<T>(rowNum());
    x.inner_normalize();

    Vector<T> y = (*this - I*mu).gauss_seidel(x,1e-7);
    T lambda = y.scalar_product(x);
    T error = 1.0;
    while (error > epsilon){
        x = y.normalize();
        y = (*this - I*mu).gauss_seidel(x,1e-7);
        lambda = y.scalar_product(x);
        mu = mu + 1.0/lambda;
        error = (y - x*lambda).norm() / y.norm();
        std::cout << error << ' ' << mu << std::endl;
    }
    return {mu,x};
}

template<class T>
std::vector<eigen_pair<T>> Matrix<T>::lanczos(int max_nb) const{
    eigen_pair<T> ep = power_method();
    uint n = rowNum();
    uint size = n;
    if (max_nb > 0)
        size = std::min(int(n),max_nb);
    Matrix<T> A = *this,P = Id<T>(n);
    std::vector<eigen_pair<T>> eigen_pairs(size);
    eigen_pairs[0] = ep;
    for (uint i = 1;i< size;i++){
        P = P * ortho_proj_matrix(eigen_pairs[i-1].vector);
        ep = (A*P).power_method();
        if (std::abs(ep.value) < 1e-6){//no more non-null eigenvalues: generate kernel
            std::vector<Vector<T>> kernel(size-i);
            for (uint k = i;k<size;k++)
                kernel[k-i] = (P*rvec<T>(n));
            kernel = gram_schmidt(kernel);
            for (uint k = i;k<size;k++)
                eigen_pairs[k] = {0.f,kernel[k-i]};
            return eigen_pairs;
        }
        else
            eigen_pairs[i] = ep;
        if (max_nb > 0 && int(i) >= max_nb)
            break;
    }
    return eigen_pairs;
}

template<class T>
class Vector : public Matrix<T> {
public:
    Vector(uint n = 1);
    Vector(const std::vector<T>& data);
    Vector(uint n,const std::function<T(T)>& generator,T a = T(0),T b = T(1));
    //Vector(const Vector& other);

    //void operator=(const Vector& other) = delete;
    Vector  operator-(const Vector& other) const;
    Vector  operator+(const Vector& other) const;

    void operator+=(const Vector& other) {
        if (other.rowNum() != Matrix<T>::rowNum())
            throw MatrixException(MATRIX_ERR_ADD_ROW_AND_COL_NOT_EQUAL);
        else
            for (uint j = 0;j<Matrix<T>::rowNum();j++)
                at(j) += other.ix(j);
    }

    inline void operator*=(const T& f) {
        for (uint j = 0;j<Matrix<T>::rowNum();j++)
            at(j) *= f;
    }

    inline T& operator()(uint j) {
        return at(j);
    }

    inline T operator()(uint j) const {
        return ix(j);
    }


    inline Vector operator*(const T& other) const{
        Vector<T> rslt(Matrix<T>::rowNum());
        for (uint j = 0;j<Matrix<T>::rowNum();j++)
            rslt.at(j) = ix(j) * other;
        return rslt;
    }

    inline Vector operator-() const{
        Vector<T> rslt(Matrix<T>::rowNum());
        for (uint j = 0;j<Matrix<T>::rowNum();j++)
            rslt.at(j) = -ix(j);
        return rslt;
    }

    T colinearity(const Vector& other) const;
    T ix(uint) const;
    T& at(uint);

    inline Vector remove_row(uint row) const {
        Vector r(Matrix<T>::rowNum()-1);
        for (uint j = 0;j<Matrix<T>::rowNum()-1;j++){
            r(j) = ix(j + (j>=row));
        }
        return r;
    }

    inline void add_row(const T& x = 0.f) {
        Matrix<T>::v.push_back(x);
        Matrix<T>::h++;
    }

    void fft(uint begin = 0,uint end = 0,uint step = 1);

    void inner_normalize();
    Vector normalize() const;

    Vector apply(const std::function<T(T)>&) const;

    inline T sum() const{
        T s = 0;
        for (uint k = 0;k<Matrix<T>::rowNum();k++)
            s += ix(k);
        return s;
    }

    T distance(const Vector<T>& b) const;
};

template<class T>
Vector<T>::Vector(uint n){
    Matrix<T>::w = 1;
    Matrix<T>::h = n;
    Matrix<T>::v = std::vector<T>(n,T(0));
}

template<class T>
Vector<T>::Vector(const std::vector<T>& data){
    Matrix<T>::v = data;
    Matrix<T>::w = 1;
    Matrix<T>::h = data.size();
}

template<class T>
Vector<T>::Vector(uint n,const std::function<T(T)>& generator,T a,T b){
    Matrix<T>::w = 1;
    Matrix<T>::h = n;
    Matrix<T>::v = std::vector<T>(n);
    const T dt = (b-a)/(n+1);
    for (uint j = 0;j<Matrix<T>::rowNum();j++){
        Matrix<T>::v[j] = generator(a + j*dt);
    }
}

/*
template<class T>
Vector<T>::Vector(const Vector<T> &other) : Matrix<T>(other.h,1){
    if ((*this).size() == other.size()){
    for (uint i = 0;i < (*this).size();i++)
        Matrix<T>::v[i] = other.v[i];
    }
    else {
    Matrix<T>::v = other.v;
    Matrix<T>::h = other.h;
    }
    //return *this;
}
*/

template<class T>
Vector<T> Vector<T>::operator-(const Vector<T>& other) const{
    if (Matrix<T>::size() != other.size()){
        throw MatrixException(MATRIX_ERR_ADD_ROW_AND_COL_NOT_EQUAL);
        return Vector<T>(1);
    }
    Vector<T> rslt(Matrix<T>::rowNum());
    for (uint j = 0;j < Matrix<T>::rowNum();j++)
        rslt.at(j) = ix(j) - other.ix(j);
    return rslt;
}

template<class T>
Vector<T> Vector<T>::operator+(const Vector<T>& other) const{
    if (Matrix<T>::size() != other.size()){
        throw MatrixException(MATRIX_ERR_ADD_ROW_AND_COL_NOT_EQUAL);
        return Vector<T>(1);
    }
    Vector<T> rslt(Matrix<T>::rowNum());
    for (uint j = 0;j < Matrix<T>::rowNum();j++)
        rslt.at(j) = ix(j) + other.ix(j);
    return rslt;
}

template<class T>
void Vector<T>::inner_normalize(){
    T l = (*this).norm();
    if (std::abs(l) < epsilon)
        return;
    for (uint j = 0;j < Matrix<T>::rowNum();j++)
        at(j) = ix(j)/l;
}

template<class T>
Vector<T> Vector<T>::normalize() const{
    Vector<T> rslt = (*this);
    rslt.inner_normalize();
    return rslt;
}

template<class T>
Vector<T> basis_vector(uint n,uint k){
    Vector<T> rslt(n);
    rslt.at(k) = T(1);
    return rslt;
}

template <class T>
Vector<T> Vector<T>::apply(const std::function<T(T)>& f) const{
    Vector<T> R(Matrix<T>::rowNum());
    for (uint k = 0;k<Matrix<T>::rowNum();k++)
        R(k) = f(ix(k));
    return R;
}

template<class T>
T Vector<T>::distance(const Vector<T> &b) const
{
    T d = 0;
    T tmp;
    for (uint k = 0;k<Matrix<T>::rowNum();k++){
        tmp = ix(k) - b.ix(k);
        d += tmp*tmp;
    }
    return std::sqrt(d);
}

template<class T>
T Vector<T>::colinearity(const Vector<T>& other) const{
    return (*this).scalar_product(other)/((*this).norm()*other.norm());
}

template<class T>
T Vector<T>::ix(uint j) const{
    return Matrix<T>::ix(0,j);
}

template<class T>
T& Vector<T>::at(uint j) {
    return Matrix<T>::at(0,j);
}

template<class T>
Vector<T> proj(const Vector<T>& u,const Vector<T>& v){
    return u*(u.scalar_product(v)/u.scalar_product(u));
}

template<class T>
Vector<T> ortho_proj(const Vector<T>& u,const Vector<T>& v,bool unitary = false){
    if (unitary)
        return v - u*u.scalar_product(v);
    return v - u*(u.scalar_product(v)/u.scalar_product(u));
}


template<class T>
Matrix<T> proj_matrix(const Vector<T>& u){
    Matrix<T> P(u.rowNum());
    T rescale = 1.0/u.scalar_product(u);
    for (uint j = 0;j<u.rowNum();j++){
        for (uint i = 0;i<u.rowNum();i++){
            P(i,j) = u.ix(i)*u.ix(j)*rescale;
        }
    }
    return P;
}

template<class T>
Matrix<T> ortho_proj_matrix(const Vector<T>& u){
    return Id<T>(u.rowNum()) - proj_matrix(u);
}

template<class T>
Vector<T> partial_gram_schmidt(const Vector<T>& v,const std::vector<Vector<T>>& F){
    Vector<T> u(v);
    for (uint i = 0;i<F.size();i++)
        u = u - proj(F[i],v);

    return u.normalize();
}

template<class T>
std::vector<Vector<T>> gram_schmidt(const std::vector<Vector<T>>& F){
    uint N = F.size();
    std::vector<Vector<T>> B(1);
    B[0] = F[0].normalize();
    for (uint k = 1;k<N;k++)
        B.push_back(partial_gram_schmidt(F[k],B));
    return B;
}

template <class T>
struct sparse_matrix{
    uint n,m,NNZ;
    std::vector<T> A,IA,JA;
};

template<class T>
sparse_matrix<T> sparsify(const Matrix<T>& M){
    sparse_matrix<T> SM;
    SM.n = M.rowNum(); SM.m = M.colNum();
    SM.NNZ = 0;
    SM.IA.push_back(0);
    for (uint j = 0;j<M.rowNum();j++){
        for (uint i = 0;i<M.colNum();i++){
            if (std::abs(M(i,j)) > epsilon){
                SM.A.push_back(M(i,j));
                SM.JA.push_back(i);

                SM.NNZ++;
            }
        }
        SM.IA.push_back(SM.NNZ);
    }

    return SM;
}

template<class T>
void mult(Vector<T>& y,const sparse_matrix<T>& SM,const Vector<T>& x){
    for (uint i = 0;i<SM.n;i++){
        y.at(i) = T(0);
        for (uint k = SM.IA[i];k<SM.IA[i+1];k++)
            y.at(i) += SM.A[k] * x.ix(SM.JA[k]);
    }
}

template<class T>
std::vector<eigen_pair<T>> sparse_lanczos(const sparse_matrix<T>& A){
    std::vector<eigen_pair<T>> rslt(A.n);

    for (uint k = 0;k<A.n;k++){
        Vector<T> x(A.n),u = rvec<T>(A.n);
        T error = 1,alpha=T(0),beta;
        uint iter = 0;
        printf("%i on %i\n",k,A.n);

        for (uint i = 0;i<k;i++)
            u = ortho_proj(rslt[i].vector,u,true);
        u.inner_normalize();
        while (error > 1e-20 && iter < 20000){
            iter++;
            mult(x,A,u);
            beta = x.scalar_product(x);
            x.inner_normalize();
            error = 1 - std::abs(u.scalar_product(x));
            u = x;
        }
        std::cout << iter << std::endl;

        rslt[k] = {std::sqrt(beta),u};
    }
    return rslt;
}

template<class T>
Matrix<T> build_basis(const std::vector<Vector<T>>& X){
    uint n = X.size();
    Matrix<T> B(n,n);
    for (uint j = 0;j< n;j++){
        for (uint i = 0;i<n;i++){
            B(i,j) = X[j](i);
        }
    }
    return B;
}

typedef Matrix<float> matR;
typedef Matrix<double> matRD;
typedef Matrix<long double> matRLD;
}

#endif	/* _MATRIX_H */
