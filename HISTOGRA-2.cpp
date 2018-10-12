#include <iostream>
#include <stack>
#include <vector>

typedef unsigned long long Int;

int main() {
  std::size_t n;
  std::vector<Int> H;
  while(true) {
    // break conditions
    if (!(std::cin >> n)) return 1;
    if (n == 0) break;
    H.resize(n);
    for(auto& h : H) {
      if (!(std::cin >> h)) return 1;
    }
    // computing
    Int result = 0;
    std::stack<Int> S;
    std::size_t l, r;
    for(std::size_t i = 0;i < n;i++) {
      while(S.size() > 0) {
        std::size_t j = S.top();
        if (H[j] <= H[i]) break;
        S.pop();
        // H[j] > H[i]
        r = i;
        l = (S.empty() ? 0 : S.top() + 1);
        result = std::max(result, H[j] * (r - l));
      }
      S.push(i);
    }
    while(S.size() > 0) {
      std::size_t j = S.top();
      S.pop();
      r = n;
      l = (S.empty() ? 0 : S.top() + 1);
      result = std::max(result, H[j] * (r - l));
    }
    std::cout << result << std::endl;
  }
  return 0;
}