#pragma once
#include <core/Base.h++>

// Extension of std::vector
template<typename T>
class Vector : public std::vector<T> {
   public:
      using iterator = typename std::vector<T>::iterator;
      using const_iterator = typename std::vector<T>::const_iterator;
      // Constructors Rule of 5
      Vector() : std::vector<T>() {}
      Vector(UInt size) : std::vector<T>(size) {}
      Vector(UInt size, const T& value) : std::vector<T>(size, value) {}
      Vector(Vector<T>&& other) : std::vector<T>(std::move(other)) {}
      Vector(const Vector<T>& other) : std::vector<T>(other) {}
      ~Vector() = default;
      template<typename... Args>
      Vector(Args... args) {
         (this->push_back(args), ...);
      }
      // Copy assignment operator
      Vector& operator=(const Vector<T>& other) {
         // Call base class copy assignment
         std::vector<T>::operator=(other);
         return *this;
      }

         // Move assignment operator
      Vector& operator=(Vector<T>&& other) noexcept {
         // Call base class move assignment
         std::vector<T>::operator=(std::move(other));
         return *this;
      }
      

      // removes element at index and swaps it with the last element
      void remove(UInt index) {
         swap(this->at(index), this->back());
         this->pop_back();
      }

      // adds an element to the front of the vector
      // via swapping all elements to the right
      void push_front(const T& value) {
         push_back(value);
         for (UInt i = this->size() - 1; i > 0; i--) {
            swap(this->at(i), this->at(i - 1));
         }
      }

      // removes first element that matches the value
      void remove_element(const T& element) {
         for (UInt i = 0; i < this->size(); i++) {
            if (this->at(i) == element) {
               remove(i);
               return;
            }
         }
      }

      // remove element at ptr
      void remove_ptr(const Ptr<T> element) {
         std::iter_swap(element, this->end()-1);
         this->pop_back();
      }

      bool operator==(const Vector<T>& other) const {
         if (this->size() != other.size()) {
            return false;
         }
         for (UInt i = 0; i < this->size(); i++) {
            if (this->at(i) != other[i]) {
               return false;
            }
         }
         return true;
      }

      bool operator!=(const Vector<T>& other) const {
         return !(*this == other);
      }
};

template<typename T>
OS& operator<<(OS& os, Vector<T> const& vec) {
   os << "Vector{";
   for(UInt i = 0; i < vec.size()-1; ++i){
      os << vec[i] << ", ";
   }
   return os << vec[vec.size()-1] << "}";
}

template<typename T>
IS& operator>>(IS& is, Vector<T>& vec) {
   vec.clear();
   is >> "Vector{";
   while (is.peek() != '}') {
      T value;
      is >> value;
      vec.push_back(value);
      if (is.peek() == ',') {
         is.ignore();
      }
   }
   return is >> "}";
}