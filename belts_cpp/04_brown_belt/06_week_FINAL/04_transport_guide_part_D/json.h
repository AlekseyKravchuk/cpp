#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

    class Node : std::variant<
            std::string,
            std::vector<Node>,
            std::map<std::string, Node>,
            int,
            double,
            bool> {
      public:
        using variant::variant;

        const auto& AsArray() const {
            return std::get<std::vector<Node>>(*this);
        }

        const auto& AsMap() const {
            return std::get<std::map<std::string, Node>>(*this);
        }

        int AsInt() const {
            return std::get<int>(*this);
        }

        const auto& AsString() const {
            return std::get<std::string>(*this);
        }

        double AsDouble() const {
            double res;
            try {
                res = std::get<double>(*this);
            } catch (std::bad_variant_access&) {
                res = static_cast<double>(std::get<int>(*this));
            }
            return res;
        }

        bool AsBool() const {
            return std::get<bool>(*this);
        }
    };

    class Document {
      public:
        explicit Document(Node root);

        const Node& GetRoot() const;

      private:
        Node root;
    };

    Document Load(std::istream& input);

}  // namespace Json