#include <iostream>
#include <fstream>
#include <iomanip>
#include "RankingTool.h"

using namespace std;

#define OUTPUT_TABLE_4(A, B, C, D) \
    std::left << setw(100) << A \
    << std::left << setw(15) << B \
    << std::left << setw(15) << C \
    << std::left << setw(15) << D << endl;

int test_ranking(const std::string& filename, const std::string& outname)
{
    WorkspaceInfo* info = new WorkspaceInfo();
    info->path = filename;
    info->workspace_name = "combined";
    info->use_asimov = false;

    auto timeStart = steady_clock::now();
    ofstream output(outname);

    RankingEngine* egn = new RankingEngine(info);
    egn->RunRanking();

    auto val = egn->GetFittedPOI();
    auto res = egn->GetFittedPOIWithFixedNPs();

    output << OUTPUT_TABLE_4("FIXED NP", "POI_VALUE", "POI_ERROR_UP", "POI_ERROR_DOWN");

    Tools::println("% ->      VALUE %      ERROR_UP %      ERROR_DOWN %",
        "No fixed NP",
        val[WorkspaceTool::ePOI::VALUE],
        val[WorkspaceTool::ePOI::ERRORUP],
        val[WorkspaceTool::ePOI::ERRORDOWN]);
    output << OUTPUT_TABLE_4("None",
        val[WorkspaceTool::ePOI::VALUE],
        val[WorkspaceTool::ePOI::ERRORUP],
        val[WorkspaceTool::ePOI::ERRORDOWN])

    for (auto& p : res) {
        Tools::println("% Fixed ->      VALUE %      ERROR_UP %      ERROR_DOWN %",
            p.first,
            p.second[WorkspaceTool::ePOI::VALUE],
            p.second[WorkspaceTool::ePOI::ERRORUP],
            p.second[WorkspaceTool::ePOI::ERRORDOWN]);
        output << OUTPUT_TABLE_4(
            p.first,
            p.second[WorkspaceTool::ePOI::VALUE],
            p.second[WorkspaceTool::ePOI::ERRORUP],
            p.second[WorkspaceTool::ePOI::ERRORDOWN]);
    }

    delete info;
    delete egn;

    auto timeEnd = steady_clock::now();
    Tools::println("Spent [%ms]", duration_cast<milliseconds>(timeEnd-timeStart).count());

    return EXIT_SUCCESS;
}

int test_ranking_plot(const char* in, const char* out)
{
    RankingPlotter* plt = new RankingPlotter();
    plt->LoadFromTxt(in);
    plt->Draw(out);

    delete plt;

    return EXIT_SUCCESS;
}
