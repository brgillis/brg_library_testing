/**********************************************************************\
 @file main.cpp
 ------------------

 TODO <Insert file description here>

 **********************************************************************

 Copyright (C) 2014  Bryan R. Gillis

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

\**********************************************************************/

#include <cmath>
#include <iostream>

#include "IceBRG_physics/astro.h"
#include "IceBRG_lensing/lensing_tNFW_profile.h"

#include "IceBRG_lensing/pair_binner.h"
//#include "IceBRG_physics/sky_obj/galaxy.h"

#include "IceBRG_main/math/calculus/integrate.hpp"
#include "IceBRG_main/units/unit_conversions.hpp"
#include "IceBRG_main/units/units.hpp"

/**
 * TODO (description)
 *
 * @param TODO (params to be passed at command-line)
 * @return
 */
int main( const int argc, const char *argv[] )
{
	using namespace IceBRG;

//	galaxy bl;
//
//	skydist2d(&bl,&bl);

	pair_binner();

	const mass_type M0 = runtime_ipow(10.,14)*unitconv::Msuntokg*kg;
	const double z = 0;

	lensing_tNFW_profile mass_prof(M0,z);
	lensing_tNFW_profile sat_prof(M0,z,2.5);

	const distance_type R_max = mass_prof.rvir();

	auto sat_dens = [&] (const distance_type & r)
	{
		return value_of(4.*pi*r*r * sat_prof.dens(r));
	};

	auto host_dens = [&] (const distance_type & r)
	{
		return mass_prof.dens(r);
	};

	const auto mean_overdens = IceBRG::integrate_weighted_Romberg(host_dens,sat_dens,0.01*R_max,2.5*R_max);
	const auto background_dens = IceBRG::redshift_obj(0).rho_crit();

	std::cout << "Mean overdensity where satellites are is: " << mean_overdens/background_dens << std::endl;

	return 0;
}
