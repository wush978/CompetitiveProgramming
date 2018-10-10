#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <stack>
#include <limits>
#include <tuple>

typedef unsigned long long Int;

struct SegmentTree {

  const std::vector<Int>& ref;

  // first: minimal value, second: index in ref
  typedef std::pair<std::size_t, std::size_t> Value;
  std::vector<Value> value;

  std::vector<std::size_t> left, right;

  SegmentTree(const std::vector<Int>& input) :
  ref(input) {
    std::size_t n = input.size();
    std::size_t treeSize = std::pow(2, std::ceil(std::log2(n)) + 1);
    left.resize(treeSize, 0);
    right.resize(treeSize, 0);
    value.resize(treeSize);
    std::stack<std::size_t> L, R, I;
    L.push(0);
    R.push(n);
    I.push(0);
    while(L.size() > 0) {
      std::size_t l = L.top(), r = R.top(), i = I.top();
      L.pop();
      R.pop();
      I.pop();
      // leave 0 as NULL
      left[i] = l + 1;
      right[i] = r + 1;
      if (r - l > 1) {
        std::size_t cut = l + (r - l) / 2;
        L.push(l);
        R.push(cut);
        I.push(2 * i + 1);
        L.push(cut);
        R.push(r);
        I.push(2 * i + 2);
      }
    }
    // insert value
    std::size_t i = treeSize;
    while(i > 0) {
      i--;
      if (left[i] > 0) {
        if (right[i] - left[i] == 1) {
          value[i].first = ref[left[i] - 1];
          value[i].second = left[i] - 1;
        } else {
          const auto& leftChild(value[2 * i + 1]);
          const auto& rightChild(value[2 * i + 2]);
          if (leftChild.first <= rightChild.first) {
            value[i] = leftChild;
          } else {
            value[i] = rightChild;
          }
        }
      }
    }
  }

  const Value find(std::size_t l, std::size_t r, std::size_t i = 0) const {
    if (left[i] >= l + 1 && right[i] <= r + 1) return value[i];
    if (right[i] <= l + 1 || left[i] >= r + 1) return Value(std::numeric_limits<Value::first_type>::max(), 0);
    const Value leftValue(find(l, r, 2 * i + 1));
    const Value rightValue(find(l, r, 2 * i + 2));
    if (leftValue.first <= rightValue.first) {
      return leftValue;
    } else {
      return rightValue;
    }
  }
};

Int getResult(std::size_t l, std::size_t r, const SegmentTree& tree) {
  auto node(tree.find(l, r));
  Int result = node.first * (r - l);
  result = std::max(result, (node.second > l ? getResult(l, node.second, tree) : 0));
  result = std::max(result, (r > node.second + 1 ? getResult(node.second + 1, r, tree) : 0));
  return result;
}

int main() {
  std::size_t n;
  std::vector<Int> H;
  while(true) {
    if (!(std::cin >> n)) return 1;
    if (n == 0) break;
    H.resize(n);
    for(auto& h : H) {
      if (!(std::cin >> h)) return 1;
    }
    SegmentTree tree(H);
    std::cout << getResult(0, n, tree) << std::endl;
  }
  return 0;
}