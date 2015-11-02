#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <new>
#include <fstream>

#include "IceBRG_main/global.h"

#include "IceBRG_main/math/calculus/DE.hpp"
#include "IceBRG_main/physics/astro.h"
#include "IceBRG_main/physics/SALTSA/stripping_orbit.h"
#include "IceBRG_main/physics/density_profile/density_profile.h"
#include "IceBRG_main/physics/density_profile/density_profile_functors.h"
#include "IceBRG_main/physics/density_profile/tNFW_profile.h"
#include "IceBRG_main/physics/density_profile/point_mass_profile.h"
#include "IceBRG_main/physics/units/unit_conversions.hpp"

int main( const int argc, const char *argv[] )
{
	const int orbit_resolution = 10000000;
	const int stripping_resolution = 1000;
	const int spline_points = 100000;
	const int spline_skip = brgastro::max(orbit_resolution/spline_points,1);

	double host_z = 0, satellite_z = host_z;
	double host_c = 17.3, satellite_c = 17.3; // From Taylor
//	double host_c = 8, satellite_c = 16; // Kamiab C
//	double host_c = 4, satellite_c = 4; // Kamiab E

	const BRG_MASS host_mass = std::pow(10,14)*brgastro::unitconv::Msuntokg;
	const BRG_MASS satellite_mass = std::pow(10,11)*brgastro::unitconv::Msuntokg; // Taylor

//	const BRG_MASS host_mass = 512*std::pow(10,9)*brgastro::unitconv::Msuntokg;
//	const BRG_MASS satellite_mass = std::pow(10,9)*brgastro::unitconv::Msuntokg; // Kamiab C

//	const BRG_MASS host_mass = 64*std::pow(10,9)*unitconv::Msuntokg;
//	const BRG_MASS satellite_mass = std::pow(10,9)*unitconv::Msuntokg; // Kamiab E

	brgastro::tNFW_profile host_group_val( host_mass, host_z, host_c), *host_group = &host_group_val;
	brgastro::point_mass_profile host_group_pm_val( host_mass, host_z );
	brgastro::density_profile *host_group_orbit = &host_group_pm_val; // Taylor
//	brgastro::density_profile *host_group_orbit = &host_group_val; // Kamiab

	brgastro::tNFW_profile test_satellite_val( satellite_mass, satellite_z, satellite_c), *test_satellite = &test_satellite_val;

	const double num_periods = 3; // Taylor
//	const double num_periods = 4.25; // Kamiab

	BRG_DISTANCE x, y, z;
	BRG_VELOCITY vx, vy, vz;
	BRG_TIME t;
	const BRG_TIME total_time = num_periods*host_group->otvir(), t_step = total_time/orbit_resolution;
	std::vector< BRG_UNITS > host_parameters;

	// Print the period in Gyr

	std::cout << "Period = " << host_group->otvir()*brgastro::unitconv::stoGyr << std::endl;

	brgastro::phase current_phase;
	brgastro::accel_functor accel_func;

	std::ofstream out;
	std::stringstream ss;

	const std::string orbital_file_name_base = "stripping_test_orbital"; // Taylor
//	const std::string orbital_file_name_base = "stripping_test_orbitalC"; // Kamiab C
//	const std::string orbital_file_name_base = "stripping_test_orbitalE"; // Kamiab E
	const std::string orbital_file_name_tail = ".dat";
	std::string orbital_file_name = "";
	const double orbital_v_factor = 1;

	const int num_orbital_circs = 13;
	double orbital_circularity[num_orbital_circs] = {1, .99, .95, .9, .8, .7, .6, .5, .4, .3, .2, .1, .05};

//	const int num_orbital_circs = 2;
//	double orbital_circularity[num_orbital_circs] = {.9,.6};

	brgastro::stripping_orbit test_orbit;

	test_orbit.set_default_step_length_power(3.);
	test_orbit.set_default_step_factor_min(0.001);

	// Tuning parameters for best fit with Taylor
	test_orbit.set_default_tidal_stripping_amplification(1.825,true);
	test_orbit.set_default_tidal_stripping_deceleration(0.025,true);
	test_orbit.set_default_tidal_stripping_radialness(0,true);
	test_orbit.set_default_tidal_shocking_amplification(3.0,true);

	// Tuning parameters to match what Taylor used
//	test_orbit.set_default_tidal_stripping_amplification(0.666,true);
//	test_orbit.set_default_tidal_stripping_deceleration(0,true);

	// Tuning parameters to match what Kamiab used
//	test_orbit.set_default_tidal_stripping_amplification(0.666,true);
//	test_orbit.set_default_tidal_stripping_deceleration(0,true);
//	test_orbit.set_default_tidal_shocking_amplification(3.4,true);
//	test_orbit.set_default_tidal_shocking_power(-2.5,true);

	// Tuning parameters for best fit with Kamiab
//	test_orbit.set_default_tidal_stripping_amplification(3.5,true);
//	test_orbit.set_default_tidal_stripping_deceleration(-1.2,true);
//	test_orbit.set_default_tidal_stripping_radialness(0,true);
//	test_orbit.set_default_tidal_shocking_amplification(3.4,true);
//	test_orbit.set_default_tidal_shocking_power(-2.5,true);


	const std::string infall_file_name_base = "stripping_test_infall";
	const std::string infall_file_name_tail = ".dat";
	std::string infall_file_name = "";
	const double infall_v_factor = 0.25;
	const int num_infall_circs = 5;
	double infall_circularity[num_infall_circs] = {0.25, 0.20, 0.15, 0.10, 0.05};

//	Vectors for setting up which parameters of the satellite we'll output
	int num_satellite_output_parameters = 4;
	std::vector<bool> satellite_output_parameters(num_satellite_output_parameters,false);
	std::vector<double> satellite_output_parameter_unitconvs(num_satellite_output_parameters,1);

//	Vectors for setting up which parameters of the host we'll output
	int num_host_output_parameters = 4;
	std::vector<bool> host_output_parameters(num_host_output_parameters,false);
	std::vector<double> host_output_parameter_unitconvs(num_host_output_parameters,1);

	satellite_output_parameters.at(3) = true; // Will output tau

	host_output_parameters.at(0) = true; // Will output mvir0
	host_output_parameters.at(1) = true; // Will output z
	host_output_parameters.at(2) = true; // Will output c
	host_output_parameters.at(3) = true; // Will output tau

	host_output_parameter_unitconvs.at(0) = brgastro::unitconv::ttMsuntokg; // Will output mvir0 in 10^10 Msun

	accel_func.set_host_ptr(host_group_orbit);

	host_parameters = host_group->get_parameters();

	for(int i = 0; i < num_orbital_circs; i += 1)
	{

        double a = acos(orbital_circularity[i]/brgastro::safe_d(orbital_v_factor));

		test_orbit.clear();
		test_orbit = brgastro::stripping_orbit(host_group, test_satellite);
		test_orbit.set_satellite_output_parameters(satellite_output_parameters);
		test_orbit.set_host_output_parameters(host_output_parameters);
		test_orbit.set_host_parameter_unitconvs(host_output_parameter_unitconvs);

		x = 0;
		y = host_group_orbit->rvir();
		z = 0;

		vx = host_group_orbit->vvir()*orbital_v_factor*cos(a);
		vy = -host_group_orbit->vvir()*orbital_v_factor*sin(a); // Taylor
//		vy = host_group_orbit->vvir()*orbital_v_factor*sin(a); // Kamiab
		vz = 0;

		t = 0;

		bool start_recording = true; // Taylor
//		bool start_recording = false; // Kamiab

		current_phase.set_phase(x,y,z,vx,vy,vz,t);
		for( int j=0; j < orbit_resolution; j++)
		{
		    if(!start_recording)
		    {
		    	double vrr = brgastro::dot_product( current_phase.vx,
					current_phase.vy, current_phase.vz,
					current_phase.x, current_phase.y,
					current_phase.z );
		    	if(vrr<=0)
		    	{
		    		t = 0;
		    		start_recording=true;
		    	}
		    }


			if( start_recording && brgastro::divisible(j,spline_skip) )
			{
				test_orbit.force_add_point(current_phase.x, current_phase.y, current_phase.z,
                         current_phase.vx, current_phase.vy, current_phase.vz, t);
			}

			// Integrate to next point with leapfrog method
			brgastro::leapfrog_step( current_phase, t_step, &accel_func );
			t += t_step;
		}

//		test_orbit.add_discontinuity_time(total_time/3.);
//		test_orbit.add_discontinuity_time(2.*total_time/3.);

		ss.str("");
		ss << orbital_file_name_base << i << orbital_file_name_tail;
		orbital_file_name = ss.str();

		std::cout << "Working on " << orbital_file_name << "... " << std::flush;

		test_orbit.set_record_full_data(true);
		test_orbit.set_resolution(stripping_resolution);

		try
		{
			test_orbit.calc();
		}
		catch(const std::runtime_error &e)
		{
			std::cerr << "ERROR in calculating stripping for orbit:\n" << e.what() << std::endl;
		}

		out.clear();
		out.open(orbital_file_name.c_str());
		test_orbit.print_full_data( &out );
		out.close();

		std::cout << "Done!\n";

		test_orbit.clear();
	}

	return 0;

	for(int i = 0; i < num_infall_circs; i += 1)
	{

        double a = acos(infall_circularity[i]/brgastro::safe_d(infall_v_factor));

		test_orbit.clear();
		test_orbit = brgastro::stripping_orbit(host_group, test_satellite);
		test_orbit.set_satellite_output_parameters( satellite_output_parameters);
		test_orbit.set_host_output_parameters( host_output_parameters);
		test_orbit.set_host_parameter_unitconvs( host_output_parameter_unitconvs);

		x = 0;
		y = host_group->rvir();
		z = 0;

		vx = host_group->vvir()*infall_v_factor*cos(a);
		vy = -host_group->vvir()*infall_v_factor*sin(a);
		vz = 0;

		current_phase.set_phase(x,y,z,vx,vy,vz,0);

		t = 0;
		for( int j=0; j < orbit_resolution; j++)
		{
		    current_phase.t = t;

			if( brgastro::divisible(j,spline_skip) )
				test_orbit.add_point(current_phase.x, current_phase.y, current_phase.z, current_phase.t);

			// Integrate to next point with leapfrog method
			leapfrog_step( current_phase, t_step, &accel_func );
			t += t_step;
		}

		ss.str("");
		ss << infall_file_name_base << i << infall_file_name_tail;
		infall_file_name = ss.str();

		std::cout << "Working on " << infall_file_name << "... " << std::flush;

		test_orbit.set_record_full_data(true);

		try
		{
			test_orbit.calc();
		}
		catch(const std::runtime_error &e)
		{
			std::cerr << "ERROR in calculating stripping for orbit:\n" << e.what() << std::endl;
		}

		out.clear();
		out.open(infall_file_name.c_str());
		test_orbit.print_full_data( &out );
		out.close();

		std::cout << "Done!\n";

		test_orbit.clear();
	}

	return 0;
}
