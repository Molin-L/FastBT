//
// Created by Molin Liu on 15/8/23.
//

#ifndef BACKTESTING_ANALYZER_H
#define BACKTESTING_ANALYZER_H
#include <memory>
#include <sstream>


#include "datafeeder.h"
#include "trade.h"


namespace BackTesting {
struct Performance {
    double total_return;
    double annual_return;
    double win_rate;
    double max_drawdown;
    double sharpe_ratio;
    double sortino_ratio;
    double calmar_ratio;
    double alpha;
    double beta;
    double volatility;
    double information_ratio;
    double downside_risk;
    double tracking_error;
    double profit_factor;
    double average_return;
    double average_loss;

    std::string ToString() {
        std::ostringstream oss;
        oss << "total_return: " << total_return << "\n"
            << "annual_return: " << annual_return << "\n"
            << "win_rate: " << win_rate << "\n"
            << "max_drawdown: " << max_drawdown << "\n"
            << "sharpe_ratio: " << sharpe_ratio << "\n"
            << "sortino_ratio: " << sortino_ratio << "\n"
            << "calmar_ratio: " << calmar_ratio << "\n"
            << "alpha: " << alpha << "\n"
            << "beta: " << beta << "\n"
            << "volatility: " << volatility << "\n"
            << "information_ratio: " << information_ratio << "\n"
            << "downside_risk: " << downside_risk << "\n"
            << "tracking_error: " << tracking_error << "\n"
            << "profit_factor: " << profit_factor << "\n"
            << "average_return: " << average_return << "\n"
            << "average_loss: " << average_loss;
        return oss.str();
    }
};
class Analyzer {
public:
    Analyzer() = default;

    Analyzer(std::shared_ptr<DataFeeder> data_feeder)
        : data_feeder(data_feeder) {}
    Performance Analyze(const std::vector<Trade> &trades,
                        const std::vector<double> &equity);

private:
    std::shared_ptr<DataFeeder> data_feeder;
};
}// namespace FastBT
#endif//BACKTESTING_ANALYZER_H
