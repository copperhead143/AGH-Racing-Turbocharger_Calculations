#pragma once
#include <cmath>

namespace turbocalc::units
{
    constexpr double PA_PER_PSI = 6894.76;
    constexpr double PSI_PER_PA = 1.0 / PA_PER_PSI;
    constexpr double BAR_PER_PSI = 0.0689476;

    inline double pa_to_psi(double pa) { return pa * PSI_PER_PA; }
    inline double psi_to_pa(double psi) { return psi * PA_PER_PSI; }
    inline double psi_to_bar(double psi) { return psi * BAR_PER_PSI; }
}