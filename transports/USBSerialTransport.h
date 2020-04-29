/*
 * USBSerialTransport.h
 *
 *  Created on: April 21, 2020
 *      Author: trowbridgec
 */

#ifndef EPL_CORE_LIBRARIES_MBED_RPC_TRANSPORTS_USBTRANSPORT_H_
#define EPL_CORE_LIBRARIES_MBED_RPC_TRANSPORTS_USBTRANSPORT_H_

#include "erpc_framed_transport.h"
#include "USBSerial.h"

namespace ep {

class USBSerialTransport: public erpc::FramedTransport, public USBSerial {
public:

	/** Create a USBSerialTransport, connected to the default USBPhy
	 *
     *  @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
     *  @param vendor_id Your vendor_id (default: 0x1f00)
     *  @param product_id Your product_id (default: 0x2012)
     *  @param product_release Your product_release (default: 0x0001)
	 *
	 */
	USBSerialTransport(bool connect_blocking = true, uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001) :
			erpc::FramedTransport(), USBSerial(connect_blocking, vendor_id, product_id, product_release) {
	}

	/*!
	 * @brief Destructor.
	 */
	virtual ~USBSerialTransport(void) {
	}

protected:

	/*!
	 * @brief Write data to Serial peripheral.
	 *
	 * @param[in] data Buffer to send.
	 * @param[in] size Size of data to send.
	 *
	 * @retval kErpcStatus_SendFailed Serial failed to receive data.
	 * @retval kErpcStatus_Success Successfully received all data.
	 */
	virtual erpc_status_t underlyingSend(const uint8_t *data, uint32_t size) {

		ssize_t bytes_sent = 0, result = 0;
		while((unsigned int)bytes_sent < size) {
			result = this->write(&data[bytes_sent], size);

			// Negative error on failure, break out
			if(result < 0) {
				break;
			}

			// Accumulate the number of bytes sent
			bytes_sent += result;

		}

		if((unsigned int)bytes_sent == size) {
			return kErpcStatus_Success;
		} else {
			return kErpcStatus_SendFailed;
		}

	}

	/*!
	 * @brief Receive data from Serial peripheral.
	 *
	 * @param[inout] data Preallocated buffer for receiving data.
	 * @param[in] size Size of data to read.
	 *
	 * @retval kErpcStatus_ReceiveFailed Serial failed to receive data.
	 * @retval kErpcStatus_Success Successfully received all data.
	 */
	virtual erpc_status_t underlyingReceive(uint8_t *data, uint32_t size) {

		ssize_t bytes_rcvd = 0, result = 0;
		while((unsigned int)bytes_rcvd < size) {
			result = this->read(&data[bytes_rcvd], size);

			// Negative error on failure, break out
			if(result < 0) {
				break;
			}

			// Accumulate the number of bytes sent
			bytes_rcvd += result;

		}

		if((unsigned int)bytes_rcvd == size) {
			return kErpcStatus_Success;
		} else {
			return kErpcStatus_ReceiveFailed;
		}

	}
};

}

#endif /* EPL_CORE_LIBRARIES_MBED_RPC_TRANSPORTS_USBTRANSPORT_H_ */
