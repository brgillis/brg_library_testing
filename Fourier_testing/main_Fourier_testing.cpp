/*
 * main_Fourier_testing.cpp
 *
 *  Created on: 11 May 2015
 *      Author: brg
 */

#include <iostream>

#include <boost/optional.hpp>
#include <Eigen/Core>

#include <brg/math/Fourier.hpp>
#include <brg/math/random/distributions.hpp>


int main( const int argc, const char *argv[] )
{
	// Let's start by testing a Gaussian, and see if the result also looks like a Gaussian

	int N = 1024;
	double max = 10;

	boost::optional<brgastro::fftw_wisdom_accumulator> wisdom = brgastro::fftw_wisdom_accumulator();

	Eigen::ArrayXd x_vals = Eigen::ArrayXd::LinSpaced(N,0,max) + max/(2*(N-1));

	auto get_Gaus = [max] (const double & val) {return brgastro::Gaus_pdf(val,0,max/5);};

	Eigen::ArrayXd input_Gaussian_vals = x_vals.unaryExpr(get_Gaus);

	auto complex_output_vals = brgastro::Fourier_transform(get_Gaus,max/(2*(N-1)),max+max/(2*(N-1)),N,wisdom);

	auto recovered_input_vals = brgastro::inverse_Fourier_transform(complex_output_vals,wisdom);

//	std::cout << complex_output_vals.transpose() << std::endl;
//	std::cout << input_Gaussian_vals.transpose() << std::endl;
//	std::cout << recovered_input_vals.transpose() << std::endl;
	std::cout << (recovered_input_vals/input_Gaussian_vals).transpose() << std::endl << std::endl;

	auto output_vals = brgastro::spherical_Fourier_transform(get_Gaus,max,N,wisdom);

	recovered_input_vals = brgastro::inverse_spherical_Fourier_transform(output_vals,wisdom);

//	std::cout << output_vals.transpose() << std::endl;
//	std::cout << input_Gaussian_vals.transpose() << std::endl;
//	std::cout << recovered_input_vals.transpose() << std::endl;
	std::cout << (recovered_input_vals/input_Gaussian_vals).transpose() << std::endl << std::endl;

	output_vals = brgastro::Fourier_sin_transform(get_Gaus,max,N,wisdom);

	recovered_input_vals = brgastro::inverse_Fourier_sin_transform(output_vals,wisdom);

//	std::cout << output_vals.transpose() << std::endl;
//	std::cout << input_Gaussian_vals.transpose() << std::endl;
//	std::cout << recovered_input_vals.transpose() << std::endl;
	std::cout << (recovered_input_vals/input_Gaussian_vals).transpose() << std::endl;
}
