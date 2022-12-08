#include <iostream>
#include "ilcplex\cplex.h"
#include "ilcplex\ilocplex.h"
// including other files that we declared our functions
#include "EV.h"
#include "EEV.h"
#include "WS.h"
#include "RP.h"





using namespace std;

int main()
{
	// we use fsSolution pointer to pass an array to EEVCalc function as an argument
	int * fsSolution;

	// flowwing functions run the models
	fsSolution = EVCalc();
	const float EEV = EEVCalc(fsSolution);
	const float WS = WSCalc();
	const float RP = RPCalc();


	// finally we generate the report
	cout << endl << endl << endl << "............REPORT..............."<<endl<<endl<<endl;
	cout << "First Stage Variables: "<<endl;
	// first stage variables
	for (int itr = 0; itr < j; itr++) {
		cout << "x(" << itr + 1 << ") = " << fsSolution[itr] << endl;
	}
	// EV, EEV, WS, RP, EVPI, VSS: 
	cout << endl << endl << "Two Stage Objective Functions, EVPI and VSS: " << endl;
	cout << "EV:  " << fsSolution[5] << endl;
	cout << "EEV:  " << EEV<<endl;
	cout << "WS:  " << EEV << endl;
	cout << "RP:  " << RP << endl;
	cout << "EVPI:  " << RP - WS << endl;
	cout << "VSS:  " << EEV - RP << endl;
	



	cout << "\nPress [Enter] to EXIT..." << endl;
	cin.get();
	return 0;
}
