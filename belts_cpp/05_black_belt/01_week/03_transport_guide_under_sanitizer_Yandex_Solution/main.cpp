
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "dbjson.h"
//#include "dbio_tests.h"
//#include "dbjson_tests.h"

using namespace std;


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

 //   ifstream in("input.txt");
 //   ofstream out("output.txt");

  //IOTests();
  //JSONTests();
//   uint lines;
//   in >> lines;
//   in.ignore();
//   const BusDB db = dbInput(lines, cin);
//   cin >> lines;
//   cin.ignore();
//   dbOutput(lines, db, cin, cout);

  Json::Document input = Json::Load(cin);
  const BusDB db = DBfromJSON(input.GetRoot());
  if (input.GetRoot().AsMap().count("stat_requests"))
    Json::Print(cout,ProcessRequests(input.GetRoot().AsMap().at("stat_requests"), db));
  
  return 0;
}
