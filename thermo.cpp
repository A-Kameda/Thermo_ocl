/*!
* thermo.cpp
* Copyright (c) 2020 Asaki Kameda
* Released under GNU GPLv3.
* see LICENSE.txt
*/
//For OpenCL version v2.2.0(2020.03.04)
#include "thermo.h"

thermo::thermo(const int &argc, char *argv[]){
	cout << scientific;
	cout << "Program Version:" << THERMO_VER << "\n";
	
	if(argc != 2){
		cout << "Please specify the ini_file!\n";
		exit(1);
	}
	
	string inifile = argv[1];
	cout << "INI filename : " << inifile << endl;
	if(f_ini.Load(inifile) == 0){
		cout << "Loading ini file is failed" << endl;
	}
}

bool thermo::lerror(string str1, string str2){
	cout << "Error in loading Section:[" << str1 << "], " << str2 << endl;
	return true;
}

int thermo::getini(){
	//iniファイルから変数の読込
	bool error = false;
	
	//[SYSTEM] section
	if( "\0" == (thermo_version = f_ini.GetSection("SYSTEM")->GetValue("THERMO_VER","\0").AsString()) ){ error = lerror("SYSTEM","THERMO_VER");}
	if(thermo_version != THERMO_VER){
		cout << "Please declare THERMO_VER on ini [SYSTEM] section for future lab members" << endl;
		cout << "THERMO_VER = " << THERMO_VER << endl;
		error = true;
	}
	if( "\0" == (DataFile = f_ini.GetSection("SYSTEM")->GetValue("DataFile","\0").AsString()) ){ error = lerror("SYSTEM","DataFile");}
	if( -1 == (Selected_Vendor = f_ini.GetSection("SYSTEM")->GetValue("Selected_Vendor", -1 ).AsInt()) ){ error = lerror("SYSTEM","Selected_Vendor");}
	if( -1 == (ld = static_cast<size_t>(f_ini.GetSection("SYSTEM")->GetValue("ld", -1 ).AsInt())) ){ error = lerror("SYSTEM","ld");}
	if( -1 == (ls = static_cast<size_t>(f_ini.GetSection("SYSTEM")->GetValue("ls", -1 ).AsInt())) ){ error = lerror("SYSTEM","ls");}
	if( -1 == (lz = static_cast<size_t>(f_ini.GetSection("SYSTEM")->GetValue("lz", -1 ).AsInt())) ){ error = lerror("SYSTEM","lz");}
	
	//[OUTPUT] section
	if( -1 == (Out4d = f_ini.GetSection("OUTPUT")->GetValue("Out4d", -1 ).AsInt()) ){ error = lerror("OUTPUT","Out4d");}
	if( -1 == (Out2dSurface = f_ini.GetSection("OUTPUT")->GetValue("Out2dSurface", -1 ).AsInt()) ){ error = lerror("OUTPUT","Out2dSurface");}
	if( -1 == (CalcRef = f_ini.GetSection("OUTPUT")->GetValue("CalcRef", -1 ).AsInt()) ){ error = lerror("OUTPUT","CalcRef");}
	if( -1 == (CalcRef_i = f_ini.GetSection("OUTPUT")->GetValue("CalcRef_i", -1 ).AsInt()) ){ error = lerror("OUTPUT","CalcRef_i");}
	if( -1 == (OutCSV = f_ini.GetSection("OUTPUT")->GetValue("OutCSV", -1 ).AsInt()) ){ error = lerror("OUTPUT","OutCSV");}
	if( -1 == (OutProfile = f_ini.GetSection("OUTPUT")->GetValue("OutProfile", -1 ).AsInt()) ){ error = lerror("OUTPUT","OutProfile");}
	if( -1 == (OutCoef = f_ini.GetSection("OUTPUT")->GetValue("OutCoef", -1 ).AsInt()) ){ error = lerror("OUTPUT","OutCoef");}
	if( -1 == (OutPeaks = f_ini.GetSection("OUTPUT")->GetValue("OutPeaks", -1 ).AsInt()) ){ error = lerror("OUTPUT","OutPeaks");}
	if( -1 == (Out2dSurface = f_ini.GetSection("OUTPUT")->GetValue("Out2dSurface", -1 ).AsInt()) ){ error = lerror("OUTPUT","Out2dSurface");}
	if( -1 == (Out4d = f_ini.GetSection("OUTPUT")->GetValue("Out4d", -1 ).AsInt()) ){ error = lerror("OUTPUT","Out4d");}
	if( -1 == (OutRef2D = f_ini.GetSection("OUTPUT")->GetValue("OutRef2D", -1 ).AsInt()) ){ error = lerror("OUTPUT","OutRef2D");}
	
	//[FDM_PARAMS] section
	if( -1 == (Nt = f_ini.GetSection("FDM_PARAMS")->GetValue("Nt", -1 ).AsInt()) ){ error = lerror("FDM_PARAMS","Nt");}
	if( -1 == (Nd = f_ini.GetSection("FDM_PARAMS")->GetValue("Nd", -1 ).AsInt()) ){ error = lerror("FDM_PARAMS","Nd");}
	if( -1 == (Ns = f_ini.GetSection("FDM_PARAMS")->GetValue("Ns", -1 ).AsInt()) ){ error = lerror("FDM_PARAMS","Ns");}
	if( -1 == (Nz = f_ini.GetSection("FDM_PARAMS")->GetValue("Nz", -1 ).AsInt()) ){ error = lerror("FDM_PARAMS","Nz");}
	if( -1 == (Ht = f_ini.GetSection("FDM_PARAMS")->GetValue("Ht", -1 ).AsDouble()) ){ error = lerror("FDM_PARAMS","Ht");}
	if( -1 == (Hd = f_ini.GetSection("FDM_PARAMS")->GetValue("Hd", -1 ).AsDouble()) ){ error = lerror("FDM_PARAMS","Hd");}
	if( -1 == (Hs = f_ini.GetSection("FDM_PARAMS")->GetValue("Hs", -1 ).AsDouble()) ){ error = lerror("FDM_PARAMS","Hs");}
	if( -1 == (Hz = f_ini.GetSection("FDM_PARAMS")->GetValue("Hz", -1 ).AsDouble()) ){ error = lerror("FDM_PARAMS","Hz");}
	
	//[OUTPUT_CONIFIG] section
	if( -1 == (Delta_t = f_ini.GetSection("OUTPUT_CONIFIG")->GetValue("Delta_t", -1 ).AsInt()) ){ error = lerror("OUTPUT_CONIFIG","Delta_t");}
	if( -1 == (Choosed_s = f_ini.GetSection("OUTPUT_CONIFIG")->GetValue("Choosed_s", -1 ).AsInt()) ){ error = lerror("OUTPUT_CONIFIG","Choosed_s");}
	if( -1 == (zCenter = f_ini.GetSection("OUTPUT_CONIFIG")->GetValue("zCenter", -1 ).AsInt()) ){ error = lerror("OUTPUT_CONIFIG","zCenter");}
	if( -1 == (Surface_Delta_t = f_ini.GetSection("OUTPUT_CONIFIG")->GetValue("Surface_Delta_t", -1 ).AsInt()) ){ error = lerror("OUTPUT_CONIFIG","Surface_Delta_t");}
	if( -1 == (Delta_t_4D = f_ini.GetSection("OUTPUT_CONIFIG")->GetValue("Delta_t_4D", -1 ).AsInt()) ){ error = lerror("OUTPUT_CONIFIG","Delta_t_4D");}
	
	//[THERMAL_CONFIG] section
	if( -1 == (Tinit = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Tinit", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Tinit");}
	if( -1 == (Power = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Power", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Power");}
	if( -1e10 == (Center = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Center", -1e10 ).AsInt()) ){ error = lerror("THERMAL_CONFIG","Center");}
	if( -1 == (Speed = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Speed", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Speed");}
	if( -1 == (Radius = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radius", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radius");}
	if( -1 == (Radius2 = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radius2", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radius2");}
	if( -1 == (Radius3 = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radius3", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radius3");}
	if( -1 == (Epsilon = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Epsilon", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Epsilon");}
	if( -1 == (Epsilon2 = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Epsilon2", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Epsilon2");}
	if( -1 == (Epsilon3 = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Epsilon3", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Epsilon3");}
	if( -1 == (Coe = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Coe", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Coe");}
	if( -1 == (Radiation_max = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radiation_max", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radiation_max");}
	if( -1 == (Radiation_mid = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radiation_mid", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radiation_mid");}
	if( -1 == (Radiation_min = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radiation_min", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radiation_min");}
	if( -1 == (Radiation_supp = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radiation_supp", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radiation_supp");}
	if( -1 == (Radiation_rate = f_ini.GetSection("THERMAL_CONFIG")->GetValue("Radiation_rate", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","Radiation_rate");}
	if( -1 == (wa = f_ini.GetSection("THERMAL_CONFIG")->GetValue("wa", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","wa");}
	if( -1 == (wb = f_ini.GetSection("THERMAL_CONFIG")->GetValue("wb", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","wb");}
	if( -1 == (wc = f_ini.GetSection("THERMAL_CONFIG")->GetValue("wc", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","wc");}
	if( -1 == (SuppCenter = f_ini.GetSection("THERMAL_CONFIG")->GetValue("SuppCenter", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","SuppCenter");}
	if( -1 == (wbsupp = f_ini.GetSection("THERMAL_CONFIG")->GetValue("wbsupp", -1 ).AsDouble()) ){ error = lerror("THERMAL_CONFIG","wbsupp");}
	
	//[REFRACTIVE_INDEX] section
	if( -1 == (WaveLength = f_ini.GetSection("REFRACTIVE_INDEX")->GetValue("WaveLength", -1 ).AsDouble()) ){ error = lerror("REFRACTIVE_INDEX","WaveLength");}
	if( -1 == (Ninit = f_ini.GetSection("REFRACTIVE_INDEX")->GetValue("Ninit", -1 ).AsDouble()) ){ error = lerror("REFRACTIVE_INDEX","Ninit");}
	if( 1e100 == (Dn_dT = f_ini.GetSection("REFRACTIVE_INDEX")->GetValue("Dn_dT", 1e100 ).AsDouble()) ){ error = lerror("REFRACTIVE_INDEX","Dn_dT");}
	if( 1e100 == (D2n_dT2 = f_ini.GetSection("REFRACTIVE_INDEX")->GetValue("D2n_dT2", 1e100 ).AsDouble()) ){ error = lerror("REFRACTIVE_INDEX","D2n_dT2");}
	
	//[OTHERS] section
	if( -1 == (RegardExpansion = static_cast<size_t>(f_ini.GetSection("OTHERS")->GetValue("RegardExpansion", -1 ).AsInt())) ){ error = lerror("OTHERS","RegardExpansion");}
	
	if(Nd%ld !=0){
		cout << "Error: ld needs to be a number that can divide Nd." << endl;
		error = true;
	}
	if(Ns%ls !=0){
		cout << "Error: ls needs to be a number that can divide Ns." << endl;
		error = true;
	}
	if(Nz%lz !=0){
		cout << "Error: lz needs to be a number that can divide Nz." << endl;
		error = true;
	}
	
	if(Choosed_s < 0|| Choosed_s >= Ns){
		cout << "Error: Choosed_s needs to be between 0 to Ns-1." << endl;
		error = true;
	}
	if(zCenter < 0|| zCenter >= Nz){
		cout << "Error: zCenter needs to be between 0 to Nz-1." << endl;
		error = true;
	}
	
	if(error == true){
		exit(EXIT_FAILURE);
	}
	
	local_item_size = {ld,ls,lz};
	t_pnts = Nt/Delta_t + 1;
	t_pnts_surface = Nt/(Surface_Delta_t) + 1;
	t_pnts_4D = Nt/(Delta_t_4D) + 1;
	cout << "Output points : " << t_pnts << "\n";
	
	return 0;
}

int thermo::make_fname(){
	vector<string> custom_filename_key;
	INI::Array ar = f_ini.GetSection("SYSTEM")->GetValue("Custom_filename").AsArray();
	for(size_t i = 0; i < ar.Size(); i++) {custom_filename_key.push_back(ar[i].AsString());}
	
	if(custom_filename_key.empty()){
		cout << "no custom_filename" << endl;
		return 1;
	}
	
	for (INI::File::sections_iter it = f_ini.SectionsBegin(); it != f_ini.SectionsEnd(); it++){
		for (INI::Section::values_iter vit = it->second->ValuesBegin(); vit != it->second->ValuesEnd(); vit++){
			for(int i = 0; i < custom_filename_key.size(); i++){
				if (vit->first == custom_filename_key.at(i)){
					fname += "_" + vit->second.AsString();
					break;
				}
			}
		}
	}
	
	cout << "Output filename prefix: " << DataFile << "_" << fname << "\n" << endl;
	
	return 0;
}

int thermo::check_ini(ostream& os, bool if_cout_true ){
	string str_tab, str_lf, str_note, str_note_end;
	if(if_cout_true == true){
		str_tab = "";
		str_lf = "\n";
		str_note = "";
		str_note_end = "";
	}else{
		str_tab = R"()";
		str_lf = R"(\r)";
		str_note = R"(X Note 'Reflectivity', ")";
		str_note_end = "\"\n";
	}
	
	os << str_note << str_tab << "THERMO_VER:" << str_tab << thermo_version << str_lf << str_note_end ;
	os << str_note << str_tab << "DataFile:" << str_tab << DataFile << str_lf << str_note_end ;
	os << str_note << str_tab << "(Out4d,Out2dSurface,CalcRef,CalcRef_i,OutCSV,OutProfile,OutCoef):" << str_tab << "(" << Out4d << "," << Out2dSurface << "," << CalcRef << "," << CalcRef_i << "," << OutCSV << "," << OutProfile << "," << OutCoef <<")" << str_lf << str_note_end;
	os << str_note << str_tab << "(Nd,Ns,Nt,Nz):" << str_tab << "(" << Nd << "," << Ns << "," << Nt << "," << Nz << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Hd,Hs,Ht,Hz):" << str_tab << "(" << Hd << "," << Hs << "," << Ht << "," << Hz << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Delta_t,Choosed_s,zCenter):" << str_tab << "(" << Delta_t << "," << Choosed_s << "," << zCenter << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Tinit):" << str_tab << "(" << Tinit << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(ld,ls,lz):" << str_tab << "(" << ld << "," << ls << "," << lz << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(WaveLength,RegardExpansion):" << str_tab << "(" << WaveLength << "," << RegardExpansion << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Ninit,Dn_dT,D2n_dT2):" << str_tab << "(" << Ninit << "," << Dn_dT << "," << D2n_dT2 << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Power,Center,Speed):" << str_tab << "(" << Power << "," << Center << "," << Speed << ")" << str_lf << str_note_end ;
	
	os << str_note << str_tab << "(Radius,Radius2,Radius3):" << str_tab << "(" << Radius << "," << Radius2 << "," << Radius3 << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Epsilon,Epsilon2,Epsilon3,Coe):" << str_tab << "(" << Epsilon << "," << Epsilon2 << ","  << Epsilon3 << "," << Coe << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(Radiation_max,Radiation_mid,Radiation_min,Radiation_rate,Radiation_supp):" << str_tab << "(" << Radiation_max << "," << Radiation_mid << "," << Radiation_min << "," << Radiation_rate << "," << Radiation_supp << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "(wa,wb,wc,SuppCenter,wbsupp):" << str_tab << "(" << wa << "," << wb << "," << wc << "," << SuppCenter << "," << wbsupp << ")" << str_lf << str_note_end ;
	os << str_note << str_tab << "Max power density : " << power_density(0,0) << "[W/mm2] = " << power_density(0,0)/10 << "[kW/cm2]" << str_lf << str_note_end; //
	
	return 0;
}


int thermo::resize(void){
	//配列確保
	distribution = vector<double>(Nd*Ns*Nz, Tinit);
	old_distribution = vector<double>(Nd*Ns*Nz, Tinit);

	temperature.assign(t_pnts, 0);
	temperature_back.assign(t_pnts, 0);
	opt_thickness.assign(t_pnts, 0);
	reflectivity.assign(t_pnts, 0);
	reflectivity_comp.assign(t_pnts, 0);
	
	global_item_size = {static_cast<size_t>(Nd),static_cast<size_t>(Ns),static_cast<size_t>(Nz)};
	
	return 0;
}

int thermo::check_stability(double temperature){
	double stability_x, stability_y, stability_z;
	
	stability_x = 2 * kakusanritsu(temperature) * Ht / (Hd * Hd);
	stability_y = 2 * kakusanritsu(temperature) * Ht / (Hs * Hs);
	stability_z = 2 * kakusanritsu(temperature) * Ht / (Hz * Hz);
	
	cout << "Stability("<< temperature << " K)" << " : (" << stability_x << " , " << stability_y << " , " << stability_z << ")\n";
	//cout << "Output points : " << t_pnts << "\n";
	return 0;
}

int thermo::out_dist_head(){
	//4次元データのヘッダ
	int d, s;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_4d.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 2d output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "IGOR\n";
	fout << "WAVES/D/O/N=(" << Ns << "," << Nz << "," << Nd << "," << t_pnts_4D << ")\t'Distribution'\n";
	fout << "BEGIN\n";
	fout.close();
	return 0;
}

int thermo::out_dist(){
	//4次元データの出力
	int d, s, z, t;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_4d.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str, ios::app);
	if (!fout){//出力ファイルの確認
		cout << "2d file output error!\n";
		return 1;
	}
	fout << scientific;
	for (t=0; t<t_pnts_4D; t++){
		for (d=0; d<Nd; d++){
			for (s=0; s<Ns; s++){
				for(z=0; z<Nz; z++){
					fout << "\t" << device_memory_4D[Nd*Ns*Nz*t+Nz*Ns*d+Nz*s+z];
				}
				fout << "\n";
			}
			fout << "\n";
		}
		fout << "\n";
	}
	fout.close();
	return 0;
}

int thermo::out_dist_foot(){
	//4次元データのフッタ
	int d, s;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_4d.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str, ios::app);
	if (!fout){//出力ファイルの確認
		cout << "2d file output error!\n";
		return 1;
	}
	fout << scientific;
	fout << "END\n";
	fout << "X SetScale/P x 0,"<< Hd*1e-3 << ",\"m\",'Distribution'\n";
	fout << "X SetScale/P y 0,"<< Hs*1e-3 << ",\"m\",'Distribution'\n";
	fout << "X SetScale/P z 0,"<< Hz*1e-3 << ",\"m\",'Distribution'\n";
	fout << "X SetScale/P t 0,"<< Ht*Delta_t_4D << ",\"s\",'Distribution'\n";
	fout << "X SetScale d 0,0,\"K\",'distribution'\n";
	
	fout.close();
	cout << "output " << Data4d << "\n";
	return 0;
}

int thermo::out_dist_surface_head(){
	//基板表面温度分布データ
	int d, s;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_2dSurface.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 2dSurface output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "IGOR\n";
	fout << "WAVES/D/O/N=(" << Ns << "," << Nz << "," << t_pnts_surface << ")\t'SurfaceDistribution'\n";
	fout << "BEGIN\n";
	fout.close();
	return 0;
}

int thermo::out_dist_surface(){
	//基板表面温度分布データ
	int s ,z, t;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_2dSurface.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str, ios::app);
	if (!fout){//出力ファイルの確認
		cout << "2dSurface file output error!\n";
		return 1;
	}
	fout << scientific;
	for (t=0; t<t_pnts_surface; t++){
		for (s=0; s<Ns; s++){
			for(z=0; z<Nz; z++){
				fout << "\t" << device_memory_2D[Ns*Nz*t+Nz*s+z];
			}
			fout << "\n";
		}
		fout << "\n";
	}
	fout << "\n";
	
	fout.close();
	return 0;
}

int thermo::out_dist_surface_foot(){
	//基板表面温度分布データフッター
	int d, s;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_2dSurface.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str, ios::app);
	if (!fout){//出力ファイルの確認
		cout << "2dSurface file output error!\n";
		return 1;
	}
	fout << scientific;
	fout << "END\n";
	fout << "X SetScale/P x 0,"<< Hs*1e-3 << ",\"m\",'SurfaceDistribution'\n";
	fout << "X SetScale/P y 0,"<< Hz*1e-3 << ",\"m\",'SurfaceDistribution'\n";
	fout << "X SetScale/P z 0,"<< Ht*Surface_Delta_t << ",\"s\",'SurfaceDistribution'\n";
	fout << "X SetScale d 0,0,\"K\",'distSurface'\n";
	
	fout.close();
	cout << "output " << Data4d << "\n";
	return 0;
}

int thermo::opt_thickness_peakfind(){
	int count = 0;
	double diff,cumul=0;
	int saisho = 0;
	char str[256];
	string Data1d;
	
	ofstream fout;
	
	Data1d = DataFile + "_" + fname +"_peak.itx";
	
	strcpy(str, Data1d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 1d output file!\n";
		return 1;
	}
	fout << scientific << setprecision(16);
	fout << "IGOR\n";
	fout << "WAVES/D/O" << "\t'PeakorValley'\t'ReflectivityP'\t'PeakCount'\t'Opt_ThicknessP'\t'Nd_Difference'\t'Nd_Diff_Cumul'";
	fout << "\nBEGIN\n";
	
	for (int t=1; t<t_pnts-1; t++){
		if(reflectivity[t-1]<=reflectivity[t] && reflectivity[t+1]<=reflectivity[t]){
			saisho = t;
			break;
		}
		if(reflectivity[t-1]>=reflectivity[t] && reflectivity[t+1]>=reflectivity[t]){
			saisho = t;
			break;
		}
	}
	for (int t=1; t<t_pnts-1; t++){
		if(reflectivity[t-1]<=reflectivity[t] && reflectivity[t+1]<=reflectivity[t]){
			diff = opt_thickness[t] - opt_thickness[saisho] - count * 3.275e-7;
			cumul += diff;
			fout << "100" << "\t" <<reflectivity[t] << "\t" << count << "\t" << opt_thickness[t] << "\t" << diff << "\t" << cumul << "\n";
			count++;
		}
		if(reflectivity[t-1]>=reflectivity[t] && reflectivity[t+1]>=reflectivity[t]){
			diff = opt_thickness[t] - opt_thickness[saisho] - count * 3.275e-7;
			cumul += diff;
			fout << "0" << "\t" << reflectivity[t] << "\t" << count << "\t" << opt_thickness[t] << "\t" << diff << "\t" << cumul << "\n";
			count++;
		}
	}
	fout << "END\n";
	fout << "X SetScale/P x 0, 1,\"s\", 'PeakorValley'\n";
	fout << "X SetScale d 0,0,\"%\",'PeakorValley'\n";
	fout << "X SetScale/P x 0, 1,\"s\", 'ReflectivityP'\n";
	fout << "X SetScale d 0,0,\"%\",'ReflectivityP'\n";
	fout << "X SetScale/P x 0, 1,\"s\", 'PeakCount'\n";
	fout << "X SetScale d 0,0,\"\",'PeakCount'\n";
	fout << "X SetScale/P x 0, 1,\"s\", 'Opt_ThicknessP'\n";
	fout << "X SetScale d 0,0,\"m\",'Opt_ThicknessP'\n";
	fout << "X SetScale/P x 0, 1,\"s\", 'Nd_Difference'\n";
	fout << "X SetScale d 0,0,\"m\",'Nd_Difference'\n";
	fout << "X SetScale/P x 0, 1,\"s\", 'Nd_Diff_Cumul'\n";
	fout << "X SetScale d 0,0,\"m\",'Nd_Diff_Cumul'\n";
	fout.close();
	cout << "output " << Data1d << "\n";
	if(saisho = 0){
		cout << "##########saisho = 0.#########" << endl;
	}
	return 0;
}

int thermo::out_calc(){
	//1次元データの出力
	int i;
	char str[256];
	string Data1d;
	
	ofstream fout;
	
	Data1d = DataFile + "_" + fname +"_1d.itx";
	
	strcpy(str, Data1d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 1d output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "IGOR\n";
	fout << "WAVES/D/O" << "\t'Time_wave'\t'Temperature'\t'Temperature_back'\t'Opt_Thickness'";
	if (CalcRef){
		fout << "\t'Reflectivity'";
			if (CalcRef_i){
				fout << "\t'ref_comp_real'\t'ref_comp_imag'\t'ref_comp_abs'\t'ref_comp_arg'";
			}
	}
	fout << "\t'Radiation'\t'LaserPositionPowerDensity'\t'LaserPosition0_GaussPosition'";
	fout << "\nBEGIN\n";
//	fout << "Time\tTemperature\tReflectivity\n";
	for (i=0; i<t_pnts; i++){
		fout << Ht*i*Delta_t << "\t" << temperature[i] << "\t" << temperature_back[i] << "\t" << opt_thickness[i];
		if (CalcRef){
			fout << "\t" << reflectivity[i];
			if (CalcRef_i){
				fout << "\t" << real(reflectivity_comp[i]) << "\t" << imag(reflectivity_comp[i]) << "\t" << abs(reflectivity_comp[i]) << "\t" << arg(reflectivity_comp[i]);
			}
		}
		double distance = Hs*Center + Ht*i*Delta_t*Speed - Hs*Choosed_s;
		double Out_Rad = Radiation_rate*(Radiation_max-Radiation_mid)*exp(- pow(distance/wa,2)/2) + (1-Radiation_rate)*(Radiation_max-Radiation_mid)*exp(- pow(distance/wb,2)/2) + (Radiation_mid-Radiation_min)*exp(- pow(distance/wc,2)/2) + Radiation_min + exp(- pow((abs(distance)-SuppCenter)/wbsupp,2)/2)*Radiation_supp;
		fout << "\t" << Out_Rad << "\t" <<power_density(distance,0) << "\t" << distance;
		fout << "\n";
	}
	fout << "END\n";
	fout << "X SetScale d 0,0,\"s\",'Time_wave'\n";
	fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'Temperature'\n";
	fout << "X SetScale d 0,0,\"K\",'Temperature'\n";
	fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'Temperature_back'\n";
	fout << "X SetScale d 0,0,\"K\",'Temperature_back'\n";
	fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'Opt_Thickness'\n";
	fout << "X SetScale d 0,0,\"m\",'Opt_Thickness'\n";
	if (CalcRef){
		fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'Reflectivity'\n";
		fout << "X SetScale d 0,0,\"%\",'Reflectivity'\n";

		if (CalcRef_i){
			fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'ref_comp_real'\n";
			fout << "X SetScale d 0,0,\"\",'ref_comp_real'\n";
			fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'ref_comp_imag'\n";
			fout << "X SetScale d 0,0,\"\",'ref_comp_imag'\n";
			fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'ref_comp_abs'\n";
			fout << "X SetScale d 0,0,\"\",'ref_comp_abs'\n";
			fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'ref_comp_arg'\n";
			fout << "X SetScale d 0,0,\"\",'ref_comp_arg'\n";
		}
	}
	fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'Radiation'\n";
	fout << "X SetScale d 0,0,\"\",'Radiation'\n";
	fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'LaserPositionPowerDensity'\n";
	fout << "X SetScale d 0,0,\"(kW/cm2)\",'LaserPositionPowerDensity'\n";
	fout << "X SetScale/P x 0," << Ht*Delta_t << ",\"s\", 'LaserPosition0_GaussPosition'\n";
	fout << "X SetScale d 0,0,\"m\",'LaserPosition0_GaussPosition'\n";
	check_ini(fout,false);
	fout.close();
	cout << "output " << Data1d << "\n";
	return 0;
}

int thermo::out_calc_csv(){
	//1次元データの出力(コンマセパレート)
	int i;
	char str[256];
	string Data1d;
	
	ofstream fout;
	
	Data1d = DataFile + "_" + fname +"_1d.csv";
	
	strcpy(str, Data1d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 1d output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "Time(s),Temperature(K),Opt_Thickness(m)";
	if (CalcRef){
		fout << ",Reflectivity(%)";
	}
	fout << "\n";
	for (i=0; i<t_pnts; i++){
		fout << Ht*i*Delta_t << "," << temperature[i] << "," << opt_thickness[i];
		if (CalcRef){
			fout << "," << reflectivity[i];
		}
		fout << "\n";
	}
	fout.close();
	cout << "output " << Data1d << "\n";
	return 0;
}


int thermo::out_profile(){
	//TPJのプロファイルの出力
	//表面3次元TPJプロファイル 180522
	int i, j;
	char str[256];
	string Data1d;
	
	ofstream fout;
	
	Data1d = DataFile + "_" + fname + "_profile.itx";
	
	strcpy(str, Data1d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 1d_profile output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "IGOR\n";
	fout << "WAVES/D/O/N=(" << 2*Ns << "," << 2*Nz <<")\t'SurfacePowerProfile'";
	fout << "\nBEGIN\n";
	for (i = -Ns; i < Ns; i++) {
		for (j = -Nz; j < Nz; j++) { 
			fout << "\t" << power_profile(Hs*i, Hz*j) / (10 * ((Hs*Hz) / (Hd*Hs*Hz)));
		}
		fout << "\n";
	}
	

	fout << "END\n";
	fout << "X SetScale/P x "<< -Hs*Ns*1e-3 << ","<< Hs*1e-3 << ",\"m\",'SurfacePowerProfile'\n";
	fout << "X SetScale/P y "<< -Hz*Nz*1e-3 << ","<< Hz*1e-3 << ",\"m\",'SurfacePowerProfile'\n";
	fout << "X SetScale d 0,0,\"(kW/cm2)\",'SurfacePowerProfile'\n";
	fout.close();
	cout << "output " << Data1d << "\n";
	return 0;
}

int thermo::out_coef(){
	//1次元データの出力
	int i;
	char str[256];
	string Data1d;
	
	ofstream fout;
	
	Data1d = DataFile + "_" + fname +"_hcoef.itx";
	
	strcpy(str, Data1d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open h coef output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "IGOR\n";
	fout << "WAVES/D/O" "\t'local_heat_transfer_coef_W/m2K'";
	fout << "\nBEGIN\n";
	for (i=-Ht*Nt*Speed; i<Ht*Nt*Speed; i++){
		double Out_Rad = Radiation_rate*(Radiation_max-Radiation_mid)*exp(- pow(i/wa,2)/2) + (1-Radiation_rate)*(Radiation_max-Radiation_mid)*exp(- pow(i/wb,2)/2) + (Radiation_mid-Radiation_min)*exp(- pow(i/wc,2)/2) + Radiation_min + exp(- pow((abs(i)-SuppCenter)/wbsupp,2)/2)*Radiation_supp;
		fout << "\t" << Out_Rad << "\n";
	}
	fout << "END\n";
	fout << "X SetScale/P x " << (-Ht*Nt*Speed)/1000 << "," << 1e-3 << ",\"m\", 'local_heat_transfer_coef_W/m2K'\n";
	fout << "X SetScale d 0,0,\"W/m2K\",'local_heat_transfer_coef_W/m2K'\n";
	fout.close();
	cout << "output " << Data1d << "\n";
	return 0;
}

int thermo::out_reflectivity_2D_head(){
	//裏面反射率マップヘッダ
	int d, s;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_reflectivity2d.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str);
	if (!fout){//出力ファイルの確認
		cout << "Can't open 2dSurface output file!\n";
		return 1;
	}
	fout << scientific;
	fout << "IGOR\n";
	fout << "WAVES/D/O/N=(" << Ns << "," << Nz << "," << t_pnts_4D << ")\t'Reflectivity_2D'\n";
	fout << "BEGIN\n";
	fout.close();
	return 0;
}

int thermo::out_reflectivity_2D(){
	//裏面反射率マップ
	int s ,z, t;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_reflectivity2d.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str, ios::app);
	if (!fout){//出力ファイルの確認
		cout << "2dSurface file output error!\n";
		return 1;
	}
	fout << scientific;
	for (t=0; t<t_pnts_4D; t++){
		for (s=0; s<Ns; s++){
			for(z=0; z<Nz; z++){
				fout << "\t" << calc_reflectivity_2D(&reflectivity_comp[t],t,s,z);
			}
			fout << "\n";
		}
		fout << "\n";
	}
	fout << "\n";
	
	fout.close();
	return 0;
}

int thermo::out_reflectivity_2D_foot(){
	//裏面反射率マップフッター
	int d, s;
	char str[256];
	string Data4d;
	
	ofstream fout;
	
	Data4d = DataFile + "_" + fname + "_reflectivity2d.itx";
	
	strcpy(str, Data4d.c_str());
	fout.open(str, ios::app);
	if (!fout){//出力ファイルの確認
		cout << "2dSurface file output error!\n";
		return 1;
	}
	fout << scientific;
	fout << "END\n";
	fout << "X SetScale/P x 0,"<< Hs*1e-3 << ",\"m\",'Reflectivity_2D'\n";
	fout << "X SetScale/P y 0,"<< Hz*1e-3 << ",\"m\",'Reflectivity_2D'\n";
	fout << "X SetScale/P z 0,"<< Ht*t_pnts_4D << ",\"s\",'Reflectivity_2D'\n";
	fout << "X SetScale d 0,0,\"%\",'Reflectivity_2D'\n";
	
	fout.close();
	cout << "output " << Data4d << "\n";
	return 0;
}

double thermo::power_profile(double r1, double r2){
	//熱発生率の計算
	double value;
	value = (Epsilon * Power * gaussian2d(r1, r2, Radius, Coe) + Epsilon2 * Power * gaussian2d(r1, r2, Radius2, Coe) + Epsilon3 * Power * gaussian2d(r1, r2, Radius3, Coe)) * (Hs*Hz)/(Hd*Hs*Hz);
	return value;
}

double thermo::power_density(double r1, double r2){
	//パワー密度
	double value;
	value = power_profile(r1,r2) / ((Hs*Hz)/(Hd*Hs*Hz));
	return value;
}

void thermo::trace(){
	//
	for(int t=0; t<t_pnts; t++){
		temperature[t] = device_memory_1D[t*Nd];
		temperature_back[t] = device_memory_1D[t*Nd+Nd-1];
		opt_thickness[t] = calc_thickness(t);
		if (CalcRef){
			reflectivity[t] = calc_reflectivity(&reflectivity_comp[t],t);
		}
	}
}

double thermo::gaussian2d(double r1, double r2, double sigma, double coe){
	//2次元ガウシアン分布の計算
	double value, pi;
	pi = 2.0 * asin(1.0);
	
	value = (1/(2*pi*sigma*coe*sigma))*exp(-((r2*r2)/(2*sigma*sigma))-((r1*r1) / (2*(coe*sigma)*(coe*sigma))));
	//value = exp(-(r*r)/(2*sigma*sigma)) / (2*pi*sigma*sigma);
	return value;
}

double thermo::calc_thickness(int t){
	//光学膜厚の計算
	double value;
	
	value = 0;
	for (int d=0; d<Nd; d++){
		value += Hd * real(refractive_index(device_memory_1D[t*Nd+d])) * thickness_expansion(device_memory_1D[t*Nd+d]);
	}
	
	value *= 1e-3; //mmからmへの単位変換
	return value;
}

complex<double> thermo::single_ref(complex<double> nBack, complex<double> nFwd, double thickness, complex<double> rhoFwd){
	//単層の振幅反射率計算
	complex<double> I = complex<double>(0.0, 1.0);
	double pi;
	pi = 2.0 * asin(1.0);

	
	complex<double> rhoBack;
	complex<double>	 fresnelR, delta;
	
	fresnelR = (nBack - nFwd) / (nBack + nFwd);
	delta = (2 * pi / WaveLength) * nFwd * thickness;

	rhoBack = fresnelR + rhoFwd * exp(-2.0 * delta * I);
	rhoBack = rhoBack / (1.0 + fresnelR * rhoFwd * exp(-2.0 * delta * I));
//	rhoBack /= 1.0 + fresnelR * rhoFwd * exp(-2 * delta * I);
	
	return rhoBack;
}


double thermo::calc_reflectivity(complex<double> *reflectivity_comp, int t){
	//反射率の計算
	int i,j;
	complex<double> rho;
	complex<double> indexB, indexF;
	double thickness;
	double value;
	
	double Ref = 0;
	complex<double> ref_comp = 0;
	
	//基板表面-空気
	indexB = refractive_index(device_memory_1D[t*Nd]);
	indexF = 1;
	rho = (indexB - indexF) / (indexB + indexF);
	
	//基板内部
	for (i=1; i<=Nd-1; i++){
		indexB = refractive_index(device_memory_1D[t*Nd+i]);
		indexF = refractive_index(device_memory_1D[t*Nd+i-1]);
		thickness = Hd * 1e6 * thickness_expansion(device_memory_1D[t*Nd+i-1]);
		
		rho = single_ref(indexB, indexF, thickness, rho);
	}
	
	//空気-基板裏面
	indexB = 1;
	indexF = refractive_index(device_memory_1D[t*Nd+Nd-1]);
	thickness = Hd * 1e6 * thickness_expansion(device_memory_1D[t*Nd+Nd-1]);
	rho = single_ref(indexB, indexF, thickness, rho);

	value = norm(rho) * 100;
	Ref += value;
	ref_comp += rho;
	*reflectivity_comp=ref_comp;
	
	return Ref;
}

double thermo::calc_reflectivity_2D(complex<double> *reflectivity_comp, int t, int s, int z){
	//反射率の計算
	int i,j;
	complex<double> rho;
	complex<double> indexB, indexF;
	double thickness;
	double value;
	
	double Ref = 0;
	complex<double> ref_comp = 0;
	
	//基板表面-空気
	indexB = refractive_index(device_memory_4D[Nd*Ns*Nz*t+Nz*s+z]);
	indexF = 1;
	rho = (indexB - indexF) / (indexB + indexF);
	
	//基板内部
	for (i=1; i<=Nd-1; i++){
		indexB = refractive_index(device_memory_4D[Nd*Ns*Nz*t+Ns*Nz*i+Nz*s+z]);
		indexF = refractive_index(device_memory_4D[Nd*Ns*Nz*t+Ns*Nz*(i-1)+Nz*s+z]);
		thickness = Hd * 1e6 * thickness_expansion(device_memory_4D[Nd*Ns*Nz*t+Ns*Nz*(i-1)+Nz*s+z]);
		
		rho = single_ref(indexB, indexF, thickness, rho);
	}
	
	//空気-基板裏面
	indexB = 1;
	indexF = refractive_index(device_memory_4D[Nd*Ns*Nz*t+Ns*Nz*(Nd-1)+Nz*s+z]);
	thickness = Hd * 1e6 * thickness_expansion(device_memory_4D[Nd*Ns*Nz*t+Ns*Nz*(Nd-1)+Nz*s+z]);
	rho = single_ref(indexB, indexF, thickness, rho);

	value = norm(rho) * 100;
	Ref += value;
	ref_comp += rho;
	*reflectivity_comp=ref_comp;
	
	return Ref;
}

void thermo::HandleCompileError(void){
	cl_int logStatus;
	char *buildLog = NULL;
	size_t buildLogSize = 0;
	//in this tutorial i only have one device
	logStatus = clGetProgramBuildInfo( program, deviceToUse, CL_PROGRAM_BUILD_LOG, buildLogSize, buildLog, &buildLogSize);
	if(logStatus != CL_SUCCESS)
	{
		printf("logging error\n");
		exit(EXIT_FAILURE);
	}

	buildLog = (char *)malloc(buildLogSize);
	if(buildLog == NULL)
	{
		printf("ERROR TO ALLOCATE MEM FOR BUILDLOG\n");
		exit(EXIT_FAILURE);
	}

	memset(buildLog, 0, buildLogSize);

	logStatus = clGetProgramBuildInfo (program, deviceToUse, CL_PROGRAM_BUILD_LOG, buildLogSize, buildLog, NULL);
	if(logStatus != CL_SUCCESS)
	{
		free(buildLog);
		cleanup();
		exit(1);
	}

	printf("\nBUILD LOG\n");
	printf("************************************************************************\n");
	printf("%s END \n", buildLog);
	printf("************************************************************************\n");
	free(buildLog);
	cleanup();
	exit(1);
}
	
void thermo::testStatus(int status, string errorMsg){
	if(status != SUCCESS)
	{
		if(errorMsg.empty())
		{
			printf("Error\n");
		}
		else
		{
			cout << "Error : " << errorMsg << " : Status : " << status << endl;
		}
		exit(EXIT_FAILURE);
	}
}
	

void thermo::initializeCL(){
	platformToUse = NULL;
	device_ids = NULL;
	deviceToUse = NULL;
	deviceTypeToUse = CL_DEVICE_TYPE_GPU;
	num_devices;
	num_platforms;

	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	testStatus(ret, "clGetPlatformIDs error 1");
	platform_ids = new cl_platform_id[num_platforms];
	
	if(platform_ids == NULL){
		cout << "Error when allocating space for the platforms" <<endl;
		exit(EXIT_FAILURE);
	}
	
	ret = clGetPlatformIDs(num_platforms, platform_ids, NULL);
	testStatus(ret, "clGetPlatformIDs error 2");
	
	string message,comp;
	if(Selected_Vendor == 0){
		message = "Intel OpenCL platform is found.";
		comp = "Intel(R) Corporation";
	}else if(Selected_Vendor == 1){
		message = "AMD OpenCL platform is found.";
		comp = "Advanced";
	}else if(Selected_Vendor == 2){
		message = "NVIDIA OpenCL platform is found.";
		comp = "NVIDIA";
	}else{
		cout << "Error : Please define Selected_Vendor in the parameter file.\n" << endl;
		cout << "Intel : 0, AMD : 1, NVIDIA : 2\n" << endl;
		exit(EXIT_FAILURE);
	}	
	
	for(unsigned int i=0;i<num_platforms;i++){
		char pbuf[100];
		ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VENDOR, sizeof(pbuf), pbuf, NULL);
		testStatus(ret, "clGetPlatformInfo error 3");
		
		if(!strncmp(pbuf, comp.c_str(), 5)){
			cout << message << endl;
			platformToUse = platform_ids[i];
			break;
		}
	}
	if(platformToUse == NULL){
		cout << "Error : Selected OpenCL vendor is not found.\n" << endl;
		exit(EXIT_FAILURE);
	}
	
	//deviceTypeToUseにマッチするデバイス数をnum_devicesへ代入
	ret = clGetDeviceIDs(platformToUse, deviceTypeToUse, 0, device_ids, &num_devices);
	testStatus(ret, "clGetDeviceIDs error 1");
	device_ids = new cl_device_id[num_devices];
	if(device_ids == NULL){
		cout << "Error when allocating space for the devices" <<endl;
		exit(EXIT_FAILURE);
	}
	//device_idsへデバイス入れる
	ret = clGetDeviceIDs(platformToUse, deviceTypeToUse, num_devices, device_ids, NULL);
	testStatus(ret, "clGetDeviceIDs error 2");
	deviceToUse = device_ids[0];
	ret = clGetDeviceInfo(deviceToUse, CL_DEVICE_TYPE, sizeof(cl_device_type), (void *)&device_type, NULL);
	testStatus(ret, "clGetDeviceInfo error 1");

	ret = clGetDeviceInfo(deviceToUse, CL_DEVICE_VENDOR, (sizeof(char)*256), vendorName, NULL);
	testStatus(ret, "clGetDeviceInfo error 2");
	
	if(device_type == CL_DEVICE_TYPE_CPU)
	{
		cout << "Device type is CPU, Vendor is " << vendorName << endl;
	}
	else if(device_type == CL_DEVICE_TYPE_GPU)
	{
		cout << "Device type is GPU, Vendor is " << vendorName << endl;
	}
	else 
	{
		printf("device type is unknown\n");
	}
	cl_context_properties ccp[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platformToUse, 0 };
	context = clCreateContext( ccp, 1, &deviceToUse, NULL, NULL, &ret);
	testStatus(ret, "clCreateContext error");
	command_queue = clCreateCommandQueue(context, deviceToUse, 0, &ret);
	testStatus(ret, "clCreateCommandQueue error");
	
	delete[] platform_ids;
	delete[] device_ids;
	
}

void thermo::initializeKernel(){
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("kernel.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.cl.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
	
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	testStatus(ret, "clCreateProgramWithSource error");
	
	ret = clBuildProgram(program, 1, &deviceToUse, NULL, NULL, NULL);
	if(ret != CL_SUCCESS)
	{
		if(ret == CL_BUILD_PROGRAM_FAILURE)
		{
			HandleCompileError();
		} //end if BUILD_PROGRAM_FAILURE
	}
	kernel = clCreateKernel(program, "distribution", &ret);
	testStatus(ret, "clCreateKernel");
	kernel2 = clCreateKernel(program, "sync_dist", &ret);
	testStatus(ret, "clCreateKernel 2");
	Store1D_kernel = clCreateKernel(program, "Store1D_k", &ret);
	testStatus(ret, "clCreateKernel Store1D_kernel");
	Store2D_kernel = clCreateKernel(program, "Store2D_k", &ret);
	testStatus(ret, "clCreateKernel Store2D_kernel");
	Store4D_kernel = clCreateKernel(program, "Store4D_k", &ret);
	testStatus(ret, "clCreateKernel Store4D_kernel");
}

void thermo::initializeData(int t){
	//clCreateBuffer
	mem_obj_1D = clCreateBuffer(context, CL_MEM_READ_WRITE, (Nd*t_pnts) * sizeof(double), NULL, &ret);
	testStatus(ret, "mem_obj_1D");
	
	if(Out4d||OutRef2D){
		mem_obj_4D = clCreateBuffer(context, CL_MEM_READ_WRITE, (Nd*Ns*Nz*t_pnts_4D) * sizeof(double), NULL, &ret);
		testStatus(ret, "4D is too large. Reduce 4D Output points. mem_obj_4D");
	}
	if(Out2dSurface){
		mem_obj_2D = clCreateBuffer(context, CL_MEM_READ_WRITE, (Ns*Nz*t_pnts_surface) * sizeof(double), NULL, &ret);
		testStatus(ret, "2DSurface buffer Allocation error. Reduce 4D/2D Output points.");
	}
	
	distribution_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, distribution.size() * sizeof(double), NULL, &ret);
	testStatus(ret, "distribution_mem_obj");
	
	old_distribution_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, old_distribution.size() * sizeof(double), NULL, &ret);
	testStatus(ret, "old_distribution_mem_obj");
	
	//WriteBuffer
	ret = clEnqueueWriteBuffer(command_queue, distribution_mem_obj, CL_TRUE, 0, distribution.size() * sizeof(double), &distribution[0], 0, NULL, NULL);
	testStatus(ret, "distribution WriteBuffer");
	
	ret = clEnqueueWriteBuffer(command_queue, old_distribution_mem_obj, CL_TRUE, 0, old_distribution.size() * sizeof(double), &old_distribution[0], 0, NULL, NULL);
	testStatus(ret, "old_distribution WriteBuffer");
	
	//calc Radiation coefs
	Rad_coefA = Radiation_rate*(Radiation_max-Radiation_mid);
	Rad_coefB = (1-Radiation_rate)*(Radiation_max-Radiation_mid);
	Rad_coefC = Radiation_mid - Radiation_min;
	//Rad_coefSupp = Radiation_supp/(wbsupp*exp(-1/2));
	Rad_coefSupp = Radiation_supp;
	
	//clSetKernelArg
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&distribution_mem_obj);
	ret += clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&old_distribution_mem_obj);
	ret += clSetKernelArg(kernel,2, sizeof(int), (void *)&Nd);
	ret += clSetKernelArg(kernel,3, sizeof(int), (void *)&Ns);
	ret += clSetKernelArg(kernel,4, sizeof(int), (void *)&Nz);
	ret += clSetKernelArg(kernel,5, sizeof(double), (void *)&Hd);
	ret += clSetKernelArg(kernel,6, sizeof(double), (void *)&Hs);
	ret += clSetKernelArg(kernel,7, sizeof(double), (void *)&Hz);
	ret += clSetKernelArg(kernel,8, sizeof(double), (void *)&Ht);
	ret += clSetKernelArg(kernel,9, sizeof(int), (void *)&t);
	ret += clSetKernelArg(kernel,10, sizeof(int), (void *)&Center);
	ret += clSetKernelArg(kernel,11, sizeof(double), (void *)&Speed);
	ret += clSetKernelArg(kernel,12, sizeof(int), (void *)&zCenter);
	ret += clSetKernelArg(kernel,13, sizeof(double), (void *)&Coe);
	ret += clSetKernelArg(kernel,14, sizeof(double), (void *)&Radius);
	ret += clSetKernelArg(kernel,15, sizeof(double), (void *)&Radius2);
	ret += clSetKernelArg(kernel,16, sizeof(double), (void *)&Radius3);
	ret += clSetKernelArg(kernel,17, sizeof(double), (void *)&Epsilon);
	ret += clSetKernelArg(kernel,18, sizeof(double), (void *)&Epsilon2);
	ret += clSetKernelArg(kernel,19, sizeof(double), (void *)&Epsilon3);
	ret += clSetKernelArg(kernel,20, sizeof(double), (void *)&Power);
	ret += clSetKernelArg(kernel,21, sizeof(double), (void *)&Rad_coefA);
	ret += clSetKernelArg(kernel,22, sizeof(double), (void *)&Rad_coefB);
	ret += clSetKernelArg(kernel,23, sizeof(double), (void *)&Rad_coefC);
	ret += clSetKernelArg(kernel,24, sizeof(double), (void *)&Radiation_min);
	ret += clSetKernelArg(kernel,25, sizeof(double), (void *)&Rad_coefSupp);
	ret += clSetKernelArg(kernel,26, sizeof(double), (void *)&wa);
	ret += clSetKernelArg(kernel,27, sizeof(double), (void *)&wb);
	ret += clSetKernelArg(kernel,28, sizeof(double), (void *)&wbsupp);
	ret += clSetKernelArg(kernel,29, sizeof(double), (void *)&wc);
	ret += clSetKernelArg(kernel,30, sizeof(double), (void *)&SuppCenter);
	ret += clSetKernelArg(kernel,31, sizeof(double), (void *)&Tinit);
	testStatus(ret, "clSetKernelArg");
	
	ret = clSetKernelArg(kernel2, 0, sizeof(cl_mem), (void *)&distribution_mem_obj);
	ret += clSetKernelArg(kernel2,1, sizeof(cl_mem), (void *)&old_distribution_mem_obj);
	ret += clSetKernelArg(kernel2,2, sizeof(int), (void *)&Nd);
	ret += clSetKernelArg(kernel2,3, sizeof(int), (void *)&Ns);
	ret += clSetKernelArg(kernel2,4, sizeof(int), (void *)&Nz);
	testStatus(ret, "clSetKernelArg kernel2");
	
	ret = clSetKernelArg(Store1D_kernel, 0, sizeof(cl_mem), (void *)&distribution_mem_obj);
	ret += clSetKernelArg(Store1D_kernel,1, sizeof(cl_mem), (void *)&mem_obj_1D);
	ret += clSetKernelArg(Store1D_kernel,2, sizeof(int), (void *)&Nd);
	ret += clSetKernelArg(Store1D_kernel,3, sizeof(int), (void *)&Ns);
	ret += clSetKernelArg(Store1D_kernel,4, sizeof(int), (void *)&Nz);
	ret += clSetKernelArg(Store1D_kernel,5, sizeof(int), (void *)&t);
	ret += clSetKernelArg(Store1D_kernel,6, sizeof(int), (void *)&Delta_t);
	ret += clSetKernelArg(Store1D_kernel,7, sizeof(int), (void *)&Choosed_s);
	ret += clSetKernelArg(Store1D_kernel,8, sizeof(int), (void *)&zCenter);
	testStatus(ret, "clSetKernelArg Store1D_kernel");
	
	ret = clSetKernelArg(Store4D_kernel, 0, sizeof(cl_mem), (void *)&distribution_mem_obj);
	if(Out4d||OutRef2D){
		ret += clSetKernelArg(Store4D_kernel,1, sizeof(cl_mem), (void *)&mem_obj_4D);
	}else {
		ret += clSetKernelArg(Store4D_kernel,1, sizeof(cl_mem), NULL);
	}
	ret += clSetKernelArg(Store4D_kernel,2, sizeof(int), (void *)&Nd);
	ret += clSetKernelArg(Store4D_kernel,3, sizeof(int), (void *)&Ns);
	ret += clSetKernelArg(Store4D_kernel,4, sizeof(int), (void *)&Nz);
	ret += clSetKernelArg(Store4D_kernel,5, sizeof(int), (void *)&t);
	ret += clSetKernelArg(Store4D_kernel,6, sizeof(int), (void *)&Delta_t_4D);
	testStatus(ret, "clSetKernelArg Store4D");
	
	ret = clSetKernelArg(Store2D_kernel, 0, sizeof(cl_mem), (void *)&distribution_mem_obj);
	if(Out2dSurface){
		ret += clSetKernelArg(Store2D_kernel,1, sizeof(cl_mem), (void *)&mem_obj_2D);
	}else {
		ret += clSetKernelArg(Store2D_kernel,1, sizeof(cl_mem), NULL);
	}
	ret += clSetKernelArg(Store2D_kernel,2, sizeof(int), (void *)&Ns);
	ret += clSetKernelArg(Store2D_kernel,3, sizeof(int), (void *)&Nz);
	ret += clSetKernelArg(Store2D_kernel,4, sizeof(int), (void *)&t);
	ret += clSetKernelArg(Store2D_kernel,5, sizeof(int), (void *)&Surface_Delta_t);
	testStatus(ret, "clSetKernelArg Store2D");
}

void thermo::setkernelargs(int t){
	clSetKernelArg(kernel,9, sizeof(int), (void *)&t);
	clSetKernelArg(Store1D_kernel,5, sizeof(int), (void *)&t);
	if(Out4d||OutRef2D){
		clSetKernelArg(Store4D_kernel,5, sizeof(int), (void *)&t);
	}
	if(Out2dSurface){
		clSetKernelArg(Store2D_kernel,4, sizeof(int), (void *)&t);
	}
}

void thermo::runKernel(){
	clEnqueueNDRangeKernel(command_queue, kernel, 3, NULL, &global_item_size[0], &local_item_size[0], 0, NULL, NULL);	
	clEnqueueNDRangeKernel(command_queue, kernel2, 3, NULL, &global_item_size[0], &local_item_size[0], 0, NULL, NULL);	
}

void thermo::Store1D(){
	if(Selected_Vendor == 0){ clFinish(command_queue);}
	clEnqueueNDRangeKernel(command_queue, Store1D_kernel, 1, NULL, &global_item_size[0], &ld, 0, NULL, NULL);
}

void thermo::Store2D(){
	if(Selected_Vendor == 0){ clFinish(command_queue);}
	clEnqueueNDRangeKernel(command_queue, Store2D_kernel, 1, NULL, &global_item_size[1], &local_item_size[1], 0, NULL, NULL);
}

void thermo::Store4D(){
	if(Selected_Vendor == 0){ clFinish(command_queue);}
	clEnqueueNDRangeKernel(command_queue, Store4D_kernel, 3, NULL, &global_item_size[0], &local_item_size[0], 0, NULL, NULL);
}

void thermo::ReadBuffer(){
	device_memory_1D = vector<double>((Nd*t_pnts), 0.0);
	clEnqueueReadBuffer(command_queue, mem_obj_1D, CL_TRUE, 0, device_memory_1D.size() * sizeof(double), &device_memory_1D[0], 0, NULL, NULL);
	
	if(Out4d||OutRef2D){
		device_memory_4D = vector<double>((Nd*Ns*Nz*t_pnts_4D), 0.0);
		clEnqueueReadBuffer(command_queue, mem_obj_4D, CL_TRUE, 0, device_memory_4D.size() * sizeof(double), &device_memory_4D[0], 0, NULL, NULL);
	}
	
	if(Out2dSurface){
		device_memory_2D = vector<double>((Ns*Nz*t_pnts_surface), 0.0);
		clEnqueueReadBuffer(command_queue, mem_obj_2D, CL_TRUE, 0, device_memory_2D.size() * sizeof(double), &device_memory_2D[0], 0, NULL, NULL);
	}
}

void thermo::cleanup(){
	testStatus(clFlush(command_queue), "cleanup:clFlush(command_queue)");
	testStatus(clFinish(command_queue), "cleanup:clFinish(command_queue)");
	testStatus(clReleaseKernel(kernel), "cleanup:clReleaseKernel(kernel)");
	testStatus(clReleaseKernel(kernel2), "cleanup:clReleaseKernel(kernel2)");
	testStatus(clReleaseKernel(Store1D_kernel), "cleanup:clReleaseKernel(Store1D_kernel)");
	testStatus(clReleaseKernel(Store2D_kernel), "cleanup:clReleaseKernel(Store2D_kernel)");
	testStatus(clReleaseKernel(Store4D_kernel), "cleanup:clReleaseKernel(Store4D_kernel)");
	testStatus(clReleaseProgram(program), "cleanup:clReleaseProgram(program)");
	testStatus(clReleaseMemObject(distribution_mem_obj), "cleanup:clReleaseMemObject(distribution_mem_obj)");
	testStatus(clReleaseMemObject(old_distribution_mem_obj), "cleanup:clReleaseMemObject(old_distribution_mem_obj)");
	testStatus(clReleaseMemObject(mem_obj_1D), "cleanup:clReleaseMemObject(mem_obj_1D)");
	if(Out4d||OutRef2D){
		testStatus(clReleaseMemObject(mem_obj_4D), "cleanup:clReleaseMemObject(mem_obj_4D)");
	}
	if(Out2dSurface){
		testStatus(clReleaseMemObject(mem_obj_2D), "cleanup:clReleaseMemObject(mem_obj_2D)");
	}
	testStatus(clReleaseCommandQueue(command_queue), "cleanup:clReleaseCommandQueue(command_queue)");
	testStatus(clReleaseContext(context), "cleanup:clReleaseContext(context)");
}
