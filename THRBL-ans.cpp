/**
 * https://www.spoj.com/problems/THRBL/
 */

#include <vector>
#include <iostream>
#include <cmath>

int main() {
  std::size_t N, M;
  if (!(std::cin >> N)) return 1;
  if (!(std::cin >> M)) return 1;
  std::size_t K = 1 + std::floor(std::log2(N));
  std::vector<std::size_t> H(N);
  for(std::size_t i = 0;i < N;i++) {
    if (!(std::cin >> H[i])) return 1;
  }
  std::size_t A, B, count = 0;;
  while(M > 0) {
    M--;
    if (!(std::cin >> A >> B)) return 1;
    if (A == 0 || B == 0) return 1;
    if (A > B) std::swap(A, B);
    A -= 1;
    B -= 1;
    if (B - A <= 1) {
      count += 1;
      continue;
    }
    std::size_t result = H[A+1];
    for(std::size_t k = A + 1;k < B;k++) {
      result = std::max(result, H[k]);
    }
    if (H[A] >= result) count += 1;
  }
  std::cout << count << std::endl;
  return 0;
}