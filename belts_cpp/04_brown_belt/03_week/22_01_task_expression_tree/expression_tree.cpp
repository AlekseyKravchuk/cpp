#include "Common.h"
#include "test_runner.h"

#include <sstream>
#include <utility>  // std::move

using namespace std;

class ValueExpression : public Expression {
  public:
    explicit ValueExpression(int value) : _value(value) {}
    virtual int Evaluate() const override { return _value; }
    virtual std::string ToString() const override;

  protected:
    int _value;
};
// ====================================================================

template <typename BinaryOperation, char OpSymbol = 'o'>
class BinaryExpression : public Expression {
  public:
    explicit BinaryExpression(ExpressionPtr left, ExpressionPtr right);
    int Evaluate() const override;
    std::string ToString() const override;

  protected:
    static const char op_symbol = OpSymbol;
    BinaryOperation _op;
    ExpressionPtr _left;
    ExpressionPtr _right;
};

// ====================================================================
std::string ValueExpression::ToString() const {
    ostringstream oss;
    oss << std::to_string(_value);
    return oss.str();
}
// ====================================================================

template<typename BinaryOperation, char OpSymbol>
BinaryExpression<BinaryOperation, OpSymbol>::BinaryExpression(ExpressionPtr left,
                                                              ExpressionPtr right)
        : _left(std::move(left)),
          _right(std::move(right)) {}

template<typename BinaryOperation, char OpSymbol>
int BinaryExpression<BinaryOperation, OpSymbol>::Evaluate() const {
    return _op(_left->Evaluate(), _right->Evaluate());
}

template<typename BinaryOperation, char OpSymbol>
std::string BinaryExpression<BinaryOperation, OpSymbol>::ToString() const {
    ostringstream oss;
    oss << '(' << _left->ToString() << ')' << op_symbol << '('
        << _right->ToString() << ')';
    return oss.str();
}
// ====================================================================
using SumExpr = BinaryExpression<std::plus<int>, '+'>;
using MultExpr = BinaryExpression<std::multiplies<int>, '*'>;

ExpressionPtr Value(int value) { return make_unique<ValueExpression>(value); }

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<SumExpr>(std::move(left), std::move(right));
}

ExpressionPtr  Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<MultExpr>(std::move(left), std::move(right));
}
// ====================================================================

string Print(const Expression* e) {
    if (!e) {
        return "Null expression provided";
    }
    stringstream output;
    output << e->ToString() << " = " << e->Evaluate();
    return output.str();
}

void Test() {
    ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
    ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

    ExpressionPtr e2 = Sum(std::move(e1), Value(5));
    ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

    ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    return 0;
}