//
// Created by Molin Liu on 5/8/23.
//

#include "btcore.h"
#include "datafeeder.h"
#include "numeric.h"
#include "strategy.h"
#include <gtest/gtest.h>

class SMA : public BackTesting::BaseStrategy {
public:
    SMA() = default;
    void Init() override {
        this->indicators["MA12"] =
                BackTesting::Numeric::MA(this->GetDataSeries("Close"), 12);
        this->indicators["MA26"] =
                BackTesting::Numeric::MA(this->GetDataSeries("Close"), 26);
    }
    void Next() override {
        if (this->time_index < 26) {
            return;
        }
        if (this->indicators["MA12"][this->time_index] >
                    this->indicators["MA26"][this->time_index] &&
            this->indicators["MA12"][this->time_index - 1] <
                    this->indicators["MA26"][this->time_index - 1]) {
            this->CloseAllPosition();
            this->Buy();
        } else {
            if (this->indicators["MA12"][this->time_index] <
                        this->indicators["MA26"][this->time_index] &&
                this->indicators["MA12"][this->time_index - 1] >
                        this->indicators["MA26"][this->time_index - 1]) {
                this->CloseAllPosition();
                this->Sell();
            }
        }
    }
};


TEST(BtCore, RunBT) {
    std::shared_ptr<BackTesting::DataFeeder> data_feeder =
            std::make_shared<BackTesting::DataFeeder>();
    data_feeder->LoadData("../data/XAUUSD.csv");
    EXPECT_EQ(data_feeder->GetScaledData().size(), 2676);

    std::shared_ptr<BackTesting::BaseStrategy> strategy =
            std::make_shared<SMA>();
    auto bt_core =
            BackTesting::BtCore(data_feeder, strategy, 1000000, 0, false);
    bt_core.Run();
}