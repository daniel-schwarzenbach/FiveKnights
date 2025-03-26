#pragma once
#include "./Vector.h++"

template<typename T>
class Matrix {
   private:
      Vector<T> data;
      uint X, Y;
   public:
      using iterator = typename std::vector<T>::iterator;
      using const_iterator = typename std::vector<T>::const_iterator;

      // basic constructors
      Matrix(uint X, uint Y) : data(X * Y), X{X}, Y{Y} { data.resize(X*Y);}
      Matrix(uint X, uint Y, T value) : data(X * Y, value) {}

      // Rule of 5
      Matrix() : data(), X{0}, Y{0} {}
      Matrix(Matrix<T>&& other) : data(move(other.data)), X{other.X}, Y{other.Y} {}
      Matrix(const Matrix<T>& other) : data(other.data), X{other.X}, Y{other.Y} {}
      ~Matrix() = default;

      template<typename T2>
      Matrix(std::initializer_list<std::initializer_list<T2>> init) {
         // Get dimensions from the initializer list
         X = init.size();
         Y = X > 0 ? init.begin()->size() : 0;
         
         // Resize data to hold all elements
         data.resize(X * Y);
         
         // Copy elements from initializer list to data
         uint x = 0;
         for (const auto& row : init) {
            if (row.size() != Y) {
               // All rows must have the same length
               throw std::invalid_argument("All rows must have the same number of columns");
            }
            
            uint y = 0;
            for (const auto& elem : row) {
               data[x * Y + y] = elem;
               ++y;
            }
            ++x;
         }
      }

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

      void resize(uint X, uint Y) {
         data.resize(X * Y);
         this->X = X;
         this->Y = Y;
      }

      void clear() {
         data.clear();
         X = 0;
         Y = 0;
      }

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

      template<typename T2>
      bool operator==(const Matrix<T2>& other) const {
         return this->X == other.X && this->Y == other.Y && this->data == other.data;
      }
};

template<typename T>
OS& operator<<(OS& os, Matrix<T> const& mat) {
   auto [X, Y] = mat.shape();
   os << "Matrix<" << X << ", " << Y << ">{";
   for(uint x = 0; x < X; ++x){
      os << "{";
      for(uint y = 0; y < Y-1; ++y){
         os << mat[x, y] << ", ";
      }
      os << mat[x, Y-1] << "}";
      if(x < X-1) os << ", ";
   }
   return os << "}";
}

template<typename T>
IS& operator>>(IS& is, Matrix<T>& mat) {
   mat.clear();
   uint X,Y;
   is >> "Matrix<" >> X >> "," >> Y >> ">{";
   mat.resize(X, Y);
   for(uint x = 0; x < X; ++x){
      is >> "{";
      for(uint y = 0; y < Y-1; ++y){
         is >> mat[x, y] >> ",";
      }
      is >> mat[x, Y-1] >> "}";
      if(x < X-1) is >> ",";
   }
   return is >> "}";
}