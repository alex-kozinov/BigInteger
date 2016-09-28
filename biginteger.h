#pragma once
#include <iostream>
#include <vector>
#include <string>
class BigInteger {
private:
    std::vector <int> numerals;
    size_t sz;
    int sign;
    void reload(const bool &subZero = 0) {
        sz = numerals.size();
        sign = (subZero ? -1 : 1);
        if (sz == 1 && numerals[0] == 0)
            sign = 0;
    }
    void changeSign() {
        sign *= -1;
    }
    void reverse() {
        for (size_t i = 0; 2 * i < sz; ++i)
            std::swap(numerals[i], numerals[sz - i - 1]);
    }
    void plus(const BigInteger &a, const BigInteger &b) {
        int trans = 0;
        size_t i = 0;
        numerals.clear();
        for (; i < ((a.sz < b.sz) ? a.sz : b.sz); ++i) {
            trans += a.numerals[i] + b.numerals[i];
            numerals.push_back(trans % 10);
            trans /= 10;
        }
        for (; i < a.sz; ++i) {
            trans += a.numerals[i];
            numerals.push_back(trans % 10);
            trans /= 10;
        }
        for (; i < b.sz; ++i) {
            trans += b.numerals[i];
            numerals.push_back(trans % 10);
            trans /= 10;
        }
        if (trans)
            numerals.push_back(trans);
        reload();
    }
    void menus(const BigInteger &a, const BigInteger &b) { // a >= b
        int carry = 0;
        numerals.clear();
        for (size_t i = 0; i < a.sz || carry; ++i) {
            numerals.push_back(a.numerals[i] - (carry + (i < b.sz ? b.numerals[i] : 0)));
            carry = numerals.back() < 0;
            if (carry)
                numerals[numerals.size() - 1] += 10;
        }
        while (numerals.size() > 1 && numerals.back() == 0)
            numerals.pop_back();

        reload();
    }
    void multu(const BigInteger &a, const BigInteger &b) {
        numerals.clear();
        numerals.assign(a.sz + b.sz, 0);
        for (size_t i = 0; i < a.sz; ++i)
            for (int j = 0, carry = 0; j < (int)b.sz || carry; ++j) {
                long long cur = numerals[i + j] + a.numerals[i] * (j < (int)b.sz ? b.numerals[j] : 0) + carry;
                numerals[i + j] = cur % 10;
                carry = cur / 10;
            }
        while (numerals.size() > 1 && numerals.back() == 0)
            numerals.pop_back();
        reload(a.sign * b.sign < 0);
    }
public:
    BigInteger() {
        sz = 1;
        sign = 0;
        numerals.push_back(0);
    }
    BigInteger(const int &num) {
        int number = num;
        bool subZero = number < 0;
        if (number == 0) {
            numerals.push_back(0);
        }
        else {
            number = abs(number);
            while (number) {
                numerals.push_back(number % 10);
                number /= 10;
            }
        }
        reload(subZero);
    }
    BigInteger(const long long &num) {
        long long number = num;
        bool subZero = number < 0;
        if (number == 0) {
            numerals.push_back(0);
        }
        else {
            number = abs(number);
            while (number) {
                numerals.push_back(number % 10);
                number /= 10;
            }
        }
        reload(subZero);
    }
    BigInteger(const unsigned long long &num) {
        unsigned long long number = num;
        if (number == 0) {
            numerals.push_back(0);
        }
        else {
            while (number) {
                numerals.push_back(number % 10);
                number /= 10;
            }
        }
        reload();
    }
    BigInteger(const size_t &num) {
        unsigned long long number = num;
        if (number == 0) {
            numerals.push_back(0);
        }
        else {
            while (number) {
                numerals.push_back(number % 10);
                number /= 10;
            }
        }
        reload();
    }
    BigInteger(const BigInteger &last) {
        numerals = last.numerals;
        sz = last.sz;
        sign = last.sign;
    }
    BigInteger(const std::string &number) {
        numerals.clear();
        bool subZero = number[0] == '-';
        for (size_t i = subZero; i < number.size(); ++i) {
            numerals.push_back(number[i] - '0');
        }
        reload(subZero);
        reverse();
    }
    BigInteger& operator = (const BigInteger &n) {
        if (&n != this) {
            numerals = n.numerals;
            sz = n.sz;
            sign = n.sign;
        }
        return *this;
    }
    BigInteger operator -() {
        BigInteger result = *this;
        result.sign *= -1;
        return result;
    }
    explicit operator bool() {
        return (sign != 0);
    }
    std::string toString() const {
        std::string resultString = ((sign < 0) ? "-" : "");
        if (sign == 0)
            return "0";
        for (size_t i = sz; i > 0; --i) {
            resultString += char(numerals[i - 1] + '0');
        }
        return resultString;
    }
    
