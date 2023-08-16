//
// Created by Molin Liu on 15/8/23.
//
#include "position.h"
#include "utils.h"
void BackTesting::Position::AddOrder(
        std::shared_ptr<BackTesting::Order> order) {
    if (this->GetSize() == 0) {
        this->position.side = ToPositionSide(order->GetSide());
    }
    // Add order to position
    if (this->GetSide() == ToPositionSide(order->GetSide())) {
        int64_t value_scaled =
                (position.size_scaled * position.entry_price_scaled +
                 order->GetSizeScaled() * order->GetPriceScaled());
        position.size += order->GetSize();
        position.size_scaled += order->GetSizeScaled();
        position.entry_price_scaled = value_scaled / position.size_scaled;
        position.entry_price =
                position.symbol_config->ToDecPrice(position.entry_price_scaled);
    } else {
        // Close position
        double value = position.size * position.entry_price -
                       order->GetSize() * order->GetPrice();
        position.size -= order->GetSize();
        position.size_scaled -= order->GetSizeScaled();
        position.entry_price = value / position.size;
    }
}
double BackTesting::Position::GetEquity(double price) const {
    return position.symbol_config->ToDecPrice(
            position.symbol_config->ToScaledPrice(price) * position.size);
}
