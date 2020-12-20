#ifndef BIGINTEGER_BIGINTEGER_H_
#define BIGINTEGER_BIGINTEGER_H_

#include <string>
#include <vector>
#include <iostream>

class BigInteger {
public:
    BigInteger();
    BigInteger(int);
    BigInteger(long long);
    BigInteger(const BigInteger&);

    BigInteger& operator=(const BigInteger&) = default;

    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);
    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    friend BigInteger operator%(const BigInteger&, const BigInteger&);

    BigInteger& operator+=(const BigInteger&);
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator*=(const BigInteger&);
    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);

    BigInteger& operator-();

    BigInteger& operator++();
    BigInteger& operator--();

    BigInteger operator++(int);
    BigInteger operator--(int);

    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);

    friend bool operator>(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend bool operator<=(const BigInteger&, const BigInteger&);

    friend std::istream& operator>>(std::istream&, BigInteger&);
    friend std::ostream& operator<<(std::ostream&, const BigInteger&);

    static BigInteger abs(const BigInteger &);

    static std::string intToStr(int);

    std::string toString() const;

    operator bool() const;

private:
    static const int BASE = 10000000;

    static const int BASE_LENGTH = 7;

    void trim();

    int sign_;
    std::vector<int> digits_;
};

#endif //BIGINTEGER_BIGINTEGER_H_
