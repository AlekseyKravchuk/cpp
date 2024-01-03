#pragma once

#include <istream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Xml {
  
class Node {
   public:
    Node(std::string name, std::unordered_map<std::string, std::string> attrs);

    const std::vector<Node>& Children() const;

    void AddChild(Node node);

    std::string_view Name() const;

    template <typename T>
    T AttributeValue(const std::string& name) const;

   private:
    std::string _name;
    std::vector<Node> _children;
    std::unordered_map<std::string, std::string> _attrs;
};

class Document {
   public:
    explicit Document(Node root);

    const Node& GetRoot() const;

   private:
    Node _root;
};

Document Load(std::istream& input);

template <typename T>
inline T Node::AttributeValue(const std::string& name) const {
    std::istringstream attr_input(_attrs.at(name));
    T result;
    attr_input >> result;

    return result;
}

}  // namespace Xml
