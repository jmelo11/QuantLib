//
//  loan.cpp
//  Loans
//
//  Created by Jose Melo on 9/22/19.
//  Copyright © 2019 Jose Melo. All rights reserved.
//


#include <ql/qldefines.hpp>

#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif

#include <ql/instruments/loans/loan.hpp>
#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/floatingratecoupon.hpp>
#include <ql/math/solvers1d/brent.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/pricingengines/bond/bondfunctions.hpp>
#include <ql/quantlib.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/cashflows/cashflows.hpp>

namespace QuantLib{
	Loan::Loan(Natural settlementDays,
		const Calendar& calendar,
		const Date& issueDate,
		const Leg& coupons)
		:Bond(settlementDays,calendar,issueDate){
		if (!coupons.empty()) {
            cashflows_ = coupons;
			std::sort(cashflows_.begin(), cashflows_.end(),
				earlier_than<ext::shared_ptr<CashFlow> >());

			if (issueDate_ != Date()) {
				QL_REQUIRE(issueDate_<cashflows_[0]->date(),
					"issue date (" << issueDate_ <<
					") must be earlier than first payment date (" <<
					cashflows_[0]->date() << ")");
			}

			maturityDate_ = coupons.back()->date();
			/*Can't use virtual method in constructor, then changed name of it.*/
			addRedemptionsToLoanCashflows();
		}

		registerWith(Settings::instance().evaluationDate());
		for (Leg::const_iterator c = cashflows_.begin(); c != cashflows_.end();
			++c)
			registerWith(*c);
	}

	Loan::Loan(Natural settlementDays,
		const Calendar& calendar,
		Real faceAmount,
		const Date& maturityDate,
		const Date& issueDate,
		const Leg& cashflows)
        : Bond(settlementDays,calendar,issueDate){            
            maturityDate_ = maturityDate;
		if (!cashflows.empty()) {
            cashflows_ = cashflows;
			std::sort(cashflows_.begin(), cashflows_.end() - 1,
				earlier_than<ext::shared_ptr<CashFlow> >());

			if (maturityDate_ == Date())
				maturityDate_ = CashFlows::maturityDate(cashflows);

			if (issueDate_ != Date()) {
				QL_REQUIRE(issueDate_<cashflows_[0]->date(),
					"issue date (" << issueDate_ <<
					") must be earlier than first payment date (" <<
					cashflows_[0]->date() << ")");
			}

			notionals_.resize(2);
			notionalSchedule_.resize(2);

			notionalSchedule_[0] = Date();
			notionals_[0] = faceAmount;

			notionalSchedule_[1] = maturityDate_;
			notionals_[1] = 0.0;

			redemptions_.push_back(cashflows.back());
		}

		registerWith(Settings::instance().evaluationDate());
		for (Leg::const_iterator c = cashflows_.begin(); c != cashflows_.end();
			++c)
			registerWith(*c);
	}

	void Loan::addRedemptionsToLoanCashflows(const std::vector<Real>& redemptions) {
		// First, we gather the notional information from the cashflows
		calculateNotionalsFromLoanCashflows();
		// Then, we create the redemptions based on the notional
		// information and we add them to the cashflows vector after
		// the coupons.
		redemptions_.clear();
		for (Size i = 1; i<notionalSchedule_.size(); ++i) {
			Real R = i < redemptions.size() ? redemptions[i] :
				!redemptions.empty() ? redemptions.back() :
				100.0;
			Real amount = (R / 100.0)*(notionals_[i - 1] - notionals_[i]);
			ext::shared_ptr<CashFlow> payment;
			if (i < notionalSchedule_.size() - 1)
				payment.reset(new AmortizingPayment(amount,
					notionalSchedule_[i]));
			else
				payment.reset(new Redemption(amount, notionalSchedule_[i]));
			cashflows_.push_back(payment);
			redemptions_.push_back(payment);
		}
		// stable_sort now moves the redemptions to the right places
		// while ensuring that they follow coupons with the same date.
		std::stable_sort(cashflows_.begin(), cashflows_.end(),
			earlier_than<ext::shared_ptr<CashFlow> >());
	}
	void Loan::calculateNotionalsFromLoanCashflows() {
		notionalSchedule_.clear();
		notionals_.clear();

		Date lastPaymentDate = Date();
		notionalSchedule_.push_back(Date());
		for (Size i = 0; i<cashflows_.size(); ++i) {
			ext::shared_ptr<Coupon> coupon =
				ext::dynamic_pointer_cast<Coupon>(cashflows_[i]);
			if (!coupon)
				continue;

			Real notional = coupon->nominal();
			// we add the notional only if it is the first one...
			if (notionals_.empty()) {
				notionals_.push_back(coupon->nominal());
				lastPaymentDate = coupon->date();
			}
			else if (!close(notional, notionals_.back())) {
				// ...or if it has changed.
				notionals_.push_back(coupon->nominal());
				// in this case, we also add the last valid date for
				// the previous one...
				notionalSchedule_.push_back(lastPaymentDate);
				// ...and store the candidate for this one.
				lastPaymentDate = coupon->date();
			}
			else {
				// otherwise, we just extend the valid range of dates
				// for the current notional.
				lastPaymentDate = coupon->date();
			}
		}
		QL_REQUIRE(!notionals_.empty(), "no coupons provided");
		notionals_.push_back(0.0);
		notionalSchedule_.push_back(lastPaymentDate);
	}
}
