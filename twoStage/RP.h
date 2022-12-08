#include <iostream>
#include "ilcplex\cplex.h"
#include "ilcplex\ilocplex.h"
#include "data.h"
using namespace std;




float RPCalc() {

	// creating the environment
	IloEnv myenv;
	// creating the model
	IloModel RP(myenv);


	// creating x, y, z variables
	// as there is one mode dimension in this model, there are several more variables
	// I think I should check that how I can declare IloNumVarArray with mode than one dimension
	IloNumVarArray xj(myenv, j, 0, 1, ILOINT);

	IloNumVarArray y1j1(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y2j1(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y3j1(myenv, j, 0, 1, ILOFLOAT);


	IloNumVarArray y1j2(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y2j2(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y3j2(myenv, j, 0, 1, ILOFLOAT);


	IloNumVarArray y1j3(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y2j3(myenv, j, 0, 1, ILOFLOAT);
	IloNumVarArray y3j3(myenv, j, 0, 1, ILOFLOAT);

	IloNumVarArray z1j11(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j21(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j31(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j11(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j21(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j31(myenv, j, 0, IloInfinity, ILOFLOAT);

	IloNumVarArray z1j12(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j22(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j32(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j12(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j22(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j32(myenv, j, 0, IloInfinity, ILOFLOAT);

	IloNumVarArray z1j13(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j23(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z1j33(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j13(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j23(myenv, j, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray z2j33(myenv, j, 0, IloInfinity, ILOFLOAT);





	// adding the objective function to our model
	// we are adding the scenario probabilities to objective function
	// it is better to cast the values, but the model is working fine
	IloExpr RPobj(myenv);
	for (int itr = 0; itr < j; itr++) {
		RPobj += establishCost[itr] * xj[itr];

		RPobj += scProb[0] * shippingCost[0][itr][0] * z1j11[itr];
		RPobj += scProb[0] * shippingCost[0][itr][1] * z1j21[itr];
		RPobj += scProb[0] * shippingCost[0][itr][2] * z1j31[itr];
		RPobj += scProb[0] * shippingCost[1][itr][0] * z2j11[itr];
		RPobj += scProb[0] * shippingCost[1][itr][1] * z1j21[itr];
		RPobj += scProb[0] * shippingCost[1][itr][2] * z1j31[itr];

		RPobj += scProb[1] * shippingCost[0][itr][0] * z1j12[itr];
		RPobj += scProb[1] * shippingCost[0][itr][1] * z1j22[itr];
		RPobj += scProb[1] * shippingCost[0][itr][2] * z1j32[itr];
		RPobj += scProb[1] * shippingCost[1][itr][0] * z2j12[itr];
		RPobj += scProb[1] * shippingCost[1][itr][1] * z1j22[itr];
		RPobj += scProb[1] * shippingCost[1][itr][2] * z1j32[itr];

		RPobj += scProb[2] * shippingCost[0][itr][0] * z1j13[itr];
		RPobj += scProb[2] * shippingCost[0][itr][1] * z1j23[itr];
		RPobj += scProb[2] * shippingCost[0][itr][2] * z1j33[itr];
		RPobj += scProb[2] * shippingCost[1][itr][0] * z2j13[itr];
		RPobj += scProb[2] * shippingCost[1][itr][1] * z1j23[itr];
		RPobj += scProb[2] * shippingCost[1][itr][2] * z1j33[itr];
	}
	RP.add(IloMinimize(myenv, RPobj));
	// clearing data
	RPobj.end();





	// adding the first type of constraints to the model
	// these constraints, are identifying that the sum of goods flow from each factory to each distribution center to each retailer, 
	// doesnt exceed the capacity of factory
	// adding the scenario dimension, makes the model more complex
	// the constraints are written for each scenario
	IloExpr con111(myenv);
	for (int itr = 0; itr < j; itr++) {
		con111 += z1j11[itr] + z1j21[itr] + z1j31[itr];
	}
	RP.add(IloRange(myenv, -IloInfinity, con111, factoryCap[0]));
	con111.end();

	IloExpr con121(myenv);
	for (int itr = 0; itr < j; itr++) {
		con121 += z2j11[itr] + z2j21[itr] + z2j31[itr];
	}
	RP.add(IloRange(myenv, -IloInfinity, con121, factoryCap[0]));
	con121.end();

	IloExpr con112(myenv);
	for (int itr = 0; itr < j; itr++) {
		con112 += z1j12[itr] + z1j22[itr] + z1j32[itr];
	}
	RP.add(IloRange(myenv, -IloInfinity, con112, factoryCap[0]));
	con112.end();

	IloExpr con122(myenv);
	for (int itr = 0; itr < j; itr++) {
		con122 += z2j12[itr] + z2j22[itr] + z2j32[itr];
	}
	RP.add(IloRange(myenv, -IloInfinity, con122, factoryCap[0]));
	con122.end();

	IloExpr con113(myenv);
	for (int itr = 0; itr < j; itr++) {
		con113 += z1j13[itr] + z1j23[itr] + z1j33[itr];
	}
	RP.add(IloRange(myenv, -IloInfinity, con113, factoryCap[0]));
	con113.end();

	IloExpr con123(myenv);
	for (int itr = 0; itr < j; itr++) {
		con123 += z2j13[itr] + z2j23[itr] + z2j33[itr];
	}
	RP.add(IloRange(myenv, -IloInfinity, con123, factoryCap[0]));
	con123.end();






	// following constraints, identify that the total allocated goods to each retailer, must meet the retailer's demand
	// the constraints are written for each scenario
	IloExpr con211(myenv);
	for (int itr = 0; itr < j; itr++) {
		con211 += y1j1[itr];
	}
	RP.add(IloRange(myenv, 1, con211, 1));
	con211.end();

	IloExpr con221(myenv);
	for (int itr = 0; itr < j; itr++) {
		con221 += y2j1[itr];
	}
	RP.add(IloRange(myenv, 1, con221, 1));
	con221.end();

	IloExpr con231(myenv);
	for (int itr = 0; itr < j; itr++) {
		con231 += y3j1[itr];
	}
	RP.add(IloRange(myenv, 1, con231, 1));
	con231.end();

	IloExpr con212(myenv);
	for (int itr = 0; itr < j; itr++) {
		con212 += y1j2[itr];
	}
	RP.add(IloRange(myenv, 1, con212, 1));
	con212.end();

	IloExpr con222(myenv);
	for (int itr = 0; itr < j; itr++) {
		con222 += y2j2[itr];
	}
	RP.add(IloRange(myenv, 1, con222, 1));
	con222.end();

	IloExpr con232(myenv);
	for (int itr = 0; itr < j; itr++) {
		con232 += y3j2[itr];
	}
	RP.add(IloRange(myenv, 1, con232, 1));
	con232.end();

	IloExpr con213(myenv);
	for (int itr = 0; itr < j; itr++) {
		con213 += y1j3[itr];
	}
	RP.add(IloRange(myenv, 1, con213, 1));
	con213.end();

	IloExpr con223(myenv);
	for (int itr = 0; itr < j; itr++) {
		con223 += y2j3[itr];
	}
	RP.add(IloRange(myenv, 1, con223, 1));
	con223.end();

	IloExpr con233(myenv);
	for (int itr = 0; itr < j; itr++) {
		con233 += y3j3[itr];
	}
	RP.add(IloRange(myenv, 1, con233, 1));
	con233.end();





	// following constraints, identifies that the sum of the good flows from each facility, dont exceed the facilities 
	// capacity(if the facility is established)
	// the constraints are written for each scenario
	for (int itr = 0; itr < j; itr++) {
		IloExpr con311(myenv);
		con311 += scenarioDemand[0][0] * y1j1[itr] + scenarioDemand[0][1] * y2j1[itr] + scenarioDemand[0][2] * y3j1[itr] - xj[itr] * facilityCap[itr];
		RP.add(IloRange(myenv, -IloInfinity, con311, 0));
		con311.end();
	}

	for (int itr = 0; itr < j; itr++) {
		IloExpr con312(myenv);
		con312 += scenarioDemand[1][0] * y1j2[itr] + scenarioDemand[1][1] * y2j2[itr] + scenarioDemand[1][2] * y3j2[itr] - xj[itr] * facilityCap[itr];
		RP.add(IloRange(myenv, -IloInfinity, con312, 0));
		con312.end();
	}

	for (int itr = 0; itr < j; itr++) {
		IloExpr con313(myenv);
		con313 += scenarioDemand[2][0] * y1j3[itr] + scenarioDemand[2][1] * y2j3[itr] + scenarioDemand[2][2] * y3j3[itr] - xj[itr] * facilityCap[itr];
		RP.add(IloRange(myenv, -IloInfinity, con313, 0));
		con313.end();
	}






	// these constraints are identifying that for each facility to each retailer, the sum of good flows from companies is equal to the 
	// total goods that are allocated to retailer from the facility
	// the constraints are written for each scenario
	for (int itr = 0; itr < j; itr++) {
		IloExpr con411(myenv);
		con411 += z1j11[itr] + z2j11[itr] - scenarioDemand[0][0] * y1j1[itr];
		RP.add(IloRange(myenv, 0, con411, 0));
		con411.end();

		IloExpr con421(myenv);
		con421 += z1j21[itr] + z2j21[itr] - scenarioDemand[0][1] * y2j1[itr];
		RP.add(IloRange(myenv, 0, con421, 0));
		con421.end();

		IloExpr con431(myenv);
		con431 += z1j31[itr] + z2j31[itr] - scenarioDemand[0][2] * y3j1[itr];
		RP.add(IloRange(myenv, 0, con431, 0));
		con431.end();
	}

	for (int itr = 0; itr < j; itr++) {
		IloExpr con412(myenv);
		con412 += z1j12[itr] + z2j12[itr] - scenarioDemand[1][0] * y1j2[itr];
		RP.add(IloRange(myenv, 0, con412, 0));
		con412.end();

		IloExpr con422(myenv);
		con422 += z1j22[itr] + z2j22[itr] - scenarioDemand[1][1] * y2j2[itr];
		RP.add(IloRange(myenv, 0, con422, 0));
		con422.end();

		IloExpr con432(myenv);
		con432 += z1j32[itr] + z2j32[itr] - scenarioDemand[1][2] * y3j2[itr];
		RP.add(IloRange(myenv, 0, con432, 0));
		con432.end();
	}

	for (int itr = 0; itr < j; itr++) {
		IloExpr con413(myenv);
		con413 += z1j13[itr] + z2j13[itr] - scenarioDemand[2][0] * y1j3[itr];
		RP.add(IloRange(myenv, 0, con413, 0));
		con413.end();

		IloExpr con423(myenv);
		con423 += z1j23[itr] + z2j23[itr] - scenarioDemand[2][1] * y2j3[itr];
		RP.add(IloRange(myenv, 0, con423, 0));
		con423.end();

		IloExpr con433(myenv);
		con433 += z1j33[itr] + z2j33[itr] - scenarioDemand[2][2] * y3j3[itr];
		RP.add(IloRange(myenv, 0, con433, 0));
		con433.end();
	}


	// solving the model
	IloCplex mycplex(myenv);
	mycplex.extract(RP);
	mycplex.solve();

	// getting the optimized value of objective function
	const float objVal = mycplex.getObjValue();

	// clearing the environment and closing it
	mycplex.clear();
	myenv.end();



	return objVal;
}

