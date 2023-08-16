//
// Created by Molin Liu on 15/8/23.
//

#ifndef BACKTESTING_BROKER_H
#define BACKTESTING_BROKER_H
#include <list>
#include <queue>
#include <vector>

#include "datafeeder.h"
#include "order.h"
#include "position.h"
#include "trade.h"
namespace BackTesting {
class Broker {
public:
    Broker() = delete;
    Broker(DataFeeder &dataFeeder, double initial_balance, double leverage,
           double commision_fee, double margin_rate, double slippage,
           bool hedge)
        : initial_balance(initial_balance), leverage(leverage),
          commision_fee(commision_fee), margin_rate(margin_rate), hedge(hedge),
          data_feeder(dataFeeder){};

    void Reset();
    double GetEquity();
    Position &GetPosition(EPosition::Side side);
    Position &GetPosition();
    Position &GetSinglePosition();
    double GetAvailableBalance() const;
    double GetCommisionFee();

    void AddOrder(std::shared_ptr<Order> order);

    void ProcessPendingOrder();
    void TriggerOrders(time_t time_index);

    void TradesToCSV(std::string file_name);

    std::vector<Trade> &GetTrades();

private:
    time_t time_index;
    double initial_balance;
    double available_balance;
    price_t available_balance_scaled;
    double leverage;
    double commision_fee;
    double margin_rate;// Todo
    bool hedge;
    DataFeeder &data_feeder;

    std::list<std::shared_ptr<Order>> order_book;
    std::queue<std::shared_ptr<Order>, std::list<std::shared_ptr<Order>>>
            pending_orders;
    std::unordered_map<EPosition::Side, Position> positions;
    Position single_position;
    std::vector<Trade> trades;
    std::vector<double> equity;

    void FillOrder(std::shared_ptr<Order> order);
    void CloseTrade(EPosition::Side side, double price, size_t remain_size);
    void CloseTrade(std::shared_ptr<Order> order);
};
}// namespace FastBT
#endif//BACKTESTING_BROKER_H
