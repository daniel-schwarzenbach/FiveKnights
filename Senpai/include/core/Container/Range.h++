#pragma once

#include "../Base.h++"

#pragma once

#include "../Base.h++"

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
    iterator begin() const;
    iterator end() const;
    Int operator[](Int i);
    bool contains(Int x);
    UInt size();
};

OS& operator<<(OS& os, Range $ range);