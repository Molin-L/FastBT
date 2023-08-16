//
// Created by Molin Liu on 15/8/23.
//
#include "datafeeder.h"
#include "numeric.h"
#include <fstream>
#include <sstream>
#include <string>

BackTesting::DataFeeder::DataFeeder() {
    this->symbol_config = SymbolConfig();
    this->symbol_config.price_scale = 4;
    this->symbol_config.size_scale = 0;
    this->symbol_config.min_size_scaled = 1;
}


BackTesting::DataFeeder::DataFeeder(BackTesting::SymbolConfig config) {
    this->symbol_config = config;
}

void BackTesting::DataFeeder::LoadData(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    std::string line;
    std::getline(file, line);

    // Parse column names
    std::stringstream ss(line);
    std::string column_name;
    while (std::getline(ss, column_name, ',')) {
        column_names.push_back(column_name);
    }

    double raw_data;

    this->ohlc_data = std::unordered_map<std::string, std::vector<double>>();
    // Parse the data
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;


        OHLCScaled ohlcData;
        int colidx = 0;
        while (std::getline(ss, field, ',')) {
            if (column_names[colidx] == "Date") {
                ohlcData.timestamp = static_cast<time_t>(
                        BackTesting::Numeric::string_to_timestamp(field));
                colidx++;
                continue;
            }
            if (this->ohlc_data.find(column_names[colidx]) == ohlc_data.end()) {
                this->ohlc_data[column_names[colidx]] = std::vector<double>();
            }
            raw_data = std::stod(field);
            this->ohlc_data[column_names[colidx]].push_back(raw_data);

            if (column_names[colidx] == "Open") {
                ohlcData.open = static_cast<price_t>(
                        BackTesting::Numeric::double_to_int64(
                                raw_data, this->symbol_config.price_scale));
            } else if (column_names[colidx] == "High") {
                ohlcData.high = static_cast<price_t>(
                        BackTesting::Numeric::double_to_int64(
                                raw_data, this->symbol_config.price_scale));
            } else if (column_names[colidx] == "Low") {
                ohlcData.low = static_cast<price_t>(
                        BackTesting::Numeric::double_to_int64(
                                raw_data, this->symbol_config.price_scale));
            } else if (column_names[colidx] == "Close") {
                ohlcData.close = static_cast<price_t>(
                        BackTesting::Numeric::double_to_int64(
                                raw_data, this->symbol_config.price_scale));
            } else if (column_names[colidx] == "Volume") {
                ohlcData.volume = static_cast<size_t>(raw_data);
            }
            colidx++;
        }

        data.push_back(ohlcData);
    }

    file.close();
}

const BackTesting::OHLCScaled &
BackTesting::DataFeeder::operator[](BackTesting::size_t index) const {
    return data[index];
}

double BackTesting::DataFeeder::GetRawPrice(BackTesting::price_t price) const {
    return BackTesting::Numeric::int64_to_double(
            price, this->symbol_config.price_scale);
}

const std::vector<BackTesting::OHLCScaled> &
BackTesting::DataFeeder::GetScaledData() const {
    return data;
}
const BackTesting::SymbolConfig &
BackTesting::DataFeeder::GetSymbolConfig() const {
    return this->symbol_config;
}
std::unordered_map<std::string, std::vector<double>> &
BackTesting::DataFeeder::GetOHLCData() {
    return this->ohlc_data;
}

double BackTesting::DataFeeder::GetPrice(BackTesting::size_t index) const {
    return this->GetRawPrice((*this)[index].close);
}

BackTesting::price_t
BackTesting::DataFeeder::GetScaledPrice(BackTesting::size_t index) const {
    return (*this)[index].close;
}
BackTesting::OHLCRaw
BackTesting::DataFeeder::operator[](BackTesting::size_t index) {
    OHLCRaw raw_data;
    raw_data.timestamp = this->GetScaledData()[index].timestamp;
    raw_data.open = this->GetRawPrice(this->GetScaledData()[index].open);
    raw_data.high = this->GetRawPrice(this->GetScaledData()[index].high);
    raw_data.low = this->GetRawPrice(this->GetScaledData()[index].low);
    raw_data.close = this->GetRawPrice(this->GetScaledData()[index].close);
    raw_data.volume = this->GetRawPrice(this->GetScaledData()[index].volume);
    return raw_data;
}
BackTesting::SymbolConfig *BackTesting::DataFeeder::GetSymbolConfigPtr() const {
    return const_cast<SymbolConfig *>(&this->symbol_config);
}
