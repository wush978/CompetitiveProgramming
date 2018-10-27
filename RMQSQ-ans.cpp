/**
 * https://www.spoj.com/problems/RMQSQ/
 */

#include <iostream>
#include <vector>
#include <cmath>

int main() {
  std::size_t N, Q;
  if (!(std::cin >> N)) return 1;
  std::vector<int> A(N, 0);
  for(int& a : A) if (!(std::cin >> a)) return 1;
  if (!(std::cin >> Q)) return 1;
  std::size_t i, j;
  while(Q > 0) {
    Q--;
    if (!(std::cin >> i >> j)) return 1;
    if (i > j) std::swap(i,j);
    int result = A[i];
    for(std::size_t k = i + 1;k <= j;k++) {
      result = std::min(result, A[k]);
    }
    std::cout << result << std::endl;
  }
  return 0;
}