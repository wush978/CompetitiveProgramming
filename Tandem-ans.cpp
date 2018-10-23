#include <string>
#include <vector>
#include <iostream>

bool checkTandem(const std::string& S, std::size_t start, std::size_t L) {
  for(std::size_t k = 0;k < L;k++) {
    if (S[start + k] != S[start + L + k]) return false;
    if (S[start + k] != S[start + 2 * L + k]) return false;
  }
  return true;
}

int main() {
  std::string S;
  if (!(std::cin >> S)) return 1;
  std::size_t N = S.size();
  std::size_t boring_count = 0, interesting_count = 0;
  for(std::size_t L = 1;3 * L <= N;L++) {
    for(std::size_t start = 0;start + 3L <= N;start++) {
      if (checkTandem(S, start, L)) {
        if (start + 3 * L < N && S[start + 3 * L] == S[start]) {
          boring_count += 1;
        } else {
          interesting_count += 1;
        }
      }
    }
  }
  std::cout << interesting_count << " " << boring_count << std::endl;
  return 0;
}