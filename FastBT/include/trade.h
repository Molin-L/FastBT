//
// Created by Molin Liu on 5/8/23.
//

#ifndef BackTesting_TRADE_H
#define BackTesting_TRADE_H

#include "dtype.h"
#include "order.h"

namespace BackTesting {
class Trade {
public:
    Trade() = delete;
    Trade(const Trade &trade);
    Trade(const Order &order, time_t time);

    Trade Remain();
    size_t Close(double price, size_t size, time_t time);
    size_t Close(const Order &order, time_t time);
    std::string ToString();

public:
    uid_t uid;
    ETrade::Status status;
    symbol_id_t symbol_id;
    EPosition::Side side;
    time_t entry_time;
    size_t entry_size;
    size_t entry_size_scaled;
    double entry_price;
    price_t entry_price_scaled;

    time_t exit_time;
    size_t exit_size;
    size_t exit_size_scaled;
    double exit_price;
    price_t exit_price_scaled;
};
}// namespace FastBT

#endif//BackTesting_TRADE_H
