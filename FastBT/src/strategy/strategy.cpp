//
// Created by Molin Liu on 15/8/23.
//
#include "strategy.h"
#include "numeric.h"
#include <memory>


void BackTesting::BaseStrategy::Reset() {
    time_index = 0;
    price = 0;
    finished = false;
}

BackTesting::Position &
BackTesting::BaseStrategy::GetPosition(BackTesting::EPosition::Side side) {
    return this->broker->GetPosition(side);
}
void BackTesting::BaseStrategy::OnTick(size_t index) {
    this->time_index = index;
    if (time_index >= data->GetScaledData().size()) {
        finished = true;
        return;
    }
    price = this->data->GetPrice(time_index);
    price_scaled = this->data->GetScaledPrice(time_index);
    // Strategy
    Next();
}
void BackTesting::BaseStrategy::Init(
        std::shared_ptr<BackTesting::DataFeeder> data,
        std::shared_ptr<Broker> broker) {
    this->data = data;
    this->broker = broker;

    Init();
    Reset();
}

const std::vector<BackTesting::OHLCScaled> &
BackTesting::BaseStrategy::GetData() const {
    return this->data->GetScaledData();
}

const BackTesting::Order &BackTesting::BaseStrategy::Buy() {
    size_t size = 0;
    EOrder::CreateType create_type = EOrder::CreateType::OPEN;

    return this->Buy(size, price_scaled, create_type,
                     EOrder::OrderType::MARKET);
}

const BackTesting::Order &BackTesting::BaseStrategy::Sell() {
    size_t size = 0;
    EOrder::CreateType create_type = EOrder::CreateType::OPEN;

    return this->Sell(size, price_scaled, create_type,
                      EOrder::OrderType::MARKET);
}

const BackTesting::Order &BackTesting::BaseStrategy::CreateOrder(
        BackTesting::EOrder::Side side, BackTesting::size_t size, price_t price,
        BackTesting::EOrder::CreateType create_type,
        BackTesting::EOrder::OrderType order_type) {
    auto order = std::make_shared<Order>(
            price_scaled, size, data->GetSymbolConfig().symbol_id, side,
            order_type, create_type, this->data->GetSymbolConfigPtr());
    this->broker->AddOrder(order);
    return *order;
}

void BackTesting::BaseStrategy::CloseAllPosition() {
    if (this->hedge) {
        this->ClosePosition(EPosition::Side::LONG);
        this->ClosePosition(EPosition::Side::SHORT);
    } else {
        this->ClosePosition(EPosition::Side::LONG);
    }
}

void BackTesting::BaseStrategy::ClosePosition(
        BackTesting::size_t size, BackTesting::price_t price,
        BackTesting::EPosition::Side side,
        BackTesting::EOrder::OrderType order_type) {}
void BackTesting::BaseStrategy::ClosePosition(
        BackTesting::EPosition::Side side) {
    Position position;
    if (this->hedge) {
        position = this->broker->GetPosition(side);
    } else {
        position = this->broker->GetSinglePosition();
    }
    if (position.GetSize() == 0) {
        return;
    }
    if (position.GetSide() == EPosition::Side::LONG) {
        this->Sell(position.GetSize(), price_scaled, EOrder::CreateType::CLOSE,
                   EOrder::OrderType::MARKET);
    } else if (position.GetSide() == EPosition::Side::SHORT) {
        this->Buy(position.GetSize(), price_scaled, EOrder::CreateType::CLOSE,
                  EOrder::OrderType::MARKET);
    }
}
std::vector<double> BackTesting::BaseStrategy::GetDataSeries(std::string col) {
    return this->data->GetOHLCData().at(col);
}
BackTesting::BaseStrategy::~BaseStrategy() {}
void BackTesting::BaseStrategy::Init() {}
void BackTesting::BaseStrategy::Next() {}

const BackTesting::Order &
BackTesting::BaseStrategy::Buy(BackTesting::size_t size_scaled,
                               BackTesting::price_t price_scaled,
                               BackTesting::EOrder::CreateType create_type,
                               BackTesting::EOrder::OrderType order_type) {
    return CreateOrder(EOrder::Side::BUY, size_scaled, price_scaled,
                       create_type, order_type);
}
const BackTesting::Order &
BackTesting::BaseStrategy::Sell(BackTesting::size_t size_scaled,
                                BackTesting::price_t price_scaled,
                                BackTesting::EOrder::CreateType create_type,
                                BackTesting::EOrder::OrderType order_type) {
    return CreateOrder(EOrder::Side::SELL, size_scaled, price_scaled,
                       create_type, order_type);
}
