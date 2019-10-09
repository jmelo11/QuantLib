//
//  structuredloans.cpp
//  Loans
//
//  Created by jose melo on 9/22/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//

#include <ql/instruments/loans/structuredloans.hpp>
#include <ql/instruments/loans/loan.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/qldefines.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <ql/math/solvers1d/brent.hpp>


namespace QuantLib{
//equal amortization loan
EqualAmortizationLoan::EqualAmortizationLoan(Natural settlementDays,
                                             Real faceAmount,
                                             const Schedule& schedule,
                                             const Real coupon,
                                             const DayCounter& accrualDayCounter,
                                             BusinessDayConvention paymentConvention,
                                             const Date& issueDate,
                                             const Calendar& paymentCalendar,
                                             const Period& exCouponPeriod,
                                             const Calendar& exCouponCalendar,
                                             const BusinessDayConvention exCouponConvention,
                                             bool exCouponEndOfMonth): Loan(settlementDays,schedule.calendar(),issueDate){
    
    Real s = schedule.size()-1;
    Real redemption = faceAmount/s;
    Real notional = faceAmount;
    std::vector<Real> notionals(1,faceAmount);
    for (Size i=0;i<schedule.size();i++){
        notional -= redemption;
        notionals.push_back(notional);
    }
    
    cashflows_ = FixedRateLeg(schedule)
    .withNotionals(notionals)
    .withCouponRates(coupon, accrualDayCounter)
    .withPaymentCalendar(schedule.calendar())
    .withPaymentAdjustment(paymentConvention)
    .withExCouponPeriod(exCouponPeriod,
                        exCouponCalendar,
                        exCouponConvention,
                        exCouponEndOfMonth);
    
    addRedemptionsToLoanCashflows();
    QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
};

EqualAmortizationLoan::EqualAmortizationLoan(Natural settlementDays,
											Real faceAmount,
											const Schedule& schedule,
											const InterestRate coupon,
											BusinessDayConvention paymentConvention,
											const Date& issueDate,
											const Calendar& paymentCalendar,
											const Period& exCouponPeriod,
											const Calendar& exCouponCalendar,
											const BusinessDayConvention exCouponConvention,
											bool exCouponEndOfMonth) : Loan(settlementDays, schedule.calendar(), issueDate) {

							Real s = schedule.size() - 1;
							Real redemption = faceAmount / s;
							Real notional = faceAmount;
							std::vector<Real> notionals(1, faceAmount);
							for (Size i = 0; i<schedule.size(); i++) {
								notional -= redemption;
								notionals.push_back(notional);
							}

							cashflows_ = FixedRateLeg(schedule)
								.withNotionals(notionals)
								.withCouponRates(coupon)
								.withPaymentCalendar(schedule.calendar())
								.withPaymentAdjustment(paymentConvention)
								.withExCouponPeriod(exCouponPeriod,
									exCouponCalendar,
									exCouponConvention,
									exCouponEndOfMonth);

							addRedemptionsToLoanCashflows();
							QL_ENSURE(!cashflows().empty(), "bond with no cashflows!");
					};
//Crea bono con amort. iguales con una curva dada
EqualAmortizationLoan::EqualAmortizationLoan(Natural settlementDays,
                                             Real faceAmount,
                                             Schedule schedule,
                                             YieldTermStructure& discountCurve,
                                             const DayCounter& accrualDayCounter,
                                             BusinessDayConvention paymentConvention,
                                             const Date& issueDate,
                                             const Calendar& paymentCalendar,
                                             const Period& exCouponPeriod,
                                             const Calendar& exCouponCalendar,
                                             const BusinessDayConvention exCouponConvention,
                                             bool exCouponEndOfMonth): Loan(settlementDays,schedule.calendar(),issueDate){
    
    Real s = schedule.size()-1;
    Real redemption = faceAmount/s;
    Real notional = faceAmount;
    std::vector<Real> notionals(1,faceAmount);
    for (Size i=0;i<schedule.size();i++){
        notional -= redemption;
        notionals.push_back(notional);
    }
    Real coupon = 0.0;
    Leg tmp_cashflows = FixedRateLeg(schedule)
    .withNotionals(notionals)
    .withCouponRates(coupon, accrualDayCounter)
    .withPaymentCalendar(schedule.calendar())
    .withPaymentAdjustment(paymentConvention)
    .withExCouponPeriod(exCouponPeriod,
                        exCouponCalendar,
                        exCouponConvention,
                        exCouponEndOfMonth);
    
    ext::shared_ptr<CashFlow> payment;
    for (Size i=0;i<schedule.size()-1;i++){
        if (i < schedule.size()-1)
            payment.reset(new AmortizingPayment(redemption,
                                                tmp_cashflows[i]->date()));
        else
            payment.reset(new Redemption(redemption, tmp_cashflows[i]->date()));
        tmp_cashflows.push_back(payment);
    }
    std::stable_sort(tmp_cashflows.begin(), tmp_cashflows.end(),
                     earlier_than<ext::shared_ptr<CashFlow> >());
    
    Rate atmRate = CashFlows::atmRate(tmp_cashflows, discountCurve, true, schedule.startDate(), schedule.startDate(), faceAmount);
    
    cashflows_ = FixedRateLeg(schedule)
    .withNotionals(notionals)
    .withCouponRates(atmRate, accrualDayCounter)
    .withPaymentCalendar(schedule.calendar())
    .withPaymentAdjustment(paymentConvention)
    .withExCouponPeriod(exCouponPeriod,
                        exCouponCalendar,
                        exCouponConvention,
                        exCouponEndOfMonth);
    
    addRedemptionsToLoanCashflows();
    QL_ENSURE(!cashflows().empty(), "loan with no cashflows!");
};

//nameless namespace -> equal coupon finder
namespace {
class CouponFinder{
public:
	CouponFinder(Real faceAmount, const Schedule& schedule, const DayCounter& dayCounter, Compounding comp, Frequency freq, const YieldTermStructure& discountCurve = FlatForward(Date(),0,DayCounter()));
    Real equalCashFlow();
    Real notionalsFromRate(Real coupon) const;
    Real operator()(Rate coupon) const;
    Real findCoupon(Real accuracy = 0.1, Size maxIterations = 100 ,Rate guess = 0.0);
    std::vector<Real> calculatedNotionals();
private:
    Real faceAmount_;
    Real targetCashFlow;
	const DayCounter dayCounter_;
    Compounding comp_;
    Frequency freq_;
    const Schedule& schedule_;
	mutable std::vector <Real> notionals_;
	mutable std::vector <Real> amortizations_;
};
CouponFinder::CouponFinder(Real faceAmount, const Schedule& schedule, const DayCounter& dayCounter, Compounding comp, Frequency freq, const YieldTermStructure& discountCurve):
	faceAmount_(faceAmount),dayCounter_(dayCounter),comp_(comp),freq_(freq), schedule_(schedule){
    Real df_sum = 0.0;
    for (Size i = 1; i < schedule.size(); i++) {
        df_sum += discountCurve.discount(schedule.at(i));
    }
    targetCashFlow = faceAmount * discountCurve.discount(schedule.startDate()) / df_sum;
};
Real CouponFinder::equalCashFlow(){
    return targetCashFlow;
};
Real CouponFinder::notionalsFromRate(Real coupon) const{
	InterestRate tmp_rate(coupon, dayCounter_, comp_, freq_);    
	Real interest;
	Real amort = 0;
	amortizations_.clear();
	notionals_ = { faceAmount_ };
    for (Size i = 0; i < schedule_.size()-1; i++) {
        interest = (tmp_rate.compoundFactor(schedule_.at(i), schedule_.at(i+1))-1)*notionals_[i];
        amortizations_.push_back(targetCashFlow - interest);
        notionals_.push_back(notionals_[i]-amortizations_[i]);
		amort += amortizations_[i];
    }
    return amort;
};
Real CouponFinder::findCoupon(Real accuracy,Size maxIterations,Rate guess){
    Real step = 0.01;
	Brent solver;
    solver.setMaxEvaluations(maxIterations);
    return solver.solve(*this, accuracy, guess, step);
}
Real CouponFinder::operator()(Rate coupon) const {
    return faceAmount_ - notionalsFromRate(coupon);
};
std::vector<Real>CouponFinder::calculatedNotionals(){
    return notionals_;
};
};

//loans with equal payments
EqualCashFlowLoan::EqualCashFlowLoan(Natural settlementDays,
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
									bool exCouponEndOfMonth):Loan(settlementDays, schedule.calendar(), issueDate) {

	CouponFinder crf(faceAmount, schedule, dayCounter, comp, freq);
	Real coupon = crf.notionalsFromRate(coupon);
	std::vector <Real > notionals = crf.calculatedNotionals();
	cashflows_ = FixedRateLeg(schedule)
		.withNotionals(notionals)
		.withCouponRates(coupon,dayCounter)
		.withPaymentCalendar(schedule.calendar())
		.withPaymentAdjustment(paymentConvention)
		.withExCouponPeriod(exCouponPeriod,
			exCouponCalendar,
			exCouponConvention,
			exCouponEndOfMonth);
	addRedemptionsToLoanCashflows();
	QL_ENSURE(!cashflows().empty(), "loan with no cashflows!");
};

EqualCashFlowLoan::EqualCashFlowLoan(Natural settlementDays,
									Real faceAmount,
									const Schedule& schedule,
									const InterestRate& coupon,
									BusinessDayConvention paymentConvention,
									const Date& issueDate,
									const Calendar& paymentCalendar,
									const Period& exCouponPeriod,
									const Calendar& exCouponCalendar,
									const BusinessDayConvention exCouponConvention,
									bool exCouponEndOfMonth) : Loan(settlementDays, schedule.calendar(), issueDate) {

									CouponFinder crf(faceAmount, schedule, coupon.dayCounter(), coupon.compounding(), coupon.frequency());
									Real coupon = crf.notionalsFromRate(coupon);
									std::vector <Real > notionals = crf.calculatedNotionals();
									cashflows_ = FixedRateLeg(schedule)
										.withNotionals(notionals)
										.withCouponRates(coupon)
										.withPaymentCalendar(schedule.calendar())
										.withPaymentAdjustment(paymentConvention)
										.withExCouponPeriod(exCouponPeriod,
											exCouponCalendar,
											exCouponConvention,
											exCouponEndOfMonth);
									addRedemptionsToLoanCashflows();
									QL_ENSURE(!cashflows().empty(), "loan with no cashflows!");

				};


EqualCashFlowLoan::EqualCashFlowLoan(Natural settlementDays,
                                     Real faceAmount,
                                     const Schedule& schedule,
                                     const YieldTermStructure& discountCurve,
                                     const DayCounter& dayCounter,
                                     Compounding comp,
                                     Frequency freq,
                                     BusinessDayConvention paymentConvention,
                                     const Date& issueDate,
                                     const Calendar& paymentCalendar,
                                     const Period& exCouponPeriod,
                                     const Calendar& exCouponCalendar,
                                     const BusinessDayConvention exCouponConvention,
                                     bool exCouponEndOfMonth): Loan(settlementDays, schedule.calendar(), issueDate) {
    
	CouponFinder crf(faceAmount,  schedule, dayCounter, comp, freq, discountCurve);
	Real coupon = crf.findCoupon();
	std::vector <Real> notionals = crf.calculatedNotionals();
    cashflows_ = FixedRateLeg(schedule)
    .withNotionals(notionals)
    .withCouponRates(coupon, dayCounter)
    .withPaymentCalendar(schedule.calendar())
    .withPaymentAdjustment(paymentConvention)
    .withExCouponPeriod(exCouponPeriod,
                        exCouponCalendar,
                        exCouponConvention,
                        exCouponEndOfMonth);
    
    addRedemptionsToLoanCashflows();
    QL_ENSURE(!cashflows().empty(), "loan with no cashflows!");
    };
};
