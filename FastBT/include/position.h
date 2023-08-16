//
// Created by Molin Liu on 5/8/23.
//

#ifndef BackTesting_POSITION_H
#define BackTesting_POSITION_H
#include <iostream>

#include "dtype.h"
#include "order.h"
#include "symbol.h"

namespace BackTesting {
struct __attribute__((packed)) RawPosition {
    double size = 0;
    price_t entry_price_scaled = 0;
    size_t size_scaled = 0;
    double entry_price = 0;
    double margin = 0;
    double leverage = 1.0;
    EPosition::Side side;

    SymbolConfig *symbol_config;
};
class Position {
public:
    Position() = default;

    Position(SymbolConfig *config, double leverage = 1.0) {
        position.symbol_config = config;
        position.leverage = leverage;
    }
    Position(int quantity, price_t price, EPosition::Side side) {
        position.size = quantity;
        position.entry_price_scaled = price;
        position.side = side;
    }

    Position(RawPosition rawPosition) : position(rawPosition) {}

    size_t GetSize() const {
        return position.size_scaled;
    }
    double GetDecSize() const {
        return position.size;
    }
    double GetDecEntryPrice() const {
        return position.entry_price;
    }
    price_t GetEntryPrice() const {
        return position.entry_price_scaled;
    }
    
    EPosition::Side GetSide() const {
        return position.side;
    }

    void SetLeverage(double leverage) {
        position.leverage = leverage;
    }

    void AddOrder(std::shared_ptr<Order> order);

    double GetEquity(double price) const;

private:
    RawPosition position;
};
}// namespace FastBT


#endif//BackTesting_POSITION_H
