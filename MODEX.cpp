/**
 * http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=24&page=show_problem&problem=3671
 */

#include <iostream>
#include <vector>
#include <cmath>

int main() {
  std::size_t c, d;
  if (!(std::cin >> c)) return 1;
  std::vector<std::size_t> x(c, 0), y(c, 0), n(c, 0);
  for(std::size_t i = 0;i < c;i++) {
    if (!(std::cin >> x[i])) return 1;
    if (!(std::cin >> y[i])) return 1;
    if (!(std::cin >> n[i])) return 1;
  }
  if (!(std::cin >> d)) return 1;
  if (d != 0) return 1;
  std::vector<std::size_t> buf(64, 0);
  std::vector<std::size_t> power_digits(64, 0);
  for(std::size_t i = 0;i < c;i++) {
    std::fill(buf.begin(), buf.end(), 0);
    std::fill(power_digits.begin(), power_digits.end(), 0);
    std::size_t base = x[i], power = y[i], module = n[i];
    // fill power_digits
    std::size_t size = std::ceil(std::log2(power));
    for(std::size_t j = 0;j < size;j++) {
      power_digits[j] = power % 2;
      power = power / 2;
    }
    buf[0] = base % module;
    for(std::size_t j = 1;j < size;j++) {
      buf[j] = (buf[j - 1] * buf[j - 1]) % module;
    }
    std::size_t result = 1;
    for(std::size_t j = 0;j < size;j++) {
      if (power_digits[j] == 1) {
        result = (result * buf[j]) % module;
      }
    }
    std::cout << result << std::endl;
  }
  return 0;
}