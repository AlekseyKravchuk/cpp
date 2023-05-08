#include "tests.h"
#include "node.h"
#include "condition_parser.h"
#include "test_runner.h"

#include <cassert>

std::string Entry(const Date &date, const std::string &event) {
    std::ostringstream ostream;
    ostream << date << " " << event;
    return ostream.str();
}

auto get_predicate(const std::string& s) {
    std::istringstream iss(s);
    auto condition = ParseCondition(iss);

    auto predicate = [condition](const Date &date, const std::string &event) {
        return condition->Evaluate(date, event);
    };

    return predicate;
}

void TestParseCondition() {
    {
        std::istringstream iss("date != 2017-11-18"s);
        std::shared_ptr<Node> root = ParseCondition(iss);
        Assert(root->Evaluate({2017, 1, 1}, ""s), "Parse condition 1"s);
        Assert(!root->Evaluate({2017, 11, 18}, ""s), "Parse condition 2"s);
    }
    {
        std::istringstream iss(R"(event == "sport event")");
        std::shared_ptr<Node> root = ParseCondition(iss);
        Assert(root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 3");
        Assert(!root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 4");
    }
    {
        std::istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 5");
        Assert(root->Evaluate({2017, 3, 1}, ""), "Parse condition 6");
        Assert(root->Evaluate({2017, 6, 30}, ""), "Parse condition 7");
        Assert(!root->Evaluate({2017, 7, 1}, ""), "Parse condition 8");
        Assert(!root->Evaluate({2016, 12, 31}, ""), "Parse condition 9");
    }
    {
        std::istringstream is(R"(event != "sport event" AND event != "Wednesday")");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 10");
        Assert(!root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 11");
        Assert(!root->Evaluate({2017, 1, 1}, "Wednesday"), "Parse condition 12");
    }
    {
        std::istringstream is(R"(event == "holiday AND date == 2017-11-18")");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(!root->Evaluate({2017, 11, 18}, "holiday"), "Parse condition 13");
        Assert(!root->Evaluate({2017, 11, 18}, "work day"), "Parse condition 14");
        Assert(root->Evaluate({1, 1, 1}, "holiday AND date == 2017-11-18"), "Parse condition 15");
    }
    {
        std::istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 16");
        Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 17");
    }
    {
        std::istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(!root->Evaluate({2016, 1, 1}, "holiday"), "Parse condition 18");
        Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 19");
        Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 20");
        Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 21");
    }
    {
        std::istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2016, 1, 1}, "event"), "Parse condition 22");
        Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 23");
        Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 24");
        Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 25");
    }
    {
        std::istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 26");
        Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 27");
    }
    {
        std::istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
        std::shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({1, 1, 1}, "2017-01-01"), "Parse condition 28");
        Assert(!root->Evaluate({2016, 1, 1}, "event"), "Parse condition 29");
        Assert(root->Evaluate({2016, 1, 2}, "event"), "Parse condition 30");
    }
}

void TestParseEvent() {
    {
        std::istringstream iss("event");
        AssertEqual(ParseEvent(iss), "event", "Parse event without leading spaces");
    }
    {
        std::stringstream iss("   sport event ");
        AssertEqual(ParseEvent(iss), "sport event ", "Parse event with leading spaces");
    }
    {
        std::istringstream iss("  first event  \n  second event");
        std::vector<std::string> events;
        events.push_back(ParseEvent(iss));
        events.push_back(ParseEvent(iss));
        AssertEqual(events, std::vector<std::string>{"first event  ", "second event"}, "Parse multiple events");
    }
}

int DoRemove(Database &db, const std::string &str) {
    std::istringstream is(str);
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date &date, const std::string &event) {
        return condition->Evaluate(date, event);
    };
    return db.RemoveIf(predicate);
}

std::string DoFind(Database &db, const std::string &str) {
    std::istringstream is(str);
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date &date, const std::string &event) {
        return condition->Evaluate(date, event);
    };
    const auto entries = db.FindIf(predicate);
    std::ostringstream os;
    for (const auto &entry: entries) {
        os << entry << std::endl;
    }
    os << entries.size();
    return os.str();
}

void TestDbAdd() {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n", out.str(), "straight ordering");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "holiday");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-01 holiday\n", out.str(), "several in one day");
    }
    {
        Database db;
        db.Add({2017, 1, 7}, "xmas");
        db.Add({2017, 1, 1}, "new year");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n", out.str(), "reverse ordering");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "xmas");
        db.Add({2017, 1, 1}, "new year");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-01 xmas\n", out.str(), "uniq adding");
    }
}

