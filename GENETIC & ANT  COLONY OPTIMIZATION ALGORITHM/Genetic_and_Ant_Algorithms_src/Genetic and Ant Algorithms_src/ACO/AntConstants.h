#pragma once
#include "../Utils.h"
namespace ACO
{
#ifndef __ANTCONST_H                                               
#define __ANTCONST_H

//-----------------------------------

class CAntConstants
{
public:
	CAntConstants(int CityNo=40,int AntNo=10,int Alpha=1,int Beta=1,double Rho=0.6);
	int MAX_ANTS;
	int MAX_CITIES;
	int ALPHA;
	int BETA;
	double RHO;
	ViewMode VM;
	PlotMode PM;	
};

//-----------------------------------

#endif
}//namespace
