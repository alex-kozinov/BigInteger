#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class BigInteger {
private:
    std::vector <int> numerals;
    size_t numberSize;
    int sign;
    void reload(bool subZero = 0) {
        numberSize = numerals.size();
        sign = (subZero ? -1 : 1);
        if (numberSize == 0) {
            numerals.push_back(0);
            numberSize = 1;
        }
        if (numberSize == 1 && numerals[0] == 0)
            sign = 0;
    }
    void changeSign() {
        sign *= -1;
    }
    void reverse() {
        for (size_t i = 0; 2 * i < numerals.size(); ++i)
            std::swap(numerals[i], numerals[numerals.size() - i - 1]);
    }
public:
    BigInteger() {
        numberSize = 1;
        sign = 0;
        numerals.push_back(0);
    }
    BigInteger(const int &num) {
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
    BigInteger(const BigInteger &last) {
        numerals = last.numerals;
        numberSize = last.numberSize;
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
    explicit operator bool() {
        return (sign != 0);
    }
    static void swap(BigInteger &one, BigInteger &other) {
        std::swap(one.numerals, other.numerals);
        std::swap(one.numberSize, other.numberSize);
        std::swap(one.sign, other.sign);
    }
    BigInteger& operator = (const BigInteger &n);
    const BigInteger operator -() const;

    BigInteger& operator *= (const BigInteger &b);
    BigInteger& operator += (const BigInteger &b);
    BigInteger& operator -= (const BigInteger &b);
    BigInteger& operator /= (const BigInteger &b);
    BigInteger& operator %= (const BigInteger &b);
    BigInteger& operator ++ ();
    BigInteger& operator -- ();
    const BigInteger operator ++ (int);
    const BigInteger operator -- (int);

    std::string toString() const;
    size_t getSize() const;
    int getSign() const;
    int at(int index) const;

};
std::istream& operator >> (std::istream &in, BigInteger &n) {
    std::string number;
    in >> number;
    n = BigInteger(number);
    return in;
}
std::ostream& operator << (std::ostream &out, const BigInteger &n) {
    return out << n.toString();
}


bool operator < (const BigInteger &a, const BigInteger &b) {
    if (a.getSign() != b.getSign())
        return a.getSign() < b.getSign();
    if (a.getSize() != b.getSize())
        return ((a.getSign() < 0) ? a.getSize() > b.getSize() : a.getSize() < b.getSize());
    for (size_t i = a.getSize(); i > 0; --i) {
        if (a.at(i - 1) != b.at(i - 1))
            return ((a.getSign() < 0) ? a.at(i - 1) > b.at(i - 1) : a.at(i - 1) < b.at(i - 1));
    }
    return 0;
}
bool operator > (const BigInteger &a, const BigInteger &b) {
    return b < a;
}
bool operator == (const BigInteger &a, const BigInteger &b) {
    if (a.getSign() != b.getSign())
        return false;
    if (a.getSize() != b.getSize())
        return false;
    for (size_t i = 0; i < a.getSize(); ++i) {
        if (a.at(i) != b.at(i))
            return false;
    }
    return true;
}
bool operator <= (const BigInteger &a, const BigInteger &b) {
    return !(a > b);
}
bool operator >= (const BigInteger &a, const BigInteger &b) {
    return !(a < b);
}
bool operator != (const BigInteger &a, const BigInteger &b) {
    return !(a == b);
}

BigInteger& BigInteger::operator = (const BigInteger &n) {
    if (&n != this) {
        numerals = n.numerals;
        numberSize = n.numberSize;
        sign = n.sign;
    }
    return *this;
}
const BigInteger BigInteger::operator -() const {
    return BigInteger(*this) *= -1;
}


BigInteger& BigInteger:: operator *= (const BigInteger &b) {
    if (sign == 0 || b.sign == 0) {
        return *this = 0;
    }
    if (b == 1) {
        return *this;
    }
    size_t sizeA = getSize(), sizeB = b.getSize();
    while (numerals.size() + 1 <= sizeA + sizeB)
        numerals.push_back(0);

    for (size_t i = sizeA; i > 0; --i) {
        int value = numerals[i - 1];
        numerals[i - 1] = 0;
        for (size_t j = 0; j < sizeB; ++j) {
            numerals[i + j - 1] += value * b.numerals[j];
        }
    }

    for (size_t i = 0; i + 1 < numerals.size(); ++i) {
        numerals[i + 1] += numerals[i] / 10;
        numerals[i] %= 10;
    }
    while (!numerals.empty() && !numerals.back())
        numerals.pop_back();
    reload((sign < 0) ^ (b.sign < 0));
    return *this;
}
const BigInteger operator * (const BigInteger &a, const BigInteger &b) {
    return BigInteger(a) *= b;
}
BigInteger& BigInteger:: operator += (const BigInteger &b) {
    if (sign == 0 || b.sign == 0) {
        return *this = ((sign == 0) ? b : *this);
    }
    if (sign == b.sign) {
        int transfer = 0;
        for (size_t i = 0; i < b.numberSize || transfer; ++i) {
            if (i == numerals.size())
                numerals.push_back(0);

            transfer += numerals[i] + ((i < b.numerals.size()) ? b.numerals[i] : 0);
            numerals[i] = transfer % 10;
            transfer /= 10;
        }
        reload(sign < 0);
        return *this;
    }

    bool firstSubZero = sign == 1;
    sign *= -1;
    if ((*this <= b) ^ firstSubZero) {
        int transfer = 0;
        for (size_t i = 0; i < b.numberSize; ++i) {
            if (i >= numerals.size())
                numerals.push_back(0);

            transfer += numerals[i];
            numerals[i] = b.numerals[i] - transfer;
            transfer = 0;
            if (numerals[i] < 0) {
                numerals[i] += 10;
                transfer = 1;
            }
        }
        while (!numerals.empty() && !numerals.back()) {
            numerals.pop_back();
        }
        reload(firstSubZero);
    }
    else {
        int transfer = 0;
        for (size_t i = 0; i < numerals.size(); ++i) {
            transfer += ((i < b.numberSize) ? b.numerals[i] : 0);
            numerals[i] -= transfer;
            transfer = 0;
            if (numerals[i] < 0) {
                numerals[i] += 10;
                transfer = 1;
            }
        }
        while (!numerals.empty() && !numerals.back()) {
            numerals.pop_back();
        }
        reload(!firstSubZero);
    }
    return *this;
}
const BigInteger operator + (const BigInteger &a, const BigInteger &b) {
    return BigInteger(a) += b;
}
BigInteger& BigInteger:: operator -= (const BigInteger &b) {
    sign *= -1;
    *this += b;
    sign *= -1;
    return *this;
}
const BigInteger operator - (const BigInteger &a, const BigInteger &b) {
    return BigInteger(a) -= b;
}
BigInteger& BigInteger:: operator /= (const BigInteger &b) {
    if (this == &b)
        return *this = BigInteger(1);
    int koefB = b.sign;
    if (koefB == 0)
        throw("division by zero");

    bool subZero = sign * b.sign < 0;
    BigInteger curr, result;
    curr = 0;
    for (size_t i = numerals.size(); i > 0; --i) {
        curr *= 10;
        curr += numerals[i - 1];
        int add = -1;
        while (curr.sign >= 0) {
            ++add;
            if (koefB > 0) {
                curr -= b;
            }
            else {
                curr += b;
            }
        }
        if (koefB > 0) {
            curr += b;
        }
        else {
            curr -= b;
        }
        result.numerals.push_back(add);
    }
    result.reverse();
    while (!result.numerals.empty() && !result.numerals.back())
        result.numerals.pop_back();
    result.reload(subZero);
    return *this = result;
}
const BigInteger operator / (const BigInteger &a, const BigInteger &b) {
    return BigInteger(a) /= b;
}
BigInteger& BigInteger:: operator %= (const BigInteger &b) {
    return (*this -= (BigInteger(*this / b) * b));
}
const BigInteger operator % (const BigInteger &a, const BigInteger &b) {
    return BigInteger(a) %= b;
}

BigInteger& BigInteger::operator ++ () {
    return *this += 1;
}
BigInteger& BigInteger::operator -- () {
    return *this += -1;
}
const BigInteger BigInteger::operator ++ (int) {
    BigInteger result = *this;
    ++(*this);
    return result;
}
const BigInteger BigInteger::operator -- (int) {
    BigInteger result = *this;
    --(*this);
    return result;
}

std::string BigInteger::toString() const {
    std::string resultString = ((sign < 0) ? "-" : "");
    if (sign == 0)
        return "0";
    for (size_t i = numberSize; i > 0; --i) {
        resultString += char(numerals[i - 1] + '0');
    }
    return resultString;
}
size_t BigInteger::getSize() const {
    return numberSize;
}
int BigInteger::getSign() const {
    return sign;
}
int BigInteger::at(int index) const {
    return numerals[index];
}

