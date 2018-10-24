/**
 * https://uva.onlinejudge.org/index.php?option=onlinejudge&page=show_problem&problem=1970
 */

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
  unsigned int T;
  if (!(std::cin >> T)) return 1;
  while(T > 0) {
    T -= 1;
    std::size_t n, k;
    if (!(std::cin >> n)) return 1;
    if (!(std::cin >> k)) return 1;
    // boundary conditions
    if (n == 0) {
      // invalid argument
      return 1;
    }
    if (k == 0) {
      // invalid argument
      return 1;
    }

    double value = std::log10(n);
    value = value * k;
    std::size_t l = 1 + std::floor(value);
    value = value + 3 - l;
    std::size_t leading_result = std::floor(std::pow(10, value));

    std::size_t binary_digit_size = 1 + std::floor(std::log2(k));
    std::vector<bool> binary_digits(binary_digit_size, false);
    for(std::size_t i = 0;i < binary_digit_size;i++) {
      binary_digits[i] = k % 2;
      k = k / 2;
    }
    std::vector<std::size_t> n_powers(binary_digit_size, 0);
    n_powers[0] = n % 1000;
    for(std::size_t i = 1;i < binary_digit_size;i++) {
      n_powers[i] = (n_powers[i - 1] * n_powers[i - 1]) % 1000;
    }
    std::size_t trailing_result = 1;
    for(std::size_t i = 0;i < binary_digit_size;i++) {
      if (binary_digits[i]) {
        trailing_result = (trailing_result * n_powers[i]) % 1000;
      }
    }

    std::cout << std::setfill('0') << std::setw(3) << leading_result << 
      "..." << std::setfill('0') << std::setw(3) << trailing_result << std::endl;
  }
  return 0;
}