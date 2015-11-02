/*
 * main_Fourier_testing.cpp
 *
 *  Created on: 11 May 2015
 *      Author: brg
 */

#include <cmath>
#include <iostream>

#include <boost/optional.hpp>
#include <Eigen/Core>

#include "IceBRG_main/container/labeled_array.hpp"
#include "IceBRG_main/math/Fourier/transform.hpp"
#include "IceBRG_main/math/Fourier/radial_vector.hpp"
#include "IceBRG_main/math/random/distributions.hpp"


int main( const int argc, const char *argv[] )
{
	// Let's start by testing a Gaussian, and see if the result also looks like a Gaussian

	int N = 20480;
	double scale = 10.;

	int new_N = 20480;
	double new_scale = 10.;

	double stddev_rat = 10;

	double function_scale = scale/stddev_rat;

	std::cout << "N = " << N << ". scale = " << scale << ". function_scale = " << function_scale << "." << std::endl;

	IceBRG::labeled_array<double> result_array;

	IceBRG::fftw_wisdom_accumulator wisdom;

	auto get_Gaus = [&function_scale] (const double & val)
			{return IceBRG::Gaus_pdf(val,0,function_scale);};
	auto get_Gaus_rt2 = [&function_scale] (const double & val)
			{return IceBRG::Gaus_pdf(val,0,function_scale*std::sqrt(2.));};

	auto get_tophat = [&function_scale] (const double & val)
	{
		if(std::fabs(val)>function_scale) return 0.;
		return 1./(2*function_scale);
	};

	auto get_spherical_Gaus = [&function_scale] (const double & val)
			{return IceBRG::spherical_Gaus_pdf(val,function_scale);};
	auto get_spherical_Gaus_rt2 = [&function_scale] (const double & val)
			{return IceBRG::spherical_Gaus_pdf(val,function_scale*std::sqrt(2.));};

	auto get_spherical_tophat = [&function_scale] (const double & val)
	{
		if(std::fabs(val)>function_scale) return 0.;
		return 3./(4.*M_PI*IceBRG::cube(function_scale));
	};

	Eigen::ArrayXd x_vals = Eigen::ArrayXd::LinSpaced(N,-scale,scale);
	Eigen::ArrayXd linear_input_Gaussian_vals = x_vals.unaryExpr(get_Gaus);
	Eigen::ArrayXd linear_input_tophat_vals = x_vals.unaryExpr(get_tophat);
	Eigen::ArrayXd expected_linear_convolved_Gaussian_vals = x_vals.unaryExpr(get_Gaus_rt2);

	result_array.insert_col(std::make_pair("x_vals",x_vals));
	result_array.insert_col(std::make_pair("linear_input_Gaussian_vals",linear_input_Gaussian_vals));
	result_array.insert_col(std::make_pair("linear_input_tophat_vals",linear_input_tophat_vals));
	result_array.insert_col(std::make_pair("expected_linear_convolved_Gaussian_vals",expected_linear_convolved_Gaussian_vals));

	auto linear_Gaussian_k_vals = IceBRG::Fourier_transform(linear_input_Gaussian_vals,wisdom);
	auto linear_tophat_k_vals = IceBRG::Fourier_transform(linear_input_tophat_vals,wisdom);

	Eigen::ArrayXd linear_recovered_convolved_Gaussian_vals = IceBRG::inverse_Fourier_transform(
			static_cast<IceBRG::complex_array_type>(linear_Gaussian_k_vals*linear_Gaussian_k_vals*2/N),
			wisdom)*scale;

	Eigen::ArrayXd linear_recovered_convolved_tophat_vals = IceBRG::inverse_Fourier_transform(
			static_cast<IceBRG::complex_array_type>(linear_tophat_k_vals*linear_tophat_k_vals*2/N),
			wisdom)*scale;

	result_array.insert_col(std::make_pair("linear_Gaussian_k_vals_real",linear_Gaussian_k_vals.real()));
	result_array.insert_col(std::make_pair("linear_Gaussian_k_vals_imag",linear_Gaussian_k_vals.imag()));
	result_array.insert_col(std::make_pair("linear_tophat_k_vals_real",linear_tophat_k_vals.real()));
	result_array.insert_col(std::make_pair("linear_tophat_k_vals_imag",linear_tophat_k_vals.imag()));

	result_array.insert_col(std::make_pair("linear_recovered_convolved_Gaussian_vals",linear_recovered_convolved_Gaussian_vals));
	result_array.insert_col(std::make_pair("linear_recovered_convolved_tophat_vals",linear_recovered_convolved_tophat_vals));

	// Set up spherical vectors now
	Eigen::ArrayXd r_vals = Eigen::ArrayXd::LinSpaced(N,0,scale) + scale/(2*(N-1));
	IceBRG::radial_vector spherical_input_Gaussian_vals(r_vals.unaryExpr(get_spherical_Gaus),
			scale,wisdom);
	IceBRG::radial_vector spherical_input_tophat_vals(r_vals.unaryExpr(get_spherical_tophat),
			scale,wisdom);
	IceBRG::radial_vector expected_spherical_convolved_Gaussian_vals(
			r_vals.unaryExpr(get_spherical_Gaus_rt2),scale,wisdom);

	// Get rescaled versions
	IceBRG::radial_vector rescaled_spherical_input_Gaussian_vals(
			spherical_input_Gaussian_vals.get_rescaled(new_scale,new_N));
	IceBRG::radial_vector rescaled_spherical_input_tophat_vals(
			spherical_input_tophat_vals.get_rescaled(new_scale,new_N));

	result_array.insert_col(std::make_pair("r_vals",r_vals));
	result_array.insert_col(std::make_pair("spherical_input_Gaussian_vals",
			spherical_input_Gaussian_vals.get_r_array()));
	result_array.insert_col(std::make_pair("spherical_input_tophat_vals",
			spherical_input_tophat_vals.get_r_array()));
	result_array.insert_col(std::make_pair("expected_spherical_convolved_Gaussian_vals",
			expected_spherical_convolved_Gaussian_vals.get_r_array()));

	result_array.insert_col(std::make_pair("spherical_input_Gaussian_k_vals",spherical_input_Gaussian_vals.get_k_array()));
	result_array.insert_col(std::make_pair("spherical_input_tophat_k_vals",spherical_input_tophat_vals.get_k_array()));
	result_array.insert_col(std::make_pair("expected_spherical_convolved_Gaussian_k_vals",expected_spherical_convolved_Gaussian_vals.get_k_array()));

	IceBRG::radial_vector spherical_recovered_convolved_Gaussian_vals(
			spherical_input_Gaussian_vals.get_convolved_with(rescaled_spherical_input_Gaussian_vals));
	IceBRG::radial_vector spherical_recovered_convolved_tophat_vals(
			spherical_input_tophat_vals.get_convolved_with(rescaled_spherical_input_tophat_vals));

	std::cout << (spherical_input_Gaussian_vals.get_r_array()
			*4*M_PI*r_vals*r_vals).sum()*scale/N << " "
			  << (expected_spherical_convolved_Gaussian_vals.get_r_array()
					  *4*M_PI*r_vals*r_vals).sum()*scale/N << " "
			  << (spherical_recovered_convolved_Gaussian_vals.get_r_array()
					  *4*M_PI*r_vals*r_vals).sum()*scale/N << std::endl;

	std::cout << (spherical_input_tophat_vals.get_r_array()*4*M_PI*r_vals*r_vals).sum()*scale/N << " "
			  << (spherical_recovered_convolved_tophat_vals.get_r_array()*4*M_PI*r_vals*r_vals).sum()*scale/N << std::endl;

	result_array.insert_col(std::make_pair("spherical_recovered_convolved_Gaussian_k_vals",spherical_recovered_convolved_Gaussian_vals.get_k_array()));
	result_array.insert_col(std::make_pair("spherical_recovered_convolved_tophat_k_vals",spherical_recovered_convolved_tophat_vals.get_k_array()));

	result_array.insert_col(std::make_pair("spherical_recovered_convolved_Gaussian_vals",spherical_recovered_convolved_Gaussian_vals.get_r_array()));
	result_array.insert_col(std::make_pair("spherical_recovered_convolved_tophat_vals",spherical_recovered_convolved_tophat_vals.get_r_array()));

//	output_vals = IceBRG::Fourier_sin_transform(get_Gaus,max,N,wisdom);
//
//	linear_recovered_input_vals = IceBRG::inverse_Fourier_sin_transform(
//			static_cast<Eigen::ArrayXd>(output_vals*output_vals/(N*N)),wisdom)*max;
//
//	std::cout << (linear_recovered_input_vals/expected_convolved_Gaussian_vals).transpose() << std::endl;
//
//	result_array.insert_col(std::make_pair("sin_recovered_convolved_input_vals",linear_recovered_input_vals));

	result_array.write("results.dat");

	std::cout << "Done!\n";
}
