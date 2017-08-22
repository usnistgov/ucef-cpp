

#include "FederateResignInteraction.hpp"




void FederateResignInteraction::init( RTI::RTIambassador *rti ) {
	static bool isInitialized = false;
	if ( isInitialized ) {
		return;
	}
	isInitialized = true;

	C2WInteractionRoot::init( rti );

	bool isNotInitialized = true;
	while( isNotInitialized ) {
		try {
			getHandle() = rti->getInteractionClassHandle( "InteractionRoot.C2WInteractionRoot.FederateResignInteraction" );
			isNotInitialized = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << getInitErrorMessage() << "Federate Not Execution Member" << std::endl;
			return;				
		} catch ( RTI::NameNotFound & ) {
			std::cerr << getInitErrorMessage() << "Name Not Found" << std::endl;
			return;				
		} catch ( ... ) {
			std::cerr << getInitErrorMessage() << "Exception caught ... retry" << std::endl;
		}
	}

	getClassNameHandleMap().insert(  std::make_pair( "FederateResignInteraction", get_handle() )  );
	getClassHandleNameMap().insert(  std::make_pair( get_handle(), "FederateResignInteraction" )  );


	isNotInitialized = true;
	while( isNotInitialized ) {
		try {		
			
			
			get_FederateId_handle_var() = rti->getParameterHandle( "FederateId", get_handle() );
			get_FederateType_handle_var() = rti->getParameterHandle( "FederateType", get_handle() );
			get_IsLateJoiner_handle_var() = rti->getParameterHandle( "IsLateJoiner", get_handle() );
			isNotInitialized = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << getInitErrorMessage() << "Federate Not Execution Member" << std::endl;
			return;				
		} catch ( RTI::InteractionClassNotDefined & ) {
			std::cerr << getInitErrorMessage() << "Interaction Class Not Defined" << std::endl;
			return;				
		} catch ( RTI::NameNotFound & ) {
			std::cerr << getInitErrorMessage() << "Name Not Found" << std::endl;
			return;				
		} catch ( ... ) {
			std::cerr << getInitErrorMessage() << "Exception caught ... retry" << std::endl;
		}
	}
	
	
	getDatamemberNameHandleMap().insert(  std::make_pair( "FederateResignInteraction,FederateId", get_FederateId_handle() )  );
	
	getDatamemberHandleNameMap().insert(  std::make_pair( get_FederateId_handle(), "FederateId" )  );
	
	getDatamemberTypeMap().insert( std::make_pair("FederateId", "String") );
	
	
	getDatamemberNameHandleMap().insert(  std::make_pair( "FederateResignInteraction,FederateType", get_FederateType_handle() )  );
	
	getDatamemberHandleNameMap().insert(  std::make_pair( get_FederateType_handle(), "FederateType" )  );
	
	getDatamemberTypeMap().insert( std::make_pair("FederateType", "String") );
	
	
	getDatamemberNameHandleMap().insert(  std::make_pair( "FederateResignInteraction,IsLateJoiner", get_IsLateJoiner_handle() )  );
	
	getDatamemberHandleNameMap().insert(  std::make_pair( get_IsLateJoiner_handle(), "IsLateJoiner" )  );
	
	getDatamemberTypeMap().insert( std::make_pair("IsLateJoiner", "boolean") );
	

}

void FederateResignInteraction::publish( RTI::RTIambassador *rti ) {
	if ( getIsPublished() ) {
		return;
	}

	init( rti );



	bool isNotPublished = true;
	while( isNotPublished ) {
		try {
			rti->publishInteractionClass( get_handle() );
			isNotPublished = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << getPublishErrorMessage() << "Federate Not Execution Member" << std::endl;
			return;
		} catch ( RTI::InteractionClassNotDefined & ) {
			std::cerr << getPublishErrorMessage() << "Interaction Class Not Defined" << std::endl;
			return;
		} catch ( ... ) {
			std::cerr << getPublishErrorMessage() << "Exception caught ... retry" << std::endl;
		}
	}

	getIsPublished() = true;
}

void FederateResignInteraction::unpublish( RTI::RTIambassador *rti ) {
	if ( !getIsPublished() ) {
		return;
	}

	init( rti );

	bool isNotUnpublished = true;
	while( isNotUnpublished ) {
		try {
			rti->unpublishInteractionClass( get_handle() );
			isNotUnpublished = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << getUnpublishErrorMessage() + "Federate Not Execution Member" << std::endl;
			return;
		} catch ( RTI::InteractionClassNotDefined & ) {
			std::cerr << getUnpublishErrorMessage() + "Interaction Class Not Defined" << std::endl;
			return;
		} catch ( RTI::InteractionClassNotPublished & ) {
			std::cerr << getUnpublishErrorMessage() + "Interaction Class Not Published" << std::endl;
			return;
		} catch ( ... ) {
			std::cerr << getUnpublishErrorMessage() << "Exception caught ... retry" << std::endl;
		}
	}

	getIsPublished() = false;
}

