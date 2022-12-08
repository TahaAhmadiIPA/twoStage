#include <iostream>
#ifndef data_H
#define data_H


using namespace std;



extern int establishCost[5] = { 24778, 31243, 19788, 22421, 34333 }; // establishment cost of each candidated distribution center
extern int factoryCap[2] = { 15000, 10000 }; // capacity of each factory to produce goods
extern int facilityCap[5] = { 4000, 8000, 5000, 7500, 7500 }; // capacity of each candidated distribution center
extern int meanDemand[3] = { 2000, 4400, 4050 }; // expected value of demands
extern float scProb[3] = { 0.25, 0.25, 0.50 }; // occurance probability of each scenario

extern int scenarioDemand[3][3] = {
	{ 1700, 4300, 3600 },
	{ 1900, 5200, 4200 },
	{ 2100, 4700, 4000 },
}; // demands in each scenario. values are not generated from simulatio

extern int shippingCost[2][5][3] = {
	{
		{ 12, 24, 22 }, { 23, 11, 42 }, { 27, 28, 7 }, { 14, 32, 6 }, { 14, 23, 23 }
	},
	{
		{ 17, 31, 12}, { 22, 11, 9}, { 31, 7, 8 }, { 21, 12, 23 }, { 22, 19, 21 }
	}
}; // shipping cost from factory to distribution center to retailers


extern const int j = 5; //number of candidated loactions to establish a distribution center
extern const int i = 3; // number of retailers
extern const int k = 2; // number of factories
extern const int sc = 3; // number of scenarios


#endif