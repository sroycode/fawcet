/**
 * @project Fawcet
 * @file src/routing/OsrmHelper.cc
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
 *  OsrmHelper.cc :  osrm c++ helper implementation
 *
 */

#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include <exception>
#include <iostream>
#include <cstdlib>

#include "routing/OsrmHelper.hpp"

/**
* Constructor : default
*
*/
fwct::routing::OsrmHelper::OsrmHelper(std::string osrmfile, bool use_ch)
	: osrmfile(osrmfile), use_ch(use_ch)
{
}

/**
* Destructor : default
*
*/
fwct::routing::OsrmHelper::~OsrmHelper() {}

/**
* GetResults : get pair data for neighbours
*
*/
void fwct::routing::OsrmHelper::GetResults(
    fwct::routing::OsrmHelper::NeighbourT* hashlist,
    fwct::routing::OsrmHelper::DistFuncT distfunc,
    size_t step_size)
{
	// Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
	if (step_size>FWCT_MAX_STEP_SIZE)
		throw fwct::BadRouteException("Step size exceeds max value");

	::osrm::EngineConfig config;

	config.storage_config = {osrmfile.c_str()};
	config.use_shared_memory = false;

	// There are two routing speed up techniques:
	// - Contraction Hierarchies (CH): requires extract+contract pre-processing
	// - Multi-Level Dijkstra (MLD): requires extract+partition+customize pre-processing
	config.algorithm = (use_ch) ? osrm::EngineConfig::Algorithm::CH : osrm::EngineConfig::Algorithm::MLD;

	// Routing machine with Route + Table + Nearest + Trip + Match
	const osrm::OSRM osrm{config};

	// The following shows how to use the Table service; configure this service
	auto gh = fwct::geohash::GeoHashHelper();

	osrm::TableParameters params;

	DLOG(INFO) << "Hashlist size = " << hashlist->size();

	for (auto& hash : *hashlist) {
		auto g = gh.Decode(hash);
		params.coordinates.push_back({osrm::util::FloatLongitude{g.lon}, osrm::util::FloatLatitude{g.lat}});
	}

	size_t m_size = hashlist->size();

	DLOG(INFO) << "Coords size = " << params.coordinates.size();

	for (size_t src_start=0; src_start < m_size; src_start+=step_size ) {
		size_t src_finish = src_start + step_size -1;
		if ( src_finish > (m_size-1) ) src_finish = m_size-1;

		for (size_t trg_start=0; trg_start < m_size; trg_start+=step_size ) {
			size_t trg_finish = trg_start + step_size -1;
			if ( trg_finish > (m_size-1) ) trg_finish = m_size-1;

			osrm::json::Object result;
			params.sources.clear();
			for (size_t i = src_start; i <= src_finish; ++i) {
				params.sources.push_back(i);
			}
			params.destinations.clear();
			for (size_t i = trg_start; i <= trg_finish; ++i) {
				params.destinations.push_back(i);
			}

			DLOG(INFO) << "Iteration : " << src_start << " - " << src_finish << " , " << trg_start << " - " << trg_finish;
			DLOG(INFO) << "Coords size = " << params.coordinates.size();
			DLOG(INFO) << "Sources size = " << params.sources.size();
			DLOG(INFO) << "Dest size = " << params.destinations.size();

			const auto status = osrm.Table(params, result);
			if (status == osrm::Status::Error)
				throw fwct::BadRouteException("This set cannot be calculated");

			auto& table = result.values["durations"].get<osrm::json::Array>();
			DLOG(INFO) << "Table :" << table.values.size() << " start " << src_start << " fin " << src_finish;
			if(table.values.size() != (src_finish+1-src_start) )
				throw fwct::BadRouteException("Size mismatch in table");

			for (size_t i = src_start; i <= src_finish; ++i) {
				const auto& line = table.values.at(i-src_start).get<osrm::json::Array>();
				DLOG(INFO) << "Line :" << line.values.size() << " start " << trg_start << " fin " << trg_finish;
				if(line.values.size() != (trg_finish+1-trg_start) )
					throw fwct::BadRouteException("Size mismatch in line");
				for (size_t j = trg_start; j <= trg_finish; ++j) {
					const auto& el = line.values.at(j-trg_start);
					if (el.is<osrm::json::Null>()) continue; // no route found
					distfunc( hashlist->at(i), hashlist->at(j), el.get<osrm::json::Number>().value );
				}
			}

		}
	}
}
