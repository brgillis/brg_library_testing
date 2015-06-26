/**********************************************************************\
 @file main_labeled_array_tester.cpp
 ------------------

 TODO <Insert file description here>

 **********************************************************************

 Copyright (C) 2015  Bryan R. Gillis

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

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <Eigen/Core>

#include "brg/container/labeled_array.hpp"

/**
 * TODO (description)
 *
 * @param TODO (params to be passed at command-line)
 * @return
 */
int main( const int argc, const char *argv[] )
{
	

	std::vector<std::vector<double>> rows;
	rows.push_back(std::vector<double>({1,4}));
	rows.push_back(std::vector<double>({2,5}));

	std::vector<std::string> header({"col_A","col_2"});

	brgastro::labeled_array<double,Eigen::RowMajor> test_array(rows,header);

	test_array.insert_row(std::vector<double>({3,6}));
	test_array.insert_col(std::vector<double>({7,8,9}));
	test_array.insert_col(std::vector<double>({10,11,12}));

	test_array["new_col"] = Eigen::Array3d({13.,14.,15.});

	const auto & const_array = test_array;

	for(const auto & row : test_array.rows())
	{
		for(const auto & elem : row )
			std::cout << elem << "\t";
//		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : const_array.rows())
	{
		for(const auto & elem : row )
			std::cout << elem << "\t";
//		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : test_array.reverse_rows())
	{
		for(const auto & elem : row )
			std::cout << elem << "\t";
//		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : const_array.reverse_rows())
	{
		for(const auto & elem : row )
			std::cout << elem << "\t";
//		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : test_array.cols())
	{
		for(const auto & elem : col )
			std::cout << elem << "\t";
//		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : const_array.cols())
	{
		for(const auto & elem : col )
			std::cout << elem << "\t";
//		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : test_array.reverse_cols())
	{
		for(const auto & elem : col )
			std::cout << elem << "\t";
//		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : const_array.reverse_cols())
	{
		for(const auto & elem : col )
			std::cout << elem << "\t";
//		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : test_array.raw_rows())
	{
//		for(const auto & elem : row )
//			std::cout << elem << "\t";
		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : const_array.raw_rows())
	{
//		for(const auto & elem : row )
//			std::cout << elem << "\t";
		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : test_array.reverse_raw_rows())
	{
//		for(const auto & elem : row )
//			std::cout << elem << "\t";
		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & row : const_array.reverse_raw_rows())
	{
//		for(const auto & elem : row )
//			std::cout << elem << "\t";
		std::cout << row;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : test_array.raw_cols())
	{
//		for(const auto & elem : col )
//			std::cout << elem << "\t";
		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : const_array.raw_cols())
	{
//		for(const auto & elem : col )
//			std::cout << elem << "\t";
		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : test_array.reverse_raw_cols())
	{
//		for(const auto & elem : col )
//			std::cout << elem << "\t";
		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(const auto & col : const_array.reverse_raw_cols())
	{
//		for(const auto & elem : col )
//			std::cout << elem << "\t";
		std::cout << col;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << std::endl << std::endl;

	test_array.col(1).raw() *= 100;
	test_array(1).raw() += 10;

	for(const auto & row : test_array.rows())
	{
		for(const auto & elem : row )
			std::cout << elem << "\t";
//		std::cout << row;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	std::cout << test_array.base();

	return 0;
}
