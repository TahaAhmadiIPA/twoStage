#include <iostream>
#include "ilcplex\cplex.h"
#include "ilcplex\ilocplex.h"
#include "data.h"
#include <vector>

using std::vector;
using namespace std;


int * EVCalc() {
	// we store the first stage variables and objective function value in fsSolution variable
	static int fsSolution[6];

	// creating the environment
	IloEnv myenv;
	// creating the model
	IloModel EV(myenv);




	// creating x, y, z variables
	IloNumVarArray xj(myenv, j, 0, 1, ILOINT);

	vector<IloNumVarArray> yij;
	yij.reserve(i);
	for (int itr = 0; itr <= i; itr++) {
		yij.push_back(IloNumVarArray(myenv, j, 0, 1, ILOINT));
	}

	IloNumVarArray y1j(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y2j(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y3j(myenv, j, 0, 1, ILOFLOAT);

	IloNumVarArray z1j1(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j2(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j3(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j1(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j2(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j3(myenv, j, 0, IloInfinity, ILOFLOAT);



	// adding the objective function to our model
	IloExpr EVobj(myenv);
	for (int itr = 0; itr < j; itr++) {
		EVobj += establishCost[itr] * xj[itr];
		EVobj += shippingCost[0][itr][0] * z1j1[itr];
		EVobj += shippingCost[0][itr][1] * z1j2[itr];
		EVobj += shippingCost[0][itr][2] * z1j3[itr];
		EVobj += shippingCost[1][itr][0] * z2j1[itr];
		EVobj += shippingCost[1][itr][1] * z1j2[itr];
		EVobj += shippingCost[1][itr][2] * z1j3[itr];
	}
	EV.add(IloMinimize(myenv, EVobj));
	// clearing data
	EVobj.end();





	// adding the first type of constraints to the model
	// these constraints, are identifying that the sum of goods flow from each factory to each distribution center to each retailer, 
	// doesnt exceed the capacity of factory
	IloExpr con11(myenv);
	for (int itr = 0; itr < j; itr++) {
		con11 += z1j1[itr] + z1j2[itr] + z1j3[itr];
	}
	EV.add(IloRange(myenv, -IloInfinity, con11, factoryCap[0]));
	con11.end();

	IloExpr con12(myenv);
	for (int itr = 0; itr < j; itr++) {
		con12 += z2j1[itr] + z2j2[itr] + z2j3[itr];
	}
	EV.add(IloRange(myenv, -IloInfinity, con12, factoryCap[0]));
	con12.end();





	// following constraints, identify that the total allocated goods to each retailer, must meet the retailer's demand
	for (int Iitr = 0; Iitr < i; Iitr++) {
		IloExpr con2(myenv);
		for (int Jitr = 0; Jitr < j; Jitr++) {
			con2 += yij[Iitr][Jitr];
			cout << Iitr << Jitr << endl;
		}
		EV.add(IloRange(myenv, 1, con2, 1));
		con2.end();
	}




	// following constraints, identifies that the sum of the good flows from each facility, dont exceed the facilities 
	// capacity(if the facility is established)
	for (int Jitr = 0; Jitr < j; Jitr++) {
		IloExpr con3(myenv);
		for (int Iitr = 0; Iitr < i; Iitr++) {
			con3 += meanDemand[Iitr] * yij[Iitr][Jitr];
		}
		con3 += -xj[Jitr] * facilityCap[Jitr];
		EV.add(IloRange(myenv, -IloInfinity, con3, 0));
		con3.end();
	}





	// these constraints are identifying that for each facility to each retailer, the sum of good flows from companies is equal to the 
	// total goods that are allocated to retailer from the facility
	for (int itr = 0; itr < j; itr++) {
		IloExpr con41(myenv);
		con41 += z1j1[itr] + z2j1[itr] - meanDemand[0] * yij[0][itr];
		EV.add(IloRange(myenv, 0, con41, 0));
		con41.end();

		IloExpr con42(myenv);
		con42 += z1j2[itr] + z2j2[itr] - meanDemand[1] * yij[1][itr];
		EV.add(IloRange(myenv, 0, con42, 0));
		con42.end();

		IloExpr con43(myenv);
		con43 += z1j3[itr] + z2j3[itr] - meanDemand[2] * yij[2][itr];
		EV.add(IloRange(myenv, 0, con43, 0));
		con43.end();
	}



	// solving the model
	IloCplex mycplex(myenv);
	mycplex.extract(EV);
	mycplex.solve();
	
	// preparing fsSolution to return
	for (int itr = 0; itr < j; itr++) {
		fsSolution[itr] = mycplex.getValue(xj[itr]);
	}
	fsSolution[5] = mycplex.getObjValue();

	// clearing the environment and closing it
	mycplex.clear();
	myenv.end();




	return fsSolution;
}

