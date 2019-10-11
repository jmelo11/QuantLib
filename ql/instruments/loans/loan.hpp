//
//  loan.hpp
//  Loans
//
//  Created by Jose Melo on 9/22/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//

#include <ql/instruments/bond.hpp>

#ifndef quantlib_loan_hpp
#define quantlib_loan_hpp

namespace QuantLib {
    class Loan : public Bond {
      public:
        Loan(Natural settlementDays,
             const Calendar& calendar,
             const Date& issueDate = Date(),
             const Leg& coupons = Leg());

        Loan(Natural settlementDays,
             const Calendar& calendar,
             Real faceAmount,
             const Date& maturityDate,
             const Date& issueDate = Date(),
             const Leg& cashflows = Leg());


        /*Same as bond method, but without notionals warning*/
        void addRedemptionsToLoanCashflows(const std::vector<Real>& redemptions = std::vector<Real>());
        void calculateNotionalsFromLoanCashflows();
    };
};
#endif
