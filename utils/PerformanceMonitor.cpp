#include "PerformanceMonitor.h"

namespace PerformanceMonitor {

bool State::parseArgument(const QByteArray &arg)
{
    bool result = false;
    if (arg == "-log-perf") {
        logging = true;
        valid = true;
        result = true;
    } else if (arg == "-no-log-perf") {
        logging = false;
        valid = true;
        result = true;
    } else if (arg == "-show-perf") {
        visible = true;
        valid = true;
        result = true;
    } else if (arg == "-hide-perf") {
        visible = false;
        valid = true;
        result = true;
    }
    return result;
}

} // namespace PerformanceMonitor
