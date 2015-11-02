/**       @file main.cpp
 *
 *     Project: lensing_profile_tester
 *        Path: /lensing_profile_tester/main.cpp
 *
 *  Created on: 12 Aug 2014
 *      Author: brg
 */

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "IceBRG_main/container/labeled_array.hpp"
#include "IceBRG_main/units/units.hpp"
#include "IceBRG_main/units/unit_conversions.hpp"
#include "IceBRG_lensing/lensing_tNFW_profile.h"
#include "IceBRG_lensing/lensing_tNFW_caches.h"

int main( const int argc, const char *argv[] )
{
	using namespace IceBRG;

	// Program flow control
	const bool print_caches = false;

	// Redshift
	const flt_type z_min = 0.2;
	const flt_type z_max = 1.3;
	const int_type z_steps = 11;
	const bool z_log = false;

	// Satellite
	const mass_type M_sat_min = 1e10*unitconv::Msuntokg*kg;
	const mass_type M_sat_max = 1e13*unitconv::Msuntokg*kg;
	const int_type M_sat_steps = 3;
	const bool M_sat_log = true;

	const distance_type offset_R = 500*unitconv::kpctom*m;

	// Group
	const mass_type M_group = 2.1e14*unitconv::Msuntokg*kg;
	const flt_type group_c = 2.5;
	const flt_type sat_frac = 0.1;

	// Points to measure lensing signal at
	const distance_type R_min =   100*unitconv::kpctom*m;
	const distance_type R_max = 10000*unitconv::kpctom*m;
	const int_type R_steps = 100;
	const bool R_log = true;

	labeled_array<flt_type> output_table;

	output_table.set_labels(std::vector<std::string>({"R_kpc","Sat_mass_msun","Group_mass_msun",
		"Sat_z","Offset_R_kpc","DS_sat", "DS_group", "DS_tot", "DS_sat_offset",
		"DS_sat_shifted", "DS_sat_shifted_no_enh"}));

	// File output names
	std::string output_name = "lensing_test_results.dat";
	std::string sig_cache_name = "sig_cache.dat";
	std::string offset_sig_cache_name = "offset_sig_cache.dat";
	std::string group_sig_cache_name = "group_sig_cache.dat";
	std::string shifted_sig_cache_name = "shifted_sig_cache.dat";
	std::string shifted_no_enh_sig_cache_name = "shifted_no_enh_sig_cache.dat";
	std::string shifting_cache_name = "shifting_cache.dat";

	// Generate caches before the parallel section, so they can be generated in parallel
	if(print_caches)
	{
		std::ofstream out;

//		IceBRG::open_file(out,sig_cache_name);
//		std::cout << "Outputting tNFW_sig_cache to " << sig_cache_name << "... ";
//		std::cout.flush();
//		IceBRG::tNFW_sig_cache().print(out);
//		std::cout << "Done!" << std::endl;

//		IceBRG::open_file(out,offset_sig_cache_name);
//		std::cout << "Outputting tNFW_offset_sig_cache to " << offset_sig_cache_name << "... ";
//		std::cout.flush();
//		IceBRG::tNFW_offset_sig_cache().print(out);
//		std::cout << "Done!" << std::endl;

//		IceBRG::open_file(out,group_sig_cache_name);
//		std::cout << "Outputting tNFW_group_sig_cache to " << group_sig_cache_name << "... ";
//		std::cout.flush();
//		IceBRG::tNFW_group_sig_cache().print(out);
//		std::cout << "Done!" << std::endl;

		IceBRG::open_file(out,shifted_sig_cache_name);
		std::cout << "Outputting tNFW_shifted_sig_cache to " << shifted_sig_cache_name << "... ";
		std::cout.flush();
		IceBRG::tNFW_shifted_sig_cache().print(out);
		std::cout << "Done!" << std::endl;

		IceBRG::open_file(out,shifted_no_enh_sig_cache_name);
		std::cout << "Outputting tNFW_shifted_no_enh_sig_cache to " << shifted_no_enh_sig_cache_name << "... ";
		std::cout.flush();
		IceBRG::tNFW_shifted_no_enh_sig_cache().print(out);
		std::cout << "Done!" << std::endl;

		IceBRG::open_file(out,shifting_cache_name);
		std::cout << "Outputting shifting_cache to " << shifting_cache_name << "... ";
		std::cout.flush();
		IceBRG::shifting_cache().print(out);
		std::cout << "Done!" << std::endl;
	}
	else
	{
		// If not printing, do a dummy get to load them
		tNFW_sig_cache().get(0.,0.,0.);
		tNFW_offset_sig_cache().get(0.,0.,0.,0.);
		tNFW_group_sig_cache().get(0.,0.,0.,0.);
		tNFW_shifted_sig_cache().get(0.,0.,0.);
		tNFW_shifted_no_enh_sig_cache().get(0.,0.,0.);
		shifting_cache().get(0.,0.);
	}

	std::cout << "Calculating data... ";
	std::cout.flush();
	// Calculate lensing signal for each point
	for(int_type z_i=0; z_i<z_steps; ++z_i)
	{
		flt_type z;
		if(z_log)
		{
			z = z_min * pow(z_max/z_min,(z_i)/static_cast<flt_type>(z_steps));
		}
		else
		{
			z = z_min + (z_max-z_min)*(z_i)/static_cast<flt_type>(z_steps);
		}

		const IceBRG::lensing_tNFW_profile group_profile(M_group,z);

		for(int_type M_sat_i=0; M_sat_i<M_sat_steps; ++M_sat_i)
		{
			mass_type M_sat;
			if(M_sat_log)
			{
				M_sat = M_sat_min * pow(M_sat_max/M_sat_min,static_cast<flt_type>(M_sat_i)/static_cast<flt_type>(M_sat_steps));
			}
			else
			{
				M_sat = M_sat_min + (M_sat_max-M_sat_min)*static_cast<flt_type>(M_sat_i)/static_cast<flt_type>(M_sat_steps);
			}

			const IceBRG::lensing_tNFW_profile satellite_profile(M_sat,z);

			#pragma omp parallel for schedule(dynamic)
			for(int_type R_i=0; R_i<R_steps; ++R_i)
			{
				distance_type R;
				if(R_log)
				{
					R = R_min * pow(R_max/R_min,static_cast<flt_type>(R_i)/static_cast<flt_type>(R_steps));
				}
				else
				{
					R = R_min + (R_max-R_min)*static_cast<flt_type>(R_i)/static_cast<flt_type>(R_steps);
				}
				const auto DS_sat = satellite_profile.Delta_Sigma(R);
				const auto DS_group = sat_frac*group_profile.quick_group_Delta_Sigma(R,group_c);
				const auto DS_tot = DS_sat + sat_frac*DS_group;
				const auto DS_sat_offset = satellite_profile.quick_offset_Delta_Sigma(R,offset_R);
				const auto DS_sat_shifted = satellite_profile.shifted_Delta_Sigma(R);
				const auto DS_sat_shifted_no_enh = satellite_profile.shifted_no_enh_Delta_Sigma(R);

				#pragma omp critical
				output_table.insert_row(std::vector<flt_type>({value_of(R)*unitconv::mtokpc,
					value_of(M_sat)*unitconv::kgtoMsun,
					value_of(M_group)*unitconv::kgtoMsun,
					value_of(z),
					value_of(offset_R)*unitconv::mtopc,
					value_of(DS_sat)*unitconv::kgtoMsun/square(unitconv::mtopc),
					value_of(DS_group)*unitconv::kgtoMsun/square(unitconv::mtopc),
					value_of(DS_tot)*unitconv::kgtoMsun/square(unitconv::mtopc),
					value_of(DS_sat_offset)*unitconv::kgtoMsun/square(unitconv::mtopc),
					value_of(DS_sat_shifted)*unitconv::kgtoMsun/square(unitconv::mtopc),
					value_of(DS_sat_shifted_no_enh)*unitconv::kgtoMsun/square(unitconv::mtopc)}));
			}
		}
	}
	std::cout << "Done!" << std::endl;

	std::cout << "Outputting data... ";
	std::cout.flush();
	output_table.write(output_name,true,14);
	std::cout << "Done!" << std::endl;

	std::cout << "Execution complete." << std::endl;

	return 0;
}
