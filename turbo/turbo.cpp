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
};



double calcWa(int HP, double AFR, double BSFC) { //potrzebny przeplyw masowy
	return HP * AFR * (BSFC / 60);
}


double calcMAPreq(double Wa, double R, int Tm, double VE, int N, double Vd) {
	return (Wa * R * (460 + Tm) / VE * Vd *(N*0.5));
}

int main() {

	//dane uniwersale do wszystkich obliczen
	double pressure_coefficient = 0.82; //wspolczynnik spadku cisnienia spowodowanego przez zwezke
	double pressure_losses = 0.5; //spadek cisnienienia na TIPie
	double pressure_losses_intercooler = 2; //2psi spadku cisnienia powodowanego poprzez intercooler


	data daneE85_100HP;
	
	daneE85_100HP.HP = 100;
	daneE85_100HP.AFR = 8;
	daneE85_100HP.BSFC = 0.55;
	daneE85_100HP.R = 639.6;
	daneE85_100HP.Tm = 104; //40 stopni celcjusza


	return 0;
}