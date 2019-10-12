/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2019 Jose Melo

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

/*! \file Chile.hpp
    \brief Chilean calendars
*/

#ifndef quantlib_chilean_calendar_hpp
#define quantlib_chilean_calendar_hpp

#include <ql/time/calendar.hpp>

namespace QuantLib {

    //! Chilean calendar
    /*! Holidays for the Santiago stock exchange
        (data from <https://www.bolsadesantiago.com/#/mercado_horarios_feriados>):
        <ul>
        <li>Saturdays</li>
        <li>Sundays</li>
        <li>New Year's Day, January 1st</li>
        <li>Holy Thursday</li>
        <li>Good Friday</li>
        <li>Labour Day, May 1st</li>
        <li>Glorias Navales, May 21th</li>
        <li>San Pedro y San Pablo, June 29th</li>
        <li>Virgen del Carmen, July 16th</li>
        <li>Independence Day, Septembre 18th </li>
        <li>Glorias del Ejercito, Septembre 19th </li>
        <li>Fiestas Patrias, Septembre 20th </li>
        <li>Encuentro de dos mundos, October 12th </li>
        <li>Evangelical Church Day, October 31th </li>
        <li>All Saints Day, November 1th </li>
        <li>Inmaculada Concepcion, November 8th </li>
        <li>Christmas, December 25th</li>
        <li>Banks Holiday, December 31th</li>
        </ul>

        \ingroup calendars
    */
    class Chile : public Calendar {
      private:
        class BCSImpl : public Calendar::WesternImpl {
          public:
            std::string name() const { return "Santiago, Chile stock exchange"; }
            bool isBusinessDay(const Date&) const;
        };
      public:
        enum Market { BCS   //!< Chile stock exchange calendar
        };
        Chile(Market m = BCS);
    };

}


#endif
