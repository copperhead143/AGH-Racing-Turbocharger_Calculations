#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>


struct data{
	int HP; //docelowa moc
	double AFR;
	double BSFC; // lb/min
	double Wa_peak_power; // lb/min
	double Wa_peak_torque; // lb/min, obliczane z MAP_req dla peak_power
	double R; //(in*lb) / (lb*R)
	int Tm; //stopnie Farenhajta
	double VE; //wydajnoœæ objêtoœciowa
	int N_peak_power; //obroty
	int N_peak_torque;
	double Vd; //pojemnosc skokowa
	double MAP_req_peak_power; // wymagane cisnienie w kolektorze dla danej mocy
	double MAP_req_peak_torque;
	double compressor_discharge_press; //cisnienie ktore musi wytworzyc turbo aby uzyskac porz¹dane MAP_req
	double intake_press; //cisnienie przed turbo po oszacowaniu strat
	double press_ratio; //stosunek ciœnieñ przed i za kompresorem
};

//struct wyniki {
//	std::vector<double> Wa;
//	std::vector<double> MAPreq;
//	std::vector<double> compressor_discharge_press;
//};



double Pa_to_psi(double pressPa) {
	return pressPa * 0.000145038; // 1 Pa = 0.000145038 psi
}

double psi_to_Pa(double pressurePsi) {
	return pressurePsi * 6894.76; // 1 psi = 6894.76 Pa
}

double psi_to_bar(double pressPsi) {
	return pressPsi * 0.0689476; // 1 psi = 0.0689476 bar
}

double calc_Wa(int HP, double AFR, double BSFC) { //potrzebny przeplyw masowy
	return HP * AFR * (BSFC / 60);
}


double calc_MAP_req(double Wa, double R, int Tm, double VE, int N, double Vd) { //cisnienie w kolektorze
	return (Wa * R * (460 + Tm) / (VE * Vd *(N*0.5)));
}

double calc_compressor_discharge_press(double MAP_req, double press_loss_intercooler) {
	return MAP_req + press_loss_intercooler;
}

double calc_intake_press(double atmospheric_press, double press_coefficient, double TIP_press_losses) {
	
	return (atmospheric_press - psi_to_Pa(TIP_press_losses)) * press_coefficient;
}

double calc_press_ratio(double press_before_turbo, double compressor_discharge_press) {
	return compressor_discharge_press / press_before_turbo;
}

double calc_Wa_peak_torque(double MAP_req_peak_power, double VE, double N, double Vd, double R, double Tm) {
	return (MAP_req_peak_power * VE * (N*0.5) * Vd) / (R*(460+Tm));
}

