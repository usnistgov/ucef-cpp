/*
 * THIS IS GENERATED CODE -- DO NOT MODIFY
 * HLAInterface.h
 *
 */

#ifndef HLAINTERFACE_H
#define HLAINTERFACE_H

#include <string>
#include <set>
#include <boost/lexical_cast.hpp>

#include <omnetpp.h>
#include <IPv4Address.h>
#include <IPv4Route.h>

#include <SynchronizedFederate.hpp>

#include <messages/InteractionMsg_m.h>
#include <messages/ObjectMsg_m.h>

class HLAInterface : public cSimpleModule, public SynchronizedFederate {

public:
	typedef SynchronizedFederate Super;
	enum MessageType { INTERACTION, OBJECT };
	
	//
    // NetworkAddress CLASS -- FOR MANAGING NETWORK ADDRESSES
    // THIS CLASS IS USED FOR:
    //   NETWORK ATTACKS
    //   REPLAY ATTACKS
    //   OUT-OF-ORDER-PACKET ATTACKS
    //
	class NetworkAddress {
	private:
		unsigned int _networkIPAddress;
		unsigned int _netMask;

		void truncateIPAddress( void ) {
			_networkIPAddress &= _netMask;
		}

	public:
		static int countBits( unsigned int number ) {
			int count = 0;
			while( number ) {
				if ( number & 1U ) ++count;
				number >>= 1;
			}
			return count;
		}

		NetworkAddress( void ) : _networkIPAddress( 0 ), _netMask( 2 ) { }
		NetworkAddress( unsigned int networkIPAddress, unsigned int netMask ) : _networkIPAddress( networkIPAddress ), _netMask( netMask ) {
			truncateIPAddress();
		}

	private:
		void initFromString( std::string networkAddressSpec );

	public:
		NetworkAddress( const char *networkAddressSpec ) : _networkIPAddress( 0 ), _netMask( 2 ) {
			initFromString(  std::string( networkAddressSpec )  );
		}
		NetworkAddress( const std::string &networkAddressSpec ) : _networkIPAddress( 0 ), _netMask( 2 ) {
			initFromString( networkAddressSpec );
		}

		NetworkAddress( const IPv4Address &ipAddress ) : _networkIPAddress( ipAddress.getInt() ), _netMask( ipAddress.getNetworkMask().getInt() ) {
			truncateIPAddress();
		}
		NetworkAddress( const IPv4Address *ipAddress ) : _networkIPAddress( ipAddress->getInt() ), _netMask( ipAddress->getNetworkMask().getInt() ) {
			truncateIPAddress();
		}
		NetworkAddress( const IPv4Address &hostIPAddress, const IPv4Address &netmaskIPAddress) :
		 _networkIPAddress( hostIPAddress.getInt() ), _netMask( netmaskIPAddress.getInt() ) {
			truncateIPAddress();
		}
		NetworkAddress( const IPv4Address *hostIPAddress, const IPv4Address *netmaskIPAddress) :
		 _networkIPAddress( hostIPAddress->getInt() ), _netMask( netmaskIPAddress->getInt() ) {
			truncateIPAddress();
		}

		NetworkAddress( const IPv4Route *ipRoute ) : _networkIPAddress( ipRoute->getDestination().getInt() ), _netMask( ipRoute->getNetmask().getInt() ) {
			truncateIPAddress();
		}

		void setNetworkIPAddress( unsigned int networkIPAddress ) { _networkIPAddress = networkIPAddress; }
		void setNetMask( unsigned int netMask ) { _netMask = netMask; }

		unsigned int getNetworkIPAddress( void ) const { return _networkIPAddress; }
		unsigned int getNetMask( void ) const { return _netMask; }

		bool isValid( void ) const {
			unsigned long bit = ~getNetMask() + 1;
			return countBits( bit ) == 1;
		}

		bool matches( unsigned int ipAddress ) const {
			ipAddress &= _netMask;
			return ipAddress == _networkIPAddress;
		}

		bool matches( const IPv4Address &ipAddress ) const {
			return matches( ipAddress.getInt() );
		}

	};

	friend std::ostream &::operator<<( std::ostream &os, const NetworkAddress &networkAddress );

	struct NetworkAddressComparator {
		bool operator()( const NetworkAddress &networkAddress1, const NetworkAddress &networkAddress2 );
	};

