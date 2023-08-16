//
// Created by Molin Liu on 15/8/23.
//
#include "broker.h"
#include "numeric.h"
#include "utils.h"
BackTesting::Position &BackTesting::Broker::GetPosition() {
    if (this->hedge) {
        return this->single_position;
    } else {
        for (auto &position: this->positions) {
            if (position.second.GetSize() != 0) {
                return position.second;
            }
        }
        return positions[EPosition::Side::LONG];
    }
}
double BackTesting::Broker::GetAvailableBalance() const {
    return available_balance;
}
BackTesting::Position &BackTesting::Broker::GetSinglePosition() {
    return single_position;
}
double BackTesting::Broker::GetCommisionFee() {
    return commision_fee;
}
BackTesting::Position &
BackTesting::Broker::GetPosition(BackTesting::EPosition::Side side) {
    return positions[side];
}

void BackTesting::Broker::AddOrder(std::shared_ptr<Order> order) {
    this->pending_orders.emplace(order);
}
void BackTesting::Broker::Reset() {
    this->time_index = 0;
    this->available_balance = this->initial_balance;
    this->available_balance_scaled =
            static_cast<price_t>(this->initial_balance) *
            pow(10, this->data_feeder.GetSymbolConfig().price_scale +
                            this->data_feeder.GetSymbolConfig().size_scale);
    this->positions.clear();
    this->positions[EPosition::Side::LONG] =
            Position(data_feeder.GetSymbolConfigPtr(), this->leverage);
    this->positions[EPosition::Side::SHORT] =
            Position(data_feeder.GetSymbolConfigPtr(), this->leverage);
    this->single_position =
            Position(data_feeder.GetSymbolConfigPtr(), this->leverage);
    this->trades = std::vector<Trade>();
    this->equity = std::vector<double>();
    this->pending_orders = std::queue<std::shared_ptr<Order>,
                                      std::list<std::shared_ptr<Order>>>();
    this->order_book = std::list<std::shared_ptr<Order>>();
}
void BackTesting::Broker::TriggerOrders(BackTesting::time_t time_index) {
    this->time_index = time_index;
    auto it = this->order_book.begin();
    while (it != this->order_book.end()) {
        auto order = *it;
        if (order->GetPrice() >= data_feeder.GetScaledData()[time_index].low &&
            order->GetPrice() <= data_feeder.GetScaledData()[time_index].high) {
            this->FillOrder(order);
            it = this->order_book.erase(it);
        } else {
            it++;
        }
    }
}
void BackTesting::Broker::ProcessPendingOrder() {
    while (!this->pending_orders.empty()) {
        auto order = this->pending_orders.front();
        this->pending_orders.pop();
        if (order->GetSize() == 0 && order->GetOrderType() == EOrder::MARKET) {
            size_t size = BackTesting::Numeric::get_max_size_t(
                    order->GetPriceScaled(), this->available_balance_scaled,
                    this->commision_fee);
            order->SetSizeScaled(size);
        }

        if (order->GetCreateType() == EOrder::CreateType::OPEN) {
            double cost = order->GetSize() * order->GetPrice() *
                          (1 + this->commision_fee);
            if (cost > this->available_balance) {
                continue;
            }
        } else {
            // Close Position
            // Reduce the size of order if the size of order is larger than the position
            if (hedge) {
                if (positions[ToOppositePositionSide(order->GetSide())]
                            .GetSize() < order->GetSize()) {
                    order->SetSize(
                            positions[ToOppositePositionSide(order->GetSide())]
                                    .GetSize());
                }
            } else {
                if (single_position.GetSize() < order->GetSize()) {
                    order->SetSize(single_position.GetSize());
                }
            }
        }
        // Check if order will be triggered
        if (order->GetPrice() >= this->data_feeder[time_index].low &&
            order->GetPrice() <= this->data_feeder[time_index].high) {
            this->FillOrder(order);
        } else {
            // Add those non-triggered order to orderbook.
            this->order_book.emplace_back(order);
        }
    }
}

void BackTesting::Broker::FillOrder(std::shared_ptr<Order> order) {
    if (hedge) {
        if (order->GetCreateType() == EOrder::CreateType::OPEN) {
            this->trades.emplace_back(*order, this->time_index);
            this->positions[ToPositionSide(order->GetSide())].AddOrder(order);
            this->available_balance -= order->GetSize() * order->GetPrice() *
                                       (1 + this->commision_fee);
            this->available_balance_scaled -= order->GetPriceScaled() *
                                              order->GetSize() *
                                              (1 + this->commision_fee);
        } else {
            this->positions[ToOppositePositionSide(order->GetSide())].AddOrder(
                    order);
            this->CloseTrade(order);
            this->available_balance += order->GetSize() * order->GetPrice();
            this->available_balance_scaled +=
                    order->GetPriceScaled() * order->GetSize();
        }
    } else {
        this->single_position.AddOrder(order);
        if (order->GetCreateType() == EOrder::CreateType::CLOSE) {
            this->CloseTrade(order);
            this->available_balance += order->GetSize() * order->GetPrice();
            this->available_balance_scaled +=
                    order->GetPriceScaled() * order->GetSize();
        } else {
            this->trades.emplace_back(*order, this->time_index);
            this->available_balance -= order->GetSize() * order->GetPrice() *
                                       (1 + this->commision_fee);
            this->available_balance_scaled -= order->GetPriceScaled() *
                                              order->GetSize() *
                                              (1 + this->commision_fee);
        }
    }
}

double BackTesting::Broker::GetEquity() {
    double equity =
            static_cast<double>(this->available_balance_scaled) /
            pow(10, this->data_feeder.GetSymbolConfig().price_scale +
                            this->data_feeder.GetSymbolConfig().size_scale);
    if (hedge) {
        equity += this->positions[EPosition::Side::LONG].GetEquity(
                this->data_feeder.GetPrice(this->time_index));
        equity += this->positions[EPosition::Side::SHORT].GetEquity(
                this->data_feeder.GetPrice(this->time_index));
    } else {
        equity += this->single_position.GetEquity(
                this->data_feeder.GetPrice(this->time_index));
    }
    return equity;
}
void BackTesting::Broker::TradesToCSV(std::string file_name) {
    std::ofstream file(file_name);

    // Optionally write a header
    file << "uid,status,symbol_id,side,entry_time,entry_size,entry_price,entry_"
            "price_scaled,entry_size_scaled, "
            "exit_time,exit_size,exit_price,exit_size_scaled,exit_price_"
            "scaled\n";

    for (auto &trade: trades) {
        std::string temp = trade.ToString();
        file << temp << "\n";
    }
    file.close();
}
void BackTesting::Broker::CloseTrade(std::shared_ptr<Order> order) {
    size_t remain;
    for (int i = this->trades.size() - 1; i >= 0; i--) {
        remain = trades[i].Close(*order, this->time_index);
        if (remain == 0) {
            break;
        }
        if (remain < 0) {
            this->trades.emplace_back(trades[i].Remain());
            break;
        }
    }
}
std::vector<BackTesting::Trade> &BackTesting::Broker::GetTrades() {
    return this->trades;
}
