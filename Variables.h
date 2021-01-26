#ifndef VARIABLES_H
#define VARIABLES_H


#include <memory>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::unique_ptr;


// ELEM
class VariableInfo
{
public:
    VariableInfo(const string& nm, const string& nmtex, unsigned rb, float* bing = nullptr) noexcept
        : name(nm), nameTeX(nmtex), nRebin(rb), binning(bing) {}

public:
    string name;
    string nameTeX;
    unsigned nRebin;
    float* binning;
};


class Variables
{
public:
    Variables();
    void add(const string& nm, const string& nmtex, unsigned rb, float* bing = nullptr) const;
    inline vector<VariableInfo*>* content() const { return m_vars.get(); }
private:
    unique_ptr<vector<VariableInfo*>> m_vars;
};

#endif // VARIABLES_H
