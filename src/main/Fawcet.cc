/**
 * @project Fawcet
 * @file src/main/FawcetMain.cc
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
 *  FawcetMain.cc :  main exec
 *
 */
#define FWCT_MAIN_EXE_NAME "fawcet_main"
#define FWCT_MAIN_EXE_VERSION "1.0.0"
#define FWCT_MAIN_EXE_COPYRIGHT "Copyright (c) 2018"

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include "utils/BaseUtils.hpp"

/* GFlags Settings Start */
DEFINE_bool(h, false, "Show help");
DECLARE_bool(help);
DECLARE_bool(helpshort);
static bool IsNonEmptyMessage(const char *flagname, const std::string &value)
{
	return ( value[0] != '\0' );
}
static bool IsValidBtype(const char *flagname, const std::string &value)
{
	return (value=="bbox");
}

// btype
DEFINE_string(btype, "", "type bbox");
DEFINE_validator(btype, &IsValidBtype);
// bbox
DEFINE_string(data, "", "type data");
DEFINE_validator(data, &IsNonEmptyMessage);
// osrm files
DEFINE_string(mldfile, "", "osrm file mld format ");
DEFINE_string(chfile, "", "osrm file ch format");
// precision
DEFINE_uint64(prec, FWCT_DEFAULT_GEOHASH_PRECISION, "precision");
// chunksize
DEFINE_uint64(chunksize, FWCT_MAX_STEP_SIZE, "chunk size");

/* GFlags Settings End */

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include "geohash/GeoHashHelper.hpp"
#include "routing/OsrmHelper.hpp"

void HandleBbox(double left, double bottom, double right, double top);

/** main */
int main(int argc, char *argv[])
{

	/** GFlags **/
	std::string usage("The program umps geohash to geohash distance.  Sample usage:\n");
	usage += std::string(argv[0]) + " -btype bbox -prec 5 -data 77.469,12.813,77.789,13.085 --mldfile filename.orsm \n" ;
	gflags::SetUsageMessage(usage);
	gflags::SetVersionString(FWCT_MAIN_EXE_VERSION);

	// read command line
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	/** Logging **/
	google::InitGoogleLogging(argv[0]);

	try {

		if ( FLAGS_mldfile.empty() == FLAGS_chfile.empty() )
			throw fwct::InitialException("one of mldfile or chfile needed");

		if (FLAGS_btype=="bbox") {
			std::vector<std::string> strs;
			boost::split(strs,FLAGS_data,boost::is_any_of(", "));
			if (strs.size()!=4)
				throw fwct::InitialException("needs 4 values");
			HandleBbox(
			    std::strtod(strs[0].c_str(), nullptr),
			    std::strtod(strs[1].c_str(), nullptr),
			    std::strtod(strs[2].c_str(), nullptr),
			    std::strtod(strs[3].c_str(), nullptr)
			);
		}
		else {
			throw fwct::InitialException("Unknown btype");
		}
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

	gflags::ShutDownCommandLineFlags();
	return 0;
}

void HandleBbox(double left, double bottom, double right, double top)
{

	if ( FLAGS_mldfile.empty() == FLAGS_chfile.empty() )
			throw fwct::BadCodeException("Only one of MLD or CH required");

	bool use_ch=(!FLAGS_chfile.empty());
	std::string use_file = ( use_ch ) ? FLAGS_chfile : FLAGS_mldfile;

	fwct::routing::OsrmHelper he{use_file,use_ch};
	fwct::geohash::GeoHashHelper gh;
	fwct::routing::OsrmHelper::NeighbourT hashes = gh.GetBbox(left,bottom,right,top,FLAGS_prec);
	he.GetResults(&hashes,
	[](std::string src, std::string trg, double val) {
		std::cout << src << "_" << trg << " " << val << std::endl;
	},
	FLAGS_chunksize);
}