void TestDbFind() {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual("2017-01-01 new year\n1", DoFind(db, "date == 2017-01-01"), "simple find by date");
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n2", DoFind(db, "date < 2017-01-31"),
                    "multiple find by date");
        AssertEqual("2017-01-01 new year\n1", DoFind(db, R"(event != "xmas")"), "multiple find by holiday");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "new year2");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n2",
                    DoFind(db, R"(date == 2017-01-07 OR event == "new year")"),
                    "complex find or");
        AssertEqual("2017-01-01 new year\n1", DoFind(db, R"(date == 2017-01-01 AND event == "new year")"),
                    "complex find and");
        AssertEqual("0", DoFind(db, R"(date == 2017-01-09 AND event == "new year")"),
                    "complex find and, nothing");
    }
}

void TestDbLast() {
    Database db;
    db.Add({2017, 1, 1}, "new year");
    db.Add({2017, 1, 7}, "xmas");
    {
        try {
            db.Last({2016, 12, 31});
            Assert(false, "last, found no entries");
        } catch (...) {
            Assert(true, "last, found no entries");
        }
    }
    {
        std::ostringstream os;
        os << db.Last({2017, 1, 2});
        AssertEqual("2017-01-01 new year", os.str(), "greater than date");
    }
    {
        std::ostringstream os;
        os << db.Last({2017, 1, 1});
        AssertEqual("2017-01-01 new year", os.str(), "eq to date");
    }
    {
        std::ostringstream os;
        os << db.Last({2017, 1, 10});
        AssertEqual("2017-01-07 xmas", os.str(), "greater than max date");
    }
}

void TestDbLastAdvanced() {
    {
        Database db;
        db.Add(Date(1, 1, 1), "aa"s);
        db.Add(Date(1, 1, 1), "ab"s);
        db.Add(Date(1, 1, 1), "ac"s);
        db.Add(Date(1, 1, 2), "ba"s);
        db.Add(Date(1, 1, 2), "bb"s);
        db.Add(Date(1, 1, 2), "bc"s);
        Assert((db.Last(Date(1, 1, 1)) == "0001-01-01 ac"s), "test007"s);
        Assert((db.Last(Date(1, 1, 2)) == "0001-01-02 bc"s), "test008"s);

        db.RemoveIf(get_predicate("(event == \"bb\")"s));
        Assert((db.Last(Date(1, 1, 1)) == "0001-01-01 ac"s), "test009"s);
        Assert((db.Last(Date(1, 1, 2)) == "0001-01-02 bc"s), "test010"s);

        db.Add(Date(1, 1, 2), "bb"s);
        Assert((db.Last(Date(1, 1, 1)) == "0001-01-01 ac"s), "test011"s);
        Assert((db.Last(Date(1, 1, 2)) == "0001-01-02 bb"s), "test012"s);

        db.RemoveIf(get_predicate("(event == \"bc\")"s));
        Assert((db.Last(Date(1, 1, 1)) == "0001-01-01 ac"s), "test013"s);
        Assert((db.Last(Date(1, 1, 2)) == "0001-01-02 bb"s), "test014"s);

        db.RemoveIf(get_predicate("(event == \"bb\")"s));
        Assert((db.Last(Date(1, 1, 2)) == "0001-01-02 ba"), "test015"s);
    }
}

void TestDbRemoveIf() {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual(0, DoRemove(db, R"(event == "something")"), "Remove nothing");
        AssertEqual(1, DoRemove(db, R"(date == "2017-01-01")"), "Remove by date");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-07 xmas\n", out.str(), "Remove by date, left");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual(1, DoRemove(db, R"(event == "xmas")"), "Remove by event");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n", out.str(), "Remove by event, left");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        db.Add({2017, 1, 7}, "new year");
        AssertEqual(2, DoRemove(db, R"(event == "new year")"), "Multiple remove by event");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-07 xmas\n", out.str(), "Multiple remove by event, left");
    }
}

void TestInsertionOrder() {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        db.Add({2017, 1, 7}, "party");
        db.Add({2017, 1, 7}, "pie");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n2017-01-07 party\n2017-01-07 pie\n", out.str(),
                    "Remove by date, left");
    }
}

