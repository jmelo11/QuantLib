

#ifndef structuredloans_hpp
#define structuredloans_hpp
#endif /* structuredloans_hpp */

#include <ql/instruments/loans/loan.hpp>
#include <ql/time/schedule.hpp>
#include <ql/qldefines.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>

namespace QuantLib{
class EqualAmortizationLoan : public Loan {
    
public:
    EqualAmortizationLoan(Natural settlementDays,
                          Real faceAmount,
                          const Schedule& schedule,
                          const Real coupon,
                          const DayCounter& accrualDayCounter,
                          BusinessDayConvention paymentConvention = Following,
                          const Date& issueDate = Date(),
                          const Calendar& paymentCalendar = Calendar(),
                          const Period& exCouponPeriod = Period(),
                          const Calendar& exCouponCalendar = Calendar(),
                          const BusinessDayConvention exCouponConvention = Unadjusted,
                          bool exCouponEndOfMonth = false);
    
	EqualAmortizationLoan(Natural settlementDays,
							Real faceAmount,
							const Schedule& schedule,
							const InterestRate coupon,
							BusinessDayConvention paymentConvention,
							const Date& issueDate,
							const Calendar& paymentCalendar,
							const Period& exCouponPeriod,
							const Calendar& exCouponCalendar,
							const BusinessDayConvention exCouponConvention,
							bool exCouponEndOfMonth);
    EqualAmortizationLoan(Natural settlementDays,
                          Real faceAmount,
                          Schedule schedule,
                          YieldTermStructure& discountCurve,
                          const DayCounter& accrualDayCounter,
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
	EqualCashFlowLoan(Natural settlementDays,
						Real faceAmount,
						const Schedule& schedule,
						const Real coupon,
						const DayCounter& dayCounter,
						Compounding comp,
						Frequency freq,
						BusinessDayConvention paymentConvention,
						const Date& issueDate,
						const Calendar& paymentCalendar,
						const Period& exCouponPeriod,
						const Calendar& exCouponCalendar,
						const BusinessDayConvention exCouponConvention,
						bool exCouponEndOfMonth);

    EqualCashFlowLoan(Natural settlementDays,
                      Real faceAmount,
                      const Schedule& schedule,
                      const InterestRate& coupon,
                      BusinessDayConvention paymentConvention = Following,
                      const Date& issueDate = Date(),
                      const Calendar& paymentCalendar = Calendar(),
                      const Period& exCouponPeriod = Period(),
                      const Calendar& exCouponCalendar = Calendar(),
                      const BusinessDayConvention exCouponConvention = Unadjusted,
                      bool exCouponEndOfMonth = false);
    
    EqualCashFlowLoan(Natural settlementDays,
						Real faceAmount,
						const Schedule& schedule,
						const YieldTermStructure& discountCurve,
						const DayCounter& accrualDayCounter,
						Compounding comp,
						Frequency freq,
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
}
