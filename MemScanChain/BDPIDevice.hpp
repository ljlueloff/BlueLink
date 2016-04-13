/*
 * BDPIDevice.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: jcassidy
 */

#ifndef MEMSCANCHAIN_BDPIDEVICE_HPP_
#define MEMSCANCHAIN_BDPIDEVICE_HPP_

#include <vector>
#include <cinttypes>

class BDPIPort;

/** BDPIDevice is a support library for C++ models that interact with Bluespec via BDPI. A device is a collection of ports
 * with an associated model of state and timing. Timing is maintained through calling the tick($time) method before any
 * of the port reads or writes.
 *
 * Device state can be updated through the preTick() and postTick() clock hooks.
 * preTick() is called when the device is instantiated, and when tick($time) is called before the new time is written.
 * postTick() is called at the end of the tick($time) method after the timebase is updated.
 *
 */

class BDPIDevice
{
public:
	BDPIDevice();
	virtual ~BDPIDevice();


	////// Functions for BDPI use
	void tick(uint64_t timebase);
	void close();

	BDPIPort*	getPort(uint8_t p) 		const;



	////// Functions for internal/child use
	uint8_t		addPort(BDPIPort* p);
	uint64_t 	timebase() const;

private:

	/// cycleStart happens right after the clock tick (just after timebase is updated, before all method calls)
	/// It should ensure that all port status bits are ready to read
	virtual void cycleStart()	{}

	/// cycleFinish happens just before the next clock tick (after all method calls)
	/// It should take care of any cleanup or committing of results
	virtual void cycleFinish()	{}

	virtual void preClose()		{}		///< Called before invoking close on all ports
	virtual void postClose()	{}		///< Called after invoking close on all ports

	uint64_t				m_timebase=0;
	std::vector<BDPIPort*>	m_ports;
};



extern "C"
{
	void		bdpi_deviceTick(uint64_t devicePtr,uint64_t timebase);
	void		bdpi_deviceClose(uint64_t devicePtr);
	uint64_t	bdpi_deviceGetPort(uint64_t devicePtr,uint8_t portNum);
}



#endif /* MEMSCANCHAIN_BDPIDEVICE_HPP_ */