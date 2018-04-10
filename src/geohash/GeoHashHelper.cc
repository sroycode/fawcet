/**
 * @project Fawcet
 * @file src/geohash/GeoHashHelper.cc
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
 *  GeoHashHelper.cc :  geohash c++ helper implementation
 *
 */
#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>

#include "geohash/GeoHashHelper.hpp"

static std::string char_map =  "0123456789bcdefghjkmnpqrstuvwxyz";
static std::string even_neighbours[4] = {
	"p0r21436x8zb9dcf5h7kjnmqesgutwvy",
	"bc01fg45238967deuvhjyznpkmstqrwx",
	"14365h7k9dcfesgujnmqp0r2twvyx8zb",
	"238967debc01fg45kmstqrwxuvhjyznp"
};
static std::string odd_neighbours[4] = {
	"bc01fg45238967deuvhjyznpkmstqrwx",
	"p0r21436x8zb9dcf5h7kjnmqesgutwvy",
	"238967debc01fg45kmstqrwxuvhjyznp",
	"14365h7k9dcfesgujnmqp0r2twvyx8zb"
};
static std::string even_borders[4] = {"prxz", "bcfguvyz", "028b", "0145hjnp"};
static std::string odd_borders[4] = {"bcfguvyz", "prxz", "0145hjnp", "028b"};

/**
* Encode: get geohash of this precision
*
*/
std::string fwct::geohash::GeoHashHelper::Encode(double lat, double lon,size_t prec)
{
	if( lon>FWCT_MAX_LON || lon<FWCT_MIN_LON || lat>FWCT_MAX_LAT || lat<FWCT_MIN_LAT)
		throw fwct::BadDataException("Invalid LonLat");
	if(prec < 1 || prec > 12) prec = 12;
	std::stringstream ss;
	prec *= 5.0;
	fwct::geohash::GeoHashHelper::IntervalT lat_interval{FWCT_MAX_LAT, FWCT_MIN_LAT};
	fwct::geohash::GeoHashHelper::IntervalT lon_interval{FWCT_MAX_LON, FWCT_MIN_LON};
	fwct::geohash::GeoHashHelper::IntervalT* interval;
	double* coord;
	double mid;
	unsigned short int hashChar = 0;
	int i;
	for(i = 1; i <= prec; ++i) {
		if (1==i%2) {
			interval = &lon_interval;
			coord = &lon;
		}
		else {
			interval = &lat_interval;
			coord = &lat;
		}
		mid = (interval->low + interval->high) / 2.0;
		hashChar = hashChar << 1;
		if(*coord > mid) {
			interval->low = mid;
			hashChar |= 0x01;
		}
		else {
			interval->high = mid;
		}
		if(!(i % 5)) {
			ss << char_map[hashChar];
			hashChar = 0;
		}
	}
	return ss.str();
}

/**
* Decode: decode geohash
*
*/
fwct::geohash::GeoHashHelper::GeoCoordT fwct::geohash::GeoHashHelper::Decode(std::string hash)
{
	if ( hash.length()==0) return {0.0,0.0,0.0,0.0,0.0,0.0};
	fwct::geohash::GeoHashHelper::IntervalT lat_interval{FWCT_MAX_LAT, FWCT_MIN_LAT};
	fwct::geohash::GeoHashHelper::IntervalT lon_interval{FWCT_MAX_LON, FWCT_MIN_LON};
	fwct::geohash::GeoHashHelper::IntervalT* interval;
	double delta = 0.0;
	size_t is_even=0;
	for (char& h : hash) {
		size_t char_mapIndex = char_map.find(h);
		if (std::string::npos == char_mapIndex) break;
		// Interpret the last 5 bits of the integer
		for(unsigned int j = 0; j < 5; ++j) {
			interval = (++is_even%2) ? &lon_interval : &lat_interval;
			delta = (interval->high - interval->low) / 2.0;
			if(((unsigned int)char_mapIndex << j) & 0x0010)
				interval->low += delta;
			else
				interval->high -= delta;
		}
	}
	return {
		lat_interval.high - ((lat_interval.high - lat_interval.low) / 2.0),
		lon_interval.high - ((lon_interval.high - lon_interval.low) / 2.0),
		{ lat_interval.high, lon_interval.high, lat_interval.low, lon_interval.low }
	};
}

