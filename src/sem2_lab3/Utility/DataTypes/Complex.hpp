#ifndef COMPLEX_HPP
#define COMPLEX_HPP
#include <iostream>
#include <utility>
#include <string>

using namespace std;

template <class TypeReal, class TypeIm>
class Complex
{
    private:
        TypeIm imaginary;
        TypeReal real;
    public:
        Complex()
        {
            real = 0;
            imaginary = 0;
        };
        Complex(TypeReal re, TypeIm im)
        {
            real = re;
            imaginary = im;
        };
        Complex(pair<TypeReal, TypeIm> pair)
        {
            real = pair.first;
            imaginary = pair.second;
        };
        Complex(Complex<TypeReal, TypeIm>* other)
        {
            imaginary = other->imaginary;
            real = other->real;
        };
        string ComplexToString()
        {
            string result;
            if(imaginary >= 0)
                result = to_string(real) + '+' + to_string(imaginary) + 'i';
            else
                result = to_string(real) + to_string(imaginary) + 'i';
            return result;
        };
        void PrintComplex(){
            cout << ComplexToString() << endl;
        };
        TypeReal GetReal(){
            return real;
        };
        TypeIm GetImaginary(){
            return imaginary;
        };
        pair<TypeReal, TypeIm> GetPair()
        {
            return make_pair(real, imaginary);
        };
        Complex<TypeReal, TypeIm> GetConjugate()
        {
            return Complex<TypeReal, TypeIm>(real, -imaginary);
        };
        void Conjugate()
        {
            imaginary = -imaginary;
        };
        Complex<TypeReal, TypeIm>& operator+=(const Complex<TypeReal, TypeIm>& complex)
        {
            imaginary += complex.imaginary;
            real += complex.real;
            return *this;
        };
        Complex<TypeReal, TypeIm>& operator-=(const Complex<TypeReal, TypeIm>& complex)
        {
            imaginary -= complex.imaginary;
            real -= complex.real;
            return *this;
        };
        Complex<TypeReal, TypeIm>& operator*=(const Complex<TypeReal, TypeIm>& complex)
        {
            real = real * complex.real - imaginary * complex.imaginary;
            imaginary = real * complex.imaginary + imaginary * complex.real;
            return *this;
        };
        Complex<TypeReal, TypeIm>& operator/=(const Complex<TypeReal, TypeIm>& complex)
        {
            real = (real * complex.real + imaginary * complex.imaginary)/
                    (complex.real * complex.real + complex.imaginary * complex.imaginary);
            imaginary = (imaginary * complex.real - real * complex.imaginary)/
                    (complex.real * complex.real + complex.imaginary * complex.imaginary);
            return *this;
        };
        const Complex<TypeReal, TypeIm> operator+(const Complex<TypeReal, TypeIm>& complex) const
        {
            return Complex<TypeReal, TypeIm>(*this) += complex;
        };
        const Complex<TypeReal, TypeIm> operator-(const Complex<TypeReal, TypeIm>& complex) const
        {
            return Complex<TypeReal, TypeIm>(*this) -= complex;
        };
        const Complex<TypeReal, TypeIm> operator*(const Complex<TypeReal, TypeIm>& complex) const
        {
            return Complex<TypeReal, TypeIm>(*this) *= complex;
        };
        const Complex<TypeReal, TypeIm> operator/(const Complex<TypeReal, TypeIm>& complex) const
        {
            return Complex<TypeReal, TypeIm>(*this) /= complex;
        };
        Complex<TypeReal, TypeIm> operator-()
        {
            Complex<TypeReal, TypeIm> tmp(*this);
            tmp.real *= -1;
            tmp.imaginary *= -1;
            return tmp;
        };
        bool operator==(const Complex<TypeReal, TypeIm>* other)
        {
            return (real == other->real && imaginary == other->imaginary);
        };
        bool operator!=(const Complex<TypeReal, TypeIm> complex)
        {
            return !(this == complex);
        };
        void operator++()
        {
            imaginary += 1;
            real += 1;
        };
        void operator--()
        {
            imaginary -= 1;
            real -= 1;
        };
};

#endif // COMPLEX_HPP
