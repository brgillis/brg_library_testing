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

#include "brg/file_access/ascii_table_map.hpp"
#include "brg/file_access/open_file.hpp"
#include "brg_lensing/lensing_tNFW_profile.h"
#include "brg_lensing/lensing_tNFW_caches.h"
#include "brg_physics/units/unit_conversions.hpp"

int main( const int argc, const char *argv[] )
{
	namespace unitconv = brgastro::unitconv;

	// Redshift
	const double z = 0.303;

	// Satellite
	const double M_sat = 1.3e11*unitconv::Msuntokg;
	const double offset_R = 500*unitconv::kpctom;
	const brgastro::lensing_tNFW_profile satellite_profile(M_sat,z);

	// Group
	const double M_group = 2.1e14*unitconv::Msuntokg;
	const double group_c = 2.5;
	const brgastro::lensing_tNFW_profile group_profile(M_group,z);

	// Generate set of input points, and each lensing signal
	std::vector< std::vector<std::string> > data(10);
	for(double R=10*unitconv::kpctom; R <= 20000.*unitconv::kpctom; R += 10*unitconv::kpctom)
	{
		std::stringstream ss;
		data[0].push_back("");
		ss.str("");
		ss << std::setprecision(4) << R*unitconv::mtokpc;
		data[1].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.quick_WLsig(R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[2].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.WLsig(R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[3].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.quick_offset_WLsig(R,offset_R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[4].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.offset_WLsig(R,offset_R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[5].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.quick_group_WLsig(R, group_c)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[6].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.semiquick_group_WLsig(R, group_c)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[7].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.quick_shifted_WLsig(R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[8].push_back(ss.str());
		ss.str("");
		ss << std::setprecision(15) << group_profile.shifted_WLsig(R)*unitconv::kgtoMsun/(brgastro::square(unitconv::mtopc));
		data[9].push_back(ss.str());
	}

	std::vector< std::string > header;
	header.push_back("#");
	header.push_back("R");
	header.push_back("Sat_sig");
	header.push_back("Sat_sig_noncache");
	header.push_back("Offset_group_sig");
	header.push_back("Offset_group_sig_noncache");
	header.push_back("Averaged_offset_group_sig");
	header.push_back("Averaged_offset_group_sig_noncache");
	header.push_back("Shifted_sat_sig");
	header.push_back("Shifted_sat_sig_noncache");

	std::string cache_test_name = "tNFW_cache_test.dat";
	std::string sig_cache_name = "sig_cache.dat";
	std::string offset_sig_cache_name = "offset_sig_cache.dat";
	std::string group_sig_cache_name = "group_sig_cache.dat";
	std::string shifted_sig_cache_name = "shifted_sig_cache.dat";

	std::ofstream out;

	brgastro::open_file(out,cache_test_name);
	brgastro::print_table(out,data,header);

	brgastro::open_file(out,sig_cache_name);
	brgastro::tNFW_sig_cache().print(out);

	brgastro::open_file(out,offset_sig_cache_name);
	brgastro::tNFW_offset_sig_cache().print(out);

	brgastro::open_file(out,group_sig_cache_name);
	brgastro::tNFW_group_sig_cache().print(out);

	brgastro::open_file(out,shifted_sig_cache_name);
	brgastro::tNFW_shifted_sig_cache().print(out);

	return 0;
}
