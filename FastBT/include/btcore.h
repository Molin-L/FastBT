//
// Created by Molin Liu on 5/8/23.
//

#ifndef BackTesting_BTCORE_H
#define BackTesting_BTCORE_H
#include <memory>
#include <unordered_map>
#include <vector>

#include "analyzer.h"
#include "datafeeder.h"
#include "order.h"
#include "position.h"
#include "strategy.h"
#include "trade.h"

namespace BackTesting {
/*
 * @brief The core of the backtest engine.
 * Provide ohlcv data
 */
class BtCore {
public:
    BtCore() = delete;
    BtCore(std::shared_ptr<DataFeeder> data_feeder,
           std::shared_ptr<BaseStrategy> strategy, double cash,
           double commission_fee = 0, bool hedge = false)
        : data_feeder(data_feeder), strategy(strategy) {
        this->broker = std::make_shared<Broker>(*data_feeder, cash, 1.0,
                                                commission_fee, 0, 0, hedge);
        this->strategy->Init(data_feeder, broker);
        this->analyzer = std::make_shared<Analyzer>(data_feeder);
    }
    ~BtCore() = default;

    void Run();
    void Reset();
    void AddStrategy(std::shared_ptr<BaseStrategy> strategy);

private:
    std::shared_ptr<DataFeeder> data_feeder;
    std::shared_ptr<BaseStrategy> strategy;
    std::shared_ptr<Broker> broker;
    std::shared_ptr<Analyzer> analyzer;
};
}// namespace FastBT
#endif//BackTesting_BTCORE_H
