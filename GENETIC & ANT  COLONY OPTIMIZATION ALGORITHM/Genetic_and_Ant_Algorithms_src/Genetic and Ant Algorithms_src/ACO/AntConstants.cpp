#include "stdafx.h"
#include "AntConstants.h"
namespace ACO {

	CAntConstants::CAntConstants(int CityNo,int AntNo,int Alpha,int Beta,double Rho)
	{
		this->MAX_ANTS=8;
		this->MAX_CITIES = 40;
		this->ALPHA=Alpha;
		this->BETA=Beta;
		this->RHO=Rho;
		this->VM= vmAll;
		this->PM=pmCircular;		
	}
 CAntConstants gAntConstants;
}//namespace