#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <sstream>
#include <iostream>

using namespace std;
class PrintUtils
{
public:
    PrintUtils();
    static void printPercentage(int status, int total);
};

#endif // PRINTUTILS_H