void TestsPrintingDeleting() {
    {
        Database db;
        db.Add({2019, 12, 18}, "Kolya");
        db.Add({2020, 1, 15}, "Katya");
        db.Add({2020, 2, 22}, "Riding");
        db.Add({2019, 12, 9}, "Go home");
        db.Add({2019, 12, 9}, "Read");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-09 Go home\n2019-12-09 Read\n2019-12-18 Kolya\n2020-01-15 Katya\n2020-02-22 Riding\n",
                    out.str(), "Order of insertion");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");
        db.Add({2019, 12, 2}, "c");
        db.Add({2019, 12, 2}, "d");
        db.Add({2019, 12, 3}, "e");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n2019-12-01 b\n2019-12-02 c\n2019-12-02 d\n2019-12-03 e\n", out.str(),
                    "Order of insertion");
    }

    {
        Database db;
        std::istringstream ss("    a");
        const auto event = ParseEvent(ss);
        db.Add({2019, 12, 1}, event);
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n", out.str(), "My test 20");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");
        db.Add({2019, 12, 2}, "c c");
        db.Add({2019, 12, 2}, "d");
        db.Add({2019, 12, 3}, "e");
        db.Add({2019, 12, 3}, "f");
        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n2019-12-01 b\n2019-12-02 c c\n2019-12-02 d\n2019-12-03 e\n2019-12-03 f\n", out.str(),
                    "My test 2");
        AssertEqual(1, DoRemove(db, R"(event == "c" OR event == "d")"), "My test 3");
        AssertEqual(1, DoRemove(db, R"(event == "e" AND event != "a")"), "My test 4");
        db.Add({2019, 11, 30}, "a");
        AssertEqual("2019-12-03 f\n1", DoFind(db, R"(date >= 2019-12-3)"), "My test 5");
        AssertEqual(Entry({2019, 12, 3}, "f"), db.Last({2019, 12, 4}), " My test 6");

        try {
            db.Last({2019, 11, 3});
        } catch (std::invalid_argument &) {
            std::cerr << "Тест на No entries OK" << std::endl;
        }

        AssertEqual(Entry({2019, 12, 2}, "c c"), db.Last({2019, 12, 2}), " My test 7");

        AssertEqual(Entry({2019, 12, 3}, "f"), db.Last({2019, 12, 4}), " My test 8");

        db.Add({2019, 12, 3}, "m");
        AssertEqual(Entry({2019, 12, 3}, "m"), db.Last({2019, 12, 3}), " My test 9");

        AssertEqual(1, DoRemove(db, R"(event == "e" AND event != "a" OR event == "m" AND date == 2019-12-3)"),
                    "My test 10");

        std::ostringstream out2;
        db.Print(out2);
        AssertEqual("2019-11-30 a\n2019-12-01 a\n2019-12-01 b\n2019-12-02 c c\n2019-12-03 f\n", out2.str(),
                    "My test 11");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 2}, "c");
        db.Add({2019, 12, 2}, "a");
        db.Add({2019, 12, 2}, "a");

        AssertEqual(2, DoRemove(db, R"(event == "a")"), "My test 12");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "aa");
        db.Add({2019, 12, 1}, "aaa");

        AssertEqual(2, DoRemove(db, R"(event >= "aa")"), "My test 13");

        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n", out.str(), "My test 14");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "aa");
        db.Add({2019, 12, 1}, "aaa");
        db.Add({2019, 12, 2}, "b");
        db.Add({2019, 12, 2}, "a");

        AssertEqual(2, DoRemove(db, R"(event > "aa")"), "My test 15");

        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n2019-12-01 aa\n2019-12-02 a\n", out.str(), "My test 16");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "aa");
        db.Add({2019, 12, 1}, "aaa");
        db.Add({2019, 12, 2}, "b");
        db.Add({2019, 12, 2}, "a");

        AssertEqual(2, DoRemove(db, R"(event < "aa")"), "My test 17");

        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 aa\n2019-12-01 aaa\n2019-12-02 b\n", out.str(), "My test 18");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");

        AssertEqual(1, DoRemove(db, R"(event != "b")"), "My test 19");

        db.Add({2019, 12, 1}, "c");

        AssertEqual(Entry({2019, 12, 1}, "c"), db.Last({2019, 12, 1}), " My test 20");

        db.Add({2019, 12, 1}, "b");
        AssertEqual(Entry({2019, 12, 1}, "c"), db.Last({2019, 12, 1}), " My test 21");

        std::ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 b\n2019-12-01 c\n", out.str(), "My test 22");
    }
}

