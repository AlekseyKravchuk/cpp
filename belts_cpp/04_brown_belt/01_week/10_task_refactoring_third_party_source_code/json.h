#pragma once

#include <istream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace Json {

class Node {
  public:
    explicit Node(std::vector<Node> array);
    explicit Node(std::map<std::string, Node> map);
    explicit Node(int value);
    explicit Node(std::string value);

    const std::vector<Node>& AsArray() const;
    const std::map<std::string, Node>& AsMap() const;
    int AsInt() const;
    const std::string& AsString() const;

  private:
    std::vector<Node> _as_array;
    std::map<std::string, Node> _as_map;
    int _as_int;
    std::string _as_string;
};

class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
};

Document Load(std::istream& input);

} // namespace Json
