/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2025 Paul Ramsey
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************/

#pragma once

namespace geos {
namespace util {

/**
 * Run this before running the function of interest,
 * to clear FE exception flags.
 */
void geos_feexcept_setup();

/**
 * Run this after running the function of interest,
 * to emit messages on std::err if it finds flags
 * set after the operation.
 */
void geos_feexcept_check();


} // geos::util
} // geos
