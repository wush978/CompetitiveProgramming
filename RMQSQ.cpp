/**
 * https://www.spoj.com/problems/RMQSQ/
 */

#include <iostream>
#include <vector>
#include <cmath>

int main() {
  std::size_t N, Q;
  if (!(std::cin >> N)) return 1;
  std::size_t K = std::floor(std::log2(N)) + 1;
  std::vector< std::vector<int> > A(N);
  for(std::size_t i = 0;i < N;i++) {
    std::size_t len = N - i;
    len = 1 + std::floor(std::log2(len));
    A[i].resize(len);
    if (!(std::cin >> A[i][0])) return 1;
  }
  std::vector<std::size_t> shifts(K, 1);
  for(std::size_t k = 1;k < K;k++) {
    shifts[k] = 2 * shifts[k - 1];
    for(std::size_t i = 0;i + shifts[k] <= N;i++) {
      A[i][k] = std::min(A[i][k-1], A[i + shifts[k-1]][k-1]);
    }
  }

  if (!(std::cin >> Q)) return 1;
  std::size_t i, j, index = 1;
  while(Q > 0) {
    Q--;
    if (!(std::cin >> i >> j)) return 0;
    if (i > j) std::swap(i, j);
    std::size_t len = j - i + 1, k;
    int result = A[i][0];
    do {
      k = std::floor(std::log2(len));
      result = std::min(result, A[i][k]);
      i += shifts[k];
      len -= shifts[k];
    } while(len > 0);
    std::cout << result << std::endl;
  }
  return 0;
}