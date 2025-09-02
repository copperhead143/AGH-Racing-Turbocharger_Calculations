#pragma once
#include <string>
#include "turbocalc/units.hpp"

namespace turbocalc
{

    struct FuelProps
    {
        double AFR{8.0};   //(-)
        double BSFC{0.65}; //[lb/(hp*h)]
        double R{639.6};   // [(in*lb)/(lb*R)]
    };

    struct EngineConfig
    {
        int target_hp{100};
        int Tm_F{104};
        double VE{0.9};
        int N_peak_power_rpm{10000};
        int N_peak_torque_rpm{5000};
        double Vd_in3{29.3};

        double atmospheric_press_Pa{101300.0};
        double pressure_coeficient{0.82};
        double TIP_losses_psi{0.5};
        double intercooler_lossees_psi{2.0};
    };
}