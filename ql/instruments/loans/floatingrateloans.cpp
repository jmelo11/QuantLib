//
//  floatingrateloans.cpp
//  Loans
//
//  Created by jose melo on 10/12/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//

#include <ql/instruments/loans/floatingrateloans.hpp>
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <ql/time/schedule.hpp>
#include <ql/indexes/swapindex.hpp>
#include <ql/indexes/iborindex.hpp>
#include <numeric>

namespace QuantLib {
    FloatingRateLoan::FloatingRateLoan(const std::vector<Real>& amortizations,
                           const Schedule& schedule,
                           const ext::shared_ptr<IborIndex>& iborIndex,
                           const DayCounter& paymentDayCounter,
                           BusinessDayConvention paymentConvention,
                           Natural fixingDays,
                           const std::vector<Spread>& spreads,
                           const std::vector<Rate>& caps,
                           const std::vector<Rate>& floors,
                           bool inArrears,
                           Real redemption,
                           const Date& issueDate,
                           const Period& exCouponPeriod,
                           const Calendar& exCouponCalendar,
                           const BusinessDayConvention exCouponConvention,
                           bool exCouponEndOfMonth)
    : Loan(0, schedule.calendar(), issueDate) {
        Real notional = std::accumulate(amortizations.begin(), amortizations.end(), 0);
        std::vector<Real> notionals(1, notional);
        for (Size i = 0; i < amortizations.size(); i++) {
            notional -= amortizations[i];
            notionals.push_back(notional);
        }      
        maturityDate_ = schedule.endDate();

        cashflows_ = IborLeg(schedule, iborIndex)
            .withNotionals(notionals)
            .withPaymentDayCounter(paymentDayCounter)
            .withPaymentAdjustment(paymentConvention)
            .withFixingDays(fixingDays)
            .withSpreads(spreads)
            .withCaps(caps)
            .withFloors(floors)
            .inArrears(inArrears)
            .withExCouponPeriod(exCouponPeriod, exCouponCalendar, exCouponConvention, exCouponEndOfMonth);

        addRedemptionsToLoanCashflows();

        QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
        registerWith(iborIndex);
    }

    FloatingRateLoan::FloatingRateLoan(const std::vector<Real>& amortizations,
										const Date& startDate,
										const Date& maturityDate,
										Frequency couponFrequency,
										const Calendar& calendar,
										const ext::shared_ptr<IborIndex>& iborIndex,
										const DayCounter& accrualDayCounter,
										BusinessDayConvention accrualConvention,
										BusinessDayConvention paymentConvention,
										Natural fixingDays,
										const std::vector<Spread>& spreads,
										const std::vector<Rate>& caps,
										const std::vector<Rate>& floors,
										bool inArrears,
										Real redemption,
										const Date& issueDate,
										const Date& stubDate,
										DateGeneration::Rule rule,
										bool endOfMonth,
										const Period& exCouponPeriod,
										const Calendar& exCouponCalendar,
										const BusinessDayConvention exCouponConvention,
										bool exCouponEndOfMonth)
    : Loan(0, calendar, issueDate) {

        maturityDate_ = maturityDate;
        Date firstDate, nextToLastDate;
        switch (rule) {
          case DateGeneration::Backward:
            firstDate = Date();
            nextToLastDate = stubDate;
            break;
          case DateGeneration::Forward:
            firstDate = stubDate;
            nextToLastDate = Date();
            break;
          case DateGeneration::Zero:
          case DateGeneration::ThirdWednesday:
          case DateGeneration::Twentieth:
          case DateGeneration::TwentiethIMM:
            QL_FAIL("stub date (" << stubDate << ") not allowed with " <<
                    rule << " DateGeneration::Rule");
          default:
            QL_FAIL("unknown DateGeneration::Rule (" << Integer(rule) << ")");
        }
        Real notional = std::accumulate(amortizations.begin(), amortizations.end(), 0);
        std::vector<Real> notionals(1, notional);
        for (Size i = 0; i < amortizations.size(); i++) {
            notional -= amortizations[i];
            notionals.push_back(notional);
        }      

        Schedule schedule(startDate, maturityDate_, Period(couponFrequency),
                          calendar_, accrualConvention, accrualConvention,
                          rule, endOfMonth,
                          firstDate, nextToLastDate);


        cashflows_ = IborLeg(schedule, iborIndex)
            .withNotionals(notionals)
            .withPaymentDayCounter(accrualDayCounter)
            .withPaymentAdjustment(paymentConvention)
            .withFixingDays(fixingDays)
            .withSpreads(spreads)
            .withCaps(caps)
            .withFloors(floors)
            .inArrears(inArrears)
            .withExCouponPeriod(exCouponPeriod,exCouponCalendar, exCouponConvention, exCouponEndOfMonth);

        addRedemptionsToLoanCashflows();

        QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
        registerWith(iborIndex);
    }

}

