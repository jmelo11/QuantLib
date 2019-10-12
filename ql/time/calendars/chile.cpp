/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/time/calendars/Chile.hpp>

namespace QuantLib {

    Chile::Chile(Market) {
        // all calendar instances share the same implementation instance
        static ext::shared_ptr<Calendar::Impl> impl(
                                                   new Chile::BCSImpl);
        impl_ = impl;
    }

    bool Chile::BCSImpl::isBusinessDay(const Date& date) const {
        Weekday w = date.weekday();
        Day d = date.dayOfMonth(), dd = date.dayOfYear();
        Month m = date.month();
        Year y = date.year();
        Day em = easterMonday(y);
        if (isWeekend(w)
            // New Year's Day
            || (d == 1 && m == January)
            // Holy Thursday
            || (dd == em-4)
            // Good Friday
            || (dd == em-3)
            // Labour Day
            || (d == 1 && m == May)
            // Glorias Navales
            || (d == 21 && m == May)
            // San Pedro y San Pablo
            || (d == 29 && m == June)
            // Virgen del Carmen
            || (d == 16 && m == July)
            // Independence Day
            || (d == 18 && m == September)
            // Glorias del Ejercito
            || (d == 19 && m == September)
            // Fiestas Patrias
            || (d == 20 && m == September)
            // Encuentro de dos mundos
            || (d == 12 && m == October)
            // Evangelical Church Day
            || (d == 31 && m == October)
            // All Saints Day
            || (d == 1 && m == November)
            // Inmaculada Concepcion
            || (d == 8 && m == November)
            // Christmas
            || (d == 25 && m == December)
            // Banks Holiday
            || ((d == 31 || (d == 30 && w == Friday)) && m == December))
            return false;
        return true;
    }

}

