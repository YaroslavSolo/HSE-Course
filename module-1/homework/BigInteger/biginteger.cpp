#include "biginteger.h"

BigInteger::BigInteger() {
    sign_ = 0;
    digits_ = {};
}

BigInteger::BigInteger(int num) : BigInteger(static_cast<long long>(num)) {}

BigInteger::BigInteger(long long num) {
    digits_ = {};

    if (num == 0) {
        sign_ = 0;
    } else if (num < 0) {
        sign_ = -1;
        num *= -1;
    } else {
        sign_ = 1;
    }

    while (num) {
        digits_.push_back(num % BASE);
        num /= BASE;
    }
}

BigInteger::BigInteger(const BigInteger& other) {
    sign_ = other.sign_;
    digits_ = std::vector<int>(other.digits_);
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
    BigInteger res = BigInteger(left);
    res += right;
    return res;
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
    BigInteger res = BigInteger(left);
    res -= right;
    return res;
}

BigInteger operator*(const BigInteger& left, const BigInteger& right) {
    BigInteger res = BigInteger(left);
    res *= right;
    return res;
}

BigInteger operator/(const BigInteger& left, const BigInteger& right) {
    BigInteger res = BigInteger(left);
    res /= right;
    return res;
}

BigInteger operator%(const BigInteger& left, const BigInteger& right) {
    BigInteger res = BigInteger(left);
    res %= right;
    return res;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    if (!sign_) {
        *this = other;
        return *this;
    }

    if (!other.sign_)
        return *this;

    if (sign_ == other.sign_) {
        if (digits_.size() < other.digits_.size())
            digits_.resize(other.digits_.size());

        unsigned int carry = 0;
        int i;
        for (i = 0; i < other.digits_.size(); ++i) {
            carry += digits_[i] + other.digits_[i];
            digits_[i] = carry % BASE;
            carry /= BASE;
        }

        if (carry != 0) {
            if (digits_.size() == i) {
                digits_.push_back(carry);
            } else {
                carry += digits_[i];
                digits_[i++] = carry % BASE;
                if (carry / BASE != 0)
                {
                    if (digits_.size() > i)
                    {
                        digits_[i] = 1;
                    } else {
                        digits_.push_back(1);
                    }
                }
            }
        }

        return *this;
    } else {
        if (sign_ == 1) {
            return *this -= abs(other);
        } else {
            return *this = other - abs(*this);
        }
    }
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (!sign_) {
        *this = other;
        sign_ *= -1;
        return *this;
    }

    if (sign_ == 1 && other.sign_ == 1) {
        if (*this > other) {
            for (int i = 0, carry = 0; i < other.digits_.size() || carry; ++i) {
                carry += other.digits_[i];
                if (digits_[i] >= carry) {
                    digits_[i] -= carry;
                    carry = 0;
                } else {
                    digits_[i] += BASE - carry;
                    carry = 1;
                }
            }
        } else {
            *this = other - *this;
            sign_ = -1;
        }
    } else if (sign_ == -1 && other.sign_ == -1) {
        *this = abs(other) - abs(*this);
    } else if (sign_ == 1 && other.sign_ == -1) {
        *this = *this + abs(other);
    } else if (sign_ == -1 && other.sign_ == 1) {
        *this = abs(*this) + abs(other);
        sign_ = -1;
    }

    trim();

    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    sign_ *= other.sign_;
    if (!sign_) {
        digits_.clear();
        return *this;
    }

    std::vector<int> res(digits_.size() + other.digits_.size());
    for (int i = 0; i < digits_.size(); ++i) {
        for (int j = 0, carry = 0; j < other.digits_.size() || carry; ++j) {
            long long temp = carry + static_cast<long long>(digits_[i]) *
                    (j < other.digits_.size() ? other.digits_[j] : 0);
            res[i + j] += temp % BASE;
            carry = static_cast<int>(temp / BASE);
            carry += res[i + j] / BASE;
            res[i + j] %= BASE;
        }
    }

    digits_ = std::move(res);
    trim();

    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (!other.sign_)
        throw std::invalid_argument("Division by zero");

    if (sign_) {
        BigInteger res, cur_value;
        res.sign_ = sign_ * other.sign_;
        cur_value.sign_ = 1;

        for (int i = digits_.size() - 1; i >= 0; --i) {
            cur_value.digits_.insert(cur_value.digits_.begin(), digits_[i]);

            int l = 0, r = BASE, x = 0;
            while (l <= r) {
                int m = (l + r) >> 1;
                BigInteger temp = other * BigInteger(m);
                if (temp <= cur_value) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }

            res.digits_.push_back(x);
            cur_value = cur_value - other * BigInteger(x);
        }

        for (int i = 0; i < res.digits_.size() / 2; ++i)
            std::swap(res.digits_[i], res.digits_[res.digits_.size() - i - 1]);
        res.trim();

        return *this = res;
    } else {
        return *this = 0;
    }
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    if (!other.sign_)
        throw std::invalid_argument("Division by zero");

    if (sign_) {
        BigInteger res, cur_value;
        cur_value.sign_ = 1;

        for (int i = digits_.size() - 1; i >= 0; --i) {
            cur_value.digits_.insert(cur_value.digits_.begin(), digits_[i]);

            int l = 0, r = BASE, x = 0;
            while (l <= r) {
                int m = (l + r) >> 1;
                BigInteger temp = other * BigInteger(m);
                if (temp <= cur_value) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }

            res.digits_.push_back(x);
            cur_value = cur_value - other * BigInteger(x);
        }

        for (int i = 0; i < res.digits_.size() / 2; ++i)
            std::swap(res.digits_[i], res.digits_[res.digits_.size() - i - 1]);
        res.trim();

        return *this = cur_value;
    } else {
        return *this = 0;
    }
}

