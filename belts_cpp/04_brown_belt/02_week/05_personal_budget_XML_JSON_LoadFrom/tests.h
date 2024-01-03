#pragma once

#include <vector>
#include <iostream>
#include <fstream>

#include "spending.h"
#include "xml.h"
#include "json.h"
#include "test_runner.h"
#include "load_from.h"

void TestSpendings();

void TestLoadFromXml();

void TestXmlLibrary();

void TestMultiple_XML_SectionsInInput();

void TestLoadFromJson();

void TestJsonLibrary();

void TestJsonLibraryFromFile();



