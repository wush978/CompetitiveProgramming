/**
 * http://codeforces.com/problemset/problem/630/I
 */

#include <deque>
#include <vector>
#include <iostream>
#include <cmath>
#include <bitset>

class BigInt {

  typedef std::deque<uint16_t> ValueType;

  ValueType digits;

public:

  BigInt() : digits() { }

  BigInt(std::size_t n) {
    std::size_t size;
    if (n == 0) size = 0;
    else size = 1 + std::floor(std::log10(n));
    digits.resize(size);
    for(std::size_t i = 0;i < size;i++) {
      digits[i] = n % 10;
      n = n / 10;
    }
  }

  BigInt(const BigInt& x) : digits(x.digits) { }

  void operator=(const BigInt& x) {
    digits = x.digits;
  }

  void operator+=(const BigInt& x) {
    std::size_t carry = 0;
    std::size_t i = 0;
    std::size_t digit_size = digits.size();
    while(true) {
      bool is_exceed_x = i >= x.digits.size();
      bool is_exceed = i >= digit_size;
      if (is_exceed_x && is_exceed && carry == 0) break;

      std::uint16_t value = carry;
      if (!is_exceed_x) value += x.digits[i];
      if (!is_exceed) value += digits[i];
      if (i < digits.size()) digits[i] = value % 10; 
      else digits.push_back(value % 10);
      carry = value / 10;
      i++;
    }
  }

  void operator*=(const std::size_t x) {
    std::size_t carry = 0;
    std::size_t digit_size = digits.size();
    for(std::size_t i = 0;i < digit_size;i++) {
      std::size_t digit = digits[i];
      std::size_t value = x * digit + carry;
      digits[i] = value % 10;
      carry = (value - digits[i]) / 10;
    }
    while(carry > 0) {
      digits.push_back(carry % 10);
      carry = carry / 10;
    }
  }

  void operator*=(const BigInt& x) {
    std::size_t carry = 0;
    BigInt result(0);
    BigInt element(0);
    for(std::size_t i = 0;i < x.digits.size();i++) {
      element = *this;
      element *= x.digits[i];
      for(std::size_t j = 0;j < i;j++) {
        element.digits.push_front(0);
      }
      result += element;
    }
    digits.swap(result.digits);
  }

  void operator^=(std::size_t k) {
    if (k == 0) {
      if (digits.size() == 0) throw std::invalid_argument("0^0");
      digits.clear();
      digits.push_back(1);
      return;
    }
    std::size_t size = 1 + std::floor(std::log2(k));
    std::bitset<sizeof(std::size_t) * 8> k_bits(k);
    BigInt tmp(*this), result(1);
    if (k_bits[0]) result = tmp;
    for(std::size_t i = 1;i < size;i++) {
      tmp *= tmp;
      if (k_bits[i]) result *= tmp;
    }
    digits.swap(result.digits);
  }

  friend std::ostream& operator<<(std::ostream& out, const BigInt& x);
};

std::ostream& operator<<(std::ostream& out, const BigInt& x) {
  for(auto i = x.digits.rbegin();i != x.digits.rend();i++) {
    out << *i;
  }
  return out;
}

int main() {
  std::size_t n = 0;
  if (!(std::cin >> n)) return 1;
  if (n < 3) return 1;
  if (n == 3) {
    std::cout << 24 << std::endl;
    return 0;
  }
  BigInt x1(2), x2(2);
  x1 ^= (2 * n - 3);
  x1 *= 3;
  x2 ^= (2 * n - 6);
  x2 *= 9;
  x2 *= n - 3;
  x1 += x2;
  std::cout << x1 << std::endl;
  return 0;
}