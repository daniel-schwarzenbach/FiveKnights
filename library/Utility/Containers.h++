#pragma once

#include "./Base.h++"
#include <initializer_list>

template<typename T>
concept Container = requires(T a, typename T::value_type val) {
    typename T::value_type;
    typename T::iterator;
    { a.begin() } -> std::same_as<typename T::iterator>;
    { a.end() } -> std::same_as<typename T::iterator>;
    { a.size() } -> std::convertible_to<UInt>;
};

template<Container C> 
void fill(C& toFill, typename C::value_type const& constant){
    for(auto& element: toFill){
        element = constant;
    }
}


// Range ⊂ Container
struct Range {
    Int a;
    Int b;
    using value_type = Int;
    struct iterator {
    public:
        Int current;
        Int step; // +1 or -1
        constexpr Int operator*() const { return current; }
        constexpr iterator& operator++() {
            current += step;
            return *this;
        }
        constexpr bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };
    constexpr iterator begin() const {return iterator{a, a<=b?1:-1};}
    constexpr iterator end() const { 
        return iterator{a<=b?(b+1):(b-1), a<=b?1:-1}; 
    }
    constexpr Int operator[](Int i){
        return a<=b ? (a+i-1) : (a-i+1);
    }
    bool contains(Int x){
        return a <= b ? (a <= x && x <= b) : (b <= x && x <= a);
    }
    UInt size() {
        return a <= b ? b-a+1 : a-b+1;
    }
};
OS& operator<<(OS& os, Range $ range){
    os <<"Range{"<<range.a<<", "<<range.b<<"}";
    return os;
}

template<typename T, UInt N>
class Field{
private:
    T cArray[N];
public:
    // Constructors
    constexpr Field() = default;
    constexpr Field(Field&&) = default;
    constexpr Field& operator=(Field&&) = default;
    constexpr ~Field() = default;
    constexpr Field(Field $ other){
        for (UInt i = 0; i < N; ++i){
            this->cArray[i] = other.cArray[i];
        }
    }
    template <Convertible<T>... Dims>
    Field(Dims... dims){
        constexpr UInt n = sizeof...(dims);
        static_assert(n == N, "Number of dimensions must match template parameter N");
        UInt i = 0;
        ([&]() { cArray[i] = dims; i++; }(), ...);
    }
    // Container types
    using iterator = T*;
    using const_iterator = const T*;
    using value_type = T;

    let(T2)
    constexpr Field(Field<T2,N> $ other){
        for (UInt i = 0; i < N; ++i){
            this->cArray[i] = other.cArray[i];
        }
    }
    let(T2)
    constexpr Field& operator=(Field<T2,N> $ other){
        *this = Field(other);
        return *this;
    }

    // Get the size of the array
    constexpr UInt size() const {
        return N;
    }
    // Access an element
    constexpr T& operator[](UInt index) {
        if (index > N || index == 0) {
            throw std::out_of_range("Index out of bounds");
        }
        return cArray[index-1];
    }
    // Access an element
    constexpr const T& operator[](UInt index) const {
        if (index > N || index == 0) {
            throw std::out_of_range("Index out of bounds");
        }
        return cArray[index-1];
    }
    constexpr Ptr<T> end(){return cArray+N;}
    constexpr Ptr<T> begin(){return cArray;}
    constexpr const_iterator end() const {return cArray+N;}
    constexpr const_iterator begin() const {return cArray;}
};
template<typename T, UInt N>
OS& operator<<(OS& os, Field<T,N> $ svec){
    os << "Field{";
    for_(i,N-1){
        os << svec[i] << ", ";
    }
    return os << svec[N] << "}";
}

template<typename T, Convertible<T> T2, UInt N>
bool operator==(Field<T,N> $ f1, Field<T2,N> $ f2){
    for_(i,N){
        if(f1[i] != f2[i]) return false;
    }
    return true;
}
template<typename T, Convertible<T> T2, UInt N>
bool operator!=(Field<T,N> $ f1, Field<T2,N> $ f2){
    return !(f1 == f2);
}

template<typename T, UInt N>
class Array{
public:
   // data types
    using value_type = T;
    using iterator = Ptr<T>;
private:
    // advanced Array
    UInt _size = 0;
    Field<UInt,N> _dims{};
    iterator _input = nullptr;
    Ptr<T> _data = nullptr;
    iterator _last = nullptr;
public:
    Array() = default;

