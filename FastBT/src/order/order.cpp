//
// Created by Molin Liu on 15/8/23.
//
#include "order.h"

BackTesting::price_t BackTesting::Order::GetPrice() const {
    return raw_order.price;
}

BackTesting::price_t BackTesting::Order::GetPriceScaled() const {
    return raw_order.price_scaled;
}

double BackTesting::Order::GetSize() const {
    return raw_order.size;
}
BackTesting::symbol_id_t BackTesting::Order::GetSymbolId() const {
    return raw_order.symbol_id;
}
BackTesting::EOrder::Side BackTesting::Order::GetSide() const {
    return raw_order.side;
}
BackTesting::EOrder::OrderType BackTesting::Order::GetOrderType() const {
    return raw_order.order_type;
}
BackTesting::EOrder::CreateType BackTesting::Order::GetCreateType() const {
    return raw_order.create_type;
}
void BackTesting::Order::SetPrice(double price) {
    raw_order.price = price;
}
void BackTesting::Order::SetSize(double size) {
    raw_order.size = size;
}
BackTesting::Order::Order(BackTesting::price_t price_scaled,
                          BackTesting::size_t size_scaled,
                          BackTesting::symbol_id_t symbol_id,
                          BackTesting::EOrder::Side side,
                          BackTesting::EOrder::OrderType order_type,
                          BackTesting::EOrder::CreateType create_type,
                          BackTesting::SymbolConfig *config) {
    this->symbol_config = config;
    raw_order = {};
    raw_order.price = this->symbol_config->ToDecPrice(price_scaled);
    raw_order.size = this->symbol_config->ToDecSize(size_scaled);
    raw_order.symbol_id = symbol_id;
    raw_order.side = side;
    raw_order.order_type = order_type;
    raw_order.create_type = create_type;
    raw_order.price_scaled = price_scaled;
    raw_order.size_scaled = size_scaled;
}
BackTesting::Order::Order(BackTesting::price_t price, BackTesting::size_t size,
                          BackTesting::symbol_id_t symbol_id,
                          BackTesting::EOrder::Side side,
                          BackTesting::SymbolConfig *config) {
    this->symbol_config = config;
    raw_order = {};
    raw_order.price = this->symbol_config->ToDecPrice(price);
    raw_order.size = this->symbol_config->ToDecSize(size);
    raw_order.symbol_id = symbol_id;
    raw_order.side = side;
    raw_order.price_scaled = price;
    raw_order.size_scaled = size;
}
BackTesting::size_t BackTesting::Order::GetSizeScaled() const {
    return raw_order.size_scaled;
}
void BackTesting::Order::SetSizeScaled(BackTesting::size_t size_scaled) {
    raw_order.size_scaled = size_scaled;
    raw_order.size = this->symbol_config->ToDecSize(size_scaled);
}
