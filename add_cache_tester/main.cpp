/**       @file main.cpp
 *
 *     Project: add_cache_tester
 *        Path: /add_cache_tester/main.cpp
 *
 *  Created on: 29 Jul 2014
 *      Author: brg
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#include "brg/brg_astro.h"
#include "brg/brg_vector.hpp"


int main( const int argc, const char *argv[] )
{
	double z1_vals[] = {0,0.01,0.02,0.03,0.04,0.045};
	double z2_vals[] = {0,0.01,0.02,0.04};

	brgastro::vector<double> z1 = z1_vals;
	brgastro::vector<double> z2 = z2_vals;

	brgastro::vector<double>::shape_t results_shape;

	results_shape.push_back(z1.size());
	results_shape.push_back(z2.size());

	brgastro::vector<double> results(results_shape,0);

	for(size_t z1_i=0; z1_i<z1.size(); ++z1_i)
	{
		for(size_t z2_i=0; z2_i<z2.size(); ++z2_i)
		{
			brgastro::vector<double>::shape_t position;
			position.push_back(z1_i);
			position.push_back(z2_i);
			results(position) = brgastro::ad_distance(z1[z1_i],z2[z2_i]);
		}
	}

	std::vector< std::vector<std::string> > data(z1.size()+1);
	for(size_t i=0; i<data.size(); ++i) data[i].resize(z2.size()+1);

	std::stringstream ss;
	data[0][0] = "";
	for(size_t i=1;i<data.size();++i)
	{
		ss.str("");
		ss << z1[i-1];
		data[i][0] = ss.str();
	}
	for(size_t i=1;i<data[0].size();++i)
	{
		ss.str("");
		ss << z2[i-1];
		data[0][i] = ss.str();
	}


	for(size_t z1_i=1; z1_i<data.size(); ++z1_i)
	{
		for(size_t z2_i=1; z2_i<data[0].size(); ++z2_i)
		{
			brgastro::vector<double>::shape_t position;
			position.push_back(z1_i-1);
			position.push_back(z2_i-1);
			ss.str("");
			ss << results.at(position);
			data[z1_i][z2_i] = ss.str();
		}
	}

	brgastro::print_table(std::cout,data.size(),data[0].size(), data);

	return 0;
}
