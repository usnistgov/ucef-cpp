//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef BASICUDPAPPWRAPPER_H
#define BASICUDPAPPWRAPPER_H

#include <omnetpp.h>
#include "BasicUdpApp.h"
#include "HLAInterface.h"
#include "INotifiable.h"
#include "NetworkPacket.hpp"


/**
 * TODO - Generated class
 */
class BasicUdpAppWrapper : public BasicUdpApp {
public:
	typedef BasicUdpApp Super;

private:
	class MessageTracker {
	public:
		typedef std::set< int > IntSet;

	private:
		IntSet _intSet;
		unsigned int _maxNums;

	public:
		MessageTracker( void ) : _maxNums( 1024 ) { }
		MessageTracker( int maxNums ) : _maxNums( maxNums ) { }

		bool addInt( int num ) {
			if ( _intSet.find( num ) != _intSet.end() ) {
				return false;
			}
			_intSet.insert( num );
			while ( _intSet.size() > _maxNums ) {
				_intSet.erase( _intSet.begin() );
			}
			return true;
		}
	};

	const std::string _appName;
	MessageTracker _messageTracker;

public:
	BasicUdpAppWrapper( void ) : _appName( "BasicUdpAppWrapper" ) { }

	const std::string &getAppName( void ) {
		return _appName;
	}

protected:
	void recordInterfaceIPAddresses( void );
	virtual int numInitStages( void ) const;
	virtual void initialize( int stage );
	virtual void receiveChangeNotification( int category, const cPolymorphic *details );
	virtual void handleMessage(cMessage *msg);
	virtual void sendToUDP( cPacket *msg, const IPv4Address& destAddr, int destPort );
	virtual NetworkPacketSP modifyIncoming( NetworkPacketSP networkPacketSP );
	virtual NetworkPacketSP modifyOutgoing( NetworkPacketSP networkPacketSP );
	virtual NetworkPacketSP tweakIncoming( NetworkPacketSP networkPacketSP, int intMultuplier = 1, int intAdder = 0, long longMultiplier = 1, long longAdder = 0, double doubleMultiplier = 1.0, double doubleAdder = 0.0, bool booleanEnableFlip = false, const std::string &stringReplacement = "");
};

#endif
