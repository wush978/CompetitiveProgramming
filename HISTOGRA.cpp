#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <stack>
#include <limits>
#include <tuple>
#include <memory>

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

struct getResultFrame {
  
  static std::stack<getResultFrame*> getResultStack;

  static const SegmentTree* pTree;

  enum State {
    begin = 0,
    end
  } state;

  std::size_t l;

  std::size_t r;

  SegmentTree::Value node;

  Int result;

  std::shared_ptr<getResultFrame> leftChild, rightChild;

  getResultFrame(std::size_t _l, std::size_t _r, State _state) :
  state(_state), l(_l), r(_r), node(), result(0), leftChild(nullptr), rightChild(nullptr)
  { }

private:
  getResultFrame(const getResultFrame&);
  void operator=(const getResultFrame&);
};

std::stack<getResultFrame*> getResultFrame::getResultStack;

const SegmentTree* getResultFrame::pTree(nullptr);

Int getResult(std::size_t l, std::size_t r, const SegmentTree& tree) {
  {
    std::stack<getResultFrame*> emptyStack;
    getResultFrame::getResultStack.swap(emptyStack);
  }
  getResultFrame::pTree = &tree;
  auto& localStack(getResultFrame::getResultStack);
  typedef getResultFrame::State State;
  getResultFrame root(l, r, State::begin);
  localStack.push(&root);
  while(!localStack.empty()) {
    auto pFrame(localStack.top());
    auto& frame(*pFrame);
    std::cout << "[" << frame.l << "," << frame.r << ") " << frame.state << std::endl;
    localStack.pop();
    switch(frame.state) {
      case State::begin : {
        frame.node = tree.find(frame.l, frame.r);
        frame.result = frame.node.first * (frame.r - frame.l);
        frame.state = State::end;
        localStack.push(&frame);
        if (frame.node.second > frame.l) {
          frame.leftChild.reset(new getResultFrame(frame.l, frame.node.second, State::begin));
          localStack.push(frame.leftChild.get());
        }
        if (frame.r > frame.node.second + 1) {
          frame.rightChild.reset(new getResultFrame(frame.node.second + 1, frame.r, State::begin));
          localStack.push(frame.rightChild.get());
        }
        break;
      }
      case State::end : {
        if (frame.leftChild.get()) {
          frame.result = std::max(frame.result, frame.leftChild->result);
          frame.leftChild.reset();
        }
        if (frame.rightChild.get()) {
          frame.result = std::max(frame.result, frame.rightChild->result);
          frame.rightChild.reset();
        }
        std::cout << "result: " << frame.result << std::endl;
      }
    }
  }
  return root.result;

  /*
  auto node(tree.find(l, r));
  Int result = node.first * (r - l);
  result = std::max(result, (node.second > l ? getResult(l, node.second, tree) : 0));
  result = std::max(result, (r > node.second + 1 ? getResult(node.second + 1, r, tree) : 0));
  return result;
  */
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