void FederateResignInteraction::subscribe( RTI::RTIambassador *rti ) {
	if ( getIsSubscribed() ) {
		return;
	}

	init( rti );

	
	bool isNotSubscribed = true;
	while( isNotSubscribed ) {
		try {
			rti->subscribeInteractionClass( get_handle() );
			isNotSubscribed = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << getSubscribeErrorMessage() << "Federate Not Execution Member" << std::endl;
			return;
		} catch ( RTI::InteractionClassNotDefined & ) {
			std::cerr << getSubscribeErrorMessage() << "Interaction Class Not Defined" << std::endl;
			return;
		} catch ( ... ) {
			std::cerr << getSubscribeErrorMessage() << "Exception caught ... retry" << std::endl;
		}
	}

	getIsSubscribed() = true;
}
	
void FederateResignInteraction::unsubscribe( RTI::RTIambassador *rti ) {
	if ( !getIsSubscribed() ) {
		return;
	}

	init( rti );
	
	bool isNotUnsubscribed = true;
	while( isNotUnsubscribed ) {
		try {
			rti->unsubscribeInteractionClass( get_handle() );
			isNotUnsubscribed = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << getUnsubscribeErrorMessage() << "Federate Not Execution Member" << std::endl;
			return;
		} catch ( RTI::InteractionClassNotDefined & ) {
			std::cerr << getUnsubscribeErrorMessage() << "Interaction Class Not Defined" << std::endl;
			return;
		} catch ( RTI::InteractionClassNotSubscribed & ) {
			std::cerr << getUnsubscribeErrorMessage() << "Interaction Class Not Subscribed" << std::endl;
			return;
		} catch ( ... ) {
			std::cerr << getUnsubscribeErrorMessage() << "Exception caught ... retry" << std::endl;
		}
	}

	getIsSubscribed() = false;
}

bool FederateResignInteraction::static_init( void ) {
	static bool isInitialized = false;
	if ( isInitialized ) {
		return true;
	}
	isInitialized = true;
	
	getClassNameSet().insert( "FederateResignInteraction" );
	
	getClassNameFactoryMap().insert(  std::make_pair( "FederateResignInteraction", &FederateResignInteraction::factory )  );
	getClassNamePublishMap().insert(   std::make_pair(  "FederateResignInteraction", (PubsubFunctionPtr)( &FederateResignInteraction::publish )  )   );
	getClassNameUnpublishMap().insert(   std::make_pair(  "FederateResignInteraction", (PubsubFunctionPtr)( &FederateResignInteraction::unpublish )  )   );
	getClassNameSubscribeMap().insert(   std::make_pair(  "FederateResignInteraction", (PubsubFunctionPtr)( &FederateResignInteraction::subscribe )  )   );
	getClassNameUnsubscribeMap().insert(   std::make_pair(  "FederateResignInteraction", (PubsubFunctionPtr)( &FederateResignInteraction::unsubscribe )  )   );

	getDatamemberClassNameVectorPtrMap().insert(  std::make_pair( "FederateResignInteraction", &getDatamemberNames() )  );
	getAllDatamemberClassNameVectorPtrMap().insert(  std::make_pair( "FederateResignInteraction", &getAllDatamemberNames() )  );
	
	
	
	getDatamemberNames().push_back( "FederateId" );
	getDatamemberNames().push_back( "FederateType" );
	getDatamemberNames().push_back( "IsLateJoiner" );
	
	
	getAllDatamemberNames().push_back( "FederateId" );
	getAllDatamemberNames().push_back( "FederateType" );
	getAllDatamemberNames().push_back( "IsLateJoiner" );



	return true;
}

std::ostream &operator<<( std::ostream &os, FederateResignInteraction::SP entitySP ) {
	return os << *entitySP;
}
std::ostream &operator<<( std::ostream &os, const FederateResignInteraction &entity ) {
	return os << "FederateResignInteraction("  << "FederateId:" << entity.get_FederateId() << ", " << "FederateType:" << entity.get_FederateType() << ", " << "IsLateJoiner:" << entity.get_IsLateJoiner() << ", " << "actualLogicalGenerationTime:" << entity.get_actualLogicalGenerationTime() << ", " << "federateFilter:" << entity.get_federateFilter() << ", " << "originFed:" << entity.get_originFed() << ", " << "sourceFed:" << entity.get_sourceFed()	<< ")";
}





FederateResignInteraction::ParameterHandleValuePairSetSP FederateResignInteraction::createDatamemberHandleValuePairSet( RTI::ULong count ) {
	ParameterHandleValuePairSetSP datamembers = Super::createDatamemberHandleValuePairSet( count + 3 );

	std::string stringConversion;
		

	
	
	stringConversion = static_cast< std::string >(  TypeMedley( get_FederateId() )  );
	datamembers->add( get_FederateId_handle(), stringConversion.c_str(), stringConversion.size() );
	
	stringConversion = static_cast< std::string >(  TypeMedley( get_FederateType() )  );
	datamembers->add( get_FederateType_handle(), stringConversion.c_str(), stringConversion.size() );
	
	stringConversion = static_cast< std::string >(  TypeMedley( get_IsLateJoiner() )  );
	datamembers->add( get_IsLateJoiner_handle(), stringConversion.c_str(), stringConversion.size() );
	

	return datamembers;
}

