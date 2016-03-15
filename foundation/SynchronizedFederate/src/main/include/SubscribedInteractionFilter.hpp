#ifndef _ISIS_SUBSCRIBEDINTERACTION_FILTER_HPP_
#define _ISIS_SUBSCRIBEDINTERACTION_FILTER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include "C2WInteractionRoot.hpp"

class SubscribedInteractionFilter {

public:
    enum OriginFedFilter { ORIGIN_FILTER_DISABLED, SELF, NON_SELF };
    enum SourceFedFilter { SOURCE_FILTER_DISABLED, MAPPER, NON_MAPPER };

    class Filter {
    private:
        OriginFedFilter _originFedFilter;
        SourceFedFilter _sourceFedFilter;

    public:
        Filter( void ) : _originFedFilter( ORIGIN_FILTER_DISABLED ), _sourceFedFilter( SOURCE_FILTER_DISABLED ) { }

        void setOriginFedFilter( OriginFedFilter originFedFilter ) {
            _originFedFilter = originFedFilter;
        }

        OriginFedFilter getOriginFedFilter( void ) { return _originFedFilter; }

        void setSourceFedFilter( SourceFedFilter sourceFedFilter ) {
            _sourceFedFilter = sourceFedFilter;
        }
        SourceFedFilter getSourceFedFilter( void ) { return _sourceFedFilter; }
    };

    typedef std::map< int, Filter > HandleFilterMap;

    struct FilterInit {
        typedef boost::shared_ptr< FilterInit > SP;

        virtual ~FilterInit( void ) { }

        static SP create( void ) {
            return SP( new FilterInit() );
        }

        virtual void execute( SubscribedInteractionFilter &subscribedInteractionFilter ) { }
    };

    typedef FilterInit::SP FilterInitSP;

private:
    HandleFilterMap _handleFilterMap;
    FilterInitSP _filterInitSP;

public:
    SubscribedInteractionFilter( void ) : _filterInitSP( FilterInit::create() ) { }

    void setOriginFedFilter( int handle, OriginFedFilter originFedFilter ) {
        HandleFilterMap::iterator hfmItr = _handleFilterMap.find( handle );
        if ( hfmItr == _handleFilterMap.end() ) {
            hfmItr = _handleFilterMap.insert(  std::make_pair( handle, Filter() )  ).first;
        }
        hfmItr->second.setOriginFedFilter( originFedFilter );
    }

    void setSourceFedFilter( int handle, SourceFedFilter sourceFedFilter ) {
        HandleFilterMap::iterator hfmItr = _handleFilterMap.find( handle );
        if ( hfmItr == _handleFilterMap.end() ) {
            hfmItr = _handleFilterMap.insert(  std::make_pair( handle, Filter() )  ).first;
        }
        hfmItr->second.setSourceFedFilter( sourceFedFilter );
    }

    void setFedFilters( int handle, OriginFedFilter originFedFilter, SourceFedFilter sourceFedFilter ) {
        HandleFilterMap::iterator hfmItr = _handleFilterMap.find( handle );
        if ( hfmItr == _handleFilterMap.end() ) {
            hfmItr = _handleFilterMap.insert(  std::make_pair( handle, Filter() )  ).first;
        }
        Filter &filter = hfmItr->second;
        filter.setOriginFedFilter( originFedFilter );
        filter.setSourceFedFilter( sourceFedFilter );
    }

    void setFilterInit( FilterInitSP filterInitSP ) {
        _filterInitSP = filterInitSP;
    }

    bool filterC2WInteraction( const std::string &federateId, C2WInteractionRootSP c2wInteractionRootSP );

    void initialize( void ) {
        _filterInitSP->execute( *this );
    }

    static SubscribedInteractionFilter &get_singleton( void ) {
        static SubscribedInteractionFilter subscribedInteractionFilter;
        return subscribedInteractionFilter;
    }

};

#endif
