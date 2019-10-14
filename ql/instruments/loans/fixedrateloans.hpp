//
//  structuredloans.hpp
//  Loans
//
//  Created by Jose Melo on 9/22/19.
//  Copyright � 2019 Jose Melo. All rights reserved.
//

#ifndef quantlib_fixedrateloans_hpp
#define quantlib_fixedrateloans_hpp


#include <ql/instruments/loans/loan.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/schedule.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>

namespace QuantLib {
    class UnEqualAmortizationLoan : public Loan {
      public:
        UnEqualAmortizationLoan(std::vector<Real> amortizations,
                                const Schedule& schedule,
                                const Real coupon,
                                const DayCounter& dayCounter,
								Compounding comp = Compounded,
								Frequency freq = Annual,
                                BusinessDayConvention paymentConvention = Following,
                                const Date& issueDate = Date(),
                                const Calendar& paymentCalendar = Calendar(),
                                const Period& exCouponPeriod = Period(),
                                const Calendar& exCouponCalendar = Calendar(),
                                const BusinessDayConvention exCouponConvention = Unadjusted,
                                bool exCouponEndOfMonth = false);

        UnEqualAmortizationLoan(std::vector<Real> amortizations,
                                const Schedule& schedule,
                                const InterestRate coupon,
                                BusinessDayConvention paymentConvention = Following,
                                const Date& issueDate = Date(),
                                const Calendar& paymentCalendar = Calendar(),
                                const Period& exCouponPeriod = Period(),
                                const Calendar& exCouponCalendar = Calendar(),
                                const BusinessDayConvention exCouponConvention = Unadjusted,
                                bool exCouponEndOfMonth = false);

        UnEqualAmortizationLoan(std::vector<Real> amortizations,
                                Schedule schedule,
                                YieldTermStructure& discountCurve,
                                const DayCounter& dayCounter,
								Compounding comp = Compounded,
								Frequency freq = Annual,
                                BusinessDayConvention paymentConvention = Following,
                                const Date& issueDate = Date(),
                                const Calendar& paymentCalendar = Calendar(),
                                const Period& exCouponPeriod = Period(),
                                const Calendar& exCouponCalendar = Calendar(),
                                const BusinessDayConvention exCouponConvention = Unadjusted,
                                bool exCouponEndOfMonth = false);

        Frequency frequency() const { return frequency_; }
        const DayCounter& dayCounter() const { return dayCounter_; }

      protected:
        Frequency frequency_;
        DayCounter dayCounter_;
    };
    class EqualAmortizationLoan : public Loan {

      public:
        EqualAmortizationLoan(Real faceAmount,
                              const Schedule& schedule,
                              const Real coupon,
                              const DayCounter& dayCounter,
                              Compounding comp = Compounded,
                              Frequency freq = Annual,
                              BusinessDayConvention paymentConvention = Following,
                              const Date& issueDate = Date(),
                              const Calendar& paymentCalendar = Calendar(),
                              const Period& exCouponPeriod = Period(),
                              const Calendar& exCouponCalendar = Calendar(),
                              const BusinessDayConvention exCouponConvention = Unadjusted,
                              bool exCouponEndOfMonth = false);

        EqualAmortizationLoan(Real faceAmount,
                              const Schedule& schedule,
                              const InterestRate coupon,
                              BusinessDayConvention paymentConvention = Following,
                              const Date& issueDate = Date(),
                              const Calendar& paymentCalendar = Calendar(),
                              const Period& exCouponPeriod = Period(),
                              const Calendar& exCouponCalendar = Calendar(),
                              const BusinessDayConvention exCouponConvention = Unadjusted,
                              bool exCouponEndOfMonth = false);

        EqualAmortizationLoan(Real faceAmount,
                              Schedule schedule,
                              YieldTermStructure& discountCurve,
                              const DayCounter& dayCounter,
							  Compounding comp = Compounded,
							  Frequency freq = Annual,
                              BusinessDayConvention paymentConvention = Following,
                              const Date& issueDate = Date(),
                              const Calendar& paymentCalendar = Calendar(),
                              const Period& exCouponPeriod = Period(),
                              const Calendar& exCouponCalendar = Calendar(),
                              const BusinessDayConvention exCouponConvention = Unadjusted,
                              bool exCouponEndOfMonth = false);


        Frequency frequency() const { return frequency_; }
        const DayCounter& dayCounter() const { return dayCounter_; }

      protected:
        Frequency frequency_;
        DayCounter dayCounter_;
    };
    class EqualCashFlowLoan : public Loan {

      public:
        EqualCashFlowLoan(Real faceAmount,
                          const Schedule& schedule,
                          const Real coupon,
                          const DayCounter& dayCounter,
                          Compounding comp = Compounded,
                          Frequency freq = Annual,
                          BusinessDayConvention paymentConvention = Following,
                          const Date& issueDate = Date(),
                          const Calendar& paymentCalendar = Calendar(),
                          const Period& exCouponPeriod = Period(),
                          const Calendar& exCouponCalendar = Calendar(),
                          const BusinessDayConvention exCouponConvention = Unadjusted,
                          bool exCouponEndOfMonth = false);

        EqualCashFlowLoan(Real faceAmount,
                          const Schedule& schedule,
                          const InterestRate& coupon,
                          BusinessDayConvention paymentConvention = Following,
                          const Date& issueDate = Date(),
                          const Calendar& paymentCalendar = Calendar(),
                          const Period& exCouponPeriod = Period(),
                          const Calendar& exCouponCalendar = Calendar(),
                          const BusinessDayConvention exCouponConvention = Unadjusted,
                          bool exCouponEndOfMonth = false);

        EqualCashFlowLoan(Real faceAmount,
                          const Schedule& schedule,
                          const YieldTermStructure& discountCurve,
                          const DayCounter& accrualDayCounter,
                          Compounding comp = Compounded,
                          Frequency freq = Annual,
                          BusinessDayConvention paymentConvention = Following,
                          const Date& issueDate = Date(),
                          const Calendar& paymentCalendar = Calendar(),
                          const Period& exCouponPeriod = Period(),
                          const Calendar& exCouponCalendar = Calendar(),
                          const BusinessDayConvention exCouponConvention = Unadjusted,
                          bool exCouponEndOfMonth = false);

        Frequency frequency() const { return frequency_; }
        const DayCounter& dayCounter() const { return dayCounter_; }

      protected:
        Frequency frequency_;
        DayCounter dayCounter_;
    };
};
#endif
