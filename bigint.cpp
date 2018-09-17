#include <cstdint>
#include <iostream>
#include <algorithm>
#include <vector>

class BigInt {
  typedef uint8_t DataType;
  std::vector<DataType> data;
public:
  BigInt(const std::string& input) :
    data(input.size(), 0)
  {
    for(std::size_t i = 0;i < input.size();i++) {
      const char c = input[i];
      if (c < 0x30 | c > 0x39) throw std::invalid_argument("invalid character");
      data[i] = c - 0x30;
    }
  }

  BigInt(const BigInt& src) :
    data(src.data) 
  { }

  BigInt(const std::vector<DataType>& src) :
    data(src)
  { }

  void operator=(const BigInt& src) {
    data.resize(src.data.size());
    std::copy(src.data.begin(), src.data.end(), data.begin());
  }

  const std::string str() const {
    std::string result(data.size(), 0x0);
    std::transform(data.begin(), data.end(), result.begin(), [ ](const DataType i) {
      return i + 0x30;
    });
    return result;
  }

  const BigInt operator+(const BigInt& input) const {
    const BigInt *pa = this;
    const BigInt *pb = &input;
    if (this->data.size() < input.data.size()) {
      std::swap(pa, pb);
    }
    const BigInt &a(*pa), &b(*pb);

    bool is_carry(false);
    auto ia(a.data.rbegin()), ib(b.data.rbegin());
    std::vector<BigInt::DataType> result;
    while(ia != a.data.rend() & ib != b.data.rend()) {
      BigInt::DataType r = *ia + *ib;
      if (is_carry) r += 1;
      if (r >= 10) {
        result.push_back(r - 10);
        is_carry = true;
      } else {
        result.push_back(r);
        is_carry = false;
      }
      ia++;
      ib++;
    }
    while(ia != a.data.rend()) {
      BigInt::DataType r;
      if (is_carry) r = *ia + 1;
      else r = *ia;
      if (r >= 10) {
        result.push_back(r - 10);
        is_carry = true;
      } else {
        result.push_back(r);
        is_carry = false;
      }
      ia++;
    }
    if (is_carry) result.push_back(1);
    std::reverse(result.begin(), result.end());
    return BigInt(std::move(result));
  }

  const BigInt operator-(const BigInt& b) const {
    const BigInt& a(*this);

  }
};

int main() {
  std::string input, operation;
  if (std::cin >> input) {
    operation = std::move(input);
  } else throw std::invalid_argument("invalid operation");

  if (operation.compare("add") == 0) {
    if (!(std::cin >> input)) {
      throw std::invalid_argument("failed to read input");
    }
    BigInt a(input);
    if (!(std::cin >> input)) {
      throw std::invalid_argument("failed to read input");
    }
    BigInt b(input);
    std::cout << a.str() << " + " << b.str() << " = " << (a+b).str() << std::endl;
  }
  else {
    throw std::invalid_argument("unknown operation");
  }
  return 0;
}
  
