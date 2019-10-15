//
//  FloatingRateLoans.cpp
//  Loans
//
//  Created by jose melo on 10/14/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//
#include <ql/qldefines.hpp>
#ifdef BOOST_MSVC
#include <ql/auto_link.hpp>
#endif

#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/instruments/bonds/floatingratebond.hpp>
#include <ql/instruments/loans/loan.hpp>
#include <ql/termstructure.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/time/daycounters/all.hpp>
#include <iostream>
#include <ql/quotes/all.hpp>

using namespace QuantLib;
int main(int, char* []) {
    // Basic Info
    Date today(10, Oct, 2019);
    Settings::instance().evaluationDate() = today;    
    Date issueDate(10, Oct, 2019);
    Date endDate(10, Oct, 2021);
    Calendar calendar = NullCalendar();
    Schedule schedule(issueDate, endDate, Period(Annual), calendar, Unadjusted, Unadjusted,
                      DateGeneration::Backward, false);

    DayCounter rateDayCounter = Thirty360();
    Real faceAmount = 100;
    Real rateCoupon = 0.06;
    boost::shared_ptr<SimpleQuote> rateQuote = boost::make_shared<SimpleQuote>(100.0);
    InterestRate classCoupon(rateCoupon, rateDayCounter, Compounded, Annual);

    boost::shared_ptr<YieldTermStructure> curve(new FlatForward(today, Handle<Quote>(rateQuote), rateDayCounter, Compounded, Annual));

    RelinkableHandle<YieldTermStructure> liborTermStructure;
    boost::shared_ptr<IborIndex> libor(new USDLibor(Period(1, Years), liborTermStructure));
    liborTermStructure.linkTo(curve);


	std::cout << liborTermStructure->discount(1) << std::endl;
    FloatingRateBond bond(0, faceAmount, schedule, libor, rateDayCounter, Following, 0,
                          std::vector<Real>(1, 1.0), std::vector<Rate>(1, 0.01));

    std::vector<boost::shared_ptr<CashFlow> > cashflows = bond.cashflows();
    for (std::size_t i = 0; i < cashflows.size(); ++i)
        std::cout << cashflows[i]->date() << "    " << cashflows[i]->amount() << "\n";

	rateQuote->setValue(0.08);
    std::cout << liborTermStructure->discount(1) << std::endl;
	cashflows = bond.cashflows();
    for (std::size_t i = 0; i < cashflows.size(); ++i)
        std::cout << cashflows[i]->date() << "    " << cashflows[i]->amount() << "\n";
}
