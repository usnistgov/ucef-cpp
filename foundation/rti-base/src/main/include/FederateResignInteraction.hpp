
/*
 * Copyright (c) 2008, Institute for Software Integrated Systems, Vanderbilt University
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
 */

/*
 * 	NOTE:  ALL "if" STATEMENTS IN THIS CODE THAT HAD THE FORM:
 *
 *  if (condition) statement;
 *
 *	OR
 *
 *  if (condition) statement1;
 *  else           statement2;
 *
 *  HAVE BEEN CONVERTED TO THE FORMS:
 *
 *  if (condition) {
 *		statement;
 *	}
 *
 *	AND
 *
 *  if (condition) {
 *		statement1;
 *	} else {
 *		statement2;
 *	}
 *
 *	RESPECTIVELY.  THIS IS BECAUSE THE OMNET DEBUGGER APPEARS TO HAVE A PROBLEM
 *  WITH THE NON-BRACED FORMS OF THE IF STATEMENT.  *PLEASE* DO NOT CHANGE IT BACK.
 *  -- H.S.N.
 *
 */

#ifndef _FederateResignInteraction_CLASS
#define _FederateResignInteraction_CLASS

#include "C2WInteractionRoot.hpp"
#include "C2WException.hpp"


class FederateResignInteraction : public C2WInteractionRoot {

public:
	typedef C2WInteractionRoot Super;
	typedef boost::shared_ptr< FederateResignInteraction > SP;

	static StringVector get_parameter_names() {
		return getDatamemberNames();
	}

	static StringVector get_all_parameter_names() {
		return getAllDatamemberNames();
	}

	FederateResignInteraction( void ) { }
	
	static SP create( void ) { return SP( new FederateResignInteraction ); }

private:
	
	
	static int &get_FederateId_handle_var( void ) {
		static int FederateId_handle;
		return FederateId_handle;
	}
	static int &get_FederateType_handle_var( void ) {
		static int FederateType_handle;
		return FederateType_handle;
	}
	static int &get_IsLateJoiner_handle_var( void ) {
		static int IsLateJoiner_handle;
		return IsLateJoiner_handle;
	}
	
public:
	
	
	static int get_FederateId_handle( void ) { return get_FederateId_handle_var(); }
	static int get_FederateType_handle( void ) { return get_FederateType_handle_var(); }
	static int get_IsLateJoiner_handle( void ) { return get_IsLateJoiner_handle_var(); }
	


private:
	static int &getHandle( void ) {
		static int handle;
		return handle;
	}

public:
	static int get_handle( void ) { return getHandle(); }
	static std::string get_class_name( void ) { return "FederateResignInteraction"; }

private:
	static StringVector &getDatamemberNames( void ) {
		static StringVector datamemberNames;
		return datamemberNames;
	}

	static StringVector &getAllDatamemberNames( void ) {
		static StringVector allDatamemberNames;
		return allDatamemberNames;
	}



private:

	static InteractionRoot::SP factory( void ) {
		return InteractionRoot::SP( new FederateResignInteraction() );
	}


	static std::string &getInitErrorMessage( void ) {
		static std::string initErrorMessage( "Error:  FederateResignInteraction:  could not initialize:  " );
		return initErrorMessage;
	}

protected:
	static void init( RTI::RTIambassador *rti );
	
private:
	static bool &getIsPublished( void ) {
		static bool isPublished = false;
		return isPublished;
	}
	
	static std::string &getPublishErrorMessage( void ) {
		static std::string publishErrorMessage = "Error:  FederateResignInteraction:  could not publish:  ";
		return publishErrorMessage;
	}
	
public:
	static void publish( RTI::RTIambassador *rti );
	
private:
	static std::string &getUnpublishErrorMessage( void ) {
		static std::string unpublishErrorMessage = "Error:  FederateResignInteraction:  could not unpublish:  ";
		return unpublishErrorMessage;
	}

public:
	static void unpublish( RTI::RTIambassador *rti );

private:
	static bool &getIsSubscribed( void ) {
		static bool isSubscribed = false;
		return isSubscribed;
	}
	static std::string &getSubscribeErrorMessage( void ) {
		static std::string subscribedErrorMessage = "Error:  FederateResignInteraction:  could not subscribe:  ";
		return subscribedErrorMessage;
	}

public:
	static void subscribe( RTI::RTIambassador *rti );
		
private:
	static std::string getUnsubscribeErrorMessage( void ) {
		static std::string unsubscribeErrorMessage = "Error:  FederateResignInteraction:  could not unsubscribe:  ";
		return unsubscribeErrorMessage;
	}
	
public:
	static void unsubscribe( RTI::RTIambassador *rti );
	
	static bool static_init( void );
	
	static bool match( int handle ) { return handle == get_handle(); }

