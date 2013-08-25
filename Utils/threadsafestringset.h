#ifndef THREADSAFESTRINGSET_H
#define THREADSAFESTRINGSET_H

#include <vector>
#include <string>
#include <boost/thread.hpp>
#include <iostream>
#include <sstream>

using namespace std;
class ThreadSafeStringSet
{
    boost::mutex mtx;
    int pos;
    vector<string> stringSet;
    void printStatus();
public:
    ThreadSafeStringSet(vector<string> &stringList);
    string getNext();
};

#endif // THREADSAFESTRINGSET_H
