#pragma once

#include "./Containers.h++"


template<typename T, Convertible<T> T2>
Vector<T> operator*(T2 $ a, Vector<T> $ v){
    UInt n = v.size();
    Vector<T> av(n);
    for_(i,n){
        av[i] = a*v[i];
    }
    return av;
}

template<typename T, Convertible<T> T2>
Vector<T>& operator*=(Vector<T> $ v, T2 $ a){
    UInt n = v.size();
    for_(i,n){
        v = a*v[i];
    }
    return v;
}

// ⊙ standart multiplication
template<typename T, Convertible<T> T2>
Vector<T> operator*(Vector<T> $ v1, Vector<T2> $ v2){
    UInt n = v1.size();
    debug_assert(n == v2.size(), "v1 | v2: Size missmatch!");
    Vector<T> v12(n);
    for_(i,n){
        v12[i] = v1[i]*v2[i];
    }
    return v12;
}

// ⊙ standart multiplication
template<typename T, Convertible<T> T2>
Vector<T>& operator*=(Vector<T> $ v1, Vector<T2> $ v2){
    UInt n = v1.size();
    debug_assert(n == v2.size(), "v1 | v2: Size missmatch!");
    for_(i,n){
        v1[i] = v1[i]*v2[i];
    }
    return v1;
}

// scalar product
template<typename T, Convertible<T> T2>
T operator|(Vector<T> $ v1, Vector<T2> $ v2){
    UInt n = v1.size();
    debug_assert(n == v2.size(), "v1 | v2: Size missmatch!");
    T v12 = 0;
    for_(i,n){
        v12 += v1[i] * v2[i];
    }
    return v12;
}

// cross product
template<typename T, Convertible<T> T2>
T operator%(Vector<T> $ v1, Vector<T> $ v2){
    UInt n = v1.size();
    if(n != v2.size){
        throw std::runtime_error("v1 % v2: Size missmatch!");
    }
    Vector<T> v12 = 0;
    for_(i,n){
        v12 += v1.periodic(i+1) * v2.periodic(i+2)
                - v1.periodic(i+2) * v2.periodic(i+1);
    }
    return v12;
}

template<typename T>
Vector<T> get_row(Matrix<T> $ A, Int i){
    Field<UInt, 2> dims = A.dims();
    UInt cols = dims[2];
    Vector<T> row(cols);
    for_(j,cols){
        row << A[i,j];
    }
    return row;
}

template<typename T>
void set_row(Matrix<T>& A, Int i, Vector<T> $ toSetTo){
    Field<UInt, 2> dims = A.dims();
    UInt cols = dims[2];
    debug_assert(cols == toSetTo.size(), "Size Missmatch");
    for_(j,cols){
        A[i,j] = toSetTo[j];
    }
}

template<typename T>
Vector<T> get_col(Matrix<T> $ A, Int j){
    Field<UInt, 2> dims = A.dims();
    UInt rows = dims[1];
    Vector<T> col(rows);
    for_(i,rows){
        col << A[i,j];
    }
    return col;
}

template<typename T>
void set_col(Matrix<T>& A, Int j, Vector<T> $ toSetTo){
    Field<UInt, 2> dims = A.dims();
    UInt rows = dims[1];
    debug_assert(rows == toSetTo.size(), "Size Missmatch");
    for_(i,rows){
        A[i,j] = toSetTo[i];
    }
}