void TestDatabaseFromGithub() {
    std::istringstream empty_is("");
    auto empty_condition = ParseCondition(empty_is);
    auto empty_predicate = [empty_condition](const Date &date, const std::string &event) {
        return empty_condition->Evaluate(date, event);
    };

    // Add 2 - Del 1 - Add deleted again
    {
        Database db;
        Date d(2019, 1, 1);
        db.Add(d, "e1");
        db.Add(d, "e2");
        std::istringstream is(R"(event == "e1")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 1, "Db Add2-Del-Add 1"s);
        db.Add(d, "e1");
        AssertEqual(db.FindIf(empty_predicate).size(), 2, "Db Add2-Del-Add 2"s);
    }

    // Add
    {
        Database db;
        Date d(2019, 1, 1);
        db.Add(d, "e1");
        db.Add(d, "e1");
        std::istringstream is("date == 2019-01-01"s);
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 1, "Db Add Duplicates 1");
    }

    // Last
    {
        Database db;
        Date d(2019, 1, 1);
        Date d1(2019, 1, 2);
        Date d2(2018, 12, 22);
        db.Add(d1, "e1");
        db.Add(d2, "e2");
        AssertEqual(db.Last(d), Entry({2018, 12, 22}, "e2"), "Db Last 1");
        Date d3(2018, 12, 24);
        db.Add(d3, "e3");
        AssertEqual(db.Last(d), Entry({2018, 12, 24}, "e3"), "Db Last 2");

//         Довольно странный тест - не понял его смысла ...
//         Get last event for date before first event
//        try {
//            Date d4(2017, 2, 2);
//            db.Last(d4);
//            Assert(false, "Db Last 3");
//        } catch (std::invalid_argument e) {
//            // Pass
//        }

        // Delete event and get last
        std::istringstream is("date == 2018-12-24");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        db.RemoveIf(predicate);
        AssertEqual(db.Last(d), Entry({2018, 12, 22}, "e2"), "Db Last 4");

        AssertEqual(db.Last(d1), Entry({2019, 1, 2}, "e1"), "Db Last 5");
        db.Add(d2, "e4");
        AssertEqual(db.Last(d2), Entry({2018, 12, 22}, "e4"), "Db Last 6");
    }

    // Del
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is("date == 2018-01-07");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 2, "Db Del 1");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 4, "Db Del 2");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        AssertEqual(db.RemoveIf(empty_predicate), 4, "Db Del 3");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is(R"(event == "e1")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 1, "Db Del 4");
    }

    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is(R"(event == "e1" OR date == 2019-01-01)");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 2, "Db Del 5");
    }

    // Find
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is("date == 2018-01-07");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 1");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 4, "Db Find 2");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        AssertEqual(db.FindIf(empty_predicate).size(), 4, "Db Find 3");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is(R"(event == "e1")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 1, "Db Find 4");
    }

    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        std::istringstream is(R"(event == "e1" OR date == 2019-01-01)");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const std::string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 5");
    }

    // Add - Del - Add - Del
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        AssertEqual(db.RemoveIf(empty_predicate), 2, "Db Add-Del-Add-Del 1");

        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        AssertEqual(db.RemoveIf(empty_predicate), 2, "Db Add-Del-Add-Del 1");
    }

    {
        Database db;
        db.Add({2017, 1, 1}, "first");
        db.Add({2017, 1, 1}, "second");
        db.Add({2017, 1, 1}, "third");
        db.Add({2017, 1, 1}, "fourth");
        db.Add({2017, 1, 1}, "five");
        AssertEqual(2, DoRemove(db, R"(event == "second" OR event == "fourth")"), "Remove several");
        std::ostringstream os;
        db.Print(os);
        AssertEqual("2017-01-01 first\n2017-01-01 third\n2017-01-01 five\n", os.str(),
                    "Check print after remove several- 3");
    }
}

