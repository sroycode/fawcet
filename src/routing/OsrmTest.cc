/**
 * @project Fawcet
 * @file src/routing/OsrmTest.cc
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
 *  OsrmTest.cc :  geohash test
 *
 */
#include "utils/BaseUtils.hpp"
#include "routing/OsrmHelper.hpp"

#include <iostream>

/** main */
int main(int argc, const char *argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " data.osrm\n";
		std::cerr << "Usage: " << argv[0] << " data.osrm use_ch\n";
		return EXIT_FAILURE;
	}
	bool use_ch = (argc >= 3);

	/** Logging **/
	google::InitGoogleLogging(argv[0]);

	try {
		// bbox
		fwct::routing::OsrmHelper he{argv[1],use_ch};
		fwct::geohash::GeoHashHelper gh;
		fwct::routing::OsrmHelper::NeighbourT hashes = gh.GetBbox(77.272,12.595,78.008,13.267,5);
		DLOG(INFO) << "Hashes size = " << hashes.size();
		he.GetResults(&hashes,
		[](std::string src, std::string trg, double val) {
			std::cout << src << "_" << trg << " " << val << std::endl;
		},
		100);
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

