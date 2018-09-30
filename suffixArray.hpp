#include <vector>
#include <cmath>
#include <climits>
#include <string>

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

  std::size_t k;
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
