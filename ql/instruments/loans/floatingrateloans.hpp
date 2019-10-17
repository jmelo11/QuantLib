//
//  floatingrateloans.hpp
//  Loans
//
//  Created by jose melo on 10/12/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//

#ifndef quantlib_floating_rate_loan_hpp
#define quantlib_floating_rate_loan_hpp

#include <ql/instruments/loans/loan.hpp>
#include <ql/time/dategenerationrule.hpp>

namespace QuantLib {

    class Schedule;
    class IborIndex;
    class YieldTermStructure;
    //! floating-rate bond (possibly capped and/or floored)
    /*! \ingroup instruments

        \test calculations are tested by checking results against
              cached values.
    */
    class FloatingRateLoan : public Loan {
      public:
        FloatingRateLoan(const std::vector<Real>& amortizations,
                         const Schedule& schedule,
                         const ext::shared_ptr<IborIndex>& iborIndex,
                         const DayCounter& accrualDayCounter,                         
						 BusinessDayConvention paymentConvention = Following,
                         Natural fixingDays = Null<Natural>(),
                         const std::vector<Spread>& spreads
                                             = std::vector<Spread>(1, 0.0),
                         const std::vector<Rate>& caps
                                             = std::vector<Rate>(),
                         const std::vector<Rate>& floors
                                            = std::vector<Rate>(),
                         bool inArrears = false,
                         Real redemption = 100.0,
                         const Date& issueDate = Date(),
                         const Period& exCouponPeriod = Period(),
                         const Calendar& exCouponCalendar = Calendar(),
                         const BusinessDayConvention exCouponConvention = Unadjusted,
                         bool exCouponEndOfMonth = false);
        
        FloatingRateLoan(const std::vector<Real>& amortizations,
                        const Schedule& schedule,
                        const ext::shared_ptr<IborIndex>& iborIndex,
                         const DayCounter& paymentDayCounter,
                        const DayCounter& accrualDayCounter,
                        const YieldTermStructure& discountCurve,
                        BusinessDayConvention paymentConvention = Following,
                        Natural fixingDays = Null<Natural>(),  
						const std::vector<Rate>& caps = std::vector<Rate>(),
                        const std::vector<Rate>& floors = std::vector<Rate>(),
                        bool inArrears = false,
                        Real redemption = 100.0,
                        const Date& issueDate = Date(),
                        const Period& exCouponPeriod = Period(),
                        const Calendar& exCouponCalendar = Calendar(),
                        const BusinessDayConvention exCouponConvention = Unadjusted,
                        bool exCouponEndOfMonth = false);
    };
}

#endif
