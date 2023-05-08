#pragma once

#include <map>
#include <set>
#include <memory>  // std::shared_ptr

#include "date.h"

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    Or,
    And
};

class Node {
   public:
    virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
    virtual ~Node() = default;
};

class AlwaysFalseNode : public Node {
    bool Evaluate(const Date &, const std::string& event) const override {
        return false;
    }
};

class EmptyNode : public Node {
   public:
    EmptyNode() noexcept;
    bool Evaluate(const Date& date, const std::string& event) const noexcept override;
};

class DateComparisonNode : public Node {
   public:
    DateComparisonNode(const Comparison& comprs, const Date& date) noexcept;
    bool Evaluate(const Date& date, const std::string& event) const noexcept override;

   private:
    const Comparison _cmp_type;
    const Date _date;
};

class EventComparisonNode : public Node {
   public:
    EventComparisonNode(const Comparison& comprs, const std::string& event);
    bool Evaluate(const Date& date, const std::string& event) const noexcept override;

   private:
    const Comparison _cmp_type;
    const std::string _event;
};

class LogicalOperationNode : public Node {
   public:
    LogicalOperationNode(const LogicalOperation& logical_operation,
                         const std::shared_ptr<Node>& lhs,
                         const std::shared_ptr<Node>& rhs) noexcept;

    bool Evaluate(const Date& date, const std::string& event) const override;

   private:
    const LogicalOperation _logic_operation;
    const std::shared_ptr<Node> _lhs_node;
    const std::shared_ptr<Node> _rhs_node;
};
