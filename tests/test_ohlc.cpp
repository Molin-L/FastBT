//
// Created by Molin Liu on 15/8/23.
//
#include "datafeeder.h"
#include <gtest/gtest.h>

TEST(DataFeeder, LoadData) {
    auto ohlc = BackTesting::DataFeeder();
    ohlc.LoadData("../Data/XAUUSD.csv");
    ASSERT_EQ(ohlc.GetScaledData().size(), 2676);
    ASSERT_EQ(ohlc.GetOHLCData()["Open"][0], 1183.949);
}
