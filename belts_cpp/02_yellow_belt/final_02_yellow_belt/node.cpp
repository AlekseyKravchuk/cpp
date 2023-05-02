#include "node.h"

#include "date.h"

EmptyNode::EmptyNode() noexcept {}

bool EmptyNode::Evaluate(const Date& date, const std::string& event) const noexcept {
    return true;
}

DateComparisonNode::DateComparisonNode(const ComparisonType& cmp_type, const Date& date) noexcept
    : _cmp_type(cmp_type), _date(date) {}

bool DateComparisonNode::Evaluate(const Date& date, const std::string& event) const noexcept {
    switch (_cmp_type) {
        case ComparisonType::Less:
            return date < _date;
        case ComparisonType::LessOrEqual:
            return date <= _date;
        case ComparisonType::Greater:
            return date > _date;
        case ComparisonType::GreaterOrEqual:
            return date >= _date;
        case ComparisonType::Equal:
            return date == _date;
        case ComparisonType::NotEqual:
            return date != _date;
        default:
            return 0;
            break;
    }
}

EventComparisonNode::EventComparisonNode(const ComparisonType& cmp_type, const std::string& event)
    : _cmp_type(cmp_type), _event(event) {}

bool EventComparisonNode::Evaluate(const Date& date, const std::string& event) const noexcept {
    switch (_cmp_type) {
        case ComparisonType::Less:
            return event < _event;
        case ComparisonType::LessOrEqual:
            return event <= _event;
        case ComparisonType::Greater:
            return event > _event;
        case ComparisonType::GreaterOrEqual:
            return event >= _event;
        case ComparisonType::Equal:
            return event == _event;
        case ComparisonType::NotEqual:
            return event != _event;
        default:
            return 0;
            break;
    }
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation& logic_operation,
                                           const std::shared_ptr<Node>& lhs,
                                           const std::shared_ptr<Node>& rhs) noexcept
    : _logic_operation(logic_operation),
      _lhs_node(lhs),
      _rhs_node(rhs) {}

bool LogicalOperationNode::Evaluate(const Date& date, const std::string& event) const {
    switch (_logic_operation) {
        case LogicalOperation::Or:
            return _lhs_node->Evaluate(date, event) || _rhs_node->Evaluate(date, event);
        case LogicalOperation::And:
            return _lhs_node->Evaluate(date, event) && _rhs_node->Evaluate(date, event);
        default:
            return 0;
            break;
    }
}
