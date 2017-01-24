#if !defined (CHATFRIENDSTATUSCORE_H_)
#define CHATFRIENDSTATUSCORE_H_

#include <Base/Archive.h>
#include <Unicode/Unicode.h>
#include <Unicode/UnicodeUtils.h>

#include "ChatEnum.h"

namespace ChatSystem 
{


	class ChatFriendStatus;

	class ChatFriendStatusCore
	{
	public:
		ChatFriendStatusCore();
		~ChatFriendStatusCore();

		const Plat_Unicode::String &getName() const { return m_name; }
		const Plat_Unicode::String &getAddress() const { return m_address; }
		int16_t getStatus() const { return m_status; }

		void load(Base::ByteStream::ReadIterator &iter, ChatFriendStatus *inf);

	private:
		ChatFriendStatus *m_interface;
		Plat_Unicode::String m_name;
		Plat_Unicode::String m_address;
		Plat_Unicode::String m_comment;

		int16_t m_status;
	};

};

#endif