	typedef std::set< NetworkAddress, NetworkAddressComparator > NetworkAddressSet;

	typedef std::string HostName;

	typedef std::set< cModule * > CModuleSet;
	typedef std::map< NetworkAddress, CModuleSet, NetworkAddressComparator > NetworkAddressCModuleSetMap;

	typedef std::map< std::string, IPv4Address > InterfaceIPAddressMap;


	class IPModuleProperties {
	public:

	private:
		cModule *_cModule;
		InterfaceIPAddressMap _interfaceIPAddressMap;

	public:
		IPModuleProperties( void ) : _cModule( 0 ) { }
		IPModuleProperties( cModule *cModule_var ) : _cModule( cModule_var ) { }

		cModule *getCModule( void ) {
			return _cModule;
		}
		IPv4Address getIPAddress( const std::string &interfaceName ) {
			InterfaceIPAddressMap::iterator iimItr = _interfaceIPAddressMap.find( interfaceName );
			return iimItr == _interfaceIPAddressMap.end() ? IPv4Address(  static_cast< uint32_t >( 0 )  ) : iimItr->second;
		}
		InterfaceIPAddressMap &getInterfaceIPAddressMap( void ) {
			return _interfaceIPAddressMap;
		}

		void setCModule( cModule *cModule_var ) {
			_cModule = cModule_var;
		}
		void setInterfaceIPAddress( const std::string &interfaceName, const IPv4Address &ipAddress ) {
			_interfaceIPAddressMap[ interfaceName ] = ipAddress;
		}
	};

	typedef std::map< HostName, IPModuleProperties > HostNameIPModulePropertiesMap;

	//
	// HLAInterface CAN ACCESS IP MODULES OF HOSTS USING THE FOLLOWING CODE */
	//

private:
	HostNameIPModulePropertiesMap _hostNameIPModulePropertiesMap;
	NetworkAddressCModuleSetMap _networkAddressCModuleSetMap;
	NetworkAddressSet _attackedNetworkAddressSet;

public:
	void registerIPModule( const HostName &hostName, cModule *module ) {
		_hostNameIPModulePropertiesMap.insert(   std::make_pair(  hostName, IPModuleProperties( module )  )   );
	}

	cModule *getIPModule( const HostName &hostName ) {
		HostNameIPModulePropertiesMap::iterator himItr = _hostNameIPModulePropertiesMap.find( hostName );
		return himItr == _hostNameIPModulePropertiesMap.end() ? (cModule *)0 : himItr->second.getCModule();
	}

	IPv4Address getIPAddress( const HostName &hostName, const std::string &interfaceName ) {
		HostNameIPModulePropertiesMap::iterator himItr = _hostNameIPModulePropertiesMap.find( hostName );
		return himItr->second.getIPAddress( interfaceName );
	}

	InterfaceIPAddressMap &getIPModuleInterfaceIPAddressMap( const HostName &hostName ) {
		return _hostNameIPModulePropertiesMap[ hostName ].getInterfaceIPAddressMap();
	}

	std::string listIPModuleProperties( void );

	void registerIPModuleNetworks( cModule *module, const NetworkAddressSet &networkAddressSet ) {
		for( NetworkAddressSet::const_iterator nasCit = networkAddressSet.begin() ; nasCit != networkAddressSet.end() ; ++nasCit ) {
			_networkAddressCModuleSetMap[ *nasCit ].insert( module );
		}
	}

	void unregisterIPModuleNetworks( cModule *module, const NetworkAddressSet &networkAddressSet ) {
		for( NetworkAddressSet::const_iterator nasCit = networkAddressSet.begin() ; nasCit != networkAddressSet.end() ; ++nasCit ) {
			_networkAddressCModuleSetMap[ *nasCit ].erase( module );
		}
	}

	std::string listIPModuleNetworks( void );

	class AppSpec {
	private:
		std::string _hostName;
		std::string _appName;
		int _appIndex;

	public:
		AppSpec( void ) { }
		AppSpec( const std::string &hostName, const std::string &appName, int appIndex ) :
		 _hostName( hostName ), _appName( appName ), _appIndex( appIndex ) { }

		const std::string &getHostName( void ) const {
			return _hostName;
		}
		const std::string &getAppName( void ) const {
			return _appName;
		}
		int getAppIndex( void ) const {
			return _appIndex;
		}

