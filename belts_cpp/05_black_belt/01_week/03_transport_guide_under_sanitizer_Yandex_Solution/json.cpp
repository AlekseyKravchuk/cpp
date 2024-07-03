#include "json.h"

#include <queue>
#include <cassert>
#include <iostream>

using namespace std;


namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      } else continue;
      
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadVal(istream& input) {
    short base = 1;
    if (input.peek() == '-') {
      base = -1;
      input.ignore();
    }
    int result = 0;
    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    } if (input.peek() == '.') {
      input.ignore();
      double result_double = result;
      int denom = 10;
      while (isdigit(input.peek())) {
	result_double += static_cast<double>(input.get() - '0') / denom;
	denom *= 10;
      }
      return Node(result_double * base);
    }
    else return Node(result * base);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        continue;
      }

      string key = LoadString(input).AsString();
      input >> c;
      assert(c != ',');
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadBool(istream& input) {
    assert(input.good());
    static const string trueStr = "true";
    static const string falseStr = "false";
    string err;
    bool check_true = true, check_false = true;
    for (size_t i = 0; i + (check_true) < falseStr.size();
	 i++) {
    char c;
    input >> c;
    err.push_back(c);
      if (check_true)
	check_true = c == trueStr[i];
      if (check_false)
	check_false = c == falseStr[i];
    }
    if (check_true == check_false) throw logic_error(err);
    //assert(check_true != check_false);
    return Node(check_true && !check_false);
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;
    assert(c != ',');
    assert(c != ':');
    assert(c != ']');
    assert(c != '}');
    
    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (c == '-' || isdigit(c)) {
      input.putback(c);
      return LoadVal(input);
    } else {
      input.putback(c);
      return LoadBool(input);
    }
  }

  queue<string> PrintNode(const Node& node);

// struct ShitPrinter {
//     queue<string> operator() (string s) {
//     return queue<string>({"\"" + s + "\""});
//   }
    
//   queue<string> operator() (vector<Node> v) {
//     queue<string> result;
//     result.push("[");
//     for (const Node& val : v) {
//       result.push("  ");
//       queue<string> valStr = PrintNode(val);
//       if (!valStr.empty()) {
// 	do {
// 	  result.back() += move(valStr.front());
// 	  valStr.pop();
// 	  result.push("  ");
// 	} while (!valStr.empty());
// 	result.back() = ",";
//       }
//     }
//     if (!v.empty()) result.back().pop_back();
//     result.push("]");
//     return result;
//   }
  
//   queue<string> operator() (map<string, Node> m) {
//     queue<string> result;
//     result.push("{");
//     for (const auto& [key, val] : m) {
//       result.push("  ");
//       result.back() += ShitPrinter{}(key).front();
//       result.back() += ": ";
//       queue<string> valStr = PrintNode(val);
//       if (!valStr.empty()) {
// 	do {
// 	  result.back() += move(valStr.front());
// 	  valStr.pop();
// 	  result.push("  ");
// 	} while (!valStr.empty());
// 	result.back() = ",";
//       }
//     }
//     if (!m.empty()) result.back().pop_back();
//     result.push("}");
//     return result;
//   }

//   queue<string> operator() (int i) {
//     return queue<string>({to_string(i)});
//   }

//   queue<string> operator() (double d) {
//     return queue<string>({to_string(d)});
//   }

//   queue<string> operator() (bool b) {
//       return queue<string>({ (b) ? "true" : "false"});
//   }
//   };
  queue<string> PrintString(string s) {
     return queue<string>({"\"" + s + "\""});
  }
  queue<string> PrintArray(vector<Node> v) {
    queue<string> result;
    result.push("[");
    for (const Node& val : v) {
      result.push("  ");
      queue<string> valStr = PrintNode(val);
      if (!valStr.empty()) {
	result.back() += valStr.front();
	valStr.pop();
	while (!valStr.empty()) {
	  result.push("  " + valStr.front());
	  valStr.pop();
	}
	result.back() += ",";
      }
    }
   if (!v.empty()) {
      result.back().pop_back();
      result.push("");
    }
    result.back() += "]";
    return result;
  }
  
  queue<string> PrintMap(map<string, Node> m) {
    queue<string> result;
    result.push("{");
    for (const auto& [key, val] : m) {
      result.push("  ");
      result.back() += PrintString(key).front();
      result.back() += ": ";
      queue<string> valStr = PrintNode(val);
      if (!valStr.empty()) {
	result.back() += valStr.front();
	valStr.pop();
	while (!valStr.empty()) {
	  result.push("  " + valStr.front());
	  valStr.pop();
	}
	result.back() += ",";
      }
    }
    if (!m.empty()) {
      result.back().pop_back();
      result.push("");
    }
    result.back() += "}";
    return result;
  }

  queue<string> PrintInt(int i) {
    return queue<string>({to_string(i)});
  }

  queue<string> PrintDouble(double d) {
    return queue<string>({to_string(d)});
  }

  queue<string> PrintBool(bool b) {
      return queue<string>({ (b) ? "true" : "false"});
  }

  queue<string> PrintNode(const Node& node) {
    if (holds_alternative<vector<Node>>(node))
      return PrintArray(node.AsArray());
    if (holds_alternative<map<string,Node>>(node))
      return PrintMap(node.AsMap());
    if (holds_alternative<int>(node))
      return PrintInt(node.AsInt());
    if (holds_alternative<double>(node))
      return PrintDouble(node.AsDouble());
    if (holds_alternative<string>(node))
      return PrintString(node.AsString());
    return PrintBool(node.AsBool());
    // return visit(overloaded{PrintArray,PrintBool,PrintDouble,PrintInt,PrintMap,PrintString},node);
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

  void Print(ostream& os, Document doc) {
    queue<string> str = PrintNode(doc.GetRoot());
    while (!str.empty()) {
      os << move(str.front()) << "\n";
      str.pop();
    }
  }
}
