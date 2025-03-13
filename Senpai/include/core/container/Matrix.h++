#pragma once
#include "./Vector.h++"

template<typename T>
class Matrix {
   private:
      Vector<T> data;
      uint X, Y;
   public:
      using iterator = typename Vector<T>::iterator;
      using const_iterator = typename Vector<T>::const_iterator;
      // basic constructors
      Matrix(uint X, uint Y) : data(X * Y), X{X}, Y{Y} {}
      Matrix(uint X, uint Y, T value) : data(X * Y, value) {}

      // Rule of 5
      Matrix() : data(), X{0}, Y{0} {}
      Matrix(Matrix<T>&& other) : data(move(other.data)), X{other.X}, Y{other.Y} {}
      Matrix(const Matrix<T>& other) : data(other.data), X{other.X}, Y{other.Y} {}
      ~Matrix() = default;

      Matrix& operator=(const Matrix<T>& other) {
         data = other.data;
         this->X = other.X;
         this->Y = other.Y;
         return *this;
      }
      Matrix& operator=(Matrix<T>&& other) noexcept {
         data = move(other.data);
         this->X = other.X;
         this->Y = other.Y;
         return *this;
      }

      // access operators
      T& operator[](uint x, uint y) {
         return data[x * Y + y];
      }
      T operator[](uint x, uint y) const {
         return data[x * Y + y];
      }

      UInt size() const {
         return data.size();
      }

      Tuple<uint, uint> shape() const {
         return {X, Y};
      }

      //
      iterator begin() {
         return data.begin();
      }
      iterator end() {
         return data.end();
      }

      const_iterator begin() const {
         return data.begin();
      }
      const_iterator end() const {
         return data.end();
      }
};

