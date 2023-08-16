//
// Created by Molin Liu on 5/8/23.
//

#ifndef BackTesting_SYMBOL_H
#define BackTesting_SYMBOL_H
#pragma once
#include <iostream>

namespace BackTesting {
class SymbolConfig {
public:
    SymbolConfig() = default;
    price_t ToScaledPrice(double num) {
        return static_cast<price_t>(num * std::pow(10, price_scale));
    }
    double ToDecPrice(price_t num) {
        return static_cast<double>(num) / std::pow(10, price_scale);
    }

    price_t ToScaledSize(double size) {
        return static_cast<price_t>(size * std::pow(10, size_scale));
    }

    double ToDecSize(size_t size) {
        return static_cast<double>(size) / std::pow(10, size_scale);
    }

public:
    int32_t symbol_id;
    int8_t size_scale;
    int8_t price_scale;
    int8_t min_size_scaled;
};
}// namespace FastBT
namespace BackTesting {}

#endif//BackTesting_SYMBOL_H