		void setHostName( const std::string &hostName ) {
			_hostName = hostName;
		}
		void setAppName( const std::string &appName ) {
			_appName = appName;
		}
		void setAppIndex( int appIndex ) {
			_appIndex = appIndex;
		}

		std::string getSpec( void ) const {
			return _hostName + "." + _appName + "[" + boost::lexical_cast< std::string >( _appIndex ) + "]";
		}
	};

	struct AppSpecComparator {
		bool operator()( const AppSpec &appSpec1, const AppSpec &appSpec2 ) {
			return appSpec1.getSpec() < appSpec2.getSpec();
		}
	};

	class AppProperties {
	private:
		cModule *_cModule;
		InterfaceIPAddressMap _interfaceIPAddressMap;
		int _port;

	public:
		AppProperties( void ) : _cModule( 0 ), _port( -1 ) { }
		AppProperties( cModule *cModule_var, int port ) : _cModule( cModule_var ), _port( port ) { }

		cModule *getCModule( void ) {
			return _cModule;
		}
		IPv4Address getIPAddress( const std::string &interfaceName ) {
			InterfaceIPAddressMap::iterator iimItr = _interfaceIPAddressMap.find( interfaceName );
			return iimItr == _interfaceIPAddressMap.end() ? IPv4Address(  static_cast< uint32_t >( 0 )  ) : iimItr->second;
		}
		InterfaceIPAddressMap &getInterfaceIPAddressMap( void ) {
			return _interfaceIPAddressMap;
		}
		int getPort( void ) {
			return _port;
		}

		void setCModule( cModule *cModule_var ) {
			_cModule = cModule_var;
		}
		void setInterfaceIPAddress( const std::string &interfaceName, const IPv4Address &ipAddress ) {
			_interfaceIPAddressMap[ interfaceName ] = ipAddress;
		}
		void setPort( int port ) {
			_port = port;
		}
	};

	class IntegrityAttackParams {
	private:
	    int _intMultiplier;
	    int _intAdder;
        long _longMultiplier;
        long _longAdder;
        double _doubleMultiplier;
        double _doubleAdder;
        bool _booleanEnableFlip;
        std::string _stringReplacement;

	public:
        IntegrityAttackParams( void ) : _intMultiplier(1), _intAdder(0), _longMultiplier(1), _longAdder(0), _doubleMultiplier(1.0), _doubleAdder(0.0), _booleanEnableFlip(false), _stringReplacement("") { }
        IntegrityAttackParams( int intMultiplier, int intAdder, long longMultiplier, long longAdder, double doubleMultiplier, double doubleAdder, bool booleanEnableFlip, const std::string &stringReplacement ) : _intMultiplier(intMultiplier), _intAdder(intAdder), _longMultiplier(longMultiplier), _longAdder(longAdder), _doubleMultiplier(doubleMultiplier), _doubleAdder(doubleAdder), _booleanEnableFlip(booleanEnableFlip), _stringReplacement(stringReplacement) { }

        int getIntMultiplier( void ) {
            return _intMultiplier;
        }
        int getIntAdder( void ) {
            return _intAdder;
        }
        long getLongMultiplier( void ) {
            return _longMultiplier;
        }
        long getLongAdder( void ) {
            return _longAdder;
        }
        double getDoubleMultiplier( void ) {
            return _doubleMultiplier;
        }
        double getDoubleAdder( void ) {
            return _doubleAdder;
        }
        bool getBooleanEnableFlip( void ) {
            return _booleanEnableFlip;
        }
        const std::string &getStringReplacement( void ) const {
            return _stringReplacement;
        }
	};

	typedef std::map< AppSpec, AppProperties, AppSpecComparator > AppSpecPropertiesMap;
	typedef std::map< HostName, IntegrityAttackParams > IntegrityAttackParamsMap;

private:
	AppSpecPropertiesMap _appSpecPropertiesMap;
	IntegrityAttackParamsMap _integrityAttackParamsMap;

public:
	void registerAppSpecProperties( const HostName &hostName, const std::string &appName, int appIndex, cModule *cModule_var, int port ) {
		_appSpecPropertiesMap.insert(   std::make_pair(  AppSpec( hostName, appName, appIndex ), AppProperties( cModule_var, port )  )   );
	}

