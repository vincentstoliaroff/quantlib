

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
/*! \file costfunction.hpp
    \brief Optimization cost function class

    \fullpath
    ql/Optimization/%costfunction.hpp
*/

#ifndef quantlib_optimization_costfunction_h
#define quantlib_optimization_costfunction_h

/*!
  Cost function abstract class for unconstrained optimization pb
*/

namespace QuantLib {

    namespace Optimization {

        class CostFunction {
          public:
            //! method to overload to compute the cost functon value in x
            virtual double value(const Array& x) = 0;

            //! method to overload to compute grad_f, the first derivative of
            //  the cost function with respect to x
            virtual void gradient(Array& grad_f, const Array& x) {
                double eps = finiteDifferenceEpsilon(), fp, fm;
                Array xx = x;
                int i, sz = x.size();
                for (i = 0; i < sz; ++i) {
                      xx[i] += eps;
                      fp = value(xx);
                      xx[i] -= 2. * eps;
                      fm = value(xx);
                      grad_f[i] = 0.5 * (fp - fm) / eps;
                }
            }

            //! method to overload to compute grad_f, the first derivative of
            //  the cost function with respect to x and also the cost function
            virtual double valueAndGradient(Array& grad_f, const Array& x) {
                gradient(grad_f, x);
                return value(x);
            }

            //! Default epsilon for finite difference method :
            virtual double finiteDifferenceEpsilon() { return 1e-8; }
        };

    }

}

#endif