BigInteger& BigInteger::operator-() {
    BigInteger* res = new BigInteger(*this);
    res->sign_ *= -1;
    return *res;
}

BigInteger& BigInteger::operator++() {
    return *this += BigInteger(1);
}

BigInteger& BigInteger::operator--() {
    return *this -= BigInteger(1);
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp = *this;
    *this += BigInteger(1);
    return temp;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp = *this;
    *this -= BigInteger(1);
    return temp;
}

std::istream& operator>>(std::istream& in, BigInteger& res) {
    res.digits_.clear();
    std::string num;
    in >> num;
    int cur_pos = 0;

    if (num[0] == '-') {
        res.sign_ = -1;
        ++cur_pos;
    } else if (num[0] == '+') {
        ++cur_pos;
    }

    while (num[cur_pos++] == 0 && cur_pos < num.length());
    if (cur_pos-- == num.length()) {
        res.sign_ = 0;
        return in;
    } else {
        res.sign_ = 1;
    }

    int num_end = cur_pos - 1;
    cur_pos = num.length() - 1;
    do {
        int i, cur_digit = 0, power = 1;
        for (i = cur_pos; i > num_end && i > cur_pos - BigInteger::BASE_LENGTH; --i) {
            if (num[i] < '0' || num[i] > '9') {
                throw std::invalid_argument("Unrecognised number format");
            } else {
                cur_digit += (num[i] - '0') * power;
                power *= 10;
            }
        }
        res.digits_.push_back(cur_digit);
        cur_pos = i;
    } while (cur_pos > num_end);

    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
    out << num.toString();
    return out;
}

bool operator==(const BigInteger& left, const BigInteger& right) {
    if (left.sign_ != right.sign_ || left.digits_.size() != right.digits_.size())
        return false;

    for (int i = 0; i < left.digits_.size(); ++i) {
        if (left.digits_[i] != right.digits_[i])
            return false;
    }

    return true;
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

bool operator>(const BigInteger& left, const BigInteger& right) {
    if (left.sign_ != right.sign_)
        return left.sign_ > right.sign_;

    if (!left.sign_)
        return false;

    if (left.digits_.size() != right.digits_.size()) {
        if (left.sign_ == 1) {
            return left.digits_.size() > right.digits_.size();
        } else {
            return left.digits_.size() < right.digits_.size();
        }
    }

    for (int i = left.digits_.size() - 1; i >= 0; --i) {
        if (left.digits_[i] != right.digits_[i]) {
            if (left.sign_ == 1) {
                return left.digits_[i] > right.digits_[i];
            } else {
                return left.digits_[i] < right.digits_[i];
            }
        }
    }

    return false;
}

bool operator<(const BigInteger& left, const BigInteger& right) {
    return !(left >= right);
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
    return (left > right) || (left == right);
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
    return !(left > right);
}

BigInteger::operator bool() const {
    return sign_ != 0;
}

std::string BigInteger::intToStr(int num) {
    std::string res;
    while (num) {
        res += '0' + static_cast<char>(num % 10);
        num /= 10;
    }

    for (int j = 0; j < res.length() / 2; ++j)
        std::swap(res[j], res[res.length() - j - 1]);

    return res;
}

std::string BigInteger::toString() const {
    if (!sign_)
        return "0";

    std::string res;
    if (sign_ == -1)
        res.push_back('-');

    res += intToStr(digits_.back());

    for (int i = digits_.size() - 2; i >= 0; --i) {
        std::string temp = intToStr(digits_[i]);
        res += std::string(BASE_LENGTH - temp.length(), '0') + temp;
    }

    return res;
}

BigInteger BigInteger::abs(const BigInteger& num) {
    BigInteger res(num);
    if (num.sign_ == -1)
        res.sign_ = 1;

    return res;
}

void BigInteger::trim() {
    while (!digits_.empty() && digits_.back() == 0)
        digits_.pop_back();

    if (digits_.empty())
        sign_ = 0;
}