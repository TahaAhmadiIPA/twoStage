#include <iostream>
#include "ilcplex\cplex.h"
#include "ilcplex\ilocplex.h"
#include "data.h"
using namespace std;




float EEVCalc(int fsSolution[]) {

	// we store the objective function's value in EEVValue variable
	float EEVValue = 0;


	// the model is going to be solved for each scenario separately
	// first stage variables(x variables) will be fixed equal to EV solutions x variables
	for (int s=0; s < sc; s++) {

		// creating the environment
		IloEnv myenv;
		// creating the model
		IloModel EEV(myenv);


		// creating x, y, z variables
		IloNumVarArray xj(myenv, j, 0, 1, ILOINT);

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
		IloExpr EEVobj(myenv);
		for (int itr = 0; itr < j; itr++) {
			EEVobj += establishCost[itr] * xj[itr];
			EEVobj += shippingCost[0][itr][0] * z1j1[itr];
			EEVobj += shippingCost[0][itr][1] * z1j2[itr];
			EEVobj += shippingCost[0][itr][2] * z1j3[itr];
			EEVobj += shippingCost[1][itr][0] * z2j1[itr];
			EEVobj += shippingCost[1][itr][1] * z1j2[itr];
			EEVobj += shippingCost[1][itr][2] * z1j3[itr];
		}
		EEV.add(IloMinimize(myenv, EEVobj));
		// clearing data
		EEVobj.end();




		// adding the first type of constraints to the model
		// these constraints, are identifying that the sum of goods flow from each factory to each distribution center to each retailer, 
		// doesnt exceed the capacity of factory
		IloExpr con11(myenv);
		for (int itr = 0; itr < j; itr++) {
			con11 += z1j1[itr] + z1j2[itr] + z1j3[itr];
		}
		EEV.add(IloRange(myenv, -IloInfinity, con11, factoryCap[0]));
		con11.end();

		IloExpr con12(myenv);
		for (int itr = 0; itr < j; itr++) {
			con12 += z2j1[itr] + z2j2[itr] + z2j3[itr];
		}
		EEV.add(IloRange(myenv, -IloInfinity, con12, factoryCap[0]));
		con12.end();





		// following constraints, identify that the total allocated goods to each retailer, must meet the retailer's demand
		IloExpr con21(myenv);
		for (int itr = 0; itr < j; itr++) {
			con21 += y1j[itr];
		}
		EEV.add(IloRange(myenv, 1, con21, 1));
		con21.end();

		IloExpr con22(myenv);
		for (int itr = 0; itr < j; itr++) {
			con22 += y2j[itr];
		}
		EEV.add(IloRange(myenv, 1, con22, 1));
		con22.end();

		IloExpr con23(myenv);
		for (int itr = 0; itr < j; itr++) {
			con23 += y3j[itr];
		}
		EEV.add(IloRange(myenv, 1, con23, 1));
		con23.end();





		// following constraints, identifies that the sum of the good flows from each facility, dont exceed the facilities 
		// capacity(if the facility is established)
		for (int itr = 0; itr < j; itr++) {
			IloExpr con31(myenv);
			con31 += scenarioDemand[s][0] * y1j[itr] + scenarioDemand[s][1] * y2j[itr] + scenarioDemand[s][2] * y3j[itr] - xj[itr] * facilityCap[itr];
			EEV.add(IloRange(myenv, -IloInfinity, con31, 0));
			con31.end();
		}





		// these constraints are identifying that for each facility to each retailer, the sum of good flows from companies is equal to the 
		// total goods that are allocated to retailer from the facility
		for (int itr = 0; itr < j; itr++) {
			IloExpr con41(myenv);
			con41 += z1j1[itr] + z2j1[itr] - scenarioDemand[s][0] * y1j[itr];
			EEV.add(IloRange(myenv, 0, con41, 0));
			con41.end();

			IloExpr con42(myenv);
			con42 += z1j2[itr] + z2j2[itr] - scenarioDemand[s][1] * y2j[itr];
			EEV.add(IloRange(myenv, 0, con42, 0));
			con42.end();

			IloExpr con43(myenv);
			con43 += z1j3[itr] + z2j3[itr] - scenarioDemand[s][2] * y3j[itr];
			EEV.add(IloRange(myenv, 0, con43, 0));
			con43.end();
		}



		// fixing the x variables
		for (int itr = 0; itr < j; itr++) {
			IloExpr fixX(myenv);
			fixX += xj[itr];
			EEV.add(IloRange(myenv, fsSolution[itr], fixX, fsSolution[itr]));
			fixX.end();
		}





		// solving the model
		IloCplex mycplex(myenv);
		mycplex.extract(EEV);
		mycplex.solve();

		// updating EEVValue
		EEVValue += scProb[s] * mycplex.getObjValue();

		// clearing the environment and closing it
		mycplex.clear();
		myenv.end();
	}



	return EEVValue;
}

