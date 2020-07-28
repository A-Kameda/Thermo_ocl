/*!
* thermo_ocl.cpp
* Copyright (c) 2020 Asaki Kameda
* Released under GNU GPLv3.
* see LICENSE.txt
*/
#define THERMO_VER "Thermo3D OpenCL v2.2.0 by Kameda"
#define CL_USE_DEPRECATED_opencl_1_2_APIS

#include <iostream>
#include <iomanip>
#include <thread>
#include <future>
#include <string> //for string type
#include <cstring> //for strcpy
#include <fstream> //for file operation
#include <cstdio> //for sscanf
#include <cstdlib> //for exit
#include <vector>
#include <complex>
#include <time.h> //for time
#include <math.h>
#include <CL/cl.h>
#include <unistd.h>//for sleep
#include <chrono>
using namespace std;
#include "iniparser.hpp"
#include "csv.h"
#include "thermo.cpp"

//以下どれかを選択
//#include "quartz.h"
#include "Si.h"
//#include "SiC.h"
//#include "eagle2000.h"

void progressbar(int &ii, int &Nt){
	int i=0,j,progress;
	chrono::steady_clock::time_point start_t, stop_t,bench_start, bench_stop;
	bench_start = chrono::steady_clock::now() - chrono::milliseconds(1);
	start_t = chrono::steady_clock::now();
	
	cout << "0%	   50%	   100%  Current i | i / sec |	Time left" << endl;
	cout << "+---------+---------+" << endl;
	while(i == 0){
		i=ii;
		cout << "";
	}
	int leftsec=0,tmp=0;
	while(i < Nt){
		progress=i*20/Nt;
		bench_stop = chrono::steady_clock::now();
		cout << "\r";
		for(j=0; j<=progress; j++){ cout << "#"; }
		for(j=progress; j<21; j++){ cout << " "; }
		int outpersec = 1000*(i-tmp)/chrono::duration_cast<chrono::milliseconds>(bench_stop - bench_start).count();
		if(outpersec != 0){
			leftsec = (Nt-i)/(outpersec);
		}else{
			leftsec -=1;
		}
		cout << "	" << setw(10) << right << i << " | " << setw(7) << outpersec << " |	" << right << leftsec << " sec		" << flush;
		bench_start = chrono::steady_clock::now();
		this_thread::sleep_for(chrono::milliseconds(1000));
		tmp = i;
		i=ii;
	}
	//progressbar finish[
	cout << "\r";
	stop_t = chrono::steady_clock::now();
	for(j=0; j<21; j++){ cout << "#"; }
	cout << "	" << setw(12) << right << Nt << " : " << setw(7) << 1000*Nt/chrono::duration_cast<std::chrono::milliseconds>(stop_t-start_t).count() << " | " << 0 << " sec left	" << endl;
}

int main(int argc, char *argv[]){
	int i=0;
	thermo Thermo(argc,argv);
	chrono::steady_clock::time_point start_t, stop_t;

	Thermo.getini(); //設定ファイルからの読込
	Thermo.make_fname();
	Thermo.check_ini(cout, true); //設定値の確認	
	//initialize_ocl
	Thermo.initializeCL();
	Thermo.resize(); //配列確保・イニシャライズ
	Thermo.initializeKernel();
	Thermo.initializeData(0);
	
	Thermo.check_stability(300); //安定条件確認
	Thermo.check_stability(1000); //安定条件確認
	Thermo.check_stability(2000); //安定条件確認

	if (Thermo.OutProfile){
		Thermo.out_profile(); //TPJプロファイル出力
	}
	if (Thermo.OutCoef){
		Thermo.out_coef(); //局所熱伝達係数出力
	}
	
	cout << "\n";
	cout << "Program started\n";
	start_t = chrono::steady_clock::now();
	auto th_progressbar = std::async(std::launch::async, progressbar, std::ref(i), std::ref(Thermo.Nt));//start progress bar
//*****************************************************
	for (i=0; i<=Thermo.Nt; i++){
		Thermo.setkernelargs(i);
		Thermo.runKernel();
		
		if (i%Thermo.Delta_t == 0){
			Thermo.Store1D();
		}
		if(Thermo.Out4d||Thermo.OutRef2D){
			if (i%Thermo.Delta_t_4D == 0){
				Thermo.Store4D();
			}
		}
		if(Thermo.Out2dSurface){
			if (i%Thermo.Surface_Delta_t == 0){
				Thermo.Store2D();
			}
		}
	}
	th_progressbar.wait();//wait progress bar
	//cout << "finished!! press any key" << endl;
	//getchar();
	Thermo.ReadBuffer();
	
	if (Thermo.Out4d){
		Thermo.out_dist_head(); //4Dヘッダ
		Thermo.out_dist();
		Thermo.out_dist_foot(); //4Dフッタ
	}
	if (Thermo.Out2dSurface){
		Thermo.out_dist_surface_head(); //2D-Depthヘッダ
		Thermo.out_dist_surface();
		Thermo.out_dist_surface_foot(); //2D-Surfaceフッタ
	}
	if (Thermo.OutRef2D){
		Thermo.out_reflectivity_2D_head(); //2D-反射率ヘッダ
		Thermo.out_reflectivity_2D();
		Thermo.out_reflectivity_2D_foot(); //2D-反射率フッタ
	}
	if(Thermo.OutPeaks){
		Thermo.opt_thickness_peakfind();//反射率山-谷検出
	}
	Thermo.trace();
	Thermo.out_calc(); //1D出力
	if (Thermo.OutCSV){
		Thermo.out_calc_csv(); //1D出力
	}

	Thermo.cleanup();
//*****************************************************
	stop_t = chrono::steady_clock::now();
	using subsec = chrono::duration<double, std::ratio<1,1>>;
	using submin = chrono::duration<double, std::ratio<60,1>>;
	cout << "Program Finished \t" << chrono::duration_cast<subsec>(stop_t-start_t).count() << " seconds / " << chrono::duration_cast<submin>(stop_t-start_t).count() << " minutes\n";
	return 0;
}
