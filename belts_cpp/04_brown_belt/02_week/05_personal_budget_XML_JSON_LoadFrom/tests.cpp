#include "tests.h"

using namespace std;

void TestSpendings() {
    const vector<Spending> spendings = {
        {"food", 2'500},
        {"transport", 1'150},
        {"restaurants", 5'780},
        {"clothes", 7'500},
        {"travel", 2'3740},
        {"sport", 12'000}};

    int total = CalculateTotalSpendins(spendings);
    int total_expected = 52670;
    ASSERT_EQUAL(total, total_expected);

    string most_expensive = MostExpensiveCategory(spendings);
    string most_expensive_expected = "travel";
    ASSERT_EQUAL(most_expensive, most_expensive_expected);

    // cout << "Total: " << CalculateTotalSpendins(spendings) << endl;
    // cout << "Most expensive category: \"" << MostExpensiveCategory(spendings) << "\"" << endl;
}

void TestLoadFromXml() {
    istringstream xml_input(R"(<july>
    <spend amount="2500" category="food"></spend>
    <spend amount="1150" category="transport"></spend>
    <spend amount="5780" category="restaurants"></spend>
    <spend amount="7500" category="clothes"></spend>
    <spend amount="23740" category="travel"></spend>
    <spend amount="12000" category="sport"></spend>
  </july>)");

    const vector<Spending> spendings = LoadFromXml(xml_input);

    const vector<Spending> expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}};
    ASSERT_EQUAL(spendings, expected);
}

void TestXmlLibrary() {
    using namespace Xml;
    // Тест демонстрирует, как пользоваться библиотекой из файла xml.h

    istringstream xml_input(R"(<july>
    <spend amount="2500" category="food"></spend>
    <spend amount="23740" category="travel"></spend>
    <spend amount="12000" category="sport"></spend>
  </july>)");

    Document doc = Load(xml_input);
    const Node& root = doc.GetRoot();
    ASSERT_EQUAL(root.Name(), "july");
    ASSERT_EQUAL(root.Children().size(), 3u);

    const Node& food = root.Children().front();
    ASSERT_EQUAL(food.AttributeValue<string>("category"), "food");
    ASSERT_EQUAL(food.AttributeValue<int>("amount"), 2500);

    const Node& sport = root.Children().back();
    ASSERT_EQUAL(sport.AttributeValue<string>("category"), "sport");
    ASSERT_EQUAL(sport.AttributeValue<int>("amount"), 12000);

    Node july("july", {});
    Node transport("spend", {{"category", "transport"}, {"amount", "1150"}});
    july.AddChild(transport);
    ASSERT_EQUAL(july.Children().size(), 1u);
}

void TestMultiple_XML_SectionsInInput() {
    string fname = "input_extended.xml";
    fstream xml_input(fname);

    if (!xml_input) {
        cerr << "File \"" << fname << "\" is not found" << endl;
        return;
    }

    const vector<Spending> july_expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}};

    const vector<Spending> august_expected = {
        {"food", 3200},
        {"transport", 1400},
        {"restaurants", 1500},
        {"clothes", 500},
        {"travel", 0},
        {"sport", 3000}};

    const vector<Spending> september_expected = {
        {"food", 4000},
        {"transport", 2000},
        {"restaurants", 0},
        {"clothes", 900},
        {"travel", 0},
        {"sport", 1000}};

    map<string, vector<Spending>> spendings_by_month = LoadFromXmlByMonths(xml_input);

    ASSERT_EQUAL(spendings_by_month["july"], july_expected);
    ASSERT_EQUAL(spendings_by_month["august"], august_expected);
    ASSERT_EQUAL(spendings_by_month["september"], september_expected);
}

void TestLoadFromJson() {
    istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 5780, "category": "restaurants"},
    {"amount": 7500, "category": "clothes"},
    {"amount": 23740, "category": "travel"},
    {"amount": 12000, "category": "sport"}
  ])");

    const vector<Spending> spendings = LoadFromJson(json_input);

    const vector<Spending> expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}};
    ASSERT_EQUAL(spendings, expected);
}

void TestJsonLibrary() {
    using namespace Json;

    istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 12000, "category": "sport"}
  ])");

    Document doc = Load(json_input);
    const vector<Node>& root = doc.GetRoot().AsArray();
    ASSERT_EQUAL(root.size(), 3u);

    const map<string, Node>& food = root.front().AsMap();
    ASSERT_EQUAL(food.at("category").AsString(), "food");
    ASSERT_EQUAL(food.at("amount").AsInt(), 2500);

    const map<string, Node>& sport = root.back().AsMap();
    ASSERT_EQUAL(sport.at("category").AsString(), "sport");
    ASSERT_EQUAL(sport.at("amount").AsInt(), 12000);

    Node transport(map<string, Node>{{"category", Node("transport")}, {"amount", Node(1150)}});
    Node array_node(vector<Node>{transport});
    ASSERT_EQUAL(array_node.AsArray().size(), 1u);
}

void TestJsonLibraryFromFile() {
    using namespace Json;
    string fname = "input.json";
    fstream json_input(fname);

    if (!json_input) {
        cerr << "File \"" << fname << "\" is not opened." << endl;
        return;
    }

    Document doc = Load(json_input);
    const vector<Node>& root = doc.GetRoot().AsArray();
    ASSERT_EQUAL(root.size(), 6u);

    const map<string, Node>& food = root.front().AsMap();
    ASSERT_EQUAL(food.at("category").AsString(), "food");
    ASSERT_EQUAL(food.at("amount").AsInt(), 2500);

    const map<string, Node>& sport = root.back().AsMap();
    ASSERT_EQUAL(sport.at("category").AsString(), "sport");
    ASSERT_EQUAL(sport.at("amount").AsInt(), 12000);

    Node transport(map<string, Node>{{"category", Node("transport")}, {"amount", Node(1150)}});
    Node array_node(vector<Node>{transport});
    ASSERT_EQUAL(array_node.AsArray().size(), 1u);
}
