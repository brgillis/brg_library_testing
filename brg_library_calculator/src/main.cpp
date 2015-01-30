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

#include "brg_physics/density_profile/tNFW_profile.h"
#include "brg_physics/units/unit_conversions.hpp"

/**
 * TODO (description)
 *
 * @param TODO (params to be passed at command-line)
 * @return
 */
int main( const int argc, const char *argv[] )
{
	constexpr double M0 = std::pow(10,10.1)*brgastro::unitconv::Msuntokg;
	constexpr double z = 0;
	constexpr double R = 6.55*brgastro::unitconv::kpctom;

	brgastro::tNFW_profile prof(M0,z);

	auto tot_mass = prof.mtot();
	auto vir_mass = prof.mvir();
	auto cont_mass = prof.enc_mass(R);

	std::cout << "Fraction of virial mass contained: " << cont_mass/vir_mass << std::endl;
	std::cout << "Fraction of total mass contained: " << cont_mass/tot_mass << std::endl;
	
	return 0;
}
