//
// Created by Molin Liu on 5/8/23.
//

#ifndef BackTesting_DTYPE_H
#define BackTesting_DTYPE_H
#pragma once
#include <iostream>

namespace BackTesting {
typedef uint8_t uid_t;
typedef int64_t price_t;
typedef int64_t size_t;
typedef int32_t symbol_id_t;
typedef int64_t time_t;
namespace EPosition {
enum Side {
    LONG = 1,
    SHORT = 2
};
inline bool OppositeSide(Side side1, Side side2) {
    return side1 != side2;
}
}// namespace EPosition
namespace EOrder {
enum Side {
    BUY = 1,
    SELL = 2
};
enum OrderType {
    LIMIT = 1,
    MARKET = 2
};
enum CreateType {
    OPEN = 1,
    CLOSE = 2
};
enum FillType {
    MAKER = 1,
    TAKER = 2
};
}// namespace EOrder
namespace ETrade {
enum Status {
    OPEN = 1,
    CLOSED = 2
};
}// namespace ETrade
}// namespace FastBT

#endif//BackTesting_DTYPE_H
