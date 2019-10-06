#include <ql/qldefines.hpp>
#include <ql/instruments/bond.hpp>
#include <ql/time/calendar.hpp>
#include <ql/cashflow.hpp>
#include <ql/compounding.hpp>
#include <vector>

#ifndef quantlib_loan_hpp
#define quantlib_loan_hpp

namespace QuantLib {
	//class DayCounter;
	class Loan : public Bond
	{
	public:

		Loan(Natural settlementDays,
			const Calendar& calendar,
			const Date& issueDate = Date(),
			const Leg& coupons = Leg());

		//! old constructor for non amortizing bonds.
		/*! \warning The last passed cash flow must be the bond
		redemption. No other cash flow can have a date
		later than the redemption date.
		*/
		Loan(Natural settlementDays,
			const Calendar& calendar,
			Real faceAmount,
			const Date& maturityDate,
			const Date& issueDate = Date(),
			const Leg& cashflows = Leg());

		/*Same as bond method, but without notionals warning*/
		void addRedemptionsToLoanCashflows(const std::vector<Real>& redemptions = std::vector<Real>());
		void calculateNotionalsFromLoanCashflows();
	};
}
#endif