	cModule *getAppSpecModule( const AppSpec &appSpec  ) {
		AppSpecPropertiesMap::iterator apmItr = _appSpecPropertiesMap.find( appSpec );
		return apmItr == _appSpecPropertiesMap.end() ? (cModule *)0 : apmItr->second.getCModule();
	}
	cModule *getAppSpecModule( const HostName &hostName, const std::string &appName, int appIndex ) {
		return getAppSpecModule(  AppSpec( hostName, appName, appIndex )  );
	}

	InterfaceIPAddressMap &getAppInterfaceIPAddressMap( const AppSpec &appSpec ) {
		return _appSpecPropertiesMap[ appSpec ].getInterfaceIPAddressMap();
	}
	InterfaceIPAddressMap &getAppInterfaceIPAddressMap( const HostName &hostName, const std::string &appName, int appIndex ) {
		return getAppInterfaceIPAddressMap(  AppSpec( hostName, appName, appIndex )  );
	}

	IPv4Address getAppSpecIPAddress( const AppSpec &appSpec, const std::string &interfaceName ) {
		AppSpecPropertiesMap::iterator apmItr = _appSpecPropertiesMap.find( appSpec );
		return apmItr == _appSpecPropertiesMap.end() ? IPv4Address(  static_cast< uint32_t >( 0 )  ) : apmItr->second.getIPAddress( interfaceName );
	}
	IPv4Address getAppSpecIPAddress( const HostName &hostName, const std::string &appName, int appIndex, const std::string &interfaceName ) {
		return getAppSpecIPAddress(  AppSpec( hostName, appName, appIndex ), interfaceName  );
	}

	void setAppSpecIPAddress( const AppSpec &appSpec, const std::string &interfaceName, const IPv4Address &ipAddress ) {
		_appSpecPropertiesMap[ appSpec ].setInterfaceIPAddress( interfaceName, ipAddress );
	}
	void setAppSpecIPAddress( const HostName &hostName, const std::string &appName, int appIndex, const std::string &interfaceName, const IPv4Address &ipAddress ) {
		setAppSpecIPAddress(  AppSpec( hostName, appName, appIndex ), interfaceName, ipAddress  );
	}

	int getAppSpecPort( const AppSpec &appSpec ) {
		AppSpecPropertiesMap::iterator apmItr = _appSpecPropertiesMap.find( appSpec );
		return apmItr == _appSpecPropertiesMap.end() ? -1 : apmItr->second.getPort();
	}
	int getAppSpecPort( const HostName &hostName, const std::string &appName, int appIndex ) {
		return getAppSpecPort(  AppSpec( hostName, appName, appIndex )  );
	}

	AppProperties &getAppSpecProperties( const AppSpec &appSpec ) {
		return _appSpecPropertiesMap[ appSpec ];
	}
	AppProperties &getAppSpecProperties( const HostName &hostName, const std::string &appName, int appIndex  ) {
		return getAppSpecProperties(  AppSpec( hostName, appName, appIndex )  );
	}

    void enableIntegrityAttack( const HostName &hostName, int intMultiplier, int intAdder, long longMultiplier, long longAdder, double doubleMultiplier, double doubleAdder, bool booleanEnableFlip, const std::string &stringReplacement ) {
        _integrityAttackParamsMap.insert(   std::make_pair(  hostName, IntegrityAttackParams( intMultiplier, intAdder, longMultiplier, longAdder, doubleMultiplier, doubleAdder, booleanEnableFlip, stringReplacement )  )   );
    }
    void disableIntegrityAttack( const HostName &hostName ) {
        _integrityAttackParamsMap.erase( hostName );
    }
    bool isIntegrityAttackEnabled( const HostName &hostName ) {
        return ( _integrityAttackParamsMap.find(hostName) != _integrityAttackParamsMap.end() );
    }
    IntegrityAttackParams &getIntegrityAttackParams( const HostName &hostName ) {
        if( !isIntegrityAttackEnabled( hostName ) ) {
            throw cRuntimeError(this, "HLAInterface::getIntegrityAttackParams()::ERROR: Integrity attack not set for '%s'", hostName.c_str());
        }

        return _integrityAttackParamsMap[hostName];
    }


	std::string listAppSpecProperties( void );


	class RouteEntry {

	private:
		std::string _nodeName;
		std::string _interfaceEntry;
		NetworkAddress _networkAddress;
		NetworkAddress _gatewayAddress;
		RouteEntry( void ) { };

