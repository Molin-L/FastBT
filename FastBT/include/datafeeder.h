//
// Created by Molin Liu on 15/8/23.
//

#ifndef BACKTESTING_DATAFEEDER_H
#define BACKTESTING_DATAFEEDER_H

#include "dtype.h"
#include "symbol.h"
#include <unordered_map>
#include <vector>


namespace BackTesting {
struct __attribute__((packed)) OHLCScaled {
    time_t timestamp;
    price_t open;
    price_t high;
    price_t low;
    price_t close;
    size_t volume;
};

struct __attribute__((packed)) OHLCRaw {
    time_t timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class DataFeeder {
public:
    DataFeeder();

    DataFeeder(SymbolConfig config);

    void LoadData(const std::string &file_path);

    const OHLCScaled &operator[](size_t index) const;

    double GetRawPrice(price_t price) const;

    const std::vector<OHLCScaled> &GetScaledData() const;

    const SymbolConfig &GetSymbolConfig() const;

    SymbolConfig *GetSymbolConfigPtr() const;

    std::unordered_map<std::string, std::vector<double>> &GetOHLCData();

    double GetPrice(size_t index) const;

    price_t GetScaledPrice(size_t index) const;

    OHLCRaw operator[](size_t index);

private:
    std::unordered_map<std::string, std::vector<double>> ohlc_data;
    std::vector<OHLCScaled> data;
    SymbolConfig symbol_config;
    std::vector<std::string> column_names;
};
}// namespace FastBT
#endif//BACKTESTING_DATAFEEDER_H
