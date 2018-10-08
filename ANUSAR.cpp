#include <vector>
#include <cmath>
#include <climits>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>


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
  std::size_t csum = 0, tmp;
  for(std::size_t i = 0;i < count.size();i++) {
    tmp = count[i];
    count[i] = csum;
    csum += tmp;
  }
  for(std::size_t i = 0;i < n;i++) {
    unsigned char j = static_cast<unsigned char>(x[i]);
    p[count[j]] = i;
    count[j] += 1;
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
    count.resize(cValue + 1);
    std::fill(count.begin(), count.end(), 0);
    for(std::size_t i = 0;i < shift;i++) {
      count[c[i + (n - shift)]] += 1;
    }
    for(std::size_t i = shift;i < n;i++) {
      count[c[i - shift]] += 1;
    }
    csum = 0;
    for(std::size_t i = 0;i < count.size();i++) {
      tmp = count[i];
      count[i] = csum;
      csum += tmp;
    }
    std::fill(ptmp.begin(), ptmp.end(), 0);
    std::fill(ctmp.begin(), ctmp.end(), 0);
    for(std::size_t& i : p) {
      std::size_t shifted = (i < shift ? i + n - shift : i - shift);
      std::size_t shiftedC = c[shifted];
      ptmp[count[shiftedC]] = shifted;
      count[shiftedC] += 1;
    }
    cValue = 0;
    ctmp[ptmp[0]] = cValue;
    for(std::size_t i = 1;i < n;i++) {
      std::size_t j = ptmp[i];
      std::size_t jShifted = j + shift;
      if (jShifted >= n) jShifted -= n;
      std::size_t c1 = c[j], c2 = c[jShifted];
      // c[jShifted] , c[j]
      j = ptmp[i - 1];
      jShifted = j + shift;
      if (jShifted >= n) jShifted -= n;
      std::size_t d1 = c[j], d2 = c[jShifted];
      if (c1 > d1 || (c1 == d1 && c2 > d2)) {
        cValue++;
      }
      if (c1 < d1) throw std::logic_error("c1 < d1");
      if (c1 == d1 && c2 < d2) throw std::logic_error("c2 < d2");
      ctmp[ptmp[i]] = cValue;
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

    struct RankComp {
      const std::vector<int>& ref;
      
      RankComp(const std::vector<int>& _ref) : 
      ref(_ref) 
      { }

      bool operator()(const std::size_t i, const std::size_t j) {
        if (i < 0 || j < 0) return i < j;
        if (i >= ref.size() || j >= ref.size()) return i < j;
        return ref[i] < ref[j];
      }
    };
    std::set<std::size_t, RankComp> rankF(F);
    for(std::size_t fi = 0;fi < q;fi++) {
      rankF.insert(fi);
    }
    const auto sa(suffixArray(input));
    const auto lcp(getLCP(input, sa));
    std::vector<std::size_t> results(q, 0), len(q, 0), lastLen(q, 0);

    for(std::size_t i = 0;i < n;i++) {
      std::multiset<std::size_t> lcpSet;
      
      for(const auto& fi : rankF) {
        std::size_t l = n - i;
        const int f = F[fi];
        std::size_t targetSize = f - 1;
        while(lcpSet.size() < targetSize) {
          lcpSet.insert(lcp[i + lcpSet.size()]);
        }
        len[fi] = (lcpSet.size() > 0 ? *lcpSet.begin() : l);
      }
    }

    for(const auto& fi : rankF) {
      const int f = F[fi];
      std::size_t result = 0;
      if (f <= 0) {
        results[fi] = result;
        continue;
      }
      if (f >= n + 1) {
        results[fi] = result;
        continue;
      }
      if (f == 1) {
        if (n % 2 == 0) {
          result = (n / 2) * (n + 1);
        }
        else {
          result = ((n + 1) / 2) * n;
        }
        results[fi] = result;
        continue;
      }
      std::multiset<std::size_t> lcpSet;
      std::size_t lastLen = 0;
      std::size_t i = 0;
      for(std::size_t j = 0;j < f - 1;j++) {
        lcpSet.insert(lcp[i + j]);
      }
      std::size_t len = *lcpSet.begin();
      result += len * f - std::min(len, lastLen) * (f - 1);
      lastLen = len;
      i++;
      while(i < n - f + 1) {
        auto it = lcpSet.find(lcp[i - 1]);
        lcpSet.erase(it);
        lcpSet.insert(lcp[i + f - 2]);
        len = *lcpSet.begin();
        result += len * f - std::min(len, lastLen) * (f - 1);
        lastLen = len;
        i++;
      }
      /*
      for(std::size_t i = 0;i < n - f + 1;i++) {
        std::size_t len = n - sa[i];
        for(std::size_t j = 0;j < f - 1;j++) {
          len = std::min(len, lcp[i + j]);
        }
        result += len * f - std::min(len, lastLen) * (f - 1);
        lastLen = len;
      }
      */
      results[fi] = result;
    }
    for(const auto& r : results) {
      std::cout << r << std::endl;
    }
  }
  return 0;
}