	virtual int getClassHandle( void ) const { return get_handle(); }
	virtual std::string getClassName( void ) const { return get_class_name(); }
	virtual StringVector getParameterNames( void ) const { return get_parameter_names(); }
	virtual StringVector getAllParameterNames( void ) const { return get_all_parameter_names(); }

	virtual void publishInteraction( RTI::RTIambassador *rti ) { publish( rti ); }
	virtual void subscribeInteraction( RTI::RTIambassador *rti ) { subscribe( rti ); }

	virtual InteractionRoot::SP clone( void ) {
		return InteractionRoot::SP(  new FederateResignInteraction( *this )  );
	}







private:
	
	
	std::string _FederateId;
	
	std::string _FederateType;
	
	bool _IsLateJoiner;
	
public:
		
	void set_FederateId( const std::string & FederateId ) { _FederateId = FederateId; }
	const std::string & get_FederateId( void ) const { return _FederateId; }
	
	void set_FederateType( const std::string & FederateType ) { _FederateType = FederateType; }
	const std::string & get_FederateType( void ) const { return _FederateType; }
	
	void set_IsLateJoiner( bool IsLateJoiner ) { _IsLateJoiner = IsLateJoiner; }
	bool get_IsLateJoiner( void ) const { return _IsLateJoiner; }
	


	FederateResignInteraction( const RTI::ParameterHandleValuePairSet &datamemberMap ) : Super( datamemberMap ) { }
	
	FederateResignInteraction( const RTI::ParameterHandleValuePairSet &datamemberMap, const RTIfedTime &logicalTime ) : Super( datamemberMap, logicalTime ) { }
	
	
public:
	TypeMedley getParameter( const std::string &datamemberName ) const {
		
		
		if ( "FederateId" == datamemberName ) {
			return TypeMedley( get_FederateId() );
		}
		else if ( "FederateType" == datamemberName ) {
			return TypeMedley( get_FederateType() );
		}
		else if ( "IsLateJoiner" == datamemberName ) {
			return TypeMedley( get_IsLateJoiner() );
		} else {
			return Super::getParameter( datamemberName );
		}
	}
	
	TypeMedley getParameter( int datamemberHandle ) const {
		
		
		if ( get_FederateId_handle() == datamemberHandle ) {
			return TypeMedley( get_FederateId() );
		}
		else if ( get_FederateType_handle() == datamemberHandle ) {
			return TypeMedley( get_FederateType() );
		}
		else if ( get_IsLateJoiner_handle() == datamemberHandle ) {
			return TypeMedley( get_IsLateJoiner() );
		} else {
			return Super::getParameter( datamemberHandle );
		}
	}

protected:
	virtual bool setParameterAux( int param_handle, const std::string &val ) {
		bool retval = true;		
		
		
		if ( param_handle == get_FederateId_handle() ) {
			set_FederateId(  TypeMedley( val )  );
		}
		else if ( param_handle == get_FederateType_handle() ) {
			set_FederateType(  TypeMedley( val )  );
		}
		else if ( param_handle == get_IsLateJoiner_handle() ) {
			set_IsLateJoiner(  TypeMedley( val )  );
		} else {
			retval = Super::setParameterAux( param_handle, val );
		}
		return retval;
	}
	
	virtual bool setParameterAux( const std::string &datamemberName, const std::string &val ) {
		bool retval = true;
		
		
		if ( "FederateId" == datamemberName ) {
			set_FederateId(  TypeMedley( val )  );
		}
		else if ( "FederateType" == datamemberName ) {
			set_FederateType(  TypeMedley( val )  );
		}
		else if ( "IsLateJoiner" == datamemberName ) {
			set_IsLateJoiner(  TypeMedley( val )  );
		} else {
			retval = Super::setParameterAux( datamemberName, val );
		}
		
		return retval;
	}
	
	virtual bool setParameterAux( const std::string &datamemberName, const TypeMedley &val ) {
		bool retval = true;
		
		
		if ( "FederateId" == datamemberName ) {\
			set_FederateId( val );
		}
		else if ( "FederateType" == datamemberName ) {\
			set_FederateType( val );
		}
		else if ( "IsLateJoiner" == datamemberName ) {\
			set_IsLateJoiner( val );
		} else {
			retval = Super::setParameterAux( datamemberName, val );
		}
		
		return retval;
	}

	virtual ParameterHandleValuePairSetSP createDatamemberHandleValuePairSet( RTI::ULong count );

	virtual ParameterHandleValuePairSetSP createDatamemberHandleValuePairSet( ) {
		return createDatamemberHandleValuePairSet( 0 );
	}
	
	
};

typedef FederateResignInteraction::SP FederateResignInteractionSP;

static bool call_FederateResignInteraction_static_init = FederateResignInteraction::static_init();

std::ostream &operator<<( std::ostream &os, FederateResignInteraction::SP entitySP );
std::ostream &operator<<( std::ostream &os, const FederateResignInteraction &entity );
#endif