	public:
		RouteEntry( const std::string& nodeName,
				    const std::string& interfaceEntry,
				    const NetworkAddress& networkAddressSpec,
				    const NetworkAddress& gatewayAddressSpec
				  ) : _nodeName( nodeName), _interfaceEntry( interfaceEntry ),
				      _networkAddress(  NetworkAddress( networkAddressSpec )  ),
				      _gatewayAddress(  NetworkAddress( gatewayAddressSpec )  ) { }

		std::string get_nodeName( void ) const { return _nodeName; }
		std::string get_interfaceEntry( void ) const { return _interfaceEntry; }
		NetworkAddress get_networkAddress( void ) const { return _networkAddress; }
		NetworkAddress get_gatewayAddress( void ) const { return _gatewayAddress; }
	};

	typedef boost::shared_ptr< RouteEntry > RouteEntrySP;

	RouteEntrySP createRouteEntrySP(
	 const std::string &nodeName, const std::string &interfaceEntry, const NetworkAddress &networkAddress, const NetworkAddress &gatewayAddress
	) {
		return RouteEntrySP(  new RouteEntry( nodeName, interfaceEntry, networkAddress, gatewayAddress )  );
	}

	typedef std::set< std::string > ModifyToHLAPacketsSet;
    typedef std::set< std::string > ModifyFromHLAPacketsSet;

private:
    ModifyToHLAPacketsSet _modifyToHLAPacketsSet;
    ModifyFromHLAPacketsSet _modifyFromHLAPacketsSet;

public:
    void setModifyToHLAPackets( const std::string nodeFullPath, bool modify ) {
        if ( modify ) {
            _modifyToHLAPacketsSet.insert( nodeFullPath );
        } else {
            _modifyToHLAPacketsSet.erase( nodeFullPath );
        }
    }

    void setModifyFromHLAPackets( const std::string nodeFullPath, bool modify ) {
        if ( modify ) {
            _modifyFromHLAPacketsSet.insert( nodeFullPath );
        } else {
            _modifyFromHLAPacketsSet.erase( nodeFullPath );
        }
    }

    bool modifyToHLAPackets( const std::string nodeFullPath ) {
        return _modifyToHLAPacketsSet.find( nodeFullPath ) != _modifyToHLAPacketsSet.end();
    }

    bool modifyFromHLAPackets( const std::string nodeFullPath ) {
        return _modifyFromHLAPacketsSet.find( nodeFullPath ) != _modifyFromHLAPacketsSet.end();
    }

private:
	std::string _federation_name;
	std::string _name;

	double _time;
	double _lookahead;
	double _step;
	
	std::string _logLevel;

	cMessage *_keepAliveMsg;
	cMessage *_readyToRunMsg;
	cMessage *_interactionArrivalMsg;
	bool _noInteractionArrivalFlag;

	static HLAInterface *&getHLAInterfacePtr( void ) {
		static HLAInterface *hlaInterfacePtr = 0;
		return hlaInterfacePtr;
	}
	
	static void setInstancePtr( HLAInterface *hlaInterfacePtr ) {
		if ( getHLAInterfacePtr() != 0 ) {
			std::cerr << "WARNING!  MULTIPLE INSTANCES OF THE HLAInterface CLASS DETECTED.  ALL BUT THE FIRST ARE IGNORED." << std::endl;
			return;
		}
		getHLAInterfacePtr() = hlaInterfacePtr;
	}

	
public:
	static HLAInterface *get_InstancePtr( void ) { return getHLAInterfacePtr(); }
	
	double getStepSize() { return _step; }
	double getLookahead() { return _lookahead; }

	HLAInterface( void );
	virtual ~HLAInterface( void ) throw();

	static int getUniqueNo( void ) {
		static int uniqueNo = -1;
		return ++uniqueNo;
	}

	//
	// SYNCHRONIZED-FEDERATE METHODS
	//
	  
    void processInteractions( void );
    
    void interactionArrival( void );

	virtual void receiveInteraction(
	 RTI::InteractionClassHandle theInteraction,
	 const RTI::ParameterHandleValuePairSet& theParameters,
	 const RTI::FedTime& theTime,
     const char *theTag,
	 RTI::EventRetractionHandle theHandle
	)
	 throw ( RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError );

