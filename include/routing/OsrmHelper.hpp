/**
 * @project Fawcet
 * @file include/routing/OsrmHelper.hpp
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
 *  OsrmHelper.hpp :  osrm routing headers
 *
 */
#ifndef _FWCT_ROUTING_OSRM_HELPER_HPP_
#define _FWCT_ROUTING_OSRM_HELPER_HPP_


#include <functional>
#include "utils/BaseUtils.hpp"
#include "geohash/GeoHashHelper.hpp"


namespace fwct {
class BadRouteException : public JdException {
	using JdException::JdException;
};

namespace routing {

class OsrmHelper {
public:
	using GeoCoordT = geohash::GeoHashHelper::GeoCoordT;
	using NeighbourT = geohash::GeoHashHelper::NeighbourT;

	using DistFuncT = std::function<void(std::string,std::string,double)>;


	/**
	* Constructor : default
	*
	* @param osrmfile
	*   std::string osrm input file
	*
	* @param use_ch
	*   bool algo is ch not mld
	*
	*/
	OsrmHelper(std::string osrmfile, bool use_ch=false);

	/**
	* make noncopyable
	*/
	OsrmHelper() = delete;
	OsrmHelper(const OsrmHelper&) = delete;
	OsrmHelper& operator=(const OsrmHelper&) = delete;

	virtual ~OsrmHelper();

	/**
	* GetResults : get pair data for neighbours
	*
	* @param hashlist
	*   NeighbourT* list of points
	*
	* @param distfunc
	*   DistFuncT function to handle output
	*
	* @param step_size
	*   size_t step size for max
	*
	* @return
	*   none
	*/
	void GetResults(NeighbourT* hashlist, DistFuncT distfunc, size_t step_size=FWCT_DEF_STEP_SIZE);

private:
	std::string osrmfile;
	bool use_ch;

};
} // namespace routing
} // namespace fwct

#endif /* _FWCT_ROUTING_OSRM_HELPER_HPP_ */
