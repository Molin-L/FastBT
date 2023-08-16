//
// Created by Molin Liu on 16/8/23.
//
#include "analyzer.h"
#include "numeric.h"
BackTesting::Performance
BackTesting::Analyzer::Analyze(const std::vector<Trade> &trades,
                               const std::vector<double> &equity) {
    Performance performance = {};
    performance.total_return = equity.back() / equity.front() - 1;
    performance.annual_return =
            performance.total_return / (equity.size() / 252);
    performance.sharpe_ratio = Numeric::calculateSharpeRatio(equity, 0);

    int win_count = 0;

    for (auto trade: trades) {

        if (trade.status == ETrade::CLOSED) {
            size_t pnl;
            if (trade.side == EPosition::Side::LONG) {
                pnl = trade.exit_price - trade.entry_price;
            } else {
                pnl = trade.entry_price - trade.exit_price;
            }
            if (pnl > 0) {
                win_count++;
            }
        }
    }
    performance.win_rate = static_cast<double>(win_count) / trades.size();
    return performance;
}
