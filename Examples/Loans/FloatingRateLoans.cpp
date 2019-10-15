//
//  FloatingRateLoans.cpp
//  Loans
//
//  Created by jose melo on 10/14/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//
#include <ql/qldefines.hpp>
#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif

#include <ql/instruments/loans/loan.hpp>
#include <ql/quantlib.hpp>

using namespace QuantLib;
int main(int, char*[]) {
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
}
