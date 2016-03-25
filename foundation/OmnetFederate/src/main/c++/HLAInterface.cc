/*
 * HLAInterface.cc
 *
 */

#include <HLAInterface.h>
#include "C2WLogger.hpp"
//#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "OmnetFOMInteractions.h"
#include "RTIRootHeaders.hpp"
#include "OmnetFOMMessageDefs.h"

#include "SubscribedInteractionFilter.hpp"

#include "C2WIPv4.h"


Define_Module(HLAInterface);


void HLAInterface::NetworkAddress::initFromString( std::string networkAddressSpec ) {

	// PATTERN FOR MATCHING A NETWORK ADDRESS
	static boost::regex networkIdPattern( "(\\d{1,3})\\.(\\d{1,3}).(\\d{1,3}).(\\d{1,3})/(\\d{1,2})" );
	static boost::regex whitespacePattern( "\\s" );
	//
	networkAddressSpec = boost::regex_replace( networkAddressSpec, whitespacePattern, "" );
	const std::string &constNetworkAddressSpec = networkAddressSpec;

	boost::match_results< std::string::const_iterator > results;
	if (  !regex_match( constNetworkAddressSpec.begin(), constNetworkAddressSpec.end(), results, networkIdPattern )  ) {
		return;
	}

	_networkIPAddress = 0;
	for( int ix = 1 ; ix <= 4 ; ++ix ) {
		_networkIPAddress <<= 8;
		int byte = boost::lexical_cast< unsigned int >(  std::string( results[ ix ].first, results[ ix ].second )  );
		_networkIPAddress |= byte;
	}

	int bits = boost::lexical_cast< unsigned int >(  std::string( results[ 5 ].first, results[ 5 ].second )  );
	_netMask = bits == 0 ? 0 : ~(  ( 1UL << (32 - bits) ) - 1  );

	truncateIPAddress();

}

bool HLAInterface::NetworkAddressComparator::operator()( const NetworkAddress &networkAddress1, const NetworkAddress &networkAddress2 ) {
	if ( networkAddress1.getNetworkIPAddress() < networkAddress2.getNetworkIPAddress() ) {
		return true;
	}
	if ( networkAddress1.getNetworkIPAddress() > networkAddress2.getNetworkIPAddress() ) {
		return false;
	}

	if ( networkAddress1.getNetMask() < networkAddress2.getNetMask() ) {
		return true;
	}
	return false;
}

std::string HLAInterface::listIPModuleProperties( void ) {
	std::string retval;
	for(
	 HostNameIPModulePropertiesMap::iterator himItr = _hostNameIPModulePropertiesMap.begin() ; himItr != _hostNameIPModulePropertiesMap.end() ; ++himItr
	) {
		retval += himItr->first + ":\n";
		IPModuleProperties &ipModuleProperties = himItr->second;
		retval += "\tModule address: " + boost::lexical_cast< std::string >( ipModuleProperties.getCModule() ) + "\n";
		retval += "\tIPAddresses:\n";
		InterfaceIPAddressMap interfaceIPAddressMap = ipModuleProperties.getInterfaceIPAddressMap();
		for( InterfaceIPAddressMap::iterator iimItr = interfaceIPAddressMap.begin() ; iimItr != interfaceIPAddressMap.end() ; ++iimItr ) {
			retval += "\t\t" + iimItr->first + ":\t" + iimItr->second.str() + "\n";
		}
	}
	return retval;
}

std::string HLAInterface::listIPModuleNetworks( void ) {
	std::string retval;

	for( NetworkAddressCModuleSetMap::iterator ncmItr = _networkAddressCModuleSetMap.begin() ; ncmItr != _networkAddressCModuleSetMap.end() ; ++ncmItr ) {
		retval += "Hosts in network " + boost::lexical_cast< std::string >( ncmItr->first ) + ":\n";
		CModuleSet &cModuleSet = ncmItr->second;
		for( CModuleSet::const_iterator cmsCit = cModuleSet.begin() ; cmsCit != cModuleSet.end() ; ++cmsCit ) {
			C2WIPv4 *c2wipv4 = static_cast< C2WIPv4 * >( *cmsCit );
			retval += "\t" + c2wipv4->getHostFullName() + "\n";
		}
		retval += "\n";
	}

	return retval;
}

std::string HLAInterface::listAppSpecProperties( void ) {
	std::string retval;
	for( AppSpecPropertiesMap::iterator apmItr = _appSpecPropertiesMap.begin() ; apmItr != _appSpecPropertiesMap.end() ; ++apmItr ) {
		retval += apmItr->first.getSpec() + ":\n";
		retval += "\tIPAddresses:\n";
		AppProperties &appProperties = apmItr->second;
		InterfaceIPAddressMap interfaceIPAddressMap = appProperties.getInterfaceIPAddressMap();
		for( InterfaceIPAddressMap::iterator iimItr = interfaceIPAddressMap.begin() ; iimItr != interfaceIPAddressMap.end() ; ++iimItr ) {
			retval += "\t\t" + iimItr->first + ":\t" + iimItr->second.str() + "\n";
		}
		retval += "\tPort: " + boost::lexical_cast< std::string >( appProperties.getPort() ) + "\n\n";
	}
	return retval;
}

