#include "SubscribedInteractionFilter.hpp"

bool SubscribedInteractionFilter::filterC2WInteraction( const std::string &federateId, C2WInteractionRootSP c2wInteractionRootSP ) {

    int handle = c2wInteractionRootSP->getClassHandle();
    HandleFilterMap::iterator hfmItr = _handleFilterMap.find( handle );
    if ( hfmItr == _handleFilterMap.end() ) {
        return false;
    }

    Filter &filter = hfmItr->second;

    bool isSourceMapper = boost::ends_with( c2wInteractionRootSP->get_sourceFed(), "Mapper" );
    bool isFromSelf = c2wInteractionRootSP->get_originFed() == federateId;

    return
     ( filter.getSourceFedFilter() == MAPPER && !isSourceMapper ) ||
     ( filter.getSourceFedFilter() == NON_MAPPER && isSourceMapper ) ||
     ( filter.getOriginFedFilter() == SELF && !isFromSelf ) ||
     ( filter.getOriginFedFilter() == NON_SELF && isFromSelf )
    ;

}
