/**
 * https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=24&page=show_problem&problem=310
 */

#include <iostream>
#include <vector>
#include <cmath>

int main() {
  std::size_t B, P, M;
  while((std::cin >> B)) {
    if (!(std::cin >> P)) return 1;
    if (!(std::cin >> M)) return 1;
    // boundary condition
    if (B == 0) {
      std::cout << 0 << std::endl;
      continue;
    }
    if (P == 0) {
      std::cout << 1 << std::endl;
      continue;
    } else if (P == 1) {
      std::cout << B % M << std::endl;
      continue;
    }
    std::size_t size = std::ceil(std::log2(P));
    std::vector<bool> P_digits(size, 0);
    std::vector<std::size_t> B_powers(size, 0);
    for(std::size_t i = 0;i < size;i++) {
      P_digits[i] = P % 2;
      P = P / 2;
    }
    B_powers[0] = B % M;
    for(std::size_t i = 1;i < size;i++) {
      B_powers[i] = (B_powers[i - 1] * B_powers[i - 1]) % M;
    }
    std::size_t result = 1;
    for(std::size_t i = 0;i < size;i++) {
      if (P_digits[i]) {
        result = (result * B_powers[i]) % M;
      }
    }
    std::cout << result << std::endl;
  }
  return 0;
}