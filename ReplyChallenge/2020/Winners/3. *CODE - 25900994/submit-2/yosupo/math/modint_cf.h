#pragma once

template <uint MD> struct ModInt {
private:
    static constexpr int c_bsr(uint md) {
        return (md == 0) ? -1 : 1 + c_bsr(md >> 1);
    }
    using M = ModInt;
    const static M G;
    static constexpr int s = c_bsr(MD);
    static constexpr uint iMD = uint(((1ULL << (32 + s)) - 1) / MD + 1);
    uint v = 0;
public:
    ModInt() {}
    ModInt(ll _v) { set_v(uint(_v % MD + MD)); }
    uint val() const { return v % MD; }
    M& set_v(uint _v) {
        v = (_v < 2 * MD) ? _v : _v - 2 * MD;
        return *this;
    }
    explicit operator bool() const { return v != 0; }
    M operator-() const { return M() - *this; }
    M operator+(const M& r) const { return M().set_v(v + r.v); }
    M operator-(const M& r) const { return M().set_v(v + 2 * MD - r.v); }
    M operator*(const M& r) const {
/*        ull c = ull(v) * r.v;
        uint q, rem;
        asm(
        "divl %4"
        : "=a"(q), "=d"(rem)
        : "d"(uint(c>>32)), "a"(uint(c)), "r"(MD)
        );
        M m;
        m.v = rem;
        return m;*/
        assert(v < 2 * MD && r.v < 2 * MD);
        ull c = ull(v) * r.v;
        uint cl = uint(c);
        uint d = ((c >> 32) * iMD + ((ull(cl) * iMD) >> 32)) >> s;
        M m;
        m.v = (cl - d * MD + MD);
        assert(m.v < 2 * MD);
        return m; //M().set_v(cl - d * MD + MD);
    }
    M operator/(const M& r) const { return *this * r.inv(); }
    M& operator+=(const M& r) { return *this = *this + r; }
    M& operator-=(const M& r) { return *this = *this - r; }
    M& operator*=(const M& r) { return *this = *this * r; }
    M& operator/=(const M& r) { return *this = *this / r; }
    bool operator==(const M& r) const { return v == r.v; }
    M pow(ll n) const {
        M x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    M inv() const { return pow(MD - 2); }
    friend ostream& operator<<(ostream& os, const M& r) { return os << r.v; }
};
// using Mint = ModInt<998244353>;
// template<> const Mint Mint::G = Mint(3);
