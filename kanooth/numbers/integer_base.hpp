#ifndef KANOOTH_NUMBERS_INTEGER_BASE_HPP
#define KANOOTH_NUMBERS_INTEGER_BASE_HPP

namespace kanooth {
namespace numbers {
        
template <typename N>
class integer_base {
    typedef N base_type;
public:
    integer_base() {}
    integer_base(long v) : number(v < 0 ? -v : v), positive(v >= 0) {}
    integer_base(unsigned long v, bool pos = true) : number(v), positive(pos) {}
    integer_base(const base_type& other, bool pos) : number(other), positive(pos) {}
    integer_base(const integer_base& other) : number(other.number), positive(other.positive) {}
    integer_base(const char* s) : number() {
        bool pos = true;
        while (*s == '+' || *s == '-') {
            if (*s == '-') pos = !pos;
            ++s;
        }
        number = s;
        positive = pos;
    }
    integer_base& operator=(long v) {
        integer_base(v).swap(*this);
        return *this;
    }
    integer_base& operator=(unsigned long v) {
        integer_base(v).swap(*this);
        return *this;
    }
    std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags f) const {
        std::string res = number.str(0, f);
        return positive ? res : "-" + res;
    }
private:
    base_type number;
    bool positive;
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_INTEGER_BASE_HPP