    template <Integer... Dims>
    Array(int d1, Dims... dims){
        constexpr UInt n = sizeof...(dims) + 1;
        static_assert(n == N, "Number of dimensions must match template parameter N");
        _size = d1;
        _dims[1] = d1;
        UInt i = 2;
        ([&]() { _dims[i] = dims; _size *= dims; i++; }(), ...);
        if(_size != 0){
            _data = new T[static_cast<UInt>(_size)];
            _input = _data;
            _last = _data + _size;
        }
    }
    template<Convertible<T> T2>
    Array(Field<UInt,N> $ dims, 
            std::initializer_list<T2> $ init){
        _size = 1;
        UInt i = 1;
        for(auto element : dims){
            _dims[i] = element;
            _size *= element;
            ++i;
        }
        if(_size != 0){
            _data = new T[static_cast<UInt>(_size)];
            _input = _data;
            _last = _data + _size;
        }
        i = 0;
        for(auto element : init){
            if(i < _size)
            _data[i] = element;
            ++i;

        }
        _input += i;
    }



    template<Container C>
    Array(Field<UInt,N> dims, C init){
        _size = 1;
        UInt i = 1;
        for(auto element : dims){
            _dims[i] = element;
            _size *= element;
            ++i;
        }
        if(_size != 0){
            _data = new T[static_cast<UInt>(_size)];
            _input = _data;
            _last = _data + _size;
        }
        i = 0;
        for(auto element : init){
            if(i < _size){
                _data[i] = element;
                ++i;
            }
        }
        _input += i;
    }
    // Copy Constructor
    Array(const Array& other) {
        _size = other._size;
        _data = new T[_size];
        _input = _data;
        _last = _data + _size;
        std::copy(other._data, other._last, _data);
    }

    Array& operator=(Array $ other){
        if(_data != nullptr){
            delete[] _data;
        }
        _size = other._size;
        _data = new T[_size];
        _input = _data;
        _last = _data + _size;
        std::copy(other._data, other._last, _data);
    }

    // Move Constructor
    Array(Array&& other) noexcept {
        _size = other._size;
        _data = other._data;
        _input = other._input;
        _last = other._last;

        // Leave 'other' in a valid state
        other._size = 0;
        other._data = nullptr;
        other._input = nullptr;
        other._last = nullptr;
    }

    Array& operator=(Array&& other){
        if(_data != nullptr){
            delete[] _data;
        }
        _size = other._size;
        _data = other._data;
        _input = other._input;
        _last = other._last;

        // Leave 'other' in a valid state
        other._size = 0;
        other._data = nullptr;
        other._input = nullptr;
        other._last = nullptr;
    }

    ~Array(){
        if(_data != nullptr)
            delete[] _data;
    }


    // acces elements
    template <Integer... Ids>
    constexpr T& operator[](Ids... ids) {
        constexpr UInt n = sizeof...(ids);
        static_assert(n == N, 
        "Number of indices must match array dimensions.");
        UInt id = 0;
        Field<UInt,N> indices;
        {
            UInt i = 1;
            ([&]() { indices[i] = ids; i++; }(), ...);
        }
        for_(i,n){
            debug_assert(indices[i] <= _dims[i] || indices[i] != 0,
            str(indices[i])+" not in dim["+str(i)+"] = "+str(_dims[i]));
            id += indices[i] - 1;
            if(i != n) id *= _dims[i+1];
        }
        return _data[id];
    }

    // acces refrence of element
    template <Integer... Ids>
    constexpr T $ operator[](Ids... ids) const {
        constexpr UInt n = sizeof...(ids);
        static_assert(n == N, 
        "Number of indices must match array dimensions.");
        UInt id = 0;
        Field<UInt,N> indices;
        {
            UInt i = 1;
            ([&]() { indices[i] = ids; i++; }(), ...);
        }
        for_(i,n){
            debug_assert(indices[i] <= _dims[i] || indices[i] != 0,
            str(indices[i])+"not in dim["+str(i)+"] = "+str(_dims[i]));
            id += indices[i] - 1;
            if(i != n) id *= _dims[i+1];
        }
        return _data[id];
    }

    // acces elements
    template <Integer... Ids>
    constexpr T& periodic(Ids... ids) {
        constexpr UInt n = sizeof...(ids);
        static_assert(n == N, 
        "Number of indices must match array dimensions.");
        UInt id = 0;
        Field<UInt,N> indices;
        {
            UInt i = 1;
            ([&]() { indices[i] = ids; i++; }(), ...);
        }
        for_(i,n){
            UInt idx = mod(indices[i]-1,_dims[i]);
            id += idx;
            if(i != n) id *= _dims[i+1];
        }
        return _data[id];
    }

