#include "GenericMessage.h"

#ifdef EXTERNAL_DISTRO
namespace NAMESPACE
{
#endif
	namespace GenericAPI
	{
		GenericMessage::GenericMessage(int16_t type)
			: m_type(type)
		{
		}

		GenericRequest::GenericRequest(int16_t type)
			: GenericMessage(type)
		{
		}

		GenericResponse::GenericResponse(int16_t type, unsigned result, void *user)
			: GenericMessage(type),
			m_result(result),
			m_user(user),
			m_track(0),
			m_timeout(100)
		{
		}
	};

#ifdef EXTERNAL_DISTRO
};
#endif