/**
 * @brief This is only an inteface class for now
 */

#ifndef STATISTICTOOL_H
#define STATISTICTOOL_H

#include <string>
#include <iostream>
#include <iomanip>

#ifndef OUTPUT_TABLE_4
#define OUTPUT_TABLE_4(A, B, C, D) \
    std::left << std::setw(100) << A \
    << std::left << std::setw(15) << B \
    << std::left << std::setw(15) << C \
    << std::left << std::setw(15) << D << endl;
#endif

class StatisticTool 
{
public:
    explicit StatisticTool(const std::string& name)
        : sToolName(name)
    {
    }

    virtual ~StatisticTool() {}
    virtual void Execute() {}
public:
    std::string sToolName;
};


#endif