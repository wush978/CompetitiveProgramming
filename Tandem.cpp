#include <vector>
#include <cmath>
#include <climits>
#include <string>
#include <algorithm>
#include <iostream>


/**
 * @param T The typename of the sorted objects
 * @param k The number of classes of the sorted objects
 * Reference: https://cp-algorithms.com/string/suffix-array.html
 */
std::vector<std::size_t> sortCyclicShifts(const std::string& x) {
  std::size_t n = x.size();
  std::vector<std::size_t> p(n, 0), c(n, 0);
  const std::size_t K = 256;
  // counting sort
  std::vector<std::size_t> count(K, 0);
  for(std::size_t i = 0;i < x.size();i++) {
    count[static_cast<unsigned char>(x[i])] += 1;
  }
  for(std::size_t i = 1;i < count.size();i++) {
    count[i] += count[i - 1];
  }
  for(std::size_t i = 0;i < n;i++) {
    unsigned char j = static_cast<unsigned char>(x[i]);
    p[count[j]] = i;
  }
  std::size_t cValue = 0;
  c[p[0]] = cValue;
  for(std::size_t i = 1;i < n;i++) {
    if (x[p[i]] != x[p[i-1]]) cValue++;
    c[p[i]] = cValue;
  }
  // recursion
  std::size_t step = 1, stepBound = std::ceil(std::log2((double) n)), shift = 1;
  std::vector<std::size_t> ctmp(n, 0), ptmp(n, 0);
  while(step <= stepBound) {
    count.resize(cValue);
    std::fill(count.begin(), count.end(), 0);
    for(std::size_t i = 0;i < shift;i++) {
      count[c[i + (n - shift)]] += 1;
    }
    for(std::size_t i = shift;i < n;i++) {
      count[c[i - shift]] += 1;
    }
    for(std::size_t i = 1;i < count.size();i++) {
      count[i] += count[i-1];
    }
    std::fill(ptmp.begin(), ptmp.end(), 0);
    std::fill(ctmp.begin(), ctmp.end(), 0);
    for(std::size_t& i : p) {
      std::size_t shifted = (i < shift ? i + n - shift : i - shift);
      std::size_t shiftedC = c[shifted];
      ptmp[count[shiftedC]] = shifted;
    }

    shift = (shift * 2) % n;
    std::swap(c, ctmp);
    std::swap(p, ptmp);
    step++;
  }
  return p;
}

std::vector<std::size_t> suffixArray(std::string& input) {
  input.push_back(0x0);
  auto result(sortCyclicShifts(input));
  result.erase(result.begin());
  input.pop_back();
  return result;
}

std::vector<std::size_t> getLCP(const std::string& input, const std::vector<std::size_t>& sa) {
  std::size_t n = input.size();
  std::vector<std::size_t> rank(n, 0);
  for(std::size_t i = 0;i < sa.size();i++) {
    rank[sa[i]] = i;
  }

  std::size_t k = 0;
  std::vector<std::size_t> result(n - 1, 0);
  for(std::size_t i = 0;i < n;i++) {
    if (rank[i] == n - 1) {
      k = 0;
      continue;
    }
    std::size_t j = sa[rank[i] + 1];
    while(i + k < n && j + k < n && input[i + k] == input[j + k]) {
      k++;
    }
    result[rank[i]] = k;
    if (k > 0) k--;
  }
  return result;
}

#include <iostream>

std::vector<std::size_t> findSubstring(const std::string& x, const std::vector<std::size_t>& sa, const std::size_t start, const std::size_t end) {
  std::size_t j = 0;
  auto comp = [&x, &j](const std::size_t i, const char c) {
    if (i + j >= x.size()) return true;
    return x[i + j] < c;
  };
  auto it1 = sa.begin(), it2 = sa.end();
  std::vector<std::size_t> result;
  while(j < end - start) {
    it1 = std::lower_bound(it1, it2, x[start + j], comp);
    if (it1 == it2) return result;
    if (x[*it1 + j] > x[start + j]) return result;
    it2 = std::lower_bound(it1, it2, x[start + j] + 1, comp);
    j++;
  }
  for(auto it = it1;it != it2;it++) {
    result.push_back(*it);
  }
  return result;
}

bool isTandem(const std::string& s, const std::size_t start, const std::size_t end) {
  if ((end - start) % 3 != 0) return false;
  std::size_t n = (end - start) / 3;
  const char* p1 = s.c_str() + start;
  const char* p2;
  for(std::size_t k = 0;k < 3;k++) {
    p2 = s.c_str() + start;
    for(std::size_t i = 0;i < n;i++) {
      if (*p1++ != *p2++) return false;
    }
  }
  return true;
}

int main() {
  std::string str;
  if (!std::getline(std::cin, str)) return 1;
  if (str.size() < 3) {
    std::cout << 0 << "  " << 0 << std::endl;
    return 0;
  }
  auto sa(suffixArray(str));
  for(auto i : sa) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return 0;
  // inverse index of suffix array
  std::vector<std::size_t> rank(sa.size(), 0);
  for(std::size_t i = 0;i < sa.size();i++) {
    rank[sa[i]] = i;
  }

  auto lcp(getLCP(str, sa));
  return 0;
/*
  for(auto i : sa) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  for(auto i : rank) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  for(auto i : lcp) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
*/
  // t1: next == first
  // t2: next != first
  // t3: no next
  std::size_t t1 = 0, t2 = 0;
  std::vector<std::size_t> lastT3Head;
  for(std::size_t i = 3;i <= str.size();i++) {
    // t3 becomes t1 or t2
    const char tail = str[i-1];
    for(auto j : lastT3Head) {
      if (str[j] == tail) t1++;
      else t2++;
    }
    lastT3Head.clear();
    // check suffixes is tandem or not
    for(std::size_t j = 0;j < i - 2;j++) {
      // consider src[j:i]
      if ((i - j) % 3 != 0) continue;
      // check if it is a monotone sequence in suffix array
      std::size_t n = (i - j) / 3;
      int s1 = (rank[j + n] > rank[j] ? 1 : -1);
      int s2 = (rank[j + 2 * n] > rank[j + n] ? 1 : -1);
      if (s1 * s2 < 0) continue;
      // check minimal lcp
      std::size_t start = rank[j];
      std::size_t end = rank[j + 2 * n];
      if (start > end) {
        auto tmp = start;
        start = end;
        end = tmp;
      }
      bool isContinue = false;
      for(std::size_t k = start;k < end;k++) {
        if (lcp[k] < n) {
          isContinue = true;
          break;
        }
      }
      if (isContinue) continue;
      lastT3Head.push_back(j);
    }
  }
  std::cout << t2 + lastT3Head.size() << "  " << t1 << std::endl;
  return 0;
}
