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
    unsigned n_bins = 8;
    double min_bkg = 5; // all bkg
    double min_mcstats = 0.2; // all bkg
    double required_mcstats = 0.2; // individual component (leading half)
    double eff_factor = 0.5; // effective factor
    double significance_delta = 1e-5; // stop adding bins if delta significance < 1e-5
    const char* atlas_label = "Simulation Internal";
    std::string parameter = "TAG";
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
     * @note 
     * Case 1 : 
     * from right to left, scan for bin edges that satisfy:
     * - # Bkg > MinBkg (typically _5_) 
     *   -> validity of asymptotic test statistic formula
     * - MC stat. error of total bkg > MinTotalErr (typically _20%_)
     *   -> generally reasonable mc stats
     * - MC stat. error of leading two components (typically _20%_)
     *   -> enough stats to show systematic variation of major bkg components
     * 
     * Q: any cons?
     */
    bool passCaseOneFromXtoY(double fBkg, double fBkgErr, double fReqFactor, std::size_t x, std::size_t y) const;
    /**
     * @note
     * Case 2 : 
     * from right to left, scan for bin edges that satisfy:
     * - # Bkg > MinBkg (typically _5_) 
     *   -> validity of asymptotic test statistic formula
     * - MC stat. error of total bkg > min of MinTotalErr (typically _20%_) 
     *   and EffectiveFactor * (# sig_i / # sig_tot) (typically _1_)
     *   -> generally reasonable mc stats
     *   -> (sometimes sharply) monotonic behaviour of bkg (and sig) distribution
     * - MC stat. error of leading two components (typically _20%_)
     *   -> enough stats to show systematic variation of major bkg components
     */
    bool passCaseTwoFromXtoY(double fBkg, double fBkgErr, double fEffErr, double fReqFactor, std::size_t x, std::size_t y) const;

public:
    std::map<std::pair<eProcessType, eProcess>, std::pair<double*, double*>> data;
    // two pointers -> current interval
    std::size_t curX;
    std::size_t curY;

private:
    std::size_t m_size;
};

// ---------------------------------------------------------------------------
// Method Enum
// ---------------------------------------------------------------------------
enum class BinningCriteria 
{
    CaseOne,
    CaseTwo
};


// ----------------------------------------------------------------------------
// Auto Binning Engines
// ----------------------------------------------------------------------------
class AutoBinningTool : public HistTool
{
public:
    AutoBinningTool(const AutoBinningInfo* info, BinningCriteria bc);
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
    BinningCriteria m_bc;
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
    AutoBinningTool_v1(const AutoBinningInfo* info, BinningCriteria bc);

public: 
    virtual void run(const Config* c) const override;
};



#endif