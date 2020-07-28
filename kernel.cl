/*!
* kernel.cl
* Copyright (c) 2020 Asaki Kameda
* Released under GNU GPLv3.
* see LICENSE.txt
*/
//For OpenCL version v2.2.0(2020.03.04)

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

double generationF(int t, int Center, int Speed, int zCenter, const int s, const int z, int Nz, double Hd, double Hs, double Hz, double Ht, double Coe, double Radius, double Radius2, double Radius3, double Epsilon, double Epsilon2, double Epsilon3, double Power){
	const double y = Hs*Center + Ht*t*Speed;
	const double r1 = (y - Hs*s);
	const double r2 = (zCenter - z)*Hz;
	const double pi_half = 0.5 * M_1_PI;
	const double radius_ml2 = Radius*Radius;
	const double radius2_ml2 = Radius2*Radius2;
	const double radius3_ml2 = Radius3*Radius3;
	const double coe_ml2 = Coe*Coe;
	const double r1_ml2 = r1*r1;
	const double r2_ml2 = r2*r2;
	const double gaussian2d1 = (pi_half/(Coe*radius_ml2))*exp(-((r2_ml2)/(2*radius_ml2))-((r1_ml2) / (2*(coe_ml2*radius_ml2))));
	const double gaussian2d2 = (pi_half/(Coe*radius2_ml2))*exp(-((r2_ml2)/(2*radius2_ml2))-((r1_ml2) / (2*(coe_ml2*radius2_ml2))));
	const double gaussian2d3 = (pi_half/(Coe*radius3_ml2))*exp(-((r2_ml2)/(2*radius3_ml2))-((r1_ml2) / (2*(coe_ml2*radius3_ml2))));
	
	return (Epsilon * Power * gaussian2d1 + Epsilon2 * Power * gaussian2d2 + Epsilon3 * Power * gaussian2d3) / Hd;
	//return (Epsilon * Power * gaussian2d1 + Epsilon2 * Power * gaussian2d2)/Hd;
}

