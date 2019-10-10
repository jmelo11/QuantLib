
#include <ql/time/schedule.hpp>
#include <ql/instruments/loans/loan.hpp>
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/qldefines.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/cashflow.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <iostream>

using namespace QuantLib;

void cashflowsToTable(Loan loan);
inline void wait_on_enter();

int main(int, char*[]) {

	// Basic Info
    Date today(10, Oct, 2019);
    Settings::instance().evaluationDate() = today;

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
}

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
}

inline void wait_on_enter() {
    std::string dummy;
    std::cout << "Enter to continue..." << std::endl;
    std::getline(std::cin, dummy);
}
