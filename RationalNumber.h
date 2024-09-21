// Your goal is to create a rational number class that would
// support each of the operations given in main.cpp.
//
// In this file you must declare only the interface of your class
// and implement the given functions separately from the class (at the bottom of
// this file inside the namespace).
// Notice that the RationalNumber is a class template, where the
// template parameter is an integer type for numerator and denominator.
//
//
//
// After you wrote RationalNumber class and the tests in the main function work
// - write at the bottom of the file the downsides of such rational numbers,
// what would you change/remove/add? Would you use such rational numbers instead
// of double/float numbers? There is no right/wrong answer, this question is
// more of a philosofical kind than technical.

// #pragma once
// #include <iostream>
// #include <stdexcept>
// #include <type_traits>
// #include <numeric>


#pragma once
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <numeric>

namespace yourname {

    template <typename T>
    class RationalNumber {
        static_assert(std::is_integral<T>::value, "Template parameter must be an integral type.");

    private:
        T numerator;
        T denominator;

        // Helper function to reduce the fraction using GCD
        void reduce() {
            T gcd = std::gcd(numerator, denominator);
            numerator /= gcd;
            denominator /= gcd;

            // Ensure the denominator is always positive
            if (denominator < 0) {
                numerator = -numerator;
                denominator = -denominator;
            }
        }

    public:
        RationalNumber(T num = 0, T denom = 1) : numerator(num), denominator(denom) {
            if (denom == 0) {
                throw std::invalid_argument("Denominator cannot be zero");
            }
            reduce();
        }

        T getNumerator() const { return numerator; }
        T getDenominator() const { return denominator; }

        // Compound assignment operators
        RationalNumber<T>& operator+=(const RationalNumber<T>& rhs) {

            T gcdDenominator = std::gcd(denominator, rhs.denominator);

            T lcmDenominator = (denominator / gcdDenominator) * rhs.denominator;

            T adjustedNumerator1 = numerator * (lcmDenominator / denominator);
            T adjustedNumerator2 = rhs.numerator * (lcmDenominator / rhs.denominator);

            numerator = adjustedNumerator1 + adjustedNumerator2;
            denominator = lcmDenominator;

            reduce();
            return *this;
        }

        RationalNumber<T>& operator-=(const RationalNumber<T>& rhs) {
            numerator = numerator * rhs.denominator - rhs.numerator * denominator;
            denominator *= rhs.denominator;
            reduce();
            return *this;
        }


        RationalNumber<T>& operator*=(const RationalNumber<T>& rhs) {

            T gcd1 = std::gcd(numerator, rhs.denominator);
            T gcd2 = std::gcd(denominator, rhs.numerator);

            numerator = (numerator / gcd1) * (rhs.numerator / gcd2);
            denominator = (denominator / gcd2) * (rhs.denominator / gcd1);

            reduce();
            return *this;
        }


        RationalNumber<T>& operator/=(const RationalNumber<T>& rhs) {
            if (rhs.numerator == 0) {
                throw std::invalid_argument("Division by zero");
            }
            numerator *= rhs.denominator;
            denominator *= rhs.numerator;
            reduce();
            return *this;
        }

        // Unary operators
        RationalNumber<T> operator+() const { return *this; }
        RationalNumber<T> operator-() const { return RationalNumber<T>(-numerator, denominator); }

        // Increment and decrement
        RationalNumber<T>& operator++() { return *this += 1; }
        RationalNumber<T> operator++(int) {
            RationalNumber<T> temp = *this;
            ++(*this);
            return temp;
        }
        RationalNumber<T>& operator--() { return *this -= 1; }
        RationalNumber<T> operator--(int) {
            RationalNumber<T> temp = *this;
            --(*this);
            return temp;
        }

        // Type conversion
        explicit operator double() const { return static_cast<double>(numerator) / denominator; }
        explicit operator float() const { return static_cast<float>(numerator) / denominator; }
        explicit operator int() const { return numerator / denominator; }

        // Comparison operators
        bool operator==(const RationalNumber<T>& rhs) const { return numerator == rhs.numerator && denominator == rhs.denominator; }
        bool operator!=(const RationalNumber<T>& rhs) const { return !(*this == rhs); }
        bool operator<(const RationalNumber<T>& rhs) const { return numerator * rhs.denominator < rhs.numerator * denominator; }
        bool operator>(const RationalNumber<T>& rhs) const { return rhs < *this; }
        bool operator<=(const RationalNumber<T>& rhs) const { return !(*this > rhs); }
        bool operator>=(const RationalNumber<T>& rhs) const { return !(*this < rhs); }

        // I/O stream operators
        friend std::ostream& operator<<(std::ostream& os, const RationalNumber<T>& rational) {
            os << rational.numerator << "/" << rational.denominator;
            return os;
        }
    };

    // Binary operators implemented via compound assignments
    template <typename T>
    RationalNumber<T> operator+(RationalNumber<T> lhs, const RationalNumber<T>& rhs) {
        lhs += rhs;
        return lhs;
    }

    template <typename T>
    RationalNumber<T> operator+(T lhs, const RationalNumber<T>& rhs) {
        return RationalNumber<T>(lhs) + rhs;
    }

    template <typename T>
    RationalNumber<T> operator-(RationalNumber<T> lhs, const RationalNumber<T>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    template <typename T>
    RationalNumber<T> operator*(RationalNumber<T> lhs, const RationalNumber<T>& rhs) {
        lhs *= rhs;
        return lhs;
    }

    template <typename T>
    RationalNumber<T> operator*(T lhs, const RationalNumber<T>& rhs) {
        return RationalNumber<T>(lhs) * rhs;
    }

    template <typename T>
    RationalNumber<T> operator/(RationalNumber<T> lhs, const RationalNumber<T>& rhs) {
        lhs /= rhs;
        return lhs;
    }

    // User-defined literals
    namespace literals {
        RationalNumber<unsigned long long> operator""_r(unsigned long long num) {
            return RationalNumber<unsigned long long>(num, 1);
        }
    }

}  // namespace yourname
