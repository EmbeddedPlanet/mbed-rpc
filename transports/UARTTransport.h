/*
 * UARTTransport.h
 *
 *  Created on: May 10, 2019
 *      Author: becksteing
 */

/**
 * TODO - Add timeout? Change to use Mbed's asynchronous serial functionality?
 * May allow other threads to run while this one is busy waiting for data to be sent/received
 */

#ifndef EPL_CORE_LIBRARIES_MBED_RPC_TRANSPORTS_UARTTRANSPORT_H_
#define EPL_CORE_LIBRARIES_MBED_RPC_TRANSPORTS_UARTTRANSPORT_H_

#include "erpc_framed_transport.h"

#include "drivers/UARTSerial.h"

namespace ep {

class UARTTransport: public erpc::FramedTransport, public mbed::UARTSerial {
public:

	/** Create a UARTTransport, connected to the specified transmit and receive pins,
	 * with the specified baud.
	 *
	 *  @param tx Transmit pin
	 *  @param rx Receive pin
	 *  @param baud The baud rate of the serial port (optional, defaults to MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
	 *
	 *  @note
	 *    Either tx or rx may be specified as NC if unused
	 */
	UARTTransport(PinName tx, PinName rx, int baud =
			MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) :
			erpc::FramedTransport(), mbed::UARTSerial(tx, rx, baud) {
		this->set_blocking(true); // We want blocking operation in this case
		this->set_format(8, mbed::SerialBase::None, 1);
	}

	/*!
	 * @brief Destructor.
	 */
	virtual ~UARTTransport(void) {
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

#endif /* EPL_CORE_LIBRARIES_MBED_RPC_TRANSPORTS_UARTTRANSPORT_H_ */