    friend std::istream& operator >> (std::istream &in, BigInteger &n);
    friend std::ostream& operator << (std::ostream &out, const BigInteger &n);
    friend bool operator < (const BigInteger &a, const BigInteger &b);
    friend bool operator == (const BigInteger &a, const BigInteger &b);
    friend BigInteger& operator += (BigInteger &a, const BigInteger &b);
    friend BigInteger& operator *= (BigInteger &a, const BigInteger &b);
    friend BigInteger& operator /= (BigInteger &a, BigInteger b);
    BigInteger& operator ++ () {
        *this += 1;
        return *this;
    }
    BigInteger& operator -- () {
        *this += -1;
        return *this;
    }
    const BigInteger operator ++ (int) {
        BigInteger result = *this;
        *this += 1;
        return result;
    }
    const BigInteger operator -- (int) {
        BigInteger result = *this;
        *this += -1;
        return result;
    }
};

std::istream& operator >> (std::istream &in, BigInteger &n) {
    int c;
    bool subZero = false;
    n.numerals.clear();
    while ((c = in.peek()) >= 0 &&
        (c == ' ' || c == '\t' || c == '\r' || c == '\n'))
        in.get();
    if (c < 0 || (c != (int)'-' && (c < '0' || c > '9'))) {
        in >> c;
        return in;
    }
    if (c == '-') {
        subZero = true;
        in.get();
    }

    while ((c = in.peek()) >= 0 && (c >= '0' && c <= '9')) {
        in.get();
        n.numerals.push_back(c - '0');
    }
    n.reload(subZero);
    n.reverse();
    return in;
}
std::ostream& operator << (std::ostream &out, const BigInteger &n) {
    return out << n.toString();
}

bool operator < (const BigInteger &a, const BigInteger &b) {
    if (a.sign != b.sign)
        return a.sign < b.sign;
    if (a.sz != b.sz)
        return ((a.sign < 0) ? a.sz > b.sz : a.sz < b.sz);
    for (size_t i = a.sz; i > 0; --i) {
        if (a.numerals[i - 1] != b.numerals[i - 1])
            return ((a.sign < 0) ? a.numerals[i - 1] > b.numerals[i - 1] : a.numerals[i - 1] < b.numerals[i - 1]);
    }
    return 0;
}
bool operator > (const BigInteger &a, const BigInteger &b) {
    return b < a;
}
bool operator == (const BigInteger &a, const BigInteger &b) {
    if (a.sz != b.sz)
        return 0;
    if (a.sign != b.sign)
        return 0;
    for (size_t i = a.sz; i > 0; --i) {
        if (a.numerals[i - 1] != b.numerals[i - 1])
            return 0;
    }
    return 1;
}
bool operator <= (const BigInteger &a, const BigInteger &b) {
    return a < b || a == b;
}
bool operator >= (const BigInteger &a, const BigInteger &b) {
    return a > b || a == b;
}
bool operator != (const BigInteger &a, const BigInteger &b) {
    return !(a == b);
}

BigInteger& operator *= (BigInteger &a, const BigInteger &b) {
    BigInteger result;
    result.multu(a, b);
    return a = result;
}
const BigInteger operator * (const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result *= b;
    return result;
}
BigInteger& operator += (BigInteger &a, const BigInteger &b) {
    BigInteger result;
    if (a.sign == 0 || b.sign == 0)
        return a = ((a.sign == 0) ? b : a);
    if (a.sign == b.sign) {
        result.plus(a, b);
        result.sign = a.sign;
        return a = result;;
    }
    if (a.sign == -1) {
        a.changeSign();
        if (a < b) {
            result.menus(b, a);
            result.reload();
        }
        else {
            result.menus(a, b);
            result.reload(1);
        }
        return a = result;
    }
    a.changeSign();
    if (a > b) {
        result.menus(b, a);
        result.reload(1);
    }
    else {
        result.menus(a, b);
        result.reload();
    }
    return a = result;
}
const BigInteger operator + (const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result += b;
    return result;
}
BigInteger& operator -= (BigInteger &a, const BigInteger &b) {
    a *= -1;
    a += b;
    a *= -1;
    return a;
}
const BigInteger operator - (const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result -= b;
    return result;
}
BigInteger& operator /= (BigInteger &a, BigInteger b) {
    bool subZero = a.sign * b.sign < 0;
    if (b.sign == 0)
        return a = BigInteger(1 / b.sign);
    a.sign = b.sign = 1;
    BigInteger result = BigInteger(0);
    if (a < b)
        return a = result;

    BigInteger ost = BigInteger(0);
    for (size_t i = a.sz; i > 0; --i) {
        ost *= 10;
        ost += a.numerals[i - 1];
        result.numerals.push_back(0);
        result.sz++;
        if (ost < b)
            continue;
        for (int j = 0; j < 9; ++j) {
            result.numerals[result.sz - 1]++;
            if (ost < b * result.numerals[result.sz - 1]) {
                result.numerals[result.sz - 1]--;
                break;
            }
        }
        ost -= b * result.numerals[result.sz - 1];
    }
    result.reverse();
    while (result.numerals.size() > 1 && result.numerals.back() == 0)
        result.numerals.pop_back();
    result.reload(subZero);
    return a = result;
}
const BigInteger operator / (const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    return result /= b;
}
BigInteger& operator %= (BigInteger &a, const BigInteger &b) {
    BigInteger chast = a / b;
    a -= chast * b;
    return a;
}
const BigInteger operator % (const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result %= b;
    return result;
}