void HLAInterface::processInteractions( void ) {

	InteractionRoot::SP interactionRootSP;
	while(  ( interactionRootSP = getNextInteraction() ) != 0  ) {

		// std::cerr << "Processing interaction of type \"" << interactionRootSP->getClassName() << "\"" << std::endl;

		int classHandle = interactionRootSP->getClassHandle();

		if (  NetworkPacket::match( classHandle )  ) {
			NetworkPacketSP networkPacketSP = boost::static_pointer_cast< NetworkPacket >( interactionRootSP );
			InteractionMsg *interactionMsg = new InteractionMsg( "Interaction+Msg" );
			interactionMsg->setToHLA( true );
			interactionMsg->setMessageNo( getUniqueNo() );
			interactionMsg->setInteractionRootSP( networkPacketSP );
	 		interactionMsg->setTimestamp( networkPacketSP->getTime() );

	 		std::string hostName = networkPacketSP->get_senderHost();
	 		std::string appName = networkPacketSP->get_senderHostApp();
	 		int appIndex = networkPacketSP->get_senderAppIndex();

	 		cModule *udpAppWrapperModule = getAppSpecModule( hostName, appName, appIndex );
	 		if ( udpAppWrapperModule != 0 ) {
		 		sendDirect( interactionMsg, udpAppWrapperModule, "hlaIn" );
	 		} else {
		 		std::cerr << "WARNING:  HLAInterface:  could not find module corresponding to (hostname,appName) = (" <<
	 			 hostName << "," << appName << ")" << std::endl;
	 			std::cerr << "Current modules are: " << std::endl;
	 			std::cerr << listAppSpecProperties();
	 			std::cerr << std::endl;
	 		}
	 		continue;
		}


		if (  StartNodeAttack::match( classHandle )  ) {
			StartNodeAttackSP startNodeAttackSP = boost::static_pointer_cast< StartNodeAttack >( interactionRootSP );

			std::string nodeFullPath = startNodeAttackSP->get_nodeFullPath();

			NodeAttackMsg *nodeAttackMsg = new NodeAttackMsg;
			nodeAttackMsg->setAttackInProgress( true );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  nodeAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StartNodeAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}


		if (  StopNodeAttack::match( classHandle )  ) {
			StopNodeAttackSP stopNodeAttackSP = boost::static_pointer_cast< StopNodeAttack >( interactionRootSP );

			std::string nodeFullPath = stopNodeAttackSP->get_nodeFullPath();

			NodeAttackMsg *nodeAttackMsg = new NodeAttackMsg;
			nodeAttackMsg->setAttackInProgress( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  nodeAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StopNodeAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}


		if (  StartDelayNodeAttack::match( classHandle )  ) {
			StartDelayNodeAttackSP startDelayNodeAttackSP = boost::static_pointer_cast< StartDelayNodeAttack >( interactionRootSP );

			std::string nodeFullPath = startDelayNodeAttackSP->get_nodeFullPath();

			DelayNodeAttackMsg *delayNodeAttackMsg = new DelayNodeAttackMsg;
			delayNodeAttackMsg->setAttackInProgress( true );
			delayNodeAttackMsg->setDelayMean( startDelayNodeAttackSP->get_delayMean() );
			delayNodeAttackMsg->setDelayStdDev( startDelayNodeAttackSP->get_delayStdDev() );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  delayNodeAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StartDelayNodeAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}


		if (  StopDelayNodeAttack::match( classHandle )  ) {
			StopDelayNodeAttackSP stopDelayNodeAttackSP = boost::static_pointer_cast< StopDelayNodeAttack >( interactionRootSP );

			std::string nodeFullPath = stopDelayNodeAttackSP->get_nodeFullPath();

			DelayNodeAttackMsg *delayNodeAttackMsg = new DelayNodeAttackMsg;
			delayNodeAttackMsg->setAttackInProgress( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  delayNodeAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StopDelayNodeAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}


		if (  StartSnifferAttack::match( classHandle )  ) {
			StartSnifferAttackSP startSnifferAttackSP = boost::static_pointer_cast< StartSnifferAttack >( interactionRootSP );

			std::string nodeFullPath = startSnifferAttackSP->get_nodeFullPath();

			SnifferAttackMsg *snifferAttackMsg = new SnifferAttackMsg;
			snifferAttackMsg->setListenerNodeFullPath( startSnifferAttackSP->get_listenerNodeFullPath().c_str() );
			snifferAttackMsg->setListenerInterface( startSnifferAttackSP->get_listenerInterface().c_str() );
			snifferAttackMsg->setListen( true );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  snifferAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StartSnifferAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
				std::cout << "Current modules are:" << std::endl;
				std::cout << listIPModuleProperties() << std::endl;
			}
			continue;
		}

		if (  StopSnifferAttack::match( classHandle )  ) {
			StopSnifferAttackSP stopSnifferAttackSP = boost::static_pointer_cast< StopSnifferAttack >( interactionRootSP );

			std::string nodeFullPath = stopSnifferAttackSP->get_nodeFullPath();

			SnifferAttackMsg *snifferAttackMsg = new SnifferAttackMsg;
			snifferAttackMsg->setListenerNodeFullPath( stopSnifferAttackSP->get_listenerNodeFullPath().c_str() );
			snifferAttackMsg->setListenerInterface( stopSnifferAttackSP->get_listenerInterface().c_str() );
			snifferAttackMsg->setListen( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  snifferAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StopSnifferAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
				std::cout << "Current modules are:" << std::endl;
				std::cout << listIPModuleProperties() << std::endl;
			}
			continue;
		}

		if (  StartNetworkFilterAttack::match( classHandle )  ) {
			StartNetworkFilterAttackSP startNetworkFilterAttackSP = boost::static_pointer_cast< StartNetworkFilterAttack >( interactionRootSP );

			std::string nodeFullPath = startNetworkFilterAttackSP->get_nodeFullPath();

			FilterAttackMsg *filterAttackMsg = new FilterAttackMsg;
			filterAttackMsg->setSourceNetworkAddress( startNetworkFilterAttackSP->get_srcNetworkAddress().c_str() );
			filterAttackMsg->setDestinationNetworkAddress( startNetworkFilterAttackSP->get_dstNetworkAddress().c_str() );
			filterAttackMsg->setEnable( true );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  filterAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StartNetworkFilterAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if (  StopNetworkFilterAttack::match( classHandle )  ) {
			StopNetworkFilterAttackSP stopNetworkFilterAttackSP = boost::static_pointer_cast< StopNetworkFilterAttack >( interactionRootSP );

			std::string nodeFullPath = stopNetworkFilterAttackSP->get_nodeFullPath();

			FilterAttackMsg *filterAttackMsg = new FilterAttackMsg;
			filterAttackMsg->setSourceNetworkAddress( stopNetworkFilterAttackSP->get_srcNetworkAddress().c_str() );
			filterAttackMsg->setDestinationNetworkAddress( stopNetworkFilterAttackSP->get_dstNetworkAddress().c_str() );
			filterAttackMsg->setEnable( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  filterAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StopNetworkFilterAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

        if (  StartNetworkIPFirewall::match( classHandle )  ) {
            StartNetworkIPFirewallSP startNetworkIPFirewallSP = boost::static_pointer_cast< StartNetworkIPFirewall >( interactionRootSP );

            std::string nodeFullPath = startNetworkIPFirewallSP->get_nodeFullPath();

            IPFirewallMsg *ipFirewallMsg = new IPFirewallMsg;
            ipFirewallMsg->setSourceNetworkAddress( startNetworkIPFirewallSP->get_srcNetworkAddress().c_str() );
            ipFirewallMsg->setDestinationNetworkAddress( startNetworkIPFirewallSP->get_dstNetworkAddress().c_str() );
            ipFirewallMsg->setEnable( true );

            cModule *cModulePtr = getIPModule( nodeFullPath );
            if ( cModulePtr != 0 ) {
                sendDirect(  ipFirewallMsg, cModulePtr, "hlaIn"  );
            } else {
                std::cout << "WARNING:  StartNetworkIPFirewall:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
            }
            continue;
        }

        if (  StopNetworkIPFirewall::match( classHandle )  ) {
            StopNetworkIPFirewallSP stopNetworkIPFirewallSP = boost::static_pointer_cast< StopNetworkIPFirewall >( interactionRootSP );

            std::string nodeFullPath = stopNetworkIPFirewallSP->get_nodeFullPath();

            IPFirewallMsg *ipFirewallMsg = new IPFirewallMsg;
            ipFirewallMsg->setSourceNetworkAddress( stopNetworkIPFirewallSP->get_srcNetworkAddress().c_str() );
            ipFirewallMsg->setDestinationNetworkAddress( stopNetworkIPFirewallSP->get_dstNetworkAddress().c_str() );
            ipFirewallMsg->setEnable( false );

            cModule *cModulePtr = getIPModule( nodeFullPath );
            if ( cModulePtr != 0 ) {
                sendDirect(  ipFirewallMsg, cModulePtr, "hlaIn"  );
            } else {
                std::cout << "WARNING:  StopNetworkIPFirewall:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
            }
            continue;
        }

		if (  StartDisableNetworkAttack::match( classHandle )  ) {

			StartDisableNetworkAttackSP startDisableNetworkAttackSP = boost::static_pointer_cast< StartDisableNetworkAttack >( interactionRootSP );

			std::string networkAddressStr = startDisableNetworkAttackSP->get_networkAddress();

			NetworkAddress networkAddress( networkAddressStr );
			NetworkAddressCModuleSetMap::iterator ncmItr = _networkAddressCModuleSetMap.find( networkAddress );
			if ( ncmItr != _networkAddressCModuleSetMap.end() ) {
				CModuleSet &cModuleSet = ncmItr->second;
				for( CModuleSet::iterator cmsItr = cModuleSet.begin() ; cmsItr != cModuleSet.end() ; ++cmsItr ) {
					NetworkAttackMsg *networkAttackMsg = new NetworkAttackMsg;
					networkAttackMsg->setNetworkAddress( networkAddressStr.c_str() );
					networkAttackMsg->setEnable( true );

					sendDirect(  networkAttackMsg, *cmsItr, "hlaIn"  );
				}
			}
			continue;
		}

		if (  StopDisableNetworkAttack::match( classHandle )  ) {
			StopDisableNetworkAttackSP stopDisableNetworkAttackSP = boost::static_pointer_cast< StopDisableNetworkAttack >( interactionRootSP );

			std::string networkAddressStr = stopDisableNetworkAttackSP->get_networkAddress();

			NetworkAddress networkAddress( networkAddressStr );
			NetworkAddressCModuleSetMap::iterator ncmItr = _networkAddressCModuleSetMap.find( networkAddress );
			if ( ncmItr != _networkAddressCModuleSetMap.end() ) {
				CModuleSet &cModuleSet = ncmItr->second;
				for( CModuleSet::iterator cmsItr = cModuleSet.begin() ; cmsItr != cModuleSet.end() ; ++cmsItr ) {
					NetworkAttackMsg *networkAttackMsg = new NetworkAttackMsg;
					networkAttackMsg->setNetworkAddress( networkAddressStr.c_str() );
					networkAttackMsg->setEnable( false );

					sendDirect(  networkAttackMsg, *cmsItr, "hlaIn"  );
				}
			}
			continue;
		}

		if (  RecordPacketsForReplayAttack::match( classHandle )  ) {
			RecordPacketsForReplayAttackSP recordPacketsForReplayAttackSP = boost::static_pointer_cast< RecordPacketsForReplayAttack >( interactionRootSP );

			std::string nodeFullPath = recordPacketsForReplayAttackSP->get_recordingNodeFullPath();

			RecordReplayAttackMsg *recordReplayAttackMsg = new RecordReplayAttackMsg;
			recordReplayAttackMsg->setSourceNetworkAddress( recordPacketsForReplayAttackSP->get_srcNetworkAddress().c_str() );
			recordReplayAttackMsg->setDestinationNetworkAddress( recordPacketsForReplayAttackSP->get_dstNetworkAddress().c_str() );
			recordReplayAttackMsg->setRecordDuration( recordPacketsForReplayAttackSP->get_recordDurationInSecs() );
			recordReplayAttackMsg->setEnable( true );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  recordReplayAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  RecordPacketsForReplayAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if (  TerminateReplayAttack::match( classHandle )  ) {
			TerminateReplayAttackSP terminateReplayAttackSP = boost::static_pointer_cast< TerminateReplayAttack >( interactionRootSP );

			std::string nodeFullPath = terminateReplayAttackSP->get_recordingNodeFullPath();

			RecordReplayAttackMsg *recordReplayAttackMsg = new RecordReplayAttackMsg;
			recordReplayAttackMsg->setSourceNetworkAddress( terminateReplayAttackSP->get_srcNetworkAddress().c_str() );
			recordReplayAttackMsg->setDestinationNetworkAddress( terminateReplayAttackSP->get_dstNetworkAddress().c_str() );
			recordReplayAttackMsg->setRecordDuration( -1 );
			recordReplayAttackMsg->setEnable( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  recordReplayAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  TerminateReplayAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if (  StartReplayAttack::match( classHandle )  ) {
			StartReplayAttackSP startReplayAttackSP = boost::static_pointer_cast< StartReplayAttack >( interactionRootSP );

			std::string nodeFullPath = startReplayAttackSP->get_recordingNodeFullPath();

			ReplayAttackMsg *replayAttackMsg = new ReplayAttackMsg;
			replayAttackMsg->setSourceNetworkAddress( startReplayAttackSP->get_srcNetworkAddress().c_str() );
			replayAttackMsg->setDestinationNetworkAddress( startReplayAttackSP->get_dstNetworkAddress().c_str() );
			replayAttackMsg->setPlay( true );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  replayAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StartReplayAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if (  CeaseReplayAttack::match( classHandle )  ) {
			CeaseReplayAttackSP ceaseReplayAttackSP = boost::static_pointer_cast< CeaseReplayAttack >( interactionRootSP );

			std::string nodeFullPath = ceaseReplayAttackSP->get_recordingNodeFullPath();

			ReplayAttackMsg *replayAttackMsg = new ReplayAttackMsg;
			replayAttackMsg->setSourceNetworkAddress( ceaseReplayAttackSP->get_srcNetworkAddress().c_str() );
			replayAttackMsg->setDestinationNetworkAddress( ceaseReplayAttackSP->get_dstNetworkAddress().c_str() );
			replayAttackMsg->setPlay( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  replayAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  CeaseReplayAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if (  StartOutOfOrderPacketsAttack::match( classHandle )  ) {
			StartOutOfOrderPacketsAttackSP startOutOfOrderPacketsAttackSP = boost::static_pointer_cast< StartOutOfOrderPacketsAttack >( interactionRootSP );

			std::string nodeFullPath = startOutOfOrderPacketsAttackSP->get_recordingNodeFullPath();

			OutOfOrderAttackMsg *outOfOrderAttackMsg = new OutOfOrderAttackMsg;
			outOfOrderAttackMsg->setSourceNetworkAddress( startOutOfOrderPacketsAttackSP->get_srcNetworkAddress().c_str() );
			outOfOrderAttackMsg->setDestinationNetworkAddress( startOutOfOrderPacketsAttackSP->get_dstNetworkAddress().c_str() );
			outOfOrderAttackMsg->setRecordDuration( startOutOfOrderPacketsAttackSP->get_recordDurationInSecs() );
			outOfOrderAttackMsg->setPlay( true );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  outOfOrderAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StartOutOfOrderPacketsAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if (  StopOutOfOrderPacketsAttack::match( classHandle )  ) {
			StopOutOfOrderPacketsAttackSP stopOutOfOrderPacketsAttackSP = boost::static_pointer_cast< StopOutOfOrderPacketsAttack >( interactionRootSP );

			std::string nodeFullPath = stopOutOfOrderPacketsAttackSP->get_recordingNodeFullPath();

			OutOfOrderAttackMsg *outOfOrderAttackMsg = new OutOfOrderAttackMsg;
			outOfOrderAttackMsg->setSourceNetworkAddress( stopOutOfOrderPacketsAttackSP->get_srcNetworkAddress().c_str() );
			outOfOrderAttackMsg->setDestinationNetworkAddress( stopOutOfOrderPacketsAttackSP->get_dstNetworkAddress().c_str() );
			outOfOrderAttackMsg->setRecordDuration( -1 );
			outOfOrderAttackMsg->setPlay( false );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect(  outOfOrderAttackMsg, cModulePtr, "hlaIn"  );
			} else {
				std::cout << "WARNING:  StopOutOfOrderPacketsAttack:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if ( AddRouteToRoutingTable::match( classHandle ) ) {
			AddRouteToRoutingTableSP addRouteToRoutingTableSP =
			 boost::static_pointer_cast< AddRouteToRoutingTable >( interactionRootSP );

			std::string nodeFullPath = addRouteToRoutingTableSP->get_nodeFullPath();
			std::string interfaceEntry = addRouteToRoutingTableSP->get_interfaceEntry();
			NetworkAddress networkAddress( addRouteToRoutingTableSP->get_networkAddress() );
			NetworkAddress gatewayAddress( addRouteToRoutingTableSP->get_gatewayAddress() );

			RouteEntrySP routeEntrySP = createRouteEntrySP( nodeFullPath, interfaceEntry, networkAddress, gatewayAddress );

			AddRouteEntryMsg *addRouteEntryMsg = new AddRouteEntryMsg;
			addRouteEntryMsg->setRouteEntrySP( routeEntrySP );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect( addRouteEntryMsg, cModulePtr, "hlaIn" );
			} else {
				std::cout << "WARNING:  AddRouteToRoutingTable:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if ( DropRouteFromRoutingTable::match( classHandle ) ) {
			DropRouteFromRoutingTableSP dropRouteFromRoutingTableSP =
			 boost::static_pointer_cast< DropRouteFromRoutingTable >( interactionRootSP );

			std::string nodeFullPath = dropRouteFromRoutingTableSP->get_nodeFullPath();
			std::string interfaceEntry = dropRouteFromRoutingTableSP->get_interfaceEntry();
			NetworkAddress networkAddress( dropRouteFromRoutingTableSP->get_networkAddress() );
			NetworkAddress gatewayAddress( dropRouteFromRoutingTableSP->get_gatewayAddress() );

			RouteEntrySP routeEntrySP = createRouteEntrySP( nodeFullPath, interfaceEntry, networkAddress, gatewayAddress );

			DropRouteEntryMsg *dropRouteEntryMsg = new DropRouteEntryMsg;
			dropRouteEntryMsg->setRouteEntrySP( routeEntrySP );

			cModule *cModulePtr = getIPModule( nodeFullPath );
			if ( cModulePtr != 0 ) {
				sendDirect( dropRouteEntryMsg, cModulePtr, "hlaIn" );
			} else {
				std::cout << "WARNING:  DropRouteFromRoutingTable:  NO MODULE FOR NODE \"" << nodeFullPath << "\"" << std::endl;
			}
			continue;
		}

		if ( StartModifyToHLAPacketsAttack::match( classHandle ) ) {
            StartModifyToHLAPacketsAttackSP startModifyToHLAPacketsAttackSP =
             boost::static_pointer_cast< StartModifyToHLAPacketsAttack >( interactionRootSP );

            std::string nodeFullPath = startModifyToHLAPacketsAttackSP->get_nodeFullPath();

            setModifyToHLAPackets( nodeFullPath, true );
            continue;
        }

        if ( StopModifyToHLAPacketsAttack::match( classHandle ) ) {
            StopModifyToHLAPacketsAttackSP stopModifyToHLAPacketsAttackSP =
             boost::static_pointer_cast< StopModifyToHLAPacketsAttack >( interactionRootSP );

            std::string nodeFullPath = stopModifyToHLAPacketsAttackSP->get_nodeFullPath();

            setModifyToHLAPackets( nodeFullPath, false );
            continue;
        }

        if ( StartModifyFromHLAPacketsAttack::match( classHandle ) ) {
            StartModifyFromHLAPacketsAttackSP startModifyFromHLAPacketsAttackSP =
             boost::static_pointer_cast< StartModifyFromHLAPacketsAttack >( interactionRootSP );

            std::string nodeFullPath = startModifyFromHLAPacketsAttackSP->get_nodeFullPath();

            setModifyFromHLAPackets( nodeFullPath, true );
            continue;
        }

        if ( StopModifyFromHLAPacketsAttack::match( classHandle ) ) {
            StopModifyFromHLAPacketsAttackSP stopModifyFromHLAPacketsAttackSP =
             boost::static_pointer_cast< StopModifyFromHLAPacketsAttack >( interactionRootSP );

            std::string nodeFullPath = stopModifyFromHLAPacketsAttackSP->get_nodeFullPath();

            setModifyFromHLAPackets( nodeFullPath, false );
            continue;
        }

        if ( StartIntegrityAttack::match( classHandle ) ) {
            StartIntegrityAttackSP startIntegrityAttackSP =
             boost::static_pointer_cast< StartIntegrityAttack >( interactionRootSP );

            std::string nodeFullPath = startIntegrityAttackSP->get_nodeFullPath();

            enableIntegrityAttack( nodeFullPath, startIntegrityAttackSP->get_intMultiplier(), startIntegrityAttackSP->get_intAdder(), startIntegrityAttackSP->get_longMultiplier(), startIntegrityAttackSP->get_longAdder(), startIntegrityAttackSP->get_doubleMultiplier(), startIntegrityAttackSP->get_doubleAdder(), startIntegrityAttackSP->get_booleanEnableFlip(), startIntegrityAttackSP->get_stringReplacement() );
            continue;
        }

        if ( StopIntegrityAttack::match( classHandle ) ) {
            StopIntegrityAttackSP stopIntegrityAttackSP =
             boost::static_pointer_cast< StopIntegrityAttack >( interactionRootSP );

            std::string nodeFullPath = stopIntegrityAttackSP->get_nodeFullPath();
            disableIntegrityAttack( nodeFullPath );
            continue;
        }

        //
		// PROCESSING FOR ATTACK-DIRECTIVE INTERACTIONS
		//

		std::cerr << "ERROR:  HLAInterface:  received unexpected interaction:" << std::endl;
		std::cerr << *interactionRootSP << std::endl << std::endl;

	}

}
    
void HLAInterface::interactionArrival( void ) {
	Enter_Method_Silent( "interactionArrival" );
	if ( _noInteractionArrivalFlag ) {
		scheduleAt( simTime(), _interactionArrivalMsg );
		_noInteractionArrivalFlag = false;
	}
}

void HLAInterface::receiveInteraction(
 RTI::InteractionClassHandle theInteraction,
 const RTI::ParameterHandleValuePairSet& theParameters,
 const RTI::FedTime& theTime,
 const char *theTag,
 RTI::EventRetractionHandle theHandle
)
 throw ( RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError ) {

    static bool notPrinted = true;
    if ( notPrinted ) {
        std::cerr << listAppSpecProperties() << std::endl;
        notPrinted = false;
    }

	InteractionRoot::SP interactionRootSP = InteractionRoot::create_interaction( theInteraction, theParameters, theTime );
	C2WInteractionRootSP c2wInteractionRootSP = boost::dynamic_pointer_cast< C2WInteractionRoot >( interactionRootSP );

    // Filter interaction if src/origin fed requirements (if any) are met
	if (  c2wInteractionRootSP != 0 && SubscribedInteractionFilter::get_singleton().filterC2WInteraction( getFederateId(), c2wInteractionRootSP )  ) {
	    return;
    }

	//std::cerr << "Received interaction with timestamp " << boost::lexical_cast< std::string >(  static_cast< RTIfedTime >( theTime ).getTime()  ) << " at federate time " <<
	// boost::lexical_cast< std::string >( getCurrentTime() ) <<  std::endl;

	Super::receiveInteraction( theInteraction, theParameters, theTime, theTag, theHandle );

	interactionArrival();
}

void HLAInterface::receiveInteraction(
 RTI::InteractionClassHandle theInteraction,
 const RTI::ParameterHandleValuePairSet& theParameters,
 const char *theTag
)
 throw ( RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::FederateInternalError ) {

	InteractionRoot::SP interactionRootSP = InteractionRoot::create_interaction( theInteraction, theParameters );
	C2WInteractionRootSP c2wInteractionRootSP = boost::dynamic_pointer_cast< C2WInteractionRoot >( interactionRootSP );
    if (  c2wInteractionRootSP != 0 && SubscribedInteractionFilter::get_singleton().filterC2WInteraction( getFederateId(), c2wInteractionRootSP )  ) {
        return;
    }

	// std::cerr << "Received interaction at federate time " << boost::lexical_cast< std::string >( getCurrentTime() ) <<  std::endl;

	Super::receiveInteraction( theInteraction, theParameters, theTag );

	interactionArrival();
}

void HLAInterface::processObjectReflectors( void ) {

	Enter_Method( "processObjectReflectors" );

//	ObjectReflector objectReflector;
//	while(  !( objectReflector = getNextObjectReflector() ).isNull()  ) {
//
//		objectReflector.reflect();
//		ObjectRootSP objectRootSP = objectReflector.getObjectRootSP();
//
// 		ObjectMsg *objectMsg = new ObjectMsg( "Object" );
// 		objectMsg->setObjectRootSP( objectRootSP );
// 		objectMsg->setTimestamp( objectRootSP->getTime() );
// 		objectMsg->setKind( OBJECT );
//
//		sendDirect( objectMsg, HLAGateway::get_InstancePtr(), "hlaIn" );
//	}
}

HLAInterface::HLAInterface( void ) :
 cSimpleModule(), SynchronizedFederate(),
 _federation_name( "" ),
 _name( "" ),
 _time( 0.0 ),
 _lookahead( 0.01 ),
 _step( 0.1 ),
 _interactionArrivalMsg(  new cMessage( "interactionArrival" )  ),
 _noInteractionArrivalFlag( true )
{ 
	setInstancePtr( this );
}

HLAInterface::~HLAInterface( void ) throw() { }


int HLAInterface::numInitStages() const {

	// UDPEndpointApp registers at stage 0
	// HLAInterface initializes at stage 1
	return 6;
}

void HLAInterface::initialize(int stage) {	
	// UDPEndpointApp registers at stage 0
	// HLAInterface initializes at stage 1
	if ( stage == 3 ) {

		this->_federation_name = par( "federation_name" ).stringValue();
		this->_name = par( "federate_name" ).stringValue();
		this->_step = par( "federate_step_size" ).doubleValue();
		this->_lookahead = par( "federate_lookahead" ).doubleValue();

		setup();
	}

	if ( stage == 5 ) {
		std::cerr << "AppSpecProperties:" << std::endl;
		std::cerr << listAppSpecProperties() << std::endl;
	}
}

void HLAInterface::setup() {

	std::string dburl(  getenv( "C2WDBURL" )  );
	std::string dbname(  getenv( "C2WDBFILE" )  );
	
	if ( !dbname.empty() ) {
	    if ( dburl.empty() ) C2WLogger::get_singleton().init( dbname );
	    else                 C2WLogger::get_singleton().init( dbname, dburl );
	    _logLevel = getenv( "C2WLOGLEVEL" );
	}
	    
    // Join the federation
    createRTI();
    joinFederation( _federation_name, _name );

    enableTimeConstrained();
    enableTimeRegulation( _lookahead );
    enableAsynchronousDelivery();

    // publish interactions
    NetworkPacket::publish( getRTI() );
    CommandExecutionStatus::publish( getRTI() );

    // subscribe interactions
    StartNodeAttack::subscribe( getRTI() );
    StartLinkAttack::subscribe( getRTI() );
    NetworkPacket::subscribe( getRTI() );
    StartDisableNetworkAttack::subscribe( getRTI() );
    StopDisableNetworkAttack::subscribe( getRTI() );
    StopNodeAttack::subscribe( getRTI() );
    StopLinkAttack::subscribe( getRTI() );
    StartDataInjectionAttack::subscribe( getRTI() );
    StopDataInjectionAttack::subscribe( getRTI() );
    StartSnifferAttack::subscribe( getRTI() );
    StopSnifferAttack::subscribe( getRTI() );
    StartMasqueradingAttack::subscribe( getRTI() );
    StopMasqueradingAttack::subscribe( getRTI() );
    CeaseReplayAttack::subscribe( getRTI() );
    RecordPacketsForReplayAttack::subscribe( getRTI() );
    StopDNSPoisiningAttack::subscribe( getRTI() );
    StartDNSPoisiningAttack::subscribe( getRTI() );
    OmnetCommand::subscribe( getRTI() );
    StartReplayAttack::subscribe( getRTI() );
    TerminateReplayAttack::subscribe( getRTI() );
    StopOutOfOrderPacketsAttack::subscribe( getRTI() );
    StartOutOfOrderPacketsAttack::subscribe( getRTI() );
    DropRouteFromRoutingTable::subscribe( getRTI() );
    AddRouteToRoutingTable::subscribe( getRTI() );
    StartNetworkFilterAttack::subscribe( getRTI() );
    StopNetworkFilterAttack::subscribe( getRTI() );
    StopDelayNodeAttack::subscribe( getRTI() );
    StartDelayNodeAttack::subscribe( getRTI() );
    StartModifyFromHLAPacketsAttack::subscribe( getRTI() );
    StopModifyFromHLAPacketsAttack::subscribe( getRTI() );
    StartModifyToHLAPacketsAttack::subscribe( getRTI() );
    StopModifyToHLAPacketsAttack::subscribe( getRTI() );
    StartIntegrityAttack::subscribe( getRTI() );
    StopIntegrityAttack::subscribe( getRTI() );

    StartNetworkIPFirewall::subscribe( getRTI() );
    StopNetworkIPFirewall::subscribe( getRTI() );


    SubscribedInteractionFilter::get_singleton().initialize();

	//publish objects
	
	//subscribe objects
	
	if ( !dbname.empty() ) {
    	NetworkPacket::enablePublishLog("NetworkPacket", "OmnetFederate", "NORMAL", _logLevel);
    	CommandExecutionStatus::enablePublishLog("CommandExecutionStatus", "OmnetFederate", "NORMAL", _logLevel);

    	StartNodeAttack::enableSubscribeLog("StartNodeAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartLinkAttack::enableSubscribeLog("StartLinkAttack", "OmnetFederate", "NORMAL", _logLevel);
    	NetworkPacket::enableSubscribeLog("NetworkPacket", "OmnetFederate", "NORMAL", _logLevel);
    	StartDisableNetworkAttack::enableSubscribeLog("StartDisableNetworkAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopDisableNetworkAttack::enableSubscribeLog("StopDisableNetworkAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopNodeAttack::enableSubscribeLog("StopNodeAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopLinkAttack::enableSubscribeLog("StopLinkAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartDataInjectionAttack::enableSubscribeLog("StartDataInjectionAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopDataInjectionAttack::enableSubscribeLog("StopDataInjectionAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartSnifferAttack::enableSubscribeLog("StartSnifferAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopSnifferAttack::enableSubscribeLog("StopSnifferAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartMasqueradingAttack::enableSubscribeLog("StartMasqueradingAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopMasqueradingAttack::enableSubscribeLog("StopMasqueradingAttack", "OmnetFederate", "NORMAL", _logLevel);
    	CeaseReplayAttack::enableSubscribeLog("CeaseReplayAttack", "OmnetFederate", "NORMAL", _logLevel);
    	RecordPacketsForReplayAttack::enableSubscribeLog("RecordPacketsForReplayAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopDNSPoisiningAttack::enableSubscribeLog("StopDNSPoisiningAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartDNSPoisiningAttack::enableSubscribeLog("StartDNSPoisiningAttack", "OmnetFederate", "NORMAL", _logLevel);
    	OmnetCommand::enableSubscribeLog("OmnetCommand", "OmnetFederate", "NORMAL", _logLevel);
    	StartReplayAttack::enableSubscribeLog("StartReplayAttack", "OmnetFederate", "NORMAL", _logLevel);
    	TerminateReplayAttack::enableSubscribeLog("TerminateReplayAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopOutOfOrderPacketsAttack::enableSubscribeLog("StopOutOfOrderPacketsAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartOutOfOrderPacketsAttack::enableSubscribeLog("StartOutOfOrderPacketsAttack", "OmnetFederate", "NORMAL", _logLevel);
    	DropRouteFromRoutingTable::enableSubscribeLog("DropRouteFromRoutingTable", "OmnetFederate", "NORMAL", _logLevel);
    	AddRouteToRoutingTable::enableSubscribeLog("AddRouteToRoutingTable", "OmnetFederate", "NORMAL", _logLevel);
    	StartNetworkFilterAttack::enableSubscribeLog("StartNetworkFilterAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopNetworkFilterAttack::enableSubscribeLog("StopNetworkFilterAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StopDelayNodeAttack::enableSubscribeLog("StopDelayNodeAttack", "OmnetFederate", "NORMAL", _logLevel);
    	StartDelayNodeAttack::enableSubscribeLog("StartDelayNodeAttack", "OmnetFederate", "NORMAL", _logLevel);
        StartIntegrityAttack::enableSubscribeLog("StartIntegrityAttack", "OmnetFederate", "NORMAL", _logLevel);
        StopIntegrityAttack::enableSubscribeLog("StopIntegrityAttack", "OmnetFederate", "NORMAL", _logLevel);

        StartNetworkIPFirewall::enableSubscribeLog("StartNetworkIPFirewall", "OmnetFederate", "NORMAL", _logLevel);
        StopNetworkIPFirewall::enableSubscribeLog("StopNetworkIPFirewall", "OmnetFederate", "NORMAL", _logLevel);
	}

    // set up dataflows
    readyToPopulate();
    // readyToRun();

    _time = getCurrentTime();

    // Keep a message in the queue so the simulation doesn't end too soon
    _keepAliveMsg = new cMessage( "keepAlive" );
    scheduleAt( simTime(), _keepAliveMsg );

    _readyToRunMsg = new cMessage ( "readyToRun" );
    scheduleAt( simTime(), _readyToRunMsg );
}

void HLAInterface::handleMessage( cMessage* msg ) {

	// Check ready to run message
	if( msg == _readyToRunMsg ) {
		readyToRun();
		return;
	}

	// PROCESS ANY INCOMING INTERACTIONS
	if ( msg == _interactionArrivalMsg ) {
		processInteractions();
		_noInteractionArrivalFlag = true;
		return;
	}

	// Keep a message in the queue so the simulation doesn't end too soon
	if ( msg == _keepAliveMsg ) {
		sendKeepAliveMsg();
		return;
	}

	double minTime = getMinTSOTimestamp();
	double timestamp = msg->getTimestamp().dbl();
	if ( timestamp < minTime ) {
		timestamp = minTime;
	}

	std::string msgName = msg->getName();
	
	if ( msgName == "Interaction+Msg" ) {
		InteractionMsg *interactionMsg = static_cast< InteractionMsg * >( msg );
		NetworkPacketSP networkPacketSP = boost::static_pointer_cast< NetworkPacket >( interactionMsg->getInteractionRootSP() );
		networkPacketSP->set_sourceFed( getFederateId() );
		networkPacketSP->sendInteraction( getRTI(), timestamp );
		// std::cerr << "Sending interaction with timestamp " << boost::lexical_cast< std::string >( timestamp ) << std::endl;
//	} else if ( msgKind == OBJECT ) {
//		ObjectMsg *objectMsgPtr = static_cast< ObjectMsg * >( msg );
//		ObjectRootSP objectRootSP = objectMsgPtr->getObjectRootSP();
//		objectRootSP->registerObject( getRTI() ); // NOP IF ALREADY REGISTERED.
//		objectRootSP->updateAttributeValues( getRTI(), timestamp );
	} else {
		std::cerr << "ERROR:  HLAInterface:  Unexpected message received (name,kind):  (" <<
		 msgName << "," << msg->getKind() << ")" << std::endl << std::endl;
	}

	cancelAndDelete( msg );
}

void HLAInterface::sendKeepAliveMsg() {
    scheduleAt(simTime() + _step, _keepAliveMsg);
}

std::ostream &operator<<( std::ostream &os, const HLAInterface::AppSpec &appSpec ) {
	os << appSpec.getSpec();
	return os;
}

std::ostream &operator<<( std::ostream &os, const HLAInterface::NetworkAddress &networkAddress ) {
	std::string output;

	unsigned int networkIPAddress = networkAddress.getNetworkIPAddress();
	bool first = true;
	for( int ix = 0 ; ix < 4 ; ++ix ) {
		if ( first ) {
			first = false;
		} else {
			output = "." + output;
		}
		unsigned int byte = networkIPAddress & 0xFF;
		output = boost::lexical_cast< std::string >( byte ) + output;
		networkIPAddress >>= 8;
	}

	unsigned int netMask = networkAddress.getNetMask();
	int bits = HLAInterface::NetworkAddress::countBits( netMask );
	if ( bits != 32 ) {
		output += "/" + boost::lexical_cast< std::string >( bits );
	}

	os << output;
	return os;
}


std::ostream &operator<<( std::ostream &os, const HLAInterface::RouteEntry &routeEntry ) {
	os << "RouteEntry( nodeName:" << routeEntry.get_nodeName() << ", interfaceEntry:" << routeEntry.get_interfaceEntry() <<
	 ", networkAddress:" << routeEntry.get_networkAddress() << ", gateway:" << routeEntry.get_gatewayAddress() << ")";
	return os;
}

std::ostream &operator<<( std::ostream &os, const HLAInterface::RouteEntrySP &routeEntrySP ) {
	return os << *routeEntrySP;
}
