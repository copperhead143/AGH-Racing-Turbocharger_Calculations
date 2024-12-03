#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>


struct data{
	int HP;
	double AFR;
	double BSFC; // lb/min
	double Wa; // lb/min
	double R; //(in*lb) / (lb*R)
	int Tm; //stopnie Farenhajta
	double VE; //wydajnoœæ objêtoœciowa
	int N; //obroty
	double Vd; //pojemnosc skokowa
	double MAP_req; // wymagane cisnienie w kolektorze
};

//struct wyniki {
//	std::vector<double> Wa;
//	std::vector<double> MAPreq;
//	std::vector<double> compressor_discharge_pressure;
//};



double calcWa(int HP, double AFR, double BSFC) { //potrzebny przeplyw masowy
	return HP * AFR * (BSFC / 60);
}


double calcMAPreq(double Wa, double R, int Tm, double VE, int N, double Vd) {
	return (Wa * R * (460 + Tm) / (VE * Vd *(N*0.5)));
}

int main() {

	//wyniki results;

	//dane uniwersale do wszystkich obliczen
	double pressure_coefficient = 0.82; //wspolczynnik spadku cisnienia spowodowanego przez zwezke
	double pressure_losses = 0.5; //spadek cisnienienia na TIPie w PSI
	double pressure_losses_intercooler = 2; //2psi spadku cisnienia powodowanego poprzez intercooler


	data daneE85_100HP;
	
	daneE85_100HP.HP = 100;
	daneE85_100HP.AFR = 8;
	daneE85_100HP.BSFC = 0.65;
	daneE85_100HP.R = 639.6;
	daneE85_100HP.Tm = 104; //40 stopni celcjusza
	daneE85_100HP.VE = 0.9;
	daneE85_100HP.N = 10000;
	daneE85_100HP.Vd = 29.3; //podane w calach szesciennych

	//results.Wa.push_back(calcWa(daneE85_100HP.HP, daneE85_100HP.AFR, daneE85_100HP.BSFC));
	//results.MAPreq.push_back(calcMAPreq(
	//	daneE85_100HP.Wa, daneE85_100HP.R, daneE85_100HP.Tm, daneE85_100HP.VE, daneE85_100HP.N, daneE85_100HP.Vd));

	daneE85_100HP.Wa = calcWa(daneE85_100HP.HP, daneE85_100HP.AFR, daneE85_100HP.BSFC);
	daneE85_100HP.MAP_req = calcMAPreq(
		daneE85_100HP.Wa, daneE85_100HP.R, daneE85_100HP.Tm, daneE85_100HP.VE, daneE85_100HP.N, daneE85_100HP.Vd);

	std::cout << "Wa = " << daneE85_100HP.Wa<<"\n";
	std::cout << "MAP_req = " << daneE85_100HP.MAP_req<<"\n";

	//do tej pory dzia³a



	return 0;
}