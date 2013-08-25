#include "threadsafestringset.h"

ThreadSafeStringSet::ThreadSafeStringSet(vector<string> &stringList): stringSet(stringList), pos(0)
{
}

string ThreadSafeStringSet::getNext(){
    mtx.lock();
        string next = pos<stringSet.size() ? stringSet[pos] : "";
        pos++;
        printStatus();
    mtx.unlock();
    return next;
}

void ThreadSafeStringSet::printStatus(){
    stringstream sstream;
    sstream << (pos*100/stringSet.size()) << "%\r";
    cout.flush();
    cout << sstream.str();
}