/**
* GetOneNeighbour : get neighbour in one dir
*
*/
std::string fwct::geohash::GeoHashHelper::GetOneNeighbour(std::string hash, size_t direction)
{
	if (direction>FWCT_WEST || hash.length()<2) throw fwct::BadGeoHashException();
	const std::string& border = (1==hash.length()%2) ? odd_borders[direction] : even_borders[direction] ;
	const std::string& neighbour = (1==hash.length()%2) ? odd_neighbours[direction] : even_neighbours[direction] ;
	// get last character
	size_t neighbour_index = neighbour.find(hash.back());
	if (neighbour_index >= char_map.length()) throw fwct::BadGeoHashException();
	// revise base
	std::string base=hash.substr(0,hash.length()-1);
	if (std::string::npos != border.find(hash.back()))
		base = fwct::geohash::GeoHashHelper::GetOneNeighbour(base,direction);
	base.append(1u,char_map[neighbour_index]);
	return base;
}

/**
* GetNeighbours : get neighbours
*
*/
fwct::geohash::GeoHashHelper::NeighbourT fwct::geohash::GeoHashHelper::GetNeighbours(std::string hash,size_t level)
{
	fwct::geohash::GeoHashHelper::NeighbourT n;
	if (level<=0) return n;
	try {
		for (size_t i=0; i<level; ++i) hash = fwct::geohash::GeoHashHelper::GetOneNeighbour(hash,FWCT_NORTH);
		n.push_back(hash);
		for (size_t i=0; i<level; ++i) {
			hash = fwct::geohash::GeoHashHelper::GetOneNeighbour(hash,FWCT_EAST);
			n.push_back(hash);
		}
		const size_t dirs[] = { FWCT_SOUTH, FWCT_WEST, FWCT_NORTH };
		for (size_t dir : dirs) {
			for (size_t i=0; i<level*2; ++i) {
				hash = fwct::geohash::GeoHashHelper::GetOneNeighbour(hash,dir);
				n.push_back(hash);
			}
		}
		for (size_t i=0; i<level-1; ++i) {
			hash = fwct::geohash::GeoHashHelper::GetOneNeighbour(hash,FWCT_EAST);
			n.push_back(hash);
		}
	}
	catch (fwct::BadGeoHashException& e) {
		return fwct::geohash::GeoHashHelper::NeighbourT();
	}
	return n;
}

/**
* Deg2Rad : degree to raidians
*
*/
double fwct::geohash::GeoHashHelper::Deg2Rad(double degrees)
{
	return degrees * FWCT_PI / FWCT_MAX_LON;
}

/**
* DistanceMeter: destance between two points in lon lat in m
* Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
*
*/
double fwct::geohash::GeoHashHelper::DistanceKm(double lat1, double lon1, double lat2, double lon2)
{

	double deltaLat = Deg2Rad(fabs(lat2 - lat1));
	double deltaLon = Deg2Rad(fabs(lon2 - lon1));
	double a = sin(deltaLat / 2) * sin(deltaLat / 2)
	           + cos(Deg2Rad(lat1)) * cos(Deg2Rad(lat2)) * sin(deltaLon / 2) * sin(deltaLon / 2);
	return 2.0 * FWCT_EARTH_RADIUS_KM * asin(sqrt(a));
}

/**
* GetBbox : get the hashes for a bbox
*
*/
fwct::geohash::GeoHashHelper::NeighbourT fwct::geohash::GeoHashHelper::GetBbox(
    double left, double bottom, double right, double top, size_t prec)
{
	if (prec<1 || prec>8)
		throw fwct::BadDataException("Invalid GeoHash Precision");

	if (left>right || bottom>top)
		throw fwct::BadDataException("Invalid latLon");


	auto lat_interval = fwct::geohash::GeoHashLatPrecs[prec-1];
	auto lon_interval = fwct::geohash::GeoHashLonPrecs[prec-1];

	std::set<std::string> nset;

	for ( double lat = bottom ; lat <= top ; lat += lat_interval ) {
		for ( double lon = left ; lon <= right ; lon += lon_interval ) {
			nset.insert( this->Encode( lat,lon,prec) );
		}
	}
	fwct::geohash::GeoHashHelper::NeighbourT nlist;
	nlist.assign(nset.begin(),nset.end());

	return nlist;
}
