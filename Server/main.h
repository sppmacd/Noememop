#pragma once

namespace pms
{
    /////// Connect to Stat Collector ///////

    /// thanks to FamTrinli
    bool startProcess(string name, string args);

    /// thanks to FamTrinli
    void sendStats(string statString);
}
