#include <vector>
#include <cmath>
#include <climits>
#include <string>
#include <algorithm>

/**
 * @param T The typename of the sorted objects
 * @param k The number of classes of the sorted objects
 * Reference: https://cp-algorithms.com/string/suffix-array.html
 */
template<typename V, std::size_t K>
std::vector<std::size_t> sortCyclicShifts(const V& input, std::size_t (*key)(const typename V::value_type&)) {
  typedef typename V::value_type T;
  if (input.size() == 0) return std::vector<std::size_t>();
  const std::size_t n = input.size();

  std::vector<std::size_t> p(n, 0);
  std::vector<std::size_t> c(n, 0);

  // counting sort
  std::vector<std::size_t> count(K, 0);
  for(auto i = input.begin();i != input.end();i++) {
    count[key(*i)] += 1;
  }
  std::size_t csum = 0, tmp;
  for(auto i = count.begin();i != count.end();i++) {
    tmp = *i;
    *i = csum;
    csum += tmp;
  }
  for(std::size_t i = 0;i < n;i++) {
    std::size_t j = key(input[i]);
    p[count[j]] = i;
    c[i] = j;
    count[j] += 1;
  }

  // recursion
  std::size_t step = 1, stepBound = std::ceil(std::log(n));
  std::size_t shift = 1;
  while(step <= stepBound) {
    // counting sort i - 2^k
    std::fill(count.begin(), count.end(), 0);
    for(std::size_t i = 0;i < shift;i++) {
      count[c[i + (n - shift)]] += 1;
    }
    for(std::size_t i = shift;i < n;i++) {
      count[c[i - shift]] += 1;
    }
    // reset count / c
    std::vector<std::size_t> cmap(count.size(), 0);
    std::size_t cmapSize = 0;
    std::vector<std::size_t> countTmp;
    for(std::size_t i = 0;i < count.size();i++) {
        if (count[i] > 0) {
            cmap[i] = cmapSize++;
            countTmp.push_back(count[i]);
        }
    }
    std::swap(count, countTmp);
    std::vector<std::size_t> ctmp(n);
    csum = 0;
    for(auto i = count.begin();i != count.end();i++) {
      tmp = *i;
      *i = csum;
      csum += tmp;
    }
    std::size_t countSize = count.size();
    std::vector<std::size_t> ptmp(n);
    for(auto i = p.begin();i != p.end();i++) {
      std::size_t shifted = (*i < shift ? *i + n - shift : *i - shift);
      std::size_t shiftedC = cmap[c[shifted]];
      ptmp[count[shiftedC]] = shifted;
      ctmp[shifted] = shiftedC * cmapSize + cmap[c[*i]];
      if (ctmp[shifted] + 1 > countSize) countSize = ctmp[shifted] + 1;
      count[shiftedC] += 1;
    }
    count.resize(countSize);
    shift = (shift * 2) % n;
    std::swap(c, ctmp);
    std::swap(p, ptmp);
    step++;
  }
  return p;
}

template<typename V, std::size_t K>
std::vector<std::size_t> sortCyclicShifts(const V& input) {
  typedef typename V::value_type T;
  typedef std::size_t (*KeyType)(const T&);
  KeyType key = [](const T& t) {
    return ((std::size_t) t);
  };
  return sortCyclicShifts<V, K>(input, key);
}

std::vector<std::size_t> suffixArray(std::string& input) {
  input.push_back('$');
  auto result(sortCyclicShifts<std::string, 256>(input));
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

std::vector<std::size_t> findSubstring(const std::string& x, const std::vector<std::size_t>& sa, const std::string& q) {
  std::size_t j = 0;
  auto comp = [&x, &j](const std::size_t i, const char c) {
    if (i + j >= x.size()) return true;
    return x[i + j] < c;
  };
  auto it1 = sa.begin(), it2 = sa.end();
  std::vector<std::size_t> result;
  while(j < q.size()) {
    it1 = std::lower_bound(it1, it2, q[j], comp);
    if (it1 == it2) return result;
    if (x[*it1 + j] > q[j]) return result;
    it2 = std::lower_bound(it1, it2, q[j] + 1, comp);
    j++;
  }
  for(auto it = it1;it != it2;it++) {
    result.push_back(*it);
  }
  return result;
}

#include <string>
#include <cstring>
#include <iostream>
#include <set>

int main() {
  std::string str;
  if (!std::getline(std::cin, str)) return 1;
  if (str.size() < 3) {
    std::cout << 0 << "  " << 0 << std::endl;
    return 0;
  }
  auto sa(suffixArray(str));
/*
  for(std::size_t i = 0;i < sa.size();i++) {
    std::cout << (sa[i]) << " ";
  }
  std::cout << std::endl;
*/
  auto lcp(getLCP(str, sa));
/*
  for(std::size_t i = 0;i < lcp.size();i++) {
    std::cout << (lcp[i]) << " ";
  }
  std::cout << std::endl;
*/
/*
  std::string query;
  if (!(std::cin >> query)) return 2;
  auto result(findSubstring(str, sa, query));
  for(std::size_t i = 0;i < result.size();i++) {
    std::cout << result[i] << " ";
  }
  std::cout << std::endl;
*/
  // find all tandem

  //'save processed substring'

  // substring of str
  typedef std::pair<const char * , std::size_t > Substring;
  auto comp = [&str](const Substring& s1, const Substring& s2) {
    std::size_t n = std::min(s1.second, s2.second);
    int r = std::strncmp(s1.first, s2.first, n);
    if (r != 0) return r < 0;
    else return s1.second < s2.second;
  };
  std::set<Substring, decltype(comp)> history(comp);
  std::size_t i = 0;
  std::string tmp;
  std::size_t interesting = 0, boring = 0;
  while(i + 2 < str.size()) {
    // the lcp of nearest three leaf node
    std::size_t lcpN = std::min(lcp[i], lcp[i + 1]);
    Substring candidate(str.data() + sa[i], 1);
    while(candidate.second <= lcpN && candidate.second * 3 <= str.size()) {
      auto hi = history.find(candidate);
      if (hi == history.end()) {
        tmp.resize(candidate.second * 3);
        char* ptmp = &tmp[0];
        std::copy(candidate.first, candidate.first + candidate.second, tmp.begin());
        std::copy(candidate.first, candidate.first + candidate.second, tmp.begin() + candidate.second);
        std::copy(candidate.first, candidate.first + candidate.second, tmp.begin() + 2 * candidate.second);
        auto rtmp(findSubstring(str, sa, tmp));
        if (rtmp.size() > 0) {
          for(std::size_t j = 0;j < rtmp.size();j++) {
      	    std::size_t target = rtmp[j] + tmp.size();
      	    if (target == str.size()) interesting += 1;
      	    else {
      	      if (str[target] == str[0]) boring += 1;
      	      else interesting += 1;
      	    }
          }
        }
        history.insert(candidate);
      }
      candidate.second++;
    }
    i++;
  }
  std::cout << interesting << "  " << boring << std::endl;
  return 0;
}
