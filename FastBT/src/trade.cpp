//
// Created by Molin Liu on 15/8/23.
//
#include "trade.h"
#include "utils.h"

BackTesting::Trade::Trade(const BackTesting::Order &order, time_t time) {
    this->symbol_id = order.GetSymbolId();
    this->side = ToPositionSide(order.GetSide());
    this->entry_time = time;
    this->entry_size = order.GetSize();
    this->entry_size_scaled = order.GetSizeScaled();
    this->entry_price = order.GetPrice();
    this->entry_price_scaled = order.GetPriceScaled();
    this->status = ETrade::Status::OPEN;
}
BackTesting::size_t BackTesting::Trade::Close(double price,
                                              BackTesting::size_t size,
                                              BackTesting::time_t time) {
    if (this->status == ETrade::Status::CLOSED) {
        return size;
    }
    this->exit_price = price;
    this->exit_time = time;
    this->status = ETrade::Status::CLOSED;
    if (size >= this->entry_size) {
        this->exit_size = this->entry_size;
    } else {
        this->exit_size = size;
    }
    return size - this->entry_size;
}
BackTesting::Trade BackTesting::Trade::Remain() {
    Trade trade(*this);
    trade.status = ETrade::Status::OPEN;
    trade.entry_size = this->entry_size - this->exit_size;
    trade.exit_price = 0.0;
    trade.exit_size = 0;
    trade.exit_time = 0;
    return trade;
}

BackTesting::Trade::Trade(const BackTesting::Trade &trade) {
    this->symbol_id = trade.symbol_id;
    this->side = trade.side;
    this->entry_time = trade.entry_time;
    this->entry_size = trade.entry_size;
    this->entry_price = trade.entry_price;
    this->exit_time = trade.exit_time;
    this->exit_size = trade.exit_size;
    this->exit_price = trade.exit_price;
    this->status = trade.status;
    this->entry_size_scaled = trade.entry_size_scaled;
    this->entry_price_scaled = trade.entry_price_scaled;
    this->exit_size_scaled = trade.exit_size_scaled;
    this->exit_price_scaled = trade.exit_price_scaled;
}
std::string BackTesting::Trade::ToString() {
    return std::to_string(this->uid) + "," + std::to_string(this->status) +
           "," + std::to_string(this->symbol_id) + "," +
           std::to_string(this->side) + "," + std::to_string(this->entry_time) +
           "," + std::to_string(this->entry_size) + "," +
           std::to_string(this->entry_price) + "," +
           std::to_string(this->entry_price_scaled) + "," +
           std::to_string(this->entry_size_scaled) + "," +
           std::to_string(this->exit_time) + "," +
           std::to_string(this->exit_size) + "," +
           std::to_string(this->exit_price) + "," +
           std::to_string(this->exit_size_scaled) + "," +
           std::to_string(this->exit_price_scaled);
}
BackTesting::size_t BackTesting::Trade::Close(const BackTesting::Order &order,
                                              BackTesting::time_t time_idx) {
    if (this->status == ETrade::Status::CLOSED) {
        return order.GetSizeScaled();
    }
    this->exit_price = order.GetPrice();
    this->exit_price_scaled = order.GetPriceScaled();
    this->exit_time = time_idx;
    this->status = ETrade::Status::CLOSED;
    if (order.GetSizeScaled() >= this->entry_size_scaled) {
        this->exit_size_scaled = this->entry_size_scaled;
        this->exit_size = this->entry_size;
    } else {
        this->exit_size = order.GetSize();
        this->exit_size_scaled = order.GetSizeScaled();
    }
    return order.GetSizeScaled() - this->entry_size_scaled;
}
