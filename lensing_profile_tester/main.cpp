/**       @file main.cpp
 *
 *     Project: lensing_profile_tester
 *        Path: /lensing_profile_tester/main.cpp
 *
 *  Created on: 12 Aug 2014
 *      Author: brg
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "brg/density_profile/lensing/lensing_tNFW_profile.h"
#include "brg/density_profile/lensing/lensing_tNFW_caches.h"
#include "brg/brg_file_functions.h"
#include "brg/brg_misc_functions.hpp"
#include "brg/brg_units.h"

int main( const int argc, const char *argv[] )
{
	// Redshift
	const double z = 0.303;

	// Satellite
	const double M_sat = 1.3e11*unitconv::Msuntokg;
	const double offset_R = 100*unitconv::kpctom;
	const brgastro::lensing_tNFW_profile satellite_profile(M_sat,z,0,1e9);

	// Group
	const double M_group = 2.1e14*unitconv::Msuntokg;
	const double group_c = 2.7;
	const brgastro::lensing_tNFW_profile group_profile(M_group,z);

	// Generate set of input points, and each lensing signal
	std::vector< std::vector<std::string> > data(6);
	for(double R=10*unitconv::kpctom; R <= 2000.*unitconv::kpctom; R += 10*unitconv::kpctom)
	{
		std::stringstream ss;
		data[0].push_back("");
		ss.str("");
		ss << R*unitconv::mtokpc;
		data[1].push_back(ss.str());
		ss.str("");
		ss << satellite_profile.quick_WLsig(R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtokpc));
		data[2].push_back(ss.str());
		ss.str("");
		ss << group_profile.offset_WLsig(R,offset_R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtokpc));
		data[3].push_back(ss.str());
		ss.str("");
		ss << group_profile.quick_offset_WLsig(R,offset_R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtokpc));
		data[4].push_back(ss.str());
		ss.str("");
		ss << group_profile.quick_group_WLsig(R, group_c)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtokpc));
		data[5].push_back(ss.str());
	}

	std::vector< std::string > header;
	header.push_back("#");
	header.push_back("R");
	header.push_back("Sat_sig");
	header.push_back("Offset_group_sig_noncache");
	header.push_back("Offset_group_sig");
	header.push_back("Averaged_group_sig");

	std::string cache_test_name = "tNFW_cache_test.dat";
	std::string sig_cache_name = "sig_cache.dat";
	std::string offset_sig_cache_name = "offset_sig_cache.dat";
	std::string group_sig_cache_name = "group_sig_cache.dat";

	std::ofstream out;

	brgastro::open_file(out,cache_test_name);
	brgastro::print_table(out,header.size(),data[0].size(),header,data);

	brgastro::open_file(out,sig_cache_name);
	brgastro::tNFW_sig_cache().print(out);

	brgastro::open_file(out,offset_sig_cache_name);
	brgastro::tNFW_offset_sig_cache().print(out);

	brgastro::open_file(out,group_sig_cache_name);
	brgastro::tNFW_group_sig_cache().print(out);

	return 0;
}
