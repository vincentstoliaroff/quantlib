

/*
 Copyright (C) 2000, 2001, 2002 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/
/*! \file timegrid.hpp
    \brief Time grid class with useful constructors

    \fullpath
    ql/InterestRateModelling/%timegrid.hpp
*/

// $Id$

#ifndef quantlib_lattices_time_grid_h
#define quantlib_lattices_time_grid_h

#include <ql/qldefines.hpp>

#include <list>

namespace QuantLib {

    namespace Lattices {

        class TimeGrid : public std::vector<Time> {
          public:
            TimeGrid() : std::vector<Time>(0) {}

            TimeGrid(const std::list<Time>& times, Size steps)
            : std::vector<Time>(0) {
                Time last = times.back();
                double dtMax = last/steps;

                Time begin = 0.0;
                std::list<Time>::const_iterator t;
                for (t = times.begin(); t != times.end(); t++) {
                    Time end = *t;
                    if (begin == end) continue;
                    Size nSteps = (Size)((end - begin)/dtMax + 1.0);
                    double dt = (end - begin)/nSteps;
                    for (Size n=0; n<nSteps; n++)
                        push_back(begin + n*dt);
                    begin = end;
                }
                push_back(begin);
            }

            Size findIndex(Time t) const {
                for (Size i=0; i<size(); i++) {
                    if ((*this)[i] == t)
                        return i;
                }
                throw Error("Using inadequate tree");
                QL_DUMMY_RETURN(0);
            }

            Time dt(Size i) {
                return (*this)[i+1]  - (*this)[i];
            }

        };

    }

}

#endif
