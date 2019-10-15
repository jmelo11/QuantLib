#include <ql/qldefines.hpp>
#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif

#include <ql/instruments/loans/loan.hpp>
#include <ql/instruments/loans/fixedrateloans.hpp>

#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/instruments/bonds/zerocouponbond.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/cashflows/couponpricer.hpp>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/termstructures/yield/bondhelpers.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>

#include <iostream>

using namespace QuantLib;
void cashflowsToTable(Loan loan);
void wait_on_enter();

void main2(int, char*[]) {
	// Basic Info
    Date today(10, Oct, 2019);


    Date issueDate(10, Oct, 2019);
    Date endDate(10, Oct, 2021);
    Calendar calendar = NullCalendar();
    Schedule schedule(issueDate, endDate, Period(Annual), calendar, Unadjusted, Unadjusted,
                      DateGeneration::Backward, false);

	DayCounter rateDayCounter = Thirty360();
    Real faceAmount = 100;
	Rate rateCoupon = 0.06;
    InterestRate classCoupon(rateCoupon, rateDayCounter, Compounded, Annual);
    FlatForward curve(today, rateCoupon, rateDayCounter, Compounded, Annual);

	std::cout << std::endl;
	std::cout << "Equal Amortization Loans:" << std::endl;
	EqualAmortizationLoan eq1(faceAmount, schedule, rateCoupon, rateDayCounter);    
    EqualAmortizationLoan eq2(faceAmount, schedule, classCoupon);
    EqualAmortizationLoan eq3(faceAmount, schedule, curve, rateDayCounter);
    cashflowsToTable(eq1);
	cashflowsToTable(eq2);
    cashflowsToTable(eq3);

	std::cout << std::endl;
	std::cout << "Equal Cashflow Loans:" << std::endl;
    EqualCashFlowLoan ec1(faceAmount, schedule, rateCoupon, rateDayCounter);
    EqualCashFlowLoan ec2(faceAmount, schedule, classCoupon);
    EqualCashFlowLoan ec3(faceAmount, schedule, curve, rateDayCounter);
    cashflowsToTable(ec1);
    cashflowsToTable(ec2);
    cashflowsToTable(ec3);

	std::cout << std::endl;
	std::cout << "Unequal Amortization Loans - First Test:   " << std::endl;
	std::vector<Real> amortizations{50, 50};
    UnEqualAmortizationLoan uneq_a1(amortizations, schedule, rateCoupon, rateDayCounter);
    UnEqualAmortizationLoan uneq_a2(amortizations, schedule, classCoupon);	
	UnEqualAmortizationLoan uneq_a3(amortizations, schedule, curve, rateDayCounter);

	cashflowsToTable(uneq_a1);
    cashflowsToTable(uneq_a2);
    cashflowsToTable(uneq_a3);
	
	amortizations.clear();
    amortizations.assign({-50, 50, 0, 20});
    endDate = Date(10, Oct, 2023);   
	schedule = Schedule(issueDate, endDate, Period(Annual), calendar, Unadjusted, Unadjusted,
                        DateGeneration::Backward, false);
        
    std::cout << "Unequal Amortization Loans - Second Test:   " << std::endl;
    UnEqualAmortizationLoan uneq_b1(amortizations, schedule, rateCoupon, rateDayCounter);
    UnEqualAmortizationLoan uneq_b2(amortizations, schedule, classCoupon);
    UnEqualAmortizationLoan uneq_b3(amortizations, schedule, curve, rateDayCounter);

    cashflowsToTable(uneq_b1);
    cashflowsToTable(uneq_b2);
    cashflowsToTable(uneq_b3);
	wait_on_enter();
};

void cashflowsToTable(Loan loan){ 
	Date date;
    Real interest;
    Real redempt;

	std::cout << std::setw(10) << "Date" << std::setw(20) << "Interest" << std::setw(30) << "Amortization" << std::endl;
    for (Size i = 0; i < loan.cashflows().size() / 2; i++) {
        date = loan.cashflows()[i * 2]->date();
        interest = loan.cashflows()[i * 2]->amount();
        redempt = loan.cashflows()[i * 2 + 1]->amount();  
		std::cout << std::setw(10) << date << std::setw(20) << interest << std::setw(30) << redempt << std::endl;
    }
};

void wait_on_enter() {
    std::string dummy;
    std::cout << "Enter to continue..." << std::endl;
    std::cin.get();
};
