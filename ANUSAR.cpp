#include <deque>
#include <vector>
#include <iostream>
#include <functional>
#include <stack>

#define ANUSAR_DEBUG

typedef unsigned long long Int;
typedef std::pair< std::deque<std::size_t> , std::vector<std::size_t> > CountingSultResult;

template<typename Vector>
void countingSort(const Vector& input, std::size_t K, CountingSultResult& result, std::function<std::size_t(const typename Vector::value_type)> converter = [](const typename Vector::value_type v) {
  return (std::size_t) v;
}, bool isSecond = true) {
  std::size_t n = input.size();
  if (n == 0) return;
  if (K == 0) throw std::invalid_argument("K == 0");
  typedef typename Vector::value_type Value;
  result.first.resize(n);
  result.second.resize(n);
  std::vector<std::size_t> count(K, 0);
  for(std::size_t i = 0;i < n;i++) {
    count[converter(input[i])] += 1;
  }
  std::size_t tmp = 0, csum = 0;
  for(std::size_t i = 0;i < K;i++) {
    tmp = count[i];
    count[i] = csum;
    csum += tmp;
  }
  for(std::size_t i = 0;i < n;i++) {
    std::size_t j = converter(input[i]);
    result.first[count[j]++] = i;
  }
  if (!isSecond) return;
  result.second[result.first[0]] = 0;
  for(std::size_t i = 1;i < n;i++) {
    std::size_t vNow = converter(input[result.first[i]]), vLast = converter(input[result.first[i-1]]);
    if (vNow > vLast) {
      result.second[result.first[i]] = result.second[result.first[i-1]] + 1;
    } else {
      #ifdef ANUSAR_DEBUG
      if (vNow < vLast) throw std::logic_error("vNow < vLast");
      #endif
      result.second[result.first[i]] = result.second[result.first[i-1]];
    }
  }
  return;
}

const std::deque<std::size_t> getCyclicSort(const std::string& S) {
  std::size_t n = S.size();
  CountingSultResult pc, pcTmp;
  countingSort(S, 27, pc, [](const char v) {
    return v - ('a' - 1);
  });
  std::size_t cyclicSize = 1;
  auto cyclicShift = [&n, &cyclicSize](std::size_t j) {
    return (j >= cyclicSize ? j - cyclicSize : j + n - cyclicSize);
  };
  auto reverseCyclicShift = [&n, &cyclicSize](std::size_t j) {
    return (j + cyclicSize >= n ? j + cyclicSize - n : j + cyclicSize);
  };
  std::deque<std::size_t> buf(n, 0);
  while(cyclicSize < S.size()) {
    countingSort(pc.first, pc.second[*pc.first.rbegin()] + 1, pcTmp, [&pc, &cyclicShift](const std::size_t i) {
      return pc.second[cyclicShift(i)];
    });
    for(std::size_t i = 0;i < n;i++) {
      std::size_t j = pc.first[pcTmp.first[i]];
      buf[i] = cyclicShift(j);;
    }
    pcTmp.second[buf[0]] = 0;
    for(std::size_t i = 1;i < n;i++) {
      std::size_t j1 = buf[i-1], j2 = buf[i];
      if (pc.second[j1] == pc.second[j2]) {
        std::size_t k1 = reverseCyclicShift(j1), k2 = reverseCyclicShift(j2);
        if (pc.second[k1] == pc.second[k2]) {
          pcTmp.second[j2] = pcTmp.second[j1];
          continue;
        }
      }
      pcTmp.second[j2] = pcTmp.second[j1] + 1;
    }
    pc.first.swap(buf);
    pc.second.swap(pcTmp.second);
    if (pc.second[*pc.first.rbegin()] == n - 1) break;
    cyclicSize *= 2;
  }
  buf.swap(pc.first);
  return buf;
}

const std::deque<std::size_t> getSuffixArray(std::string& S) {
  S.push_back('a' - 1);
  auto result(getCyclicSort(S));
  S.pop_back();
  result.pop_front();
  return result;
}