int main() {
    double atmospheric_press_Pa = 101300;
    double press_coefficient = 0.82;
    double TIP_press_losses = 0.5;
    double press_losses_intercooler = 2;

    int powers[] = { 120, 110, 100, 90, 80, 70 };


    std::cout << "==================\nObliczenia dla wody E85R\n==================\n\n";
    for (int power : powers) {
        std::cout << "=========================\nObliczenia dla E85, target_hp=" << power << "\n=========================\n";

        data daneE85_custom;

        daneE85_custom.HP = power;
        daneE85_custom.AFR = 8;
        daneE85_custom.BSFC = 0.65;
        daneE85_custom.R = 639.6;
        daneE85_custom.Tm = 104;
        daneE85_custom.VE = 0.9;
        daneE85_custom.N_peak_power = 10000;
        daneE85_custom.N_peak_torque = 6000;
        daneE85_custom.Vd = 29.3;

        daneE85_custom.Wa_peak_power = calc_Wa(daneE85_custom.HP, daneE85_custom.AFR, daneE85_custom.BSFC);
        daneE85_custom.MAP_req_peak_power = calc_MAP_req(
            daneE85_custom.Wa_peak_power, daneE85_custom.R, daneE85_custom.Tm, daneE85_custom.VE,
            daneE85_custom.N_peak_power, daneE85_custom.Vd);

        std::cout << "Wa = " << daneE85_custom.Wa_peak_power << "\n";
        std::cout << "MAP_req = " << daneE85_custom.MAP_req_peak_power << "\n";

        daneE85_custom.compressor_discharge_press = calc_compressor_discharge_press(
            daneE85_custom.MAP_req_peak_power, press_losses_intercooler);
        std::cout << "compressor_dischage_pressure [psi] = " << daneE85_custom.compressor_discharge_press << "\n";

        daneE85_custom.intake_press = Pa_to_psi(calc_intake_press(
            atmospheric_press_Pa, press_coefficient, TIP_press_losses));
        std::cout << "intake pressure [psi]= " << daneE85_custom.intake_press << "\n";

        daneE85_custom.press_ratio = calc_press_ratio(daneE85_custom.intake_press, daneE85_custom.compressor_discharge_press);
        std::cout << "PRESSURE RATIO = " << daneE85_custom.press_ratio << "\n";

        daneE85_custom.Wa_peak_torque = calc_Wa_peak_torque(
            daneE85_custom.MAP_req_peak_power, daneE85_custom.VE, daneE85_custom.N_peak_torque,
            daneE85_custom.Vd, daneE85_custom.R, daneE85_custom.Tm);
        std::cout << "Wa for peak torque = " << daneE85_custom.Wa_peak_torque << "\n";

        std::cout << "gauge boost pressure [bar] = " << psi_to_bar(
            daneE85_custom.compressor_discharge_press - Pa_to_psi(atmospheric_press_Pa)) << "\n\n";
    }

    //RON98
    std::cout << "==================\nObliczenia dla benzyny RON98\n==================\n\n";
    for (int power : powers) {
        std::cout << "=========================\nObliczenia dla RON98, target_hp=" << power << "\n=========================\n";

        data daneRon98_custom;

        daneRon98_custom.HP = power;
        daneRon98_custom.AFR = 11.5;
        daneRon98_custom.BSFC = 0.55;
        daneRon98_custom.R = 639.6;
        daneRon98_custom.Tm = 104;
        daneRon98_custom.VE = 0.9;
        daneRon98_custom.N_peak_power = 10000;
        daneRon98_custom.N_peak_torque = 6000;
        daneRon98_custom.Vd = 29.3;

        //obliczenia dla benzyny RON98
        daneRon98_custom.Wa_peak_power = calc_Wa(daneRon98_custom.HP, daneRon98_custom.AFR, daneRon98_custom.BSFC);
        daneRon98_custom.MAP_req_peak_power = calc_MAP_req(
            daneRon98_custom.Wa_peak_power, daneRon98_custom.R, daneRon98_custom.Tm, daneRon98_custom.VE,
            daneRon98_custom.N_peak_power, daneRon98_custom.Vd);

        std::cout << "Wa = " << daneRon98_custom.Wa_peak_power << "\n";
        std::cout << "MAP_req = " << daneRon98_custom.MAP_req_peak_power << "\n";

        daneRon98_custom.compressor_discharge_press = calc_compressor_discharge_press(
            daneRon98_custom.MAP_req_peak_power, press_losses_intercooler);
        std::cout << "compressor_dischage_pressure [psi] = " << daneRon98_custom.compressor_discharge_press << "\n";

        daneRon98_custom.intake_press = Pa_to_psi(calc_intake_press(
            atmospheric_press_Pa, press_coefficient, TIP_press_losses));
        std::cout << "intake pressure [psi]= " << daneRon98_custom.intake_press << "\n";

        daneRon98_custom.press_ratio = calc_press_ratio(daneRon98_custom.intake_press, daneRon98_custom.compressor_discharge_press);
        std::cout << "PRESSURE RATIO = " << daneRon98_custom.press_ratio << "\n";

        daneRon98_custom.Wa_peak_torque = calc_Wa_peak_torque(
            daneRon98_custom.MAP_req_peak_power, daneRon98_custom.VE, daneRon98_custom.N_peak_torque,
            daneRon98_custom.Vd, daneRon98_custom.R, daneRon98_custom.Tm);
        std::cout << "Wa for peak torque = " << daneRon98_custom.Wa_peak_torque << "\n";

        std::cout << "gauge boost pressure [bar] = " << psi_to_bar(
            daneRon98_custom.compressor_discharge_press - Pa_to_psi(atmospheric_press_Pa)) << "\n\n";
    }

    return 0;
}