    // acces refrence of element
    template <typename... Ids>
    constexpr T $ periodic(Ids... ids) const {
        constexpr UInt n = sizeof...(ids);
        static_assert(n == N, 
        "Number of indices must match array dimensions.");
        UInt id = 0;
        Field<UInt,N> indices;
        {
            UInt i = 1;
            ([&]() { indices[i] = ids; i++; }(), ...);
        }
        for_(i,n){
            UInt idx = mod(indices[i]-1,_dims[i]);
            id += idx;
            if(i != n) id *= _dims[i+1];
        }
        return _data[id];
    }

    constexpr T& operator[](UInt id) {
        debug_assert(id <= _size || id != 0, 
        str(id)+" ∉ Range{1, "+str(_size)+"}");
        return _data[id-1];
    }
    constexpr T operator[](UInt id) const {
        debug_assert(id <= _size || id != 0, 
        str(id)+" ∉ Range{1, "+str(_size)+"}");
        return _data[id-1];
    }

    let(T2)
    Array& operator<<(std::initializer_list<T2> list) {
        const UInt sizeList = std::min(list.size(), (_last - _input));
        for_(i,sizeList){
            _input[0] = list[i-1];
            ++_input;
        }
        return *this;
    }

    Array& operator<<(T const& t) {
        if(_input < _last){
            _input[0] = t;
            ++_input;
        }
        return *this;
    }

    template<Container C>
    Array& operator<<(C const& container) {
        for(auto element : container)
            if(_input < _last){
                _input[0] = element;
                ++_input;
            } else {
                break;
            }
        return *this;
    }

    Ptr<T> data() const {return _data;}
    iterator begin() const { return _data;}
    iterator end() const {return _last;}
    UInt size() const { return _size;}
    Field<UInt, N> dims() const {return _dims;}

    // template<typename... SubArrays>
    // Array(Array<T,N-1> $ subArray1, SubArrays... subArrays){
    //     constexpr UInt n = sizeof...(subArrays) + 1;
    //     // static_assert(
    //     // (std::is_convertible_v<SubArrays, Array<N - 1, T> > && ...),
    //     // "All subarrays must be of type Array<N - 1, T∼>.");
    //     UInt subSize = 1;
    //     for_(i,N-1){
    //         _dims[i+1] = subArray1.dims()[i];
    //         subSize *= _dims[i+1];
    //     }
    //     _dims[1] = n;
    //     _size = n*subSize;
    //     _data = new T[_size];
    //     _input = _data + _size;
    //     _last = _data + _size;
    //     for_(i,subSize){
    //         (*this)[i] = subArray1[i];
    //     }
    //     UInt i = subSize+1;
    //     ([&]() {
    //         Array<T,N-1> tmp(subArrays);
    //         for_(j,subSize){
    //             (*this)[i] = tmp[j];
    //             ++i;
    //         }
    //     }(), ...);
    // }
};


template<typename T, UInt N>
OS& operator<<(OS& os, Array<T,N> $ array){
    Field<UInt, N> dimsCounter;
    fill(dimsCounter, 0);
    UInt n = array.size();
    Field<UInt, N> dims = array.dims();
    for_(i,n){
        UInt ii = N;
        while(ii > 0 && dimsCounter[ii] == 0){
            dimsCounter[ii] = 1;
            os << "{";
            --ii; 
        }
        os << array[i];
        ++dimsCounter[N];
        ii = N;
        bool setComma = true;
        while(ii > 0 && dimsCounter[ii] > dims[ii]){
            os << "}";
            dimsCounter[ii] = 0;
            if(ii != 1) dimsCounter[ii-1] += 1;
            ii--;
            // set a comma if this isn't the last dimention
            setComma = ii != 0;
        }
        if(setComma && i != n) os << ", ";
    }
    return os;
}


template<typename T>
class Array<T,1>{
    public:
   // data types
    using value_type = T;
    using iterator = Ptr<T>;
private:
    static constexpr f64 overSize = 1.5;
    // c Array
    UInt cSize = 0;
    // advanced Array
    UInt _size = 0;
    iterator _input = nullptr;
    iterator _data = nullptr;
    iterator _last = nullptr;
public:
    Array() = default;

    Array(UInt dim){
        _size = dim;
        cSize = overSize*_size;
        _data = new T[cSize];
        _input = _data;
        _last = _data + _size;
    }
        // Copy Constructor
    Array(const Array& other) {
        _size = other._size;
        cSize = other.cSize;
        _data = new T[cSize];
        _input = _data;
        _last = _data + _size;
        std::copy(other._data, other._last, _data);
    }

    Array& operator=(Array $ other){
        if(_data != nullptr){
            delete[] _data;
        }
        _size = other._size;
        cSize = other.cSize;
        _data = new T[cSize];
        _input = _data;
        _last = _data + _size;
        std::copy(other._data, other._last, _data);

        return *this;
    }

