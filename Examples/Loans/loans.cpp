#include <ql/qldefines.hpp>
#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif

#include <ql/instruments/loans/loan.hpp>
#include <ql/instruments/loans/structuredloans.hpp>

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

int main(int, char*[]) {
	// Basic Info
    Date today(10, Oct, 2019);


    Date issueDate(10, Oct, 2019);
    Date endDate(10, Oct, 2025);
    Calendar calendar = NullCalendar();
    Schedule schedule(issueDate, endDate, Period(Annual), calendar, Unadjusted, Unadjusted,
                      DateGeneration::Backward, false);

	DayCounter rateDayCounter = Thirty360();
    Real faceAmount = 100;
	Rate rateCoupon = 0.06;
    InterestRate classCoupon(rateCoupon, rateDayCounter, Compounded, Annual);
    FlatForward curve(today, rateCoupon, rateDayCounter);

	EqualCashFlowLoan eq1(faceAmount, schedule, rateCoupon, rateDayCounter);
	cashflowsToTable(eq1);
	wait_on_enter();
};

void cashflowsToTable(Loan loan){ 
	Date date;
    Real interest;
    Real redempt;

	std::cout << "Date"
              << "Interest"
              << "Amortization" << std::endl;
    for (Size i = 0; i <= loan.cashflows().size() / 2; i++) {
        date = loan.cashflows()[i * 2]->date();
        interest = loan.cashflows()[i * 2]->amount();
        redempt = loan.cashflows()[i * 2 + 1]->amount();  
		std::cout << date << interest << redempt << std::endl;
    }
};

void wait_on_enter() {
    std::string dummy;
    std::cout << "Enter to continue..." << std::endl;
    std::cin.get();
};
