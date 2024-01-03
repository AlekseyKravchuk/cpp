#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "test_runner.h"
#include "xml.h"
#include "json.h"
#include "tests.h"

int main() {
    TestRunner tr;
    
    // ========== XML tests ==========
    RUN_TEST(tr, TestSpendings);
    RUN_TEST(tr, TestXmlLibrary);
    RUN_TEST(tr, TestLoadFromXml);
    RUN_TEST(tr, TestMultiple_XML_SectionsInInput);

    // ========== JSON tests ==========
    RUN_TEST(tr, TestJsonLibrary);
    RUN_TEST(tr, TestLoadFromJson);
    RUN_TEST(tr, TestJsonLibraryFromFile);

    return 0;
}
