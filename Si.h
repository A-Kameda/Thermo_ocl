/*!
* thermo.cpp
* Copyright (c) 2020 Asaki Kameda
* Released under GNU GPLv3.
* see LICENSE.txt
*/

double thermo::kakusanritsu(double temperature) {
	//The material's temperature dependency of thermal diffusivity
	//You need to return that value simply.
}

double thermo::lambda_tokusei(double temperature) {
	//The material's temperature dependency of thermal conductivity
	//You need to return that value simply.
}

complex<double> thermo::refractive_index(double temperature){
	//The material's temperature dependency of complex refractive index
	//You need to return that value in complex<double>.
	//N=n-i*k
}

double thermo::thickness_expansion(double temperature){
	//The material's coefficient of thermal expansion
	//You need to return that value simply.
	//If you do not consider it, return 1.
}
