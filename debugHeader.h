#ifndef DEBUGHEADER_H
#define DEBUGHEADER_H

//#define NDEBUG
#define MEMCHECK

#include <cassert>

#ifdef NDEBUG
#define D(x)
#else
#define D(x) x
#endif

#ifdef MEMCHECK
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

class DBMEM{
public:
    static int parseLine(char* line){
        int i = strlen(line);
        while (*line < '0' || *line > '9') line++;
        line[i-3] = '\0';
        i = atoi(line);
        return i;
    }


    static int getVirtualMemoryValue(){ //Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];


        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmSize:", 7) == 0){
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    static int getPhysicalMemoryValue(){ //Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];


        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmRSS:", 6) == 0){
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }
};
#define PRINTMEM(x) cout<<x<<DBMEM::getVirtualMemoryValue()<<endl;
#else
#define PRINTMEM(x)
#endif //MEMCHECK

#endif // DEBUGHEADER_H
