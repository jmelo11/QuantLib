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
                      const InterestRate& coupon,
                      const DayCounter& accrualDayCounter,
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
private:
    Leg buildEqualCashFlows(Real cf, Real faceAmount, InterestRate coupon,
                            Schedule schedule,
                            BusinessDayConvention paymentConvention,
                            const Calendar& paymentCalendar,
                            const Period& exCouponPeriod,
                            const Calendar& exCouponCalendar,
                            const BusinessDayConvention exCouponConvention,
                            bool exCouponEndOfMonth);
protected:
    Frequency frequency_;
    DayCounter dayCounter_;
};
}
