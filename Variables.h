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
    VariableInfo(const string& nm, const string& nmtex, unsigned rb, double* bing = nullptr, std::size_t n = 0) noexcept
        : name(nm), name_tex(nmtex), n_rebin(rb), binning(bing), n_bins(n) {}

public:
    string name;
    string name_tex;
    unsigned n_rebin;
    double* binning;
    std::size_t n_bins;
};


class Variables
{
public:
    Variables();
    ~Variables();
    Variables(Variables& vs) = delete;
    Variables& operator=(Variables& vs) = delete;

public:
    void add(const string& nm, const string& nmtex, unsigned rb, double* bing = nullptr, std::size_t n = 0) const;
    inline vector<VariableInfo*>* content() const { return m_vars.get(); }

private:
    unique_ptr<vector<VariableInfo*>> m_vars;
};

#endif // VARIABLES_H
