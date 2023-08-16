//
// Created by Molin Liu on 15/8/23.
//

#ifndef BACKTESTING_NUMERIC_H
#define BACKTESTING_NUMERIC_H

#include "dtype.h"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace BackTesting {
namespace Numeric {

inline std::vector<double> MA(const std::vector<double> &data, int period) {
    std::vector<double> ma;
    ma.reserve(data.size());
    double sum = 0;
    for (int i = 0; i < data.size(); ++i) {
        if (i < period) {
            sum += data[i];
            ma.push_back(sum / (i + 1));
        } else {
            sum += data[i] - data[i - period];
            ma.push_back(sum / period);
        }
    }
    return ma;
}

inline int64_t double_to_int64(double d, int64_t scale) {
    return static_cast<int64_t>(d * pow(10, scale));
}

inline double int64_to_double(int64_t i, int64_t scale) {
    return static_cast<double>(i) / pow(10, scale);
}

inline std::time_t string_to_timestamp(const std::string &date_string) {
    std::tm tm = {};
    std::stringstream ss(date_string);
    ss >> std::get_time(&tm, "%d.%m.%Y %H:%M:%S");
    std::time_t time_stamp = mktime(&tm);
    return time_stamp;
}

inline size_t get_max_size_t(price_t price_scaled,
                             price_t available_balance_scaled,
                             double commision_fee) {
    return static_cast<size_t>(std::floor(
            (static_cast<double>(available_balance_scaled) /
             (static_cast<double>(price_scaled) * (1 + commision_fee)))));
}
inline double mean(std::vector<double> &returns) {
    double sum = 0;
    for (auto &i: returns) {
        sum += i;
    }
    return sum / returns.size();
}

inline double stdDev(std::vector<double> &returns) {
    double mean_val = mean(returns);
    double sum = 0;
    for (auto &i: returns) {
        sum += pow(i - mean_val, 2);
    }
    return sqrt(sum / (returns.size() - 1));
}

inline std::vector<double> calculateReturns(const std::vector<double> &equity) {
    std::vector<double> returns;
    for (size_t i = 1; i < equity.size(); i++) {
        double daily_return = (equity[i] - equity[i - 1]) / equity[i - 1];
        returns.push_back(daily_return);
    }
    return returns;
}

inline double calculateSharpeRatio(const std::vector<double> &equity,
                                   double riskFreeRate) {
    std::vector<double> returns = calculateReturns(equity);
    double mean_return = mean(returns);
    double std_dev = stdDev(returns);

    return (mean_return - riskFreeRate) / std_dev;
}


}// namespace Numeric
}// namespace FastBT
#endif//BACKTESTING_NUMERIC_H