__kernel void distribution(__global double *distribution, __global double *old_distribution, int Nd,  int Ns, int Nz, double Hd, double Hs, double Hz, double Ht, int t, int Center, double Speed, int zCenter, double Coe, double Radius, double Radius2, double Radius3, double Epsilon, double Epsilon2,  double Epsilon3, double Power, double Rad_coefA, double Rad_coefB,double Rad_coefC, double Rad_min, double Rad_Supp, double wa, double wb, double wbsupp, double wc, double SuppCenter, double Tinit) {

	const int i = get_global_id(0);
	const int s = get_global_id(1);
	const int z = get_global_id(2);
	const int lookinat = Ns*Nz*i+s*Nz+z;
	const int nsnz = Ns*Nz;
	const double old_dist = old_distribution[lookinat];
	const double kakusanritsu = -956.32+15.581*pow(old_dist, 1 )-0.092135*pow(old_dist, 2 )+0.00029033*pow(old_dist, 3 )-5.5339e-007*pow(old_dist, 4 )+6.7097e-010*pow(old_dist, 5 )-5.2203e-013*pow(old_dist, 6 )+2.5289e-016*pow(old_dist, 7 )-6.9551e-020*pow(old_dist, 8 )+8.3044e-024*pow(old_dist, 9 );
//7.5405 + 3750100 * pow(old_dist,-1.8928);
	const double lambda_tokusei = -2.6152+0.03737*pow(old_dist, 1 )-0.0002062*pow(old_dist, 2 )+6.172e-007*pow(old_dist, 3 )-1.1256e-009*pow(old_dist,  4)+1.3103e-012*pow(old_dist, 5 )-9.8034e-016*pow(old_dist, 6 )+4.572e-019*pow(old_dist, 7 )-1.2117e-022*pow(old_dist, 8 )+1.396e-026*pow(old_dist, 9 );
//0.026222 + 0.47161 * exp(-0.0046004 * old_dist);

	const double Fd = kakusanritsu*Ht/(Hd*Hd);
	const double Fs = kakusanritsu*Ht/(Hs*Hs);
	const double Fz = kakusanritsu*Ht/(Hz*Hz);
	
	const double y = Hs*Center + Ht*t*Speed;
	const double r1 = (y - Hs*s);
	const double r2 = (zCenter - z)*Hz;
	const double distance_2 = r1*r1+r2*r2;
	const double wa_2 = wa*wa*2;
	const double wb_2 = wb*wb*2;
	const double wbsupp_2 = wbsupp*wbsupp*2;
	const double wc_2 = wc*wc*2;
	const double Radiation = Rad_coefA*exp(- distance_2/wa_2) + Rad_coefB*exp(- distance_2/wb_2) + Rad_coefC*exp(- distance_2/wc_2) + Rad_min + Rad_Supp*exp(- (sqrt(distance_2)-SuppCenter)*(sqrt(distance_2)-SuppCenter)/wbsupp_2);
	const double New_BRadiation = Rad_coefC*exp(- distance_2/wc_2) + Rad_min;
	
	double dist;
	dist = (1-2*(Fd+Fs+Fz))*old_dist;
	
	if (i==0){
		dist += Fd * (old_dist + old_distribution[lookinat + nsnz]);
		dist += Ht * (kakusanritsu/lambda_tokusei)*generationF(t, Center,  Speed, zCenter, s, z, Nz, Hd, Hs, Hz, Ht, Coe, Radius, Radius2, Radius3, Epsilon, Epsilon2, Epsilon3, Power);
		dist -= 1e-6 * Radiation * (old_dist - Tinit) * kakusanritsu * Ht / (lambda_tokusei * Hd);
	}
	else if (i==Nd-1){
		dist += Fd * (old_dist + old_distribution[lookinat - nsnz]);
		dist -= 1e-6 * New_BRadiation * (old_dist - Tinit) * kakusanritsu * Ht / (lambda_tokusei * Hd);
	}
	else{
		dist += Fd * (old_distribution[lookinat - nsnz] + old_distribution[lookinat + nsnz]);
	}
	
	if (s==0){
		dist += Fs * (old_dist + old_distribution[lookinat + Nz]);
	}
	else if (s==Ns-1){
		dist += Fs * (old_dist + old_distribution[lookinat - Nz]);
	}
	else{
		dist += Fs * (old_distribution[lookinat + Nz] + old_distribution[lookinat - Nz]);
	}

	if (z==0){
		dist += Fz * (old_dist + old_distribution[lookinat + 1]);
	}
	else if (z==Nz-1){
		dist += Fz * (old_dist + old_distribution[lookinat - 1]);
	}
	else{
		dist += Fz * (old_distribution[lookinat - 1] + old_distribution[lookinat + 1]);
	}
	
	distribution[lookinat] = dist;

}


__kernel void sync_dist(__global double *distribution, __global double *old_distribution, int Nd,  int Ns, int Nz){
	const int i = get_global_id(0);
	const int s = get_global_id(1);
	const int z = get_global_id(2);
	const int lookinat = Ns*Nz*i+s*Nz+z;

	old_distribution[lookinat]  = distribution[lookinat];

}

__kernel void Store1D_k(__global double *distribution, __global double *Store1DSeries, int Nd, int Ns, int Nz, int t, int Delta_t, int Choosed_s, int zCenter){
	const int i = get_global_id(0);
	const int lookinat = Ns*Nz*i+Choosed_s*Nz+zCenter;
	
	Store1DSeries[Nd*(t/Delta_t)+i] = distribution[lookinat];//
}


__kernel void Store2D_k(__global double *distribution, __global double *Store2DSeries, int Ns, int Nz, int t, int Surface_Delta_t){
	const int s = get_global_id(0);
	const int z = get_global_id(1);
	const int lookinat = Nz*s+z;
	
	Store2DSeries[lookinat+Ns*Nz*(t/Surface_Delta_t)] = distribution[lookinat];
}

__kernel void Store4D_k(__global double *distribution, __global double *Store4DSeries, int Nd, int Ns, int Nz, int t, int Delta_t_4D){
	const int i = get_global_id(0);
	const int s = get_global_id(1);
	const int z = get_global_id(2);
	const int lookinat = Ns*Nz*i+s*Nz+z;
	
	Store4DSeries[lookinat+Nd*Ns*Nz*(t/Delta_t_4D)] = distribution[lookinat];
}