void TestDateComparisonNode() {
    {
        DateComparisonNode dcn(Comparison::Equal, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 1");
        Assert(!dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 2");
    }
    {
        DateComparisonNode dcn(Comparison::NotEqual, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 3");
        Assert(!dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 4");
    }
    {
        DateComparisonNode dcn(Comparison::Less, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 5");
        Assert(!dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 6");
        Assert(!dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 7");
    }
    {
        DateComparisonNode dcn(Comparison::Greater, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 8");
        Assert(!dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 9");
        Assert(!dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 10");
    }
    {
        DateComparisonNode dcn(Comparison::LessOrEqual, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 11");
        Assert(dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 12");
        Assert(!dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 13");
    }
    {
        DateComparisonNode dcn(Comparison::GreaterOrEqual, {2017, 11, 18});
        Assert(dcn.Evaluate(Date{2017, 11, 19}, ""), "DateComparison 14");
        Assert(dcn.Evaluate(Date{2017, 11, 18}, ""), "DateComparison 15");
        Assert(!dcn.Evaluate(Date{2017, 11, 17}, ""), "DateComparison 16");
    }
}

void TestEventComparisonNode() {
    {
        EventComparisonNode ecn(Comparison::Equal, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 1");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "cbe"), "EventComparison 2");
    }
    {
        EventComparisonNode ecn(Comparison::NotEqual, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "cde"), "EventComparison 3");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 4");
    }
    {
        EventComparisonNode ecn(Comparison::Less, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 5");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 6");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 7");
    }
    {
        EventComparisonNode ecn(Comparison::Greater, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 8");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 9");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 10");
    }
    {
        EventComparisonNode ecn(Comparison::LessOrEqual, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 11");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 12");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 13");
    }
    {
        EventComparisonNode ecn(Comparison::GreaterOrEqual, "abc");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abd"), "EventComparison 14");
        Assert(ecn.Evaluate(Date{0, 1, 1}, "abc"), "EventComparison 15");
        Assert(!ecn.Evaluate(Date{0, 1, 1}, "abb"), "EventComparison 16");
    }
}

void TestLogicalOperationNode() {
    {
        LogicalOperationNode lon(LogicalOperation::And, std::make_shared<EmptyNode>(), std::make_shared<EmptyNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 1");
    }
    {
        LogicalOperationNode lon(LogicalOperation::And, std::make_shared<AlwaysFalseNode>(),
                                 std::make_shared<EmptyNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 2");
    }
    {
        LogicalOperationNode lon(LogicalOperation::And, std::make_shared<EmptyNode>(),
                                 std::make_shared<AlwaysFalseNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 3");
    }
    {
        LogicalOperationNode lon(LogicalOperation::And, std::make_shared<AlwaysFalseNode>(),
                                 std::make_shared<AlwaysFalseNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 4");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, std::make_shared<EmptyNode>(), std::make_shared<EmptyNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 5");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, std::make_shared<AlwaysFalseNode>(),
                                 std::make_shared<EmptyNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 6");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, std::make_shared<EmptyNode>(),
                                 std::make_shared<AlwaysFalseNode>());
        Assert(lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 7");
    }
    {
        LogicalOperationNode lon(LogicalOperation::Or, std::make_shared<AlwaysFalseNode>(),
                                 std::make_shared<AlwaysFalseNode>());
        Assert(!lon.Evaluate(Date{0, 1, 1}, "abc"), "LogicalOperationNode 8");
    }
}

void TestEmptyNode() {
    {
        EmptyNode en;
        Assert(en.Evaluate(Date{0, 1, 1}, "abc"), "EmptyNode 1");
        Assert(en.Evaluate(Date{2017, 11, 18}, "def"), "EmptyNode 2");
        Assert(en.Evaluate(Date{9999, 12, 31}, "ghi"), "EmptyNode 3");
    }
}

void TestFromSergey() {
    std::stringstream ss("2017-11-17"s);
    auto test_date = ParseDate(ss);
    Database db;
    db.Add(test_date, "One more event");
    db.Add(test_date, "Friday");
    db.Add(test_date, "Hey ho!");
    assert(db.Last(test_date) == "2017-11-17 Hey ho!");
    db.RemoveIf([](const Date &, const std::string &e) { return e == "Hey ho!"; });
    assert(db.Last(test_date) == "2017-11-17 Friday");
    db.RemoveIf([](const Date &, const std::string &e) { return e == "One more event"; });
    assert(db.Last(test_date) == "2017-11-17 Friday");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestParseCondition, "TestParseCondition");

    // тесты с https://gist.github.com/SergeiShumilin/a030350c6226b8091b57ed0c7ccba779
    tr.RunTest(TestEmptyNode, "Тест 2 из Coursera"s);
    tr.RunTest(TestDbAdd, "Тест 3(1) из Coursera"s);
    tr.RunTest(TestDbFind, "Тест 3(2) из Coursera"s);
    tr.RunTest(TestDbLast, "Тест 3(3) из Coursera"s);
    tr.RunTest(TestDbRemoveIf, "Тест 3(4) из Coursera"s);
    tr.RunTest(TestInsertionOrder, "Тест на порядок вывода"s);
    tr.RunTest(TestsPrintingDeleting, "TestsPrintingDeleting"s);
    tr.RunTest(TestDatabaseFromGithub, "TestDatabaseFromGithub"s);
    tr.RunTest(TestDateComparisonNode, "TestDateComparisonNode"s);
    tr.RunTest(TestEventComparisonNode, "TestEventComparisonNode"s);
    tr.RunTest(TestLogicalOperationNode, "TestLogicalOperationNode"s);
    tr.RunTest(TestFromSergey, "TestFromSergey"s);
    tr.RunTest(TestDbLastAdvanced, "TestDbLastAdvanced"s);
}
