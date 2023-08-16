//
// Created by Molin Liu on 5/8/23.
//

#ifndef BackTesting_ORDER_H
#define BackTesting_ORDER_H
#include "dtype.h"
#include "symbol.h"
#include <iostream>

namespace BackTesting {
namespace INTERNAL {
struct __attribute__((packed)) RawOrder {
    int64_t order_id;
    symbol_id_t symbol_id;
    double size;
    double price;
    price_t price_scaled;
    size_t size_scaled;

    EOrder::Side side;
    EOrder::OrderType order_type;
    EOrder::CreateType create_type;
};
}// namespace INTERNAL

class Order {
public:
    Order() = delete;

    Order(price_t price_scaled, size_t size_scaled, symbol_id_t symbol_id,
          EOrder::Side side, EOrder::OrderType order_type,
          EOrder::CreateType create_type, SymbolConfig *config);

    Order(price_t price_scaled, size_t size_scaled, symbol_id_t symbol_id,
          EOrder::Side side, SymbolConfig *config);

    price_t GetPrice() const;
    double GetSize() const;
    symbol_id_t GetSymbolId() const;
    EOrder::Side GetSide() const;
    price_t GetPriceScaled() const;
    size_t GetSizeScaled() const;
    EOrder::OrderType GetOrderType() const;
    EOrder::CreateType GetCreateType() const;

    void SetPrice(double price);
    void SetSizeScaled(size_t size_scaled);
    void SetSize(double size);

private:
    INTERNAL::RawOrder raw_order;
    SymbolConfig *symbol_config;
};
}// namespace FastBT

#endif//BackTesting_ORDER_H