	virtual void receiveInteraction(
	 RTI::InteractionClassHandle theInteraction,
	 const RTI::ParameterHandleValuePairSet& theParameters,
     const char *theTag
	)
	 throw ( RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::FederateInternalError );


    void processObjectReflectors( void );
    
    void reflectAttributeValues(
     RTI::ObjectHandle theObject, 
     const RTI::AttributeHandleValuePairSet& theAttributes,
     const RTI::FedTime& theTime,
     const char *theTag,
     RTI::EventRetractionHandle theHandle
    ) throw ( RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::InvalidFederationTime, RTI::FederateInternalError ) {
        SynchronizedFederate::reflectAttributeValues( theObject, theAttributes, theTime, theTag, theHandle );
        processObjectReflectors();
    }

    void reflectAttributeValues ( RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttributes, const char *theTag )
     throw ( RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::FederateInternalError ) {
        SynchronizedFederate::reflectAttributeValues( theObject, theAttributes, theTag );
        processObjectReflectors();
    }


    //
    // OMNET CMODULE METHODS
    //
    
	virtual int numInitStages( void ) const;
	virtual void initialize( int stage );
	virtual void handleMessage( cMessage *msg );

	ObjectMsg *wrapObject( ObjectRootSP objectRootSP, const std::string &name, simtime_t timestamp ) {
	
		Enter_Method( "wrapObject" );

		ObjectMsg *objectMsgPtr = new ObjectMsg( name.c_str() );
		objectMsgPtr->setObjectRootSP( objectRootSP );
		objectMsgPtr->setKind( OBJECT );
		objectMsgPtr->setTimestamp( timestamp );
		
		return objectMsgPtr;
	}
	
	ObjectMsg *wrapObject( ObjectRootSP objectRootSP, const std::string &name ) {
		return wrapObject( objectRootSP, name, simTime() );
	}
	
	ObjectMsg *wrapObject( ObjectRoot &objectRoot, const std::string &name, simtime_t timestamp ) {
		return wrapObject( objectRoot.clone(), name, timestamp );
	}

	ObjectMsg *wrapObject( ObjectRoot &objectRoot, const std::string &name ) {
		return wrapObject( objectRoot.clone(), name );
	}

	void updateObject( ObjectMsg *objectMsgPtr ) {
		Enter_Method( "updateObject" );
		take( objectMsgPtr );
		scheduleAt( simTime(), objectMsgPtr );
	}
	
	void updateObject( ObjectRootSP objectRootSP, const std::string &name, simtime_t timestamp ) {		
		ObjectMsg *objectMsgPtr = wrapObject( objectRootSP, name, timestamp );
		updateObject( objectMsgPtr );
	}
	
	void updateObject( ObjectRootSP objectRootSP, const std::string &name, double timestamp ) {
		updateObject(  objectRootSP, name, SimTime( timestamp )  );
	}

	void updateObject( ObjectRootSP objectRootSP, const std::string &name ) {
		updateObject( objectRootSP, name, simTime() );
	}

	void updateObject( ObjectRoot &objectRoot, const std::string &name, simtime_t timestamp ) {
		updateObject( objectRoot.clone(), name, timestamp );
	}
	
	void updateObject( ObjectRoot &objectRoot, const std::string &name, double timestamp ) {
		updateObject(  objectRoot, name, SimTime( timestamp )  );
	}
	
	void updateObject( ObjectRoot &objectRoot, const std::string &name ) {
		updateObject( objectRoot, name, simTime() );
	}

	static ObjectRootSP unwrapObject( cMessage *msg ) {
		if ( msg->getKind() != OBJECT ) return ObjectRootSP(  static_cast< ObjectRoot * >( 0 )  );
		return static_cast< ObjectMsg * >( msg )->getObjectRootSP();
	}	

private:
    void setup( void );
	void sendKeepAliveMsg( void );

};

std::ostream &operator<<( std::ostream &os, const HLAInterface::AppSpec &appSpec );

std::ostream &operator<<( std::ostream &os, const HLAInterface::NetworkAddress &networkAddress );

std::ostream &operator<<( std::ostream &os, const HLAInterface::RouteEntry &routeEntry );
std::ostream &operator<<( std::ostream &os, const HLAInterface::RouteEntrySP &routeEntrySP );

#endif /* HLAINTERFACE_H */
