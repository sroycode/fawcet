/**
 * @project Fawcet
 * @file include/geohash/GeoHashHelper.hpp
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
 *  GeoHashHelper.hpp :  geohash c++ helper headers
 *
 */
#ifndef _FWCT_GEOHASH_GEOHASH_HELPER_HPP_
#define _FWCT_GEOHASH_GEOHASH_HELPER_HPP_

#include <vector>
#include "utils/BaseUtils.hpp"

#define FWCT_NORTH               0
#define FWCT_EAST                1
#define FWCT_SOUTH               2
#define FWCT_WEST                3

#define FWCT_EARTH_RADIUS_M 6371000.8
#define FWCT_EARTH_RADIUS_KM 6371.0
#define FWCT_PI 3.14159265359
#define FWCT_LENGTH_OF_DEGREE  111100        // meters

#define FWCT_MIN_LAT -90.0
#define FWCT_MAX_LAT  90.0
#define FWCT_MIN_LON -180.0
#define FWCT_MAX_LON  180.0

namespace fwct {
class BadGeoHashException : public JdException {
	using JdException::JdException;
};

namespace geohash {

const static std::vector<double> GeoHashLatPrecs {23.0, 2.8, 0.7, 0.087, 0.022, 0.0027, 0.00068, 0.000085};
const static std::vector<double> GeoHashLonPrecs {23.0, 5.6, 0.7, 0.18, 0.022, 0.0055, 0.00068, 0.00017};
const static std::vector<double> GeoHashKmPrecs {2500.0, 630.0, 78.0, 20.0, 2.4, 0.61, 0.076, 0.019};

class GeoHashHelper {
public:
	struct IntervalT {
		double high;
		double low;
		IntervalT(double h,double l) : high(h), low(l) {}
	};

	struct NewsT {
		double north; // maxlat
		double east; // maxlon
		double south; // minlat
		double west; // minlon
	};

	struct GeoBoxDimensionT {
		double height;
		double width;
	};

	struct GeoCoordT {
		double lat;
		double lon;
		NewsT news;
		GeoBoxDimensionT dimension;
	};


	using NeighbourT = std::vector<std::string>;

	/**
	* Encode: get geohash of this precision
	*
	* @param lat
	*   double latitude
	*
	* @param lon
	*   double longitude
	*
	* @param prec
	*   double precesion
	*
	* @return
	*   std::string
	*/
	std::string Encode(double lat, double lon, size_t prec=FWCT_DEFAULT_GEOHASH_PRECISION);

	/**
	* Decode: decode geohash
	*
	* @param hash
	*   std::string hash to decode
	*
	* @return
	*   GeoCoordT
	*/
	GeoCoordT Decode(std::string hash);

	/**
	* GetNeighbours : get neighbours
	*
	* @param hash
	*   std::string hash
	*
	* @param level
	*   size_t level of neighbour default 1
	*
	* @return
	*   NeighbourT
	*/
	NeighbourT GetNeighbours(std::string hash,size_t level=1);

	/**
	* Deg2Rad : degree to raidians
	*
	* @param degrees
	*   double degrees
	*
	* @return
	*   double radians
	*/
	double Deg2Rad(double degrees);

	/**
	* DistanceKm: Haversine distance between two points lon-lat points in km
	*
	* @param lat1
	*   double degrees
	*
	* @param lon1
	*   double degrees
	*
	* @param lat2
	*   double degrees
	*
	* @param lon2
	*   double degrees
	*
	* @return
	*   double radians
	*/
	double DistanceKm(double lat1, double lon1, double lat2, double lon2);

	/**
	* GetOneNeighbour : get neighbour in one dir
	*
	* @param hash
	*   std::string hash
	*
	* @param direction
	*   size_t direction
	*
	* @return
	*   NeighbourT
	*/
	std::string GetOneNeighbour(std::string hash, size_t direction);

	/**
	* GetBbox : get the hashes for a bbox
	*
	* @param left
	*   double left
	*
	* @param bottom
	*   double bottom
	*
	* @param right
	*   double right
	*
	* @param top
	*   double top
	*
	* @param prec
	*   double precesion
	*
	* @return
	*   NeighbourT
	*/
	NeighbourT GetBbox(double left, double bottom, double right, double top, size_t prec=FWCT_DEFAULT_GEOHASH_PRECISION);

private:

};
} // namespace geohash
} // namespace fwct

#endif /* _FWCT_GEOHASH_GEOHASH_HELPER_HPP_ */
