#pragma warning(disable: 4786)
#include <assert.h>

#include <iostream>

#include "csvfile.hpp"

using namespace util;

void Test()
{
    CsvFile csv;
    csv.SetColumnCount(3);
    std::vector<std::string> row;
    row.push_back("ok");
    csv.AddRow(row);
    row.push_back("row2");
    csv.AddRow(row);
    csv.Save("a.csv");
}


int main()
{
    Test();
    return 0;
}
