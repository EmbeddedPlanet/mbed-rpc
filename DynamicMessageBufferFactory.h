/*
 * DynamicMessageBuffer.h
 *
 * Pulled class definition out of erpc_setup_mbf_dynamic.cpp
 * for use without C wrapper
 *
 *  Created on: May 13, 2019
 *      Author: becksteing
 */

#ifndef EPL_CORE_LIBRARIES_MBED_RPC_DYNAMICMESSAGEBUFFERFACTORY_H_
#define EPL_CORE_LIBRARIES_MBED_RPC_DYNAMICMESSAGEBUFFER_H_

#include "erpc_message_buffer.h"
#include "platform/mbed_assert.h"

namespace ep
{

	/*!
	 * @brief Dynamic message buffer factory.
	 */
	class DynamicMessageBufferFactory : public erpc::MessageBufferFactory
	{
	public:
		virtual erpc::MessageBuffer create()
		{
			uint8_t *buf = new uint8_t[ERPC_DEFAULT_BUFFER_SIZE];
			return erpc::MessageBuffer(buf, ERPC_DEFAULT_BUFFER_SIZE);
		}

		virtual void dispose(erpc::MessageBuffer *buf)
		{
			MBED_ASSERT(buf);
			if (buf->get())
			{
				delete[] buf->get();
			}
		}
	};
}
#endif /* EPL_CORE_LIBRARIES_MBED_RPC_DYNAMICMESSAGEBUFFERFACTORY_H_ */
