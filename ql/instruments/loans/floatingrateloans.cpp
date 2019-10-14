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

namespace QuantLib {
    FloatingRateLoan::FloatingRateLoan(Real faceAmount,
                           const Schedule& schedule,
                           const ext::shared_ptr<IborIndex>& iborIndex,
                           const DayCounter& paymentDayCounter,
                           BusinessDayConvention paymentConvention,
                           Natural fixingDays,
                           const std::vector<Real>& gearings,
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

        maturityDate_ = schedule.endDate();

        cashflows_ = IborLeg(schedule, iborIndex)
            .withNotionals(faceAmount)
            .withPaymentDayCounter(paymentDayCounter)
            .withPaymentAdjustment(paymentConvention)
            .withFixingDays(fixingDays)
            .withGearings(gearings)
            .withSpreads(spreads)
            .withCaps(caps)
            .withFloors(floors)
            .inArrears(inArrears)
            .withExCouponPeriod(exCouponPeriod, exCouponCalendar, exCouponConvention, exCouponEndOfMonth);

        addRedemptionsToLoanCashflows(std::vector<Real>(1, redemption));

        QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
        QL_ENSURE(redemptions_.size() == 1, "multiple redemptions created");

        registerWith(iborIndex);
    }

    FloatingRateLoan::FloatingRateLoan(Real faceAmount,
                           const Date& startDate,
                           const Date& maturityDate,
                           Frequency couponFrequency,
                           const Calendar& calendar,
                           const ext::shared_ptr<IborIndex>& iborIndex,
                           const DayCounter& accrualDayCounter,
                           BusinessDayConvention accrualConvention,
                           BusinessDayConvention paymentConvention,
                           Natural fixingDays,
                           const std::vector<Real>& gearings,
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

        Schedule schedule(startDate, maturityDate_, Period(couponFrequency),
                          calendar_, accrualConvention, accrualConvention,
                          rule, endOfMonth,
                          firstDate, nextToLastDate);

        cashflows_ = IborLeg(schedule, iborIndex)
            .withNotionals(faceAmount)
            .withPaymentDayCounter(accrualDayCounter)
            .withPaymentAdjustment(paymentConvention)
            .withFixingDays(fixingDays)
            .withGearings(gearings)
            .withSpreads(spreads)
            .withCaps(caps)
            .withFloors(floors)
            .inArrears(inArrears)
            .withExCouponPeriod(exCouponPeriod,exCouponCalendar, exCouponConvention, exCouponEndOfMonth);

        addRedemptionsToLoanCashflows(std::vector<Real>(1, redemption));

        QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
        QL_ENSURE(redemptions_.size() == 1, "multiple redemptions created");

        registerWith(iborIndex);
    }

}

