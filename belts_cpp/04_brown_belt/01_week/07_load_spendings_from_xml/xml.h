#pragma once

#include <istream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

class Node {
  public:
    Node(string name, unordered_map<string, string> attrs);

    const vector<Node>& Children() const;
    void AddChild(Node node);
    string_view Name() const;

    template <typename T>
    T AttributeValue(const string& name) const;

  private:
    string _name;
    vector<Node> _children;
    unordered_map<string, string> _attrs;
};

class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node _doc_root;
};

Document Load(istream& input);

template <typename T>
inline T Node::AttributeValue(const string& name) const {
    istringstream attr_input(_attrs.at(name));
    T result;
    attr_input >> result;
    return result;
}
