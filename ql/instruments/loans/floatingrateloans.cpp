//
//  floatingrateloans.cpp
//  Loans
//
//  Created by jose melo on 10/12/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//

#include <ql/cashflows/iborcoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <ql/indexes/iborindex.hpp>
#include <ql/indexes/swapindex.hpp>
#include <ql/instruments/loans/floatingrateloans.hpp>
#include <ql/time/schedule.hpp>
#include <numeric>






namespace QuantLib {

	 namespace {
        class BPSCalculator : public AcyclicVisitor,
                              public Visitor<CashFlow>,
                              public Visitor<Coupon> {
          public:
            explicit BPSCalculator(const YieldTermStructure& discountCurve)
            : discountCurve_(discountCurve), bps_(0.0), nonSensNPV_(0.0) {}
            void visit(Coupon& c) {
                Real bps = c.nominal() * c.accrualPeriod() * discountCurve_.discount(c.date());
                bps_ += bps;
            }
            void visit(CashFlow& cf) {
                nonSensNPV_ += cf.amount() * discountCurve_.discount(cf.date());
            }
            Real bps() const { return bps_; }
            Real nonSensNPV() const { return nonSensNPV_; }

          private:
            const YieldTermStructure& discountCurve_;
            Real bps_, nonSensNPV_;
        };
        const Spread basisPoint_;
    
    } // anonymous namespace ends here
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
                         .withExCouponPeriod(exCouponPeriod, exCouponCalendar, exCouponConvention,
                                             exCouponEndOfMonth);

        addRedemptionsToLoanCashflows();

        QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
        registerWith(iborIndex);
    }

    FloatingRateLoan::FloatingRateLoan(const std::vector<Real>& amortizations,
                                       const Schedule& schedule,
                                       const ext::shared_ptr<IborIndex>& iborIndex,
                                       const DayCounter& paymentDayCounter,
                                       const DayCounter& accrualDayCounter,
                                       const YieldTermStructure& discountCurve,
                                       BusinessDayConvention paymentConvention,
                                       Natural fixingDays,
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

        Leg tmp_leg = IborLeg(schedule, iborIndex)
                         .withNotionals(notionals)
                         .withPaymentDayCounter(paymentDayCounter)
                         .withPaymentAdjustment(paymentConvention)
                         .withFixingDays(fixingDays)
                         .withSpreads(0.0)
                         .withCaps(caps)
                         .withFloors(floors)
                         .inArrears(inArrears)
                         .withExCouponPeriod(exCouponPeriod, exCouponCalendar, exCouponConvention,
                                             exCouponEndOfMonth);
        BPSCalculator calc(discountCurve);
        for (Size i = 0; i < tmp_leg.size(); ++i) {
            tmp_leg[i]->accept(calc);
        }
        Spread s = basisPoint_ * calc.bps() / discountCurve.discount(schedule.startDate());
        cashflows_ = IborLeg(schedule, iborIndex)
                      .withNotionals(notionals)
                      .withPaymentDayCounter(paymentDayCounter)
                      .withPaymentAdjustment(paymentConvention)
                      .withFixingDays(fixingDays)
                      .withSpreads(s)
                      .withCaps(caps)
                      .withFloors(floors)
                      .inArrears(inArrears)
                      .withExCouponPeriod(exCouponPeriod, exCouponCalendar, exCouponConvention,
                                          exCouponEndOfMonth);
        addRedemptionsToLoanCashflows();

        QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
        registerWith(iborIndex);
	}
    }
