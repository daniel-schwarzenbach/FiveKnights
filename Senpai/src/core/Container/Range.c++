#include <core/container/Range.h++>

// Definitions of Range member functions
Range::iterator Range::begin() const { return iterator{a, a <= b ? 1 : -1}; }

Range::iterator Range::end() const {
   return iterator{a <= b ? (b + 1) : (b - 1), a <= b ? 1 : -1};
}

Int Range::operator[](Int i) { return a <= b ? (a + i - 1) : (a - i + 1); }

bool Range::contains(Int x) {
   return a <= b ? (a <= x && x <= b) : (b <= x && x <= a);
}

UInt Range::size() { return a <= b ? b - a + 1 : a - b + 1; }

// Definition of the operator<< for Range
OS& operator<<(OS& os, Range const& range) {
   os << "Range{" << range.a << ", " << range.b << "}";
   return os;
}

IS& operator>>(IS& is, Range& range) {
   is >> "Range{" >> range.a >> ", " >> range.b >> "}";
   return is;
}