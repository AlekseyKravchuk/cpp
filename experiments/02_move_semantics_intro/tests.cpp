#include "tests.h"

std::string MakeString() {
    return std::string(1'000'000'000, 'a');
}

void ProfileCopyAssignment_vs_Moving() {
    {
        LOG_DURATION("Assignment (copy assignment) WITH variable");
        std::string target_string = "old value";
        std::string source_string = MakeString();
        target_string = source_string;
    }

    {
        LOG_DURATION("Assignment (copy assignment) WITHOUT additional variable");
        std::string target_string = "old value";
        target_string = MakeString();
    }
}

void ProfileInsertingToSet_Copying_vs_Moving() {
    {
        LOG_DURATION("set::insert, with variable");
        std::set<std::string> strings;
        std::string heavy_string = MakeString();
        strings.insert(heavy_string);
    }

    {
        LOG_DURATION("set::insert, WITHOUT variable");
        std::set<std::string> strings;
        strings.insert(MakeString());
    }
}

void ProfileAddingKeyValuePairToEmptyMap_Copying_vs_Moving() {
    {
        LOG_DURATION("map::operator[], with BOTH variables: for key and for value");
        std::map<std::string, std::string> strings;
        std::string key = MakeString();
        std::string value = MakeString();
        strings[key] = value;
    }

    {
        LOG_DURATION("map::operator[], variable ONLY for key");
        std::map<std::string, std::string> strings;
        std::string key = MakeString();
        strings[key] = MakeString();
    }

    {
        LOG_DURATION("map::operator[], WITHOUT variables");
        std::map<std::string, std::string> strings;
        strings[MakeString()] = MakeString();
    }
}