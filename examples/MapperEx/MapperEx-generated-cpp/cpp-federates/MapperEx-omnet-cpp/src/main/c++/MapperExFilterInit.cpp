
/*
 * Copyright (c) 2016, Institute for Software Integrated Systems, Vanderbilt University
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
 * UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * @author Himanshu Neema
 */

#include "MapperExFilterInit.h"
#include "OmnetFOMInteractions.h"
//#include "MapperExNonOmnetNonGenericSources.hpp"

void MapperExFilterInit::execute( SubscribedInteractionFilter &subscribedInteractionFilter ) {
	
	
	subscribedInteractionFilter.setFedFilters( StopDisableNetworkAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartModifyFromHLAPacketsAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( DropRouteFromRoutingTable::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartModifyToHLAPacketsAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( RecordPacketsForReplayAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopNetworkFilterAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopNodeAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( AddRouteToRoutingTable::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( CeaseReplayAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartDNSPoisoningAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartNetworkFilterAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartDelayNodeAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( NetworkPacket::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::MAPPER );
	subscribedInteractionFilter.setFedFilters( StartSnifferAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopSnifferAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopNetworkIPFirewall::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopDNSPoisoningAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartIntegrityAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopIntegrityAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartReplayAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopOutOfOrderPacketsAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopLinkAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartNetworkIPFirewall::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopModifyToHLAPacketsAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartNodeAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartDataInjectionAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopDelayNodeAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartOutOfOrderPacketsAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartMasqueradingAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( TerminateReplayAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartDisableNetworkAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopModifyFromHLAPacketsAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopMasqueradingAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StopDataInjectionAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );
	subscribedInteractionFilter.setFedFilters( StartLinkAttack::get_handle(), SubscribedInteractionFilter::ORIGIN_FILTER_DISABLED, SubscribedInteractionFilter::SOURCE_FILTER_DISABLED );

}

static bool init( void ) {
    SubscribedInteractionFilter::get_singleton().setFilterInit( MapperExFilterInit::create() );
    return true;
}

static bool static_init = init();


