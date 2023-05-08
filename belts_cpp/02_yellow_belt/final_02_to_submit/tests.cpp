#include "tests.h"
#include "node.h"
#include "condition_parser.h"
#include "test_runner.h"

void TestParseCondition() {
    {
        std::istringstream iss("date != 2017-11-18"s);
        std::shared_ptr <Node> root = ParseCondition(iss);
        Assert(root->Evaluate({2017, 1, 1}, ""s), "Parse condition 1"s);
        Assert(!root->Evaluate({2017, 11, 18}, ""s), "Parse condition 2"s);
    }
    {
        std::istringstream iss(R"(event == "sport event")");
        std::shared_ptr <Node> root = ParseCondition(iss);
        Assert(root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 3");
        Assert(!root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 4");
    }
    {
        std::istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 5");
        Assert(root->Evaluate({2017, 3, 1}, ""), "Parse condition 6");
        Assert(root->Evaluate({2017, 6, 30}, ""), "Parse condition 7");
        Assert(!root->Evaluate({2017, 7, 1}, ""), "Parse condition 8");
        Assert(!root->Evaluate({2016, 12, 31}, ""), "Parse condition 9");
    }
    {
        std::istringstream is(R"(event != "sport event" AND event != "Wednesday")");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 10");
        Assert(!root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 11");
        Assert(!root->Evaluate({2017, 1, 1}, "Wednesday"), "Parse condition 12");
    }
    {
        std::istringstream is(R"(event == "holiday AND date == 2017-11-18")");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(!root->Evaluate({2017, 11, 18}, "holiday"), "Parse condition 13");
        Assert(!root->Evaluate({2017, 11, 18}, "work day"), "Parse condition 14");
        Assert(root->Evaluate({1, 1, 1}, "holiday AND date == 2017-11-18"), "Parse condition 15");
    }
    {
        std::istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 16");
        Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 17");
    }
    {
        std::istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(!root->Evaluate({2016, 1, 1}, "holiday"), "Parse condition 18");
        Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 19");
        Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 20");
        Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 21");
    }
    {
        std::istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(root->Evaluate({2016, 1, 1}, "event"), "Parse condition 22");
        Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 23");
        Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 24");
        Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 25");
    }
    {
        std::istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
        std::shared_ptr <Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 26");
        Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 27");
    }
    {
        std::istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
        std::shared_ptr <Node> root = ParseCondition(is);
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
        std::vector <std::string> events;
        events.push_back(ParseEvent(iss));
        events.push_back(ParseEvent(iss));
        AssertEqual(events, std::vector < std::string > {"first event  ", "second event"}, "Parse multiple events");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestParseCondition, "TestParseCondition");
}
