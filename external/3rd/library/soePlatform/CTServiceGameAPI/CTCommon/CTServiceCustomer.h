#ifndef CTSERVICECUSTOMER_H
#define CTSERVICECUSTOMER_H

#include <string.h>

namespace CTService 
{

//--------------------------------------------------------
class CTServiceCustomer
//--------------------------------------------------------
{
	inline void copy_wide_string(uint16_t * target, const uint16_t * source, int length)
	{
		if (!target)
			return;
		if (!source)
		{
			target[0] = 0;
			return;
		}
		
		int offset = 0;
		while (offset < length && (target[(offset+1)] = source[offset])){
			target[length-1] = 0;
			offset++;
		}
	}

	public:
		enum {	SHORT_NAME_SIZE=64,			SHORT_NAME_BUFFER,
				PHONE_SIZE=32,				PHONE_BUFFER, 
				EMAIL_SIZE=128,				EMAIL_BUFFER, 
				CARD_NAME_SIZE=255,			CARD_NAME_BUFFER, 
				CARD_ADDRESS1_SIZE=255,		CARD_ADDRESS1_BUFFER, 
				CARD_ADDRESS2_SIZE=255,		CARD_ADDRESS2_BUFFER, 
				CARD_CITY_SIZE=255,			CARD_CITY_BUFFER, 
				CARD_STATE_SIZE=255,		CARD_STATE_BUFFER, 
				CARD_ZIP_SIZE=255,			CARD_ZIP_BUFFER, 
				CARD_COUNTRY_SIZE=2,		CARD_COUNTRY_BUFFER, 
				CARD_NUMBER_SIZE=255,		CARD_NUMBER_BUFFER, 
				CARD_EXPIRATION_MONTH_SIZE=2,CARD_EXPIRATION_MONTH_BUFFER, 
				CARD_EXPIRATION_YEAR_SIZE=4,CARD_EXPIRATION_YEAR_BUFFER };
		CTServiceCustomer() : mFirstName(), mLastName(), mPhone(), mEmail(), mStationID(0), mCardName(), mCardAddress1(), mCardAddress2(), mCardCity(), mCardState(), mCardZip(), mCountry(), mCardNumber(), mCardExpirationMonth(), mCardExpirationYear()
			{  mFirstName[0]	= 0;
			  mLastName[0]	= 0;
			  mPhone[0]	= 0;
			  mEmail[0]	= 0;
			  mCardName[0]	= 0;
			  mCardAddress1[0]	= 0;
			  mCardAddress2[0]	= 0;
			  mCardCity[0]	= 0;
			  mCardState[0]	= 0;
			  mCardZip[0]	= 0;
			  mCountry[0]	= 0;
			  mCardNumber[0]	= 0;
			  mCardExpirationMonth[0]= 0;
			  mCardExpirationYear[0]= 0; }


		void			SetFirstName(const uint16_t * value)			{ copy_wide_string(mFirstName, value, SHORT_NAME_BUFFER); }
		uint16_t	*GetFirstName()										{ return mFirstName; }

		void			SetLastName(const uint16_t * value)			{ copy_wide_string(mLastName, value, SHORT_NAME_BUFFER); }
		uint16_t	*GetLastName()										{ return mLastName; }

		void			SetPhone(const uint16_t * value)				{ copy_wide_string(mPhone, value, PHONE_BUFFER); }
		uint16_t	*GetPhone()											{ return mPhone; }

		void			SetEmail(const uint16_t * value)				{ copy_wide_string(mEmail, value, EMAIL_BUFFER); }
		uint16_t	*GetEmail()											{ return mEmail; }

		void			SetStationID(const unsigned value)					{ mStationID = value; }
		unsigned		GetStationID()										{ return mStationID; }

		void			SetCardName(const uint16_t * value)			{ copy_wide_string(mCardName, value, CARD_NAME_BUFFER); }
		uint16_t	*GetCardName()										{ return mCardName; }

		void			SetCardAddress1(const uint16_t * value)		{ copy_wide_string(mCardAddress1, value, CARD_ADDRESS1_BUFFER); }
		uint16_t	*GetCardAddress1()									{ return mCardAddress1; }

		void			SetCardAddress2(const uint16_t * value)		{ copy_wide_string(mCardAddress2, value, CARD_ADDRESS2_BUFFER); }
		uint16_t	*GetCardAddress2()									{ return mCardAddress2; }

		void			SetCardCity(const uint16_t * value)			{ copy_wide_string(mCardCity, value, CARD_CITY_BUFFER); }
		uint16_t	*GetCardCity()										{ return mCardCity; }

		void			SetCardState(const uint16_t * value)			{ copy_wide_string(mCardState, value, CARD_STATE_BUFFER); }
		uint16_t	*GetCardState()										{ return mCardState; }

		void			SetCardZip(const uint16_t * value)			{ copy_wide_string(mCardZip, value, CARD_ZIP_BUFFER); }
		uint16_t	*GetCardZip()										{ return mCardZip; }

		void			SetCountry(const uint16_t * value)			{ copy_wide_string(mCountry, value, CARD_COUNTRY_BUFFER); }
		uint16_t	*GetCountry()										{ return mCountry; }

		void			SetCardNumber(const uint16_t * value)			{ copy_wide_string(mCardNumber, value, CARD_NUMBER_BUFFER); }
		uint16_t	*GetCardNumber()									{ return mCardNumber; }

		void			SetCardExpirationMonth(const uint16_t * value){ copy_wide_string(mCardExpirationMonth, value, CARD_EXPIRATION_MONTH_BUFFER); }
		uint16_t	*GetCardExpirationMonth()							{ return mCardExpirationMonth; }

		void			SetCardExpirationYear(const uint16_t * value)	{ copy_wide_string(mCardExpirationYear, value, CARD_EXPIRATION_YEAR_BUFFER); }
		uint16_t	*GetCardExpirationYear()							{ return mCardExpirationYear; }


	private:
		uint16_t	mFirstName[SHORT_NAME_BUFFER];
		uint16_t	mLastName[SHORT_NAME_BUFFER];
		uint16_t	mPhone[PHONE_BUFFER];
		uint16_t	mEmail[EMAIL_BUFFER];
		unsigned		mStationID;
		uint16_t	mCardName[CARD_NAME_BUFFER];
		uint16_t	mCardAddress1[CARD_ADDRESS1_BUFFER];
		uint16_t	mCardAddress2[CARD_ADDRESS2_BUFFER];
		uint16_t	mCardCity[CARD_CITY_BUFFER];
		uint16_t	mCardState[CARD_STATE_BUFFER];
		uint16_t	mCardZip[CARD_ZIP_BUFFER];
		uint16_t	mCountry[CARD_COUNTRY_BUFFER];
		uint16_t	mCardNumber[CARD_NUMBER_BUFFER];
		uint16_t	mCardExpirationMonth[CARD_EXPIRATION_MONTH_BUFFER];
		uint16_t	mCardExpirationYear[CARD_EXPIRATION_YEAR_BUFFER];
	};

}; // namespace

#endif

