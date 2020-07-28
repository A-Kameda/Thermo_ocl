/*!
* thermo.h
* Copyright (c) 2020 Asaki Kameda
* Released under GNU GPLv3.
* see LICENSE.txt
*/
//For OpenCL version v2.2.0(2020.03.04)

#define SUCCESS 0
#define FAIL -1
#define MAX_SOURCE_SIZE (0x100000)

class thermo{
	//
	private:
		INI::File f_ini;
		string DataFile, thermo_version, fname;
		int Selected_Vendor;
		int Choosed_s, Half_s_range;
		double WaveLength;
		double Ninit, Dn_dT, D2n_dT2;
		int RegardExpansion;
		int t_pnts,t_pnts_surface,t_pnts_4D;
		int TempOut_s_Offset;
		
		vector<double> temperature, temperature_back;
		vector<double> opt_thickness;
		vector<double> reflectivity;
		vector<complex<double> > reflectivity_comp;
		
		int user_getini();
		bool lerror(string str1, string str2);
		double kakusanritsu(double temperature);
		double lambda_tokusei(double temperature);
		double gaussian2d(double r1, double r2, double sigma, double Coe);
		complex<double> refractive_index(double temperature);
		double thickness_expansion(double temperature);
		complex<double> single_ref(complex<double> nBack, complex<double> nFwd, double thickness, complex<double> rhoFwd);
		double calc_thickness(int t);
		double calc_reflectivity(complex<double> *reflectivity_comp, int t);
		double calc_reflectivity_2D(complex<double> *reflectivity_comp, int t, int s, int z);

		
		//ocl
		vector<double> device_memory_1D;
		vector<double> device_memory_2D;
		vector<double> device_memory_4D;
		
		cl_platform_id *platform_ids;
		cl_platform_id platformToUse;
		cl_device_id* device_ids;
		cl_device_id deviceToUse;
		cl_device_info deviceTypeToUse;
		cl_device_type device_type;
		cl_uint num_devices;
		cl_uint num_platforms;
		cl_int ret;
		cl_program program;
		char vendorName[256];
		cl_kernel kernel, kernel2, Store1D_kernel, Store2D_kernel, Store4D_kernel;
		
		cl_command_queue command_queue;
		cl_context context;
		cl_mem mem_obj_1D;
		cl_mem mem_obj_2D;
		cl_mem mem_obj_4D;
		cl_mem distribution_mem_obj;
		cl_mem old_distribution_mem_obj;

		vector<size_t> global_item_size;
		vector<size_t> local_item_size;
		
		size_t ld,ls,lz;
		
	public:
		thermo(const int &argc, char *argv[] );
		int Nt;
		int Nd, Ns, Nz;
		double Ht, Hd, Hs, Hz;
		double Radius, Epsilon;
		double Radius2, Epsilon2, Coe;
		double Radius3, Epsilon3;
		double Power, Speed;
		int Center;
		double Radiation_max, Radiation_mid, Radiation_min, Radiation_rate, Radiation_supp;
		double Rad_coefA,Rad_coefB,Rad_coefC,Rad_coefSupp;
		double wa, wb, wbsupp, wc, SuppCenter;
		
		int Delta_t, Surface_Delta_t;
		int Delta_t_4D;
		int Out4d;
		int Out2dSurface;
		int OutRef2D;
		int CalcRef, CalcRef_i;
		int OutCSV,OutProfile,OutCoef,OutPeaks;
		int zCenter,RefOut_z_Offset;
		
		double Tinit;
		
		vector<double> distribution;
		vector<double> old_distribution;
		
		int getini();
		int make_fname();
		int check_ini(ostream& os, bool if_cout_true);
		int resize(void);
		int check_stability(double temperature);
		
		int out_dist_head();
		int out_dist();
		int out_dist_foot();
		int out_dist_surface_head();
		int out_dist_surface();
		int out_dist_surface_foot();
		int out_calc();
		int out_coef();
		int out_calc_csv();
		int out_profile();
		
		int out_reflectivity_2D_head();
		int out_reflectivity_2D();
		int out_reflectivity_2D_foot();
		
		int opt_thickness_peakfind();
		
		void trace(void);
		
		double power_profile(double r1, double r2);
		double power_density(double r1, double r2);
		
		//ocl
		void setkernelargs(int t);
		void HandleCompileError(void);
		void testStatus(int status, string errorMsg);
		void initializeCL(void);
		void initializeKernel(void);
		void initializeData(int t);
		void runKernel(void);
		void Store1D(void);
		void Store2D(void);
		void Store4D(void);
		void ReadBuffer(void);
		void cleanup(void);
};
