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
        : name(nm), name_tex(nmtex), n_rebin(rb), binning(bing) {}

public:
    string name;
    string name_tex;
    unsigned n_rebin;
    float* binning;
};


class Variables
{
public:
    Variables();
    ~Variables();
    Variables(Variables& vs) = delete;
    Variables& operator=(Variables& vs) = delete;

public:
    void add(const string& nm, const string& nmtex, unsigned rb, float* bing = nullptr) const;
    inline vector<VariableInfo*>* content() const { return m_vars.get(); }

private:
    unique_ptr<vector<VariableInfo*>> m_vars;
};

#endif // VARIABLES_H
