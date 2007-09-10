/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Katiuscia Manzoni

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file capvolsurface.hpp
    \brief Cap/floor smile volatility surface
*/

#ifndef quantlib_cap_volatility_surface_hpp
#define quantlib_cap_volatility_surface_hpp

#include <ql/termstructures/capvolstructures.hpp>
#include <ql/math/interpolations/interpolation2d.hpp>
#include <ql/quote.hpp>
#include <ql/patterns/lazyobject.hpp>
#include <vector>

namespace QuantLib {

    //! Cap/floor smile volatility surface
    /*! This class provides the volatility for a given cap by 
        interpolating a volatility surface whose elements are the 
        market term volatilities of a set of caps/floors with given
        length and given strike.
    */
    class CapVolatilitySurface : public CapVolatilityStructure,
                                 public LazyObject {
      public:
        //! floating reference date, floating market data
        CapVolatilitySurface(Natural settlementDays,
                             const Calendar& calendar,
                             const std::vector<Period>& optionTenors,
                             const std::vector<Rate>& strikes,
                             const std::vector<std::vector<Handle<Quote> > >&,
                             const DayCounter& dc = Actual365Fixed());
        //! fixed reference date, floating market data
        CapVolatilitySurface(const Date& settlementDate,
                             const Calendar& calendar,
                             const std::vector<Period>& optionTenors,
                             const std::vector<Rate>& strikes,
                             const std::vector<std::vector<Handle<Quote> > >&,
                             const DayCounter& dc = Actual365Fixed());
        //! fixed reference date, fixed market data
        CapVolatilitySurface(const Date& settlementDate,
                             const Calendar& calendar,
                             const std::vector<Period>& optionTenors,
                             const std::vector<Rate>& strikes,
                             const Matrix& volatilities,
                             const DayCounter& dc = Actual365Fixed());
        //! floating reference date, fixed market data
        CapVolatilitySurface(Natural settlementDays,
                             const Calendar& calendar,
                             const std::vector<Period>& optionTenors,
                             const std::vector<Rate>& strikes,
                             const Matrix& volatilities,
                             const DayCounter& dc = Actual365Fixed());
        //! \name TermStructure interface
        //@{
        Date maxDate() const;
        //@}
        //! \name CapVolatilityStructure interface
        //@{
        Real minStrike() const;
        Real maxStrike() const;
        //@}
        //! \name LazyObject interface
        void update(){
            TermStructure::update();
            LazyObject::update();
        };
        void performCalculations() const;
        //@}
        //! \name some inspectors
        const std::vector<Period>& optionTenors() const;

        const std::vector<Time>& optionTimes() const;
        const std::vector<Rate>& strikes() const;
        //@}
      private:
        void checkInputs(Size volatilitiesRows,
                         Size volatilitiesColumns) const;
        void registerWithMarketData();
        std::vector<Period> optionTenors_;
        mutable std::vector<Time> optionTimes_;
        std::vector<Rate> strikes_;
        std::vector<std::vector<Handle<Quote> > > volHandles_;
        mutable Matrix volatilities_;
        mutable Interpolation2D interpolation_;
        void interpolate() const;
        mutable Date maxDate_;
        Volatility volatilityImpl(Time t,
                                  Rate strike) const;
    };

    // inline definitions

    inline Date CapVolatilitySurface::maxDate() const {
        calculate();
        return referenceDate()+optionTenors_.back();
    }

    inline Real CapVolatilitySurface::minStrike() const {
        return strikes_.front();
    }

    inline Real CapVolatilitySurface::maxStrike() const {
        return strikes_.back();
    }

    inline Volatility CapVolatilitySurface::volatilityImpl(Time t,
                                                           Rate strike) const {
        calculate();
        return interpolation_(strike, t);
    }

    inline const std::vector<Period>& CapVolatilitySurface::optionTenors() const {
        return optionTenors_;
    }

    inline const std::vector<Time>& CapVolatilitySurface::optionTimes() const {
        return optionTimes_;
    }
    
    inline const std::vector<Rate>& CapVolatilitySurface::strikes() const {
        return strikes_;
    }
}

#endif
