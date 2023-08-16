//
// Created by Molin Liu on 5/8/23.
//
#include "btcore.h"
#include "utils.h"
#include <fstream>

void BackTesting::BtCore::Run() {
    this->Reset();
    std::vector<double> equity(this->data_feeder->GetScaledData().size(), 0);
    for (int i = 0; i < this->data_feeder->GetScaledData().size(); i++) {
        this->broker->TriggerOrders(i);
        this->strategy->OnTick(i);
        if (this->strategy->IsFinished()) {
            break;
        }
        this->broker->ProcessPendingOrder();
        equity[i] = this->broker->GetEquity();
    }
    this->strategy->CloseAllPosition();
    this->broker->ProcessPendingOrder();
    this->broker->TradesToCSV("../data/trades.csv");
    // Performance analysis
    Performance performance =
            this->analyzer->Analyze(this->broker->GetTrades(), equity);
    ExportEquity("../data/equity.csv", equity);
    std::ofstream file("../data/performance.txt");
    file << performance.ToString();
    file.close();
}
void BackTesting::BtCore::Reset() {
    this->strategy->Reset();
    this->broker->Reset();
}