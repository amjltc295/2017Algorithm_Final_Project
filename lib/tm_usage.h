// **************************************************************************
// File       [ tm_usage.h ]
// Author     [ littleshamoo ]
// Synopsis   [ CPU time and Memory usage calculator ]
// Date       [ Ver 2.0 started 2010/03/23 ]
// History    [ created TmStat structure to store usage ]
// **************************************************************************

#ifndef _COMMON_TM_USAGE_H_
#define _COMMON_TM_USAGE_H_

namespace CommonNs {

// **************************************************************************
// Usage
//
// int main() {
//     TmUsage tmusg;
//     tmusg.totalStart();
//
//     // do something...
//
//     tmusg.periodStart();
//
//     // do something...
//
//     TmStat stat;
//     tmusg.getPeriodUsage(stat);
//     cout << stat.uTime / 1000.0 << "s" << endl; // print period user time
//
//     // do something...
//
//     tmusg.getTotalUsage(stat);
//     cout << stat.vmSize / 1024.0 << "MB" << endl; // print current memory
//     cout << stat.vmPeak / 1024.0 << "MB" << endl; // print peak memory
//
//     return 0;
// }
// **************************************************************************

struct TmStat {
    long vmSize; // current memory in kilobytes
    long vmPeak; // peak memory in kilobytes
    long vmDiff; // memory difference in kilobytes
    long rTime;  // real time in micro seconds
    long uTime;  // user time in micro seconds
    long sTime;  // system time in micro seconds
};

class TmUsage {
public:
         TmUsage();
         ~TmUsage();

    bool totalStart();
    bool periodStart();
    bool getTotalUsage(TmStat &st) const;
    bool getPeriodUsage(TmStat &st) const;
    bool checkUsage(TmStat &st) const;

private:
    TmStat tStart_;
    TmStat pStart_;
};

};

#endif

