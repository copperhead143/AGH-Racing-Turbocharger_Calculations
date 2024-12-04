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

	//wyniki results;

	//dane uniwersale do wszystkich obliczen
	double atmospheric_press_Pa = 101300; //cisnienie atmosferyczne w paskalach
	double press_coefficient = 0.82; //wspolczynnik spadku cisnienia spowodowanego przez zwezke
	double TIP_press_losses = 0.5; //spadek cisnienienia na TIPie w PSI
	double press_losses_intercooler = 2; //2psi spadku cisnienia powodowanego poprzez intercooler


	std::cout << "=========================\nObliczenia dla E85, target_hp=100\n=========================\n";


	data daneE85_100HP;
	
	daneE85_100HP.HP = 100;
	daneE85_100HP.AFR = 8;
	daneE85_100HP.BSFC = 0.65;
	daneE85_100HP.R = 639.6;
	daneE85_100HP.Tm = 104; //40 stopni celcjusza
	daneE85_100HP.VE = 0.9;
	daneE85_100HP.N_peak_power = 10000;
	daneE85_100HP.N_peak_torque = 6000;
	daneE85_100HP.Vd = 29.3; //podane w calach szesciennych

	//results.Wa.push_back(calcWa(daneE85_100HP.HP, daneE85_100HP.AFR, daneE85_100HP.BSFC));
	//results.MAPreq.push_back(calcMAPreq(
	//	daneE85_100HP.Wa, daneE85_100HP.R, daneE85_100HP.Tm, daneE85_100HP.VE, daneE85_100HP.N, daneE85_100HP.Vd));

	daneE85_100HP.Wa_peak_power = calc_Wa(daneE85_100HP.HP, daneE85_100HP.AFR, daneE85_100HP.BSFC);
	daneE85_100HP.MAP_req_peak_power = calc_MAP_req(
		daneE85_100HP.Wa_peak_power, daneE85_100HP.R, daneE85_100HP.Tm, daneE85_100HP.VE, daneE85_100HP.N_peak_power, daneE85_100HP.Vd);

	std::cout << "Wa = " << daneE85_100HP.Wa_peak_power<<"\n";
	std::cout << "MAP_req = " << daneE85_100HP.MAP_req_peak_power<<"\n";

	//do tej pory dzia³a

	//obliczenie stosunku cisnien

	daneE85_100HP.compressor_discharge_press = calc_compressor_discharge_press(
		daneE85_100HP.MAP_req_peak_power, press_losses_intercooler);
	std::cout << "compressor_dischage_pressure [psi] = " << daneE85_100HP.compressor_discharge_press << "\n";

	daneE85_100HP.intake_press = Pa_to_psi(calc_intake_press(
		atmospheric_press_Pa, press_coefficient, TIP_press_losses));
	std::cout << "intake pressure [psi]= " << daneE85_100HP.intake_press << "\n";

	daneE85_100HP.press_ratio = calc_press_ratio(daneE85_100HP.intake_press, daneE85_100HP.compressor_discharge_press);
	std::cout << "PRESSURE RATIO = " << daneE85_100HP.press_ratio << "\n";

	daneE85_100HP.Wa_peak_torque = calc_Wa_peak_torque(
		daneE85_100HP.MAP_req_peak_power,daneE85_100HP.VE,daneE85_100HP.N_peak_torque,daneE85_100HP.Vd,daneE85_100HP.R,daneE85_100HP.Tm);
	std::cout << "Wa for peak torque = " << daneE85_100HP.Wa_peak_torque << "\n";

	std::cout << "gauge boost pressure [bar] = " << psi_to_bar(
		daneE85_100HP.compressor_discharge_press - Pa_to_psi(atmospheric_press_Pa))<<"\n";

	return 0;
}