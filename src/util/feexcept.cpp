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

#include <geos/export.h>
#include <geos/util.h>

#ifdef HAVE_FENV
#include <cfenv>
#include <iostream>
#endif

namespace geos {
namespace util { // geos.util

/*
 * For MacOS FE_DIVBYZERO has to be explicitly trapped.
 * For Linux using feenableexcept() throws an error just
 * fine.
 */
// #define ENABLE_DIVBYZERO_EXCEPTION

#ifdef ENABLE_DIVBYZERO_EXCEPTION
#if defined(__APPLE__) && defined(__aarch64__)

    // Apple Silicon (M1/M2/etc) specific polyfill
    // IOE (Bit 8): Invalid Operation Exception enable.
    #define FE_INVALID_TRAP   0x0100
    // DZE (Bit 9): Divide-by-Zero Exception enable.
    #define FE_DIVBYZERO_TRAP 0x0200
    // OFE (Bit 10): Overflow Exception enable.
    #define FE_OVERFLOW_TRAP  0x0400
    // UFE (Bit 11): Underflow Exception enable.
    #define FE_UNDERFLOW_TRAP 0x0800
    // IXE (Bit 12): Inexact Exception enable.
    #define FE_INEXACT_TRAP   0x1000

    void enable_divbyzero_trap() {
        fenv_t env;
        fegetenv(&env);
        // Unmask the bit (on ARM, setting the bit *enables* the trap)
        // Note: This relies on the specific layout of fenv_t on macOS ARM64
        env.__fpcr |= FE_DIVBYZERO_TRAP;
        fesetenv(&env);
    }
#else
    // Linux / Standard approach
    void enable_divbyzero_trap() {
        feenableexcept(FE_DIVBYZERO);
    }
#endif
#endif

void
geos_feexcept_setup()
{
#ifdef HAVE_FENV
    std::feclearexcept(FE_ALL_EXCEPT); // clear floating-point status flags
#ifdef ENABLE_DIVBYZERO_EXCEPTION
    enable_divbyzero_trap();
#endif
#endif

    return;
}



void
geos_feexcept_check()
{
#ifdef HAVE_FENV

#ifdef FE_INEXACT
    // Catch everything except for FE_INEXACT, which is usually harmless
    const int fpexp = std::fetestexcept(FE_ALL_EXCEPT ^ FE_INEXACT);
#else
    const int fpexp = std::fetestexcept(FE_ALL_EXCEPT);
#endif
    if (fpexp != 0) {
        std::cerr << "Operation raised floating-point environment flag(s):";

#ifdef FE_DIVBYZERO
        if (fpexp & FE_DIVBYZERO)
            std::cerr << " FE_DIVBYZERO";
#endif

#ifdef FE_INEXACT
        if (fpexp & FE_INEXACT)
            std::cerr << " FE_INEXACT";
#endif

#ifdef FE_INVALID
        if (fpexp & FE_INVALID)
            std::cerr << " FE_INVALID";
#endif

#ifdef FE_OVERFLOW
        if (fpexp & FE_OVERFLOW)
            std::cerr << " FE_OVERFLOW";
#endif

#ifdef FE_UNDERFLOW
        if (fpexp & FE_UNDERFLOW)
            std::cerr << " FE_UNDERFLOW";
#endif

        std::cerr << std::endl;
    }

#endif // HAVE_FENV

    return;
}



} // geos::util
} // geos