    // Move Constructor
    Array(Array&& other) noexcept {
        _size = other._size;
        cSize = other.cSize;
        _data = other._data;
        _input = other._input;
        _last = other._last;

        // Leave 'other' in a valid state
        other._size = 0;
        other.cSize = 0;
        other._data = nullptr;
        other._input = nullptr;
        other._last = nullptr;
    }

    Array& operator=(Array&& other){
        if(_data != nullptr){
            delete[] _data;
        }
        _size = other._size;
        cSize = other.cSize;
        _data = other._data;
        _input = other._input;
        _last = other._last;

        // Leave 'other' in a valid state
        other._size = 0;
        other.cSize = 0;
        other._data = nullptr;
        other._input = nullptr;
        other._last = nullptr;

        return *this;
    }
    ~Array(){
        if(_data != nullptr){
            delete[] _data;
        }
    }

    template<Container C>
    Array(C $ container){
        _size = container.size();
        cSize = overSize*_size;
        _data = new T[static_cast<UInt>(cSize)];
        _input = _data + _size;
        _last = _data + _size;
        UInt i = 0;
        for(auto element : container){
            _data[i] = element;
            ++i;
        }
    }

    template <Convertible<T>... Elements>
    Array(Elements... elements){
        _size = sizeof...(elements);
        cSize = overSize * _size;
        _data = new T[static_cast<UInt>(cSize)];
        _last = _data + _size;
        _input = _data + _size;
        UInt i = 0;
        ([&]() { _data[i] = elements; i++; }(), ...);
    }

    constexpr T& operator[](UInt id) {
        debug_assert(id <= _size || id != 0, 
        str(id)+" ∉ Range{1, "+str(_size)+"}");
        return _data[id-1];
    }

    constexpr T $ operator[](UInt id) const {
        debug_assert(id <= _size || id != 0, 
        str(id)+" ∉ Range{1, "+str(_size)+"}");
        return _data[id-1];
    }

    constexpr T $ periodic(Int id) const {
        UInt idx = mod(id-1,_size);
        return _data[idx];
    }
    constexpr T& periodic(Int id) {
        UInt idx = mod(id-1,_size);
        return _data[idx];
    }

    void push_back(T const& toPush){
        if(_last+1 <= (_data + cSize)){
            _last[0] = toPush;
            ++_size;
            ++_last;
            _input = _last;
        } else {
            ++_size;
            UInt newCSize = _size * overSize;
            iterator newFirst = new T[static_cast<UInt>(newCSize)];
            // copy all elements to 
            for_(i,_size-1){
                newFirst[i-1] = _data[i-1];
            }
            newFirst[_size-1] = toPush;
            // delete old elemets
            delete[] _data;
            _data = newFirst;
            cSize = newCSize;
            _last = _data + _size;
            _input = _last;
        }
    }

    // pseudo push_front
    void push_front(T const& toPush){
        this->push_back(toPush);
        back_wards(i, _size-1, 1){
            _data[i] = _data[i-1];
        }
        _data[0] = toPush;
    }

    void reserve(UInt size){
        UInt newCSize = size;
        iterator newFirst = new T[static_cast<UInt>(newCSize)];
        // copy all elements to 
        for_(i,_size){
            newFirst[i-1] = _data[i-1];
        }
        // delete old elemets
        delete[] _data;
        _data = newFirst;
        cSize = newCSize;
        _last = _data + _size;
        _input = _last;
    }

    Array& operator<<(T const& t) {
        if(_input < _last){
            _input[0] = t;
            ++_input;
        }
        return *this;
    }

    template<Container C>
    Array& operator<<(C const& container) {
        for(auto element : container)
            if(_input < _last){
                _input[0] = element;
                ++_input;
            } else {
                break;
            }
        return *this;
    }

    Ptr<T> data() const {return _data;}
    iterator begin() const { return _data;}
    iterator end() const {return _last;}
    UInt size() const { return _size;}
    Field<UInt,1> dims() const {return {_size};}
};

template<typename T, Convertible<T> T2, UInt N>
bool operator==(Array<T,N> $ A, Array<T2,N> $ B){
    UInt n = A.size();
    if (n != B.size()) return false;
    for_(i,n){
        if(A[i] != B[i]) return false;
    }
    return true;
}

template<typename T, Convertible<T> T2, UInt N>
bool operator!=(Array<T,N> $ A, Array<T2,N> $ B){
    return !(A == B);
}


template<typename T>
using Vector = Array<T,1>;

template<typename T>
using Matrix = Array<T,2>;