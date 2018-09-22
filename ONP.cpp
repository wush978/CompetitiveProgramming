#include <memory>
#include <iostream>

struct Expression;
typedef std::shared_ptr<Expression> pExp;

struct Expression {
  
  virtual ~Expression() = default;
  
  friend std::ostream& operator<<(std::ostream& os, Expression& exp) {
    exp.print(os, 0);
    return os;
  }

  virtual void print(std::ostream& os, std::size_t lv) = 0;

  virtual void printRPN(std::ostream& os) = 0;
};

struct BracetExpression : public Expression {
  pExp child;
  BracetExpression(pExp _child) : 
    child(_child) 
    { }
  virtual void print(std::ostream& os, std::size_t lv) {
    child->print(os, lv);
  }
  virtual void printRPN(std::ostream& os) { child->printRPN(os); }
};

struct ValueExpression : public Expression {
  const char sym;
  ValueExpression(const char c) : 
    sym(c)
    { }
  virtual void print(std::ostream& os, std::size_t lv) {
    for(std::size_t i = 0;i < lv;i++) {
      os << "  " ;
    }
    os << sym << "\n";
  }
  virtual void printRPN(std::ostream& os) { os << sym; }
};

struct BinaryExpression : public Expression {
  const char op;
  pExp left;
  pExp right;
  BinaryExpression(const char _op, pExp _l, pExp _r) :
    op(_op), left(_l), right(_r)
    {
      if (op != '+' & op != '-' & op != '*' & op != '/' & op != '^') throw std::invalid_argument("invalid operator");
    }
  virtual void print(std::ostream& os, std::size_t lv) {
    left->print(os, lv + 1);
    for(std::size_t i = 0;i < lv;i++) {
      os << "  ";
    }
    os << op << "\n";
    right->print(os, lv + 1); 
  }
  virtual void printRPN(std::ostream& os) {
    left->printRPN(os);
    right->printRPN(os);
    os << op;
  }
};

std::size_t getSize(const char* start, std::size_t size) {
  if (size < 3) throw std::invalid_argument("extract (<sub exp>) from expression whose length < 3");
  std::size_t depth = 1;
  const char* p = start + 1;
  while(true) {
    if (*p == '(') depth++;
    else if (*p == ')') {
      if (depth == 1) break;
      depth--;
    }
    p++;
  }
  std::size_t retval = p - start + 1;
  return retval;
}

pExp parse(const char* start, const std::size_t size) {
  if (size == 0) throw std::invalid_argument("empty");
  const char l = start[0];
  size_t lsize;
  pExp L;
  if (l >= 'a' & l <= 'z') {
    L.reset(new ValueExpression(l));
    lsize = 1;
  } else if (l == '(') {
    lsize = getSize(start, size);
    L = parse(start + 1, lsize - 2);
  } else throw std::runtime_error("failed to parse left");
  if (lsize == size) {
    return pExp(new BracetExpression(L));
  }
  const char op = start[lsize];
  const char r = start[lsize + 1];
  size_t rsize;
  pExp R;
  if (r >= 'a' & r <= 'z') {
    R.reset(new ValueExpression(r));
    rsize = 1;
  } else if (r == '(') {
    rsize = getSize(start + lsize + 1, size - lsize - 1);
    R = parse(start + lsize + 2, rsize - 2);
  } else throw std::runtime_error("failed to parse right");
  if (lsize + 1 + rsize != size) throw std::invalid_argument("size inconsistent");
  return pExp(new BinaryExpression(op, L, R));
}

pExp parse(const std::string& exp) {
  return parse(exp.c_str(), exp.size());
}

int main() {
  int t;
  if (!(std::cin >> t)) return 1;
  for(t;t > 0;t--) {
    std::string exp;
    if (!(std::cin >> exp)) return 1;
    pExp pexp(parse(exp));
    pexp->printRPN(std::cout);
    std::cout << std::endl;
  }
  return 0;
}
