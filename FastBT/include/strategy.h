//
// Created by Molin Liu on 15/8/23.
//

#ifndef BACKTESTING_STRATEGY_H
#define BACKTESTING_STRATEGY_H
#include <list>

#include "broker.h"
#include "datafeeder.h"
#include "dtype.h"
#include "order.h"
#include "trade.h"

namespace BackTesting {
class BaseStrategy {
public:
    BaseStrategy(){};
    virtual ~BaseStrategy();
    void Init(std::shared_ptr<DataFeeder> data_feeder,
              std::shared_ptr<Broker> broker);
    void Reset();
    void OnTick(size_t time_index);

    virtual void Init();
    virtual void Next();

    const Order &Buy(size_t size_scaled, price_t price_scaled,
                     EOrder::CreateType create_type,
                     EOrder::OrderType order_type);
    const Order &Buy();
    const Order &Sell(size_t size_scaled, price_t price_scaled,
                      EOrder::CreateType create_type,
                      EOrder::OrderType order_type);
    const Order &Sell();
    void ClosePosition(size_t size, price_t price, EPosition::Side side,
                       EOrder::OrderType order_type);
    void ClosePosition(EPosition::Side side);
    void CloseAllPosition();

    Position &GetPosition(EPosition::Side side);
    const std::vector<OHLCScaled> &GetData() const;

    bool IsFinished() const {
        return finished;
    };

    double GetAvailableBalance() {
        return broker->GetAvailableBalance();
    }

    std::vector<double> GetDataSeries(std::string col);

private:
    double price;
    price_t price_scaled;
    bool finished;
    bool hedge = false;

    std::shared_ptr<DataFeeder> data;
    std::shared_ptr<BackTesting::Broker> broker;

    const Order &CreateOrder(EOrder::Side side, size_t size, price_t price,
                             EOrder::CreateType create_type,
                             EOrder::OrderType order_type);

protected:
    time_t time_index;
    std::unordered_map<std::string, std::vector<double>> indicators;
};

}// namespace FastBT

#endif//BACKTESTING_STRATEGY_H
