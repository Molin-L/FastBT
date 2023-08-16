//
// Created by Molin Liu on 15/8/23.
//

#ifndef BACKTESTING_UTILS_H
#define BACKTESTING_UTILS_H
#include "dtype.h"
#include <fstream>
#include <iostream>

inline std::string generate_uuid() {
    std::string uuid;
    FILE *file = popen("uuidgen", "r");
    char buffer[37];// UUID length is 36 characters
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        uuid = buffer;
    }
    pclose(file);
    uuid.pop_back();// remove the newline character
    return uuid;
}
inline BackTesting::EPosition::Side
ToPositionSide(BackTesting::EOrder::Side side) {
    switch (side) {
        case BackTesting::EOrder::Side::BUY:
            return BackTesting::EPosition::Side::LONG;
        case BackTesting::EOrder::Side::SELL:
            return BackTesting::EPosition::Side::SHORT;
        default:
            throw std::runtime_error("Unknown side");
    }
}

inline BackTesting::EPosition::Side
ToOppositePositionSide(BackTesting::EOrder::Side side) {
    switch (side) {
        case BackTesting::EOrder::Side::BUY:
            return BackTesting::EPosition::Side::SHORT;
        case BackTesting::EOrder::Side::SELL:
            return BackTesting::EPosition::Side::LONG;
        default:
            throw std::runtime_error("Unknown side");
    }
}

inline void ExportEquity(std::string file_path,
                         const std::vector<double> &equity) {
    std::ofstream file(file_path);
    file << "index,equity\n";
    file << std::fixed << std::setprecision(2);
    for (int i = 0; i < equity.size(); i++) {
        file << i << "," << equity[i] << "\n";
    }
    file.close();
}


#endif//BACKTESTING_UTILS_H
