#include "load_from.h"
#include "xml.h"
#include "json.h"

using namespace std;

vector<Spending> LoadFromXml(istream& input) {
    // Реализуйте эту функцию с помощью библиотеки xml.h
    using namespace Xml;

    Document doc = Load(input);
    vector<Spending> spendings{};

    for (const Node& node : doc.GetRoot().Children()) {
        spendings.push_back({node.AttributeValue<string>("category"),
                             node.AttributeValue<int>("amount")});
    }

    return spendings;
}

void IgnoreBlankLines(istream& input) {
    string line;
    while (input && input.peek() != '<') {
        getline(input, line);
    }
}

map<string, vector<Spending>> LoadFromXmlByMonths(istream& input) {
    using namespace Xml;

    IgnoreBlankLines(input);

    map<string, vector<Spending>> result;
    while (input.peek() == '<') {
        Document doc = Load(input);
        string_view month = doc.GetRoot().Name();
        vector<Spending> spendings{};

        for (const Node& node : doc.GetRoot().Children()) {
            spendings.push_back({node.AttributeValue<string>("category"),
                                 node.AttributeValue<int>("amount")});
        }

        result.insert({string(month), move(spendings)});
        IgnoreBlankLines(input);
    }

    return result;
}

vector<Spending> LoadFromJson(istream& input) {
    using namespace Json;
    Document doc = Load(input);

    vector<Spending> spendings;
    for (const Node& node : doc.GetRoot().AsArray()) {
        spendings.push_back({node.AsMap().at("category").AsString(),
                             node.AsMap().at("amount").AsInt()});
    }

    return spendings;
}