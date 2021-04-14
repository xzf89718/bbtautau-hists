#ifndef AUTOBINNINGTOOL_H
#define AUTOBINNINGTOOL_H

#include "HistTool.h"

#include <vector>
#include <map>
#include <queue>

using std::vector;

// ----------------------------------------------------------------------------
// Info
// ----------------------------------------------------------------------------
class AutoBinningInfo
{
public:
    bool logx = false;
    bool logy = false;
    bool atlas = true;
    bool shape_only = false;
    const char* atlas_label = "Simulation Internal";
    std::string parameter;
    unsigned n_bins = 8;
    double min_bkg = 5; // all bkg
    double min_mcstats = 0.2; // all bkg
    double required_mcstats = 0.2; // individual component (leading half)
};

// ----------------------------------------------------------------------------
// Bin Content Data Structure
// ----------------------------------------------------------------------------
class BinContent
{
public:
    explicit BinContent(const Config* c) noexcept { fill(c); }
    ~BinContent() noexcept { free(); }
    BinContent(BinContent& bc) = delete;
    BinContent& operator=(BinContent& bc) = delete;

    std::size_t size() const noexcept;

private:
    void fill(const Config* c) noexcept;
    void free() noexcept;

public:
    /**
     * Helper functions
     */
    std::pair<double*, double*> getArrayFromContentAndError(ProcessInfo* p) const;
    void showData() const;
    bool checkRangeXY(std::size_t x, std::size_t y) const;
    double sumFromXtoY(double* arr, std::size_t x, std::size_t y) const;
    double errSumFromXtoY(double* arr, std::size_t x, std::size_t y) const;
    // asympotitic significance
    // Z = sqrt(2*(s+b)log(1+s/b)-2*s)
    double significance(double fSig, double fBkg) const;
    // by algebra, the asymptotic significance can be added in quadraure
    // pass by value so that the queue can be reused
    double significanceBinByBin(std::priority_queue<std::size_t, std::vector<std::size_t>, std::greater<std::size_t>> pq) const; 

public:
    /**
     * Data checks case 1 : allBkgMCStatsAndSumPassFromXtoY
     */
    bool passCaseOneFromXtoY(double fBkg, double fBkgErr, double fReqErr, std::size_t x, std::size_t y) const;

public:
    std::map<std::pair<eProcessType, eProcess>, std::pair<double*, double*>> data;
    // two pointers -> current interval
    std::size_t curX;
    std::size_t curY;

private:
    std::size_t m_size;
};

// ----------------------------------------------------------------------------
// Auto Binning Engines
// ----------------------------------------------------------------------------
class AutoBinningTool : public HistTool
{
public:
    explicit AutoBinningTool(const AutoBinningInfo* info);
    virtual ~AutoBinningTool() override { delete m_binEdges; }

public:
    virtual bool check(const Config* c) const override;
    // colorful
    // usually rebinning is done here, but for auto-binning it's not the case!
    virtual void paint(const Config* c) const override;
    // drawing
    virtual void run(const Config* c) const override { (void)c; }
    inline vector<double> binning() const { return *m_binEdges; }

protected:
    const AutoBinningInfo* m_info;
    vector<double>* m_binEdges;
};

/**
 * @brief
 * v1: conservative binning for MVA score
 * always require certain MC stat unc. on all components at right most bins
 * until all bins satisfy this criteria
 * testing with hadhad PNNs and BDT
 */
class AutoBinningTool_v1 : public AutoBinningTool
{
public:
    explicit AutoBinningTool_v1(const AutoBinningInfo* info);

public: 
    virtual void run(const Config* c) const override;
};



#endif