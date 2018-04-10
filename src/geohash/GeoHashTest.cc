/**
 * @project Fawcet
 * @file src/geohash/GeoHashTest.cc
 * @author  S Roychowdhury <sroycode AT gmail DOT com>
 * @version 1.0.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2018 S Roychowdhury.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 *  GeoHashTest.cc :  geohash test
 *
 */
#include "utils/BaseUtils.hpp"
#include "geohash/GeoHashHelper.hpp"

#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/algorithm/string.hpp>

/** main */
int main(int argc, const char *argv[])
{

	/** Logging **/
	// google::InitGoogleLogging(argv[0]);
	try {
		double center_lat = 23.7963;
		double center_lon = 77.4975;
		size_t prec=5;
		auto lat_interval = fwct::geohash::GeoHashLatPrecs[prec-1];
		auto lon_interval = fwct::geohash::GeoHashLonPrecs[prec-1];
		size_t steps=2;

		// bbox
		fwct::geohash::GeoHashHelper gh;
		double left = center_lon-(lon_interval*(double)steps);
		double bottom = center_lat-(lat_interval*(double)steps);
		double right = center_lon+(lon_interval*(double)steps);
		double top = center_lat+(lat_interval*(double)steps);
		
		auto bbox = gh.GetBbox(left,bottom,right,top, prec);
		std::cout <<  "Bbox : " << left << "," << bottom << "," << right << "," << top << std::endl;
		size_t counter=0;
		for (auto& bb : bbox) {
			std::cout <<  bb << " ";
			if (++counter%10==0) std::cout << std::endl;
		}
		std::cout << std::endl;

		// neighbours
		
		auto hash = gh.Encode(center_lat,center_lon,prec);
		std::cout <<  "Center : " << hash << " " << center_lon << "," << center_lat << std::endl;
		counter=0;
		auto neigh = gh.GetNeighbours(hash,2);
		for (auto& bb : neigh) {
			std::cout <<  bb << " ";
			if (++counter%10==0) std::cout << std::endl;
		}
		std::cout << std::endl;

		// two points
		std::cout <<  "Points : " 
		<< gh.Encode(12.935416, 77.614435,7) << " "
		<< gh.Encode(13.044424, 77.621632, 7) << std::endl;

	}
	catch(fwct::JdException& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Error: UNKNOWN " << std::endl;
	}

	return 0;
}

