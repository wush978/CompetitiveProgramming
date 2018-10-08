#include <vector>
#include <string>
#include <iostream>

int main() {
  int t;
  if (!(std::cin >> t)) return 1;
  for(int ti = 0;ti < t;ti++) {
    std::string input;
    if (!(std::cin >> input)) return 1;
    std::size_t n = input.size();
    int q;
    if (!(std::cin >> q)) return 1;
    std::vector<int> F(q, 0);
    for(auto& f : F) {
      if (!(std::cin >> f)) return 1;
    }
    for(const auto& f : F) {
      std::size_t result = 0;
      for(std::size_t i = 0;i < input.size();i++) {
        for(std::size_t j = i + 1;j <= input.size();j++) {
          // input[i:j]
          std::size_t len = j - i;
          std::size_t matchSize = 0;
          for(std::size_t k = 0;k < input.size();k++) {
            if (k + len > input.size()) continue;
            bool isMatch = true;
            for(std::size_t l = 0;l < len;l++) {
              if (input[i + l] != input[k + l]) {
                isMatch = false;
                break;
              }
            }
            if (isMatch) matchSize += 1;
          }
          if (matchSize >= f) result += 1;
        }
      }
      std::cout << result << std::endl;
    }
  }
  return 0;
}