const std::vector<std::size_t> getLCP(const std::string& S, const std::deque<std::size_t>& suffixArray) {
  std::size_t n = S.size();
  std::vector<std::size_t> rank(n, 0);
  std::vector<std::size_t> lcp(n - 1, 0);
  for(std::size_t i = 0;i < n;i++) {
    rank[suffixArray[i]] = i;
  }
  std::size_t k = 0;
  for(std::size_t i = 0;i < n;i++) {
    if (rank[i] + 1 == n) {
      k = 0;
      continue;
    }
    std::size_t j = suffixArray[rank[i] + 1];
    while(i + k < n && j + k < n && S[i + k] == S[j + k]) {
      k++;
    }
    lcp[rank[i]] = k;
    if (k) k--;
  }
  return lcp;
}

// For all j \in [L[i], R[i]), lcp[j] >= lcp[i], lcp[R[i]+1] < lcp[i] and lcp[L[i]-1] <= lcp[i]
void getLR(const std::vector<std::size_t>& lcp, std::vector<std::size_t>& L, std::vector<std::size_t>& R) {
  std::size_t n = lcp.size();
  L.resize(n, 0);
  R.resize(n, 0);
  std::stack<std::size_t> buf;
  for(std::size_t i = 0;i < n;i++) {
    while(buf.size() > 0) {
      std::size_t j = buf.top();
      if (lcp[j] <= lcp[i]) break;
      // lcp[j] > lcp[i]
      buf.pop();
      R[j] = i;
      L[j] = (buf.empty() ? 0 : buf.top() + 1);
    }
    buf.push(i);
  }
  while(buf.size() > 0) {
    std::size_t j = buf.top();
    buf.pop();
    R[j] = n;
    L[j] = (buf.empty() ? 0 : buf.top() + 1);
  }
}

int main() {
  std::size_t T;
  if (!(std::cin >> T)) return 1;
  // problems
  std::string S;
  std::size_t Q;
  std::vector<std::size_t> F;
  while(T > 0) {
    if (!(std::cin >> S)) return 1;
    for(const auto s : S) if (s < 'a' || s > 'z') return 1;
    if (!(std::cin >> Q)) return 1;
    F.resize(Q);
    for(auto& f : F) {
      if (!(std::cin >> f)) return 1;
    }
    // boundary condition
    if (S.size() == 0) {
      for(auto& f : F) {
        std::cout << 0 << std::endl;
      }
      continue;
    }
    const auto suffixArray(getSuffixArray(S));
    const auto lcp(getLCP(S, suffixArray));
    std::vector<std::size_t> L, R;
    getLR(lcp, L, R);
    // result[i] is the number of substring which occurs exactly i + 1 times
    std::vector<Int> result;
    for(std::size_t i = 0;i < lcp.size();i++) {
      if (lcp[i] == 0) continue;
      // whether this is a duplicated [L[i],R[i])
      bool isDuplicated = (L[i] == 0 ? false : lcp[L[i] - 1] == lcp[i]);
      if (isDuplicated) continue;
      std::size_t boundaryHeight = 0;
      boundaryHeight = std::max(boundaryHeight, (L[i] == 0 ? 0 : lcp[L[i] - 1]));
      boundaryHeight = std::max(boundaryHeight, (R[i] == lcp.size() ? 0 : lcp[R[i]]));
      Int height = lcp[i] - boundaryHeight;
      Int len = R[i] - L[i];
      if (len + 1> result.size()) result.resize(len + 1, 0);
      result[len] += height * (len + 1);
    }
    Int n = S.size();
    for(std::size_t i = 1;i + 1 < result.size();i++) {
      std::size_t j = (result.size() - i - 1);
      result[j] += result[j+1];
    }
    if (result.size() == 0) result.resize(1, 0);
    result[0] = (n % 2 == 0 ? (n / 2) * (n + 1) : ((n + 1) / 2) * n);
    for(auto& f : F) {
      if (f - 1 < result.size()) {
        std::cout << result[f - 1] << std::endl;
      } else {
        std::cout << 0 << std::endl;
      }
    }
    T--;
  }
}
