#ifndef UTILS_H
#define UTILS_H

#include "Processes.h"
#include "Regions.h"
#include "Variables.h"
#include "Systematics.h"

#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>

using std::string;
using std::vector;
using std::pair;

namespace Utils {

    void histAssign(TH1* h, ProcessInfo* p, RegionInfo* r, VariableInfo* v);
    void histAssignSyst(TH1* h, ProcessInfo *p, const std::string& systname);

    /**
     * @note: so far only support CxAODReader like naming
     */
    string histString(const ProcessInfo* p, const RegionInfo* r, const VariableInfo* v);
    string systString(const SystematicInfo* s);
    string histStringSyst(const ProcessInfo* p, const RegionInfo* r, const VariableInfo* v, const SystematicInfo* s);
    string systStringShort(const string& sSyst);

    static vector<pair<unsigned, unsigned>> paletteSysts = {         
        {kViolet, kAzure},
        {kViolet-1, kAzure-1},
        {kViolet-2, kAzure-2},
        {kViolet-3, kAzure-3},
        {kViolet-4, kAzure-4},
        {kViolet+1, kAzure+1},
        {kViolet+2, kAzure+2},
        {kViolet+3, kAzure+3},
        {kViolet+4, kAzure+4},
    };
}

class Tools
{

public:
    static void print(const char* fmt)
    {
        std::cout << fmt;
    }

    static void println(const char* fmt)
    {
        Tools::print(fmt);
        Tools::print("\n");
    }

    template<typename T, typename ... Targs>
    static void print(const char* fmt, T value, Targs ... args)
    {
        for(; *fmt != '\0'; fmt++)
        {
            if (*fmt == '%') {
                std::cout << value;
                Tools::print(fmt+1, args...);
                return;
            }
            std::cout << *fmt;
        }
    }

    template<typename T, typename ... Targs>
    static void println(const char* fmt, T value, Targs ... args)
    {
        Tools::print(fmt, value, args...);
        Tools::print("\n");
    }

    static int getInteger(const std::string& prompt = "Type in an integer: ", 
                          const std::string& reprompt = "It is not integer. Retry. \n")
    {
        while (1)
        {
            std::cout << prompt;
            std::string line;
            if (!getline(std::cin, line)) throw std::domain_error("Failed to get line from cin.");

            std::istringstream iss(line);
            int i; char a;
            if (iss >> i && !(iss >> a))
            {
                return i;
            }
            std::cout << reprompt;
        }
        return INT_MIN;
    }

};

#endif // UTILS_H
