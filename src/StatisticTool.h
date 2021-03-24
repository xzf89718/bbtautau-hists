/**
 * @brief This is only an inteface class for now
 */

#ifndef STATISTICTOOL_H
#define STATISTICTOOL_H

#include <string>

class StatisticTool 
{
public:
    explicit StatisticTool(const std::string& name)
        : sToolName(name)
    {
    }

    virtual ~StatisticTool() {}
public:
    std::string sToolName;
};


#endif