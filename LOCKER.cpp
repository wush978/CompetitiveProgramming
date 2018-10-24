/**
 * https://www.spoj.com/problems/LOCKER/
 */

#include <cmath>
#include <iostream>
#include <unordered_map>
#include <bitset>

const std::size_t mod = 1000000000 + 7;

const uint64_t pow3(uint64_t k) {
  if (k == 0) return 1;
  std::bitset<sizeof(uint64_t) * 8> digits(k);
  std::size_t size = 1 + std::floor(std::log2(k));
  uint64_t tmp = 3, result = (digits[0] ? 3 : 1);
  for(std::size_t i = 1;i < size;i++) {
    tmp *= tmp;
    tmp = tmp % mod;
    if (digits[i]) {
      result *= tmp;
      result = result % mod;
    }
  }
  return result;
}

const uint64_t get_answer(uint64_t n) {
  if (n == 1) return 1;
  if (n == 2) return 2;
  if (n == 3) return 3;
  uint64_t k = n / 3;
  switch (n % 3) {
    case 0 : {
      // return 3^k % mod
      return pow3(k);
    }
    case 1 : {
      // return (4 * 3^(k - 1)) % mod
      return (4 * pow3(k - 1)) % mod;
    }
    case 2 : {
      // return (2 * 3^k) % mod
      return (2 * pow3(k)) % mod;
    }
  }
}

int main() {
  std::size_t t;
  std::uint64_t n;
  if (!(std::cin >> t)) return 1;
  while(t > 0) {
    t--;
    if (!(std::cin >> n)) return 1;
    std::cout << get_answer(n) << std::endl;
  }
  return 0;
}