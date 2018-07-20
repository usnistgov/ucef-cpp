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
 * @author Himanshu Neema
 * @author Harmon Nine
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

#ifndef _INTERACTION_ROOT
#define _INTERACTION_ROOT

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef RTI_USES_STD_FSTREAM
#define RTI_USES_STD_FSTREAM
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <set>
#include <map>
#include <list>
#include <ctype.h>
#include <stdlib.h>

#include <boost/shared_ptr.hpp>

#include <TypeMedley.hpp>
#include <StringCollections.hpp>

#include "RTI.hh"
#include "fedtime.hh"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "RTILogger.hpp"

#ifndef C2W_RTI_LOGGER_CLS
#define C2W_RTI_LOGGER_CLS C2WConsoleLogger
#endif

#include "InteractionIf.hpp"

class InteractionRoot : public InteractionIf{
public:
	typedef std::map< std::string, StringSet * > NameStringSetPtrMap;
	typedef std::map< std::string, StringVector * > NameStringVectorPtrMap;
	typedef std::map< std::string, int > StringIntegerMap;
	typedef std::map< int, std::string > IntegerStringMap;

	typedef boost::shared_ptr< InteractionRoot > SP;
	typedef boost::shared_ptr< RTI::ParameterHandleValuePairSet > ParameterHandleValuePairSetSP;

	typedef SP (*FactoryFunctionPtr)( void );
	typedef std::map< std::string, FactoryFunctionPtr > ClassNameFactoryMap;

	typedef void (*PubsubFunctionPtr)( RTI::RTIambassador * );
	typedef std::map< std::string, PubsubFunctionPtr > ClassNamePubSubMap;
	typedef std::map< std::string, std::string> DatamemberTypeMap;

	static DatamemberTypeMap &getDatamemberTypeMap( void ) {
		static DatamemberTypeMap datamemberTypeMap;
		return datamemberTypeMap;
	}

protected:
	static StringSet &getClassNameSet( void ) {
		static StringSet classNameSet;
		return classNameSet;
	}

	static ClassNameFactoryMap &getClassNameFactoryMap( void ) {
		static ClassNameFactoryMap classNameFactoryMap;
		return classNameFactoryMap;
	}

	static ClassNamePubSubMap &getClassNamePublishMap( void ) {
		static ClassNamePubSubMap classNamePublishMap;
		return classNamePublishMap;
	}

	static ClassNamePubSubMap &getClassNameUnpublishMap( void ) {
		static ClassNamePubSubMap classNameUnpublishMap;
		return classNameUnpublishMap;
	}

	static ClassNamePubSubMap &getClassNameSubscribeMap( void ) {
		static ClassNamePubSubMap classNameSubscribeMap;
		return classNameSubscribeMap;
	}

	static ClassNamePubSubMap &getClassNameUnsubscribeMap( void ) {
		static ClassNamePubSubMap classNameUnsubscribeMap;
		return classNameUnsubscribeMap;
	}

	static NameStringVectorPtrMap &getDatamemberClassNameVectorPtrMap( void ) {
		static NameStringVectorPtrMap datamemberClassNameVectorPtrMap;
		return datamemberClassNameVectorPtrMap;
	}

	static NameStringVectorPtrMap &getAllDatamemberClassNameVectorPtrMap( void ) {
		static NameStringVectorPtrMap allDatamemberClassNameVectorPtrMap;
		return allDatamemberClassNameVectorPtrMap;
	}

	static StringIntegerMap &getClassNameHandleMap( void ) {
		static StringIntegerMap classNameHandleMap;
		return classNameHandleMap;
	}
	static IntegerStringMap &getClassHandleNameMap( void ) {
		static IntegerStringMap classHandleNameMap;
		return classHandleNameMap;
	}

	static StringIntegerMap &getDatamemberNameHandleMap( void ) {
		static StringIntegerMap datamemberNameHandleMap;
		return datamemberNameHandleMap;
	}
	static IntegerStringMap &getDatamemberHandleNameMap( void ) {
		static IntegerStringMap datamemberHandleNameMap;
		return datamemberHandleNameMap;
	}

private:
	static int &getHandle( void ) {
		static int handle;
		return handle;
	}

public:
	static int get_handle( void ) { return getHandle(); }
	static std::string get_class_name( void ) { return "InteractionRoot"; }

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
		return InteractionRoot::SP( new InteractionRoot() );
	}

	static std::string &getInitErrorMessage( void ) {
		static std::string initErrorMessage( "Error:  InteractionRoot:  could not initialize:  " );
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
		static std::string publishErrorMessage = "Error:  InteractionRoot:  could not publish:  ";
		return publishErrorMessage;
	}

public:
	static void publish( RTI::RTIambassador *rti );

private:
	static std::string &getUnpublishErrorMessage( void ) {
		static std::string unpublishErrorMessage = "Error:  InteractionRoot:  could not unpublish:  ";
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
		static std::string subscribedErrorMessage = "Error:  InteractionRoot:  could not subscribe:  ";
		return subscribedErrorMessage;
	}

public:
	static void subscribe( RTI::RTIambassador *rti );

private:
	static std::string getUnsubscribeErrorMessage( void ) {
		static std::string unsubscribeErrorMessage = "Error:  InteractionRoot:  could not unsubscribe:  ";
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

	virtual std::string getParameterName( int datamemberHandle ) const {
		return std::string();
	}

	int getParameterHandle( const std::string &datamemberName ) const {
		return get_parameter_handle(getClassName(), datamemberName);
	}

	virtual void publishInteraction( RTI::RTIambassador *rti ) { publish( rti ); }
	virtual void subscribeInteraction( RTI::RTIambassador *rti ) { subscribe( rti ); }

	virtual InteractionRoot::SP clone( void ) {
		return InteractionRoot::SP(  new InteractionRoot( *this )  );
	}

public:
	static StringSet get_interaction_names( void ) { return getClassNameSet(); }
	static StringVector get_parameter_names( const std::string &className ) {
		NameStringVectorPtrMap::iterator nssItr = getDatamemberClassNameVectorPtrMap().find( className );
		return nssItr == getDatamemberClassNameVectorPtrMap().end() ? StringVector() : *nssItr->second;
	}

	static StringVector get_all_parameter_names( const std::string &className ) {
		NameStringVectorPtrMap::iterator nssItr = getAllDatamemberClassNameVectorPtrMap().find( className );
		return nssItr == getAllDatamemberClassNameVectorPtrMap().end() ? StringVector() : *nssItr->second;
	}

	static std::string get_class_name( int classHandle ) {
		IntegerStringMap::iterator ismItr = getClassHandleNameMap().find( classHandle );
		return ismItr == getClassHandleNameMap().end() ? std::string() : ismItr->second;
	}

	static int get_handle( const std::string &className ) {
		StringIntegerMap::iterator simItr = getClassNameHandleMap().find( className );
		if ( simItr == getClassNameHandleMap().end() ) {
			std::cerr << "Bad class name \"" << className << "\" on get_handle." << std::endl;
			return -1;
		}

		return simItr->second;
	}

	static std::string get_parameter_name( int datamemberHandle ) {
		IntegerStringMap::iterator ismItr = getDatamemberHandleNameMap().find( datamemberHandle );
		return ismItr == getDatamemberHandleNameMap().end() ? std::string() : ismItr->second;
	}

	static int get_parameter_handle( const std::string &className, const std::string &datamemberName ) {
		StringIntegerMap::iterator simItr = getDatamemberNameHandleMap().find( className + "." + datamemberName );
		if ( simItr == getDatamemberNameHandleMap().end() ) {
			std::cerr << "Bad parameter \"" << datamemberName << "\" for class \"" << className << "\" on get_parameter_handle." << std::endl;
			return -1;
		}

		return simItr->second;
	}

	static void publish( const std::string &className, RTI::RTIambassador *rti ) {
		ClassNamePubSubMap::iterator cpmItr = getClassNamePublishMap().find( className );
		if ( cpmItr == getClassNamePublishMap().end() ) {
			std::cerr << "Bad class name \"" << className << "\" on publish." << std::endl;
			return;
		}

		(*cpmItr->second)( rti );
	}

	static void unpublish( const std::string &className, RTI::RTIambassador *rti ) {
		ClassNamePubSubMap::iterator cpmItr = getClassNameUnpublishMap().find( className );
		if ( cpmItr == getClassNameUnpublishMap().end() ) {
			std::cerr << "Bad class name \"" << className << "\" on unpublish." << std::endl;
			return;
		}

		(*cpmItr->second)( rti );
	}

	static void subscribe( const std::string &className, RTI::RTIambassador *rti ) {
		ClassNamePubSubMap::iterator cpmItr = getClassNameSubscribeMap().find( className );
		if ( cpmItr == getClassNameSubscribeMap().end() ) {
			std::cerr << "Bad class name \"" << className << "\" on subscribe." << std::endl;
			return;
		}

		(*cpmItr->second)( rti );
	}

	static void unsubscribe( const std::string &className, RTI::RTIambassador *rti ) {
		ClassNamePubSubMap::iterator cpmItr = getClassNameUnsubscribeMap().find( className );
		if ( cpmItr == getClassNameSubscribeMap().end() ) {
			std::cerr << "Bad class name \"" << className << "\" on unsubscribe." << std::endl;
			return;
		}

		(*cpmItr->second)( rti );
	}

public:
	static SP create_interaction( const std::string &className ) {
		ClassNameFactoryMap::iterator cfmItr = getClassNameFactoryMap().find( className );
		return cfmItr == getClassNameFactoryMap().end() ? SP( (InteractionRoot *)0 ) : (*cfmItr->second)();
	}

	static SP create_interaction( const std::string &className, const RTIfedTime &dlc13FedTime ) {
		ClassNameFactoryMap::iterator cfmItr = getClassNameFactoryMap().find( className );
		if ( cfmItr == getClassNameFactoryMap().end() ) {
			return SP( (InteractionRoot *)0 );
		}

		SP sp = (*cfmItr->second)();
		sp->setTime( dlc13FedTime.getTime() );
		return sp;
	}

	static SP create_interaction( int classHandle ) {
		IntegerStringMap::iterator ismItr = getClassHandleNameMap().find( classHandle );
		return ismItr == getClassHandleNameMap().end() ? SP( (InteractionRoot *)0 ) : create_interaction( ismItr->second );
	}

	static SP create_interaction( int classHandle, const RTIfedTime &dlc13FedTime ) {
		IntegerStringMap::iterator ismItr = getClassHandleNameMap().find( classHandle );
		return ismItr == getClassHandleNameMap().end() ? SP( (InteractionRoot *)0 ) : create_interaction( ismItr->second, dlc13FedTime );
	}

	static SP create_interaction( int classHandle, const RTI::ParameterHandleValuePairSet &datamemberMap );
	static SP create_interaction( int classHandle, const RTI::ParameterHandleValuePairSet &datamemberMap, const RTIfedTime &dlc13FedTime );

	static StringVector get_parameter_names() {
		return StringVector();
	}

	static StringVector get_all_parameter_names() {
		return StringVector();
	}

private:
	double _time;
	static C2WLogger* _logger;

public:
	double getTime( void ) { return _time; }

	void setTime( double time ) { _time = time; }
	void setTime( const RTIfedTime &dlc13FedTime ) {
		setTime( dlc13FedTime.getTime() );
	}

	InteractionRoot( void ) : _time( -1 ) { }

	InteractionRoot( const RTI::ParameterHandleValuePairSet &datamemberMap ) : _time( -1 ) {
		setParameters( datamemberMap );
	}

	InteractionRoot( const RTI::ParameterHandleValuePairSet &datamemberMap, const RTIfedTime &dlc13FedTime ) : _time( dlc13FedTime.getTime() ) {
		setParameters( datamemberMap );
	}

	static SP create( void ) { return SP( new InteractionRoot ); }
	static SP create( const RTI::ParameterHandleValuePairSet &datamemberMap ) { return SP(  new InteractionRoot( datamemberMap )  ); }
	static SP create( const RTI::ParameterHandleValuePairSet &datamemberMap, const RTIfedTime &dlc13FedTime ) {
		return SP(  new InteractionRoot( datamemberMap, dlc13FedTime )  );
	}

	virtual TypeMedley getParameter( const std::string &datamemberName ) const {
		throw std::invalid_argument( getClassName() + " class has no parameter named \"" + datamemberName + "\"" );
	}

	TypeMedley getParameter( int datamemberHandle ) const {
		const std::string &datamemberName = getParameterName(datamemberHandle);
		if (datamemberName.empty()) {
			throw std::invalid_argument(
				getClassName() + " class has no parameter with handle (" + boost::lexical_cast< std::string >( datamemberHandle ) + ")"
			);
		}
		return getParameter(datamemberName);
	}

	void setParameters( const RTI::ParameterHandleValuePairSet &datamemberMap );

private:
	void setParameter( RTI::Handle handle, const std::string &val ) {
		if (  !setParameterAux( handle, val )  ) {
			std::cerr << "set:  bad parameter handle in class \"" + getClassName() + "\"" << std::endl;
		}
	}

public:
	void setParameter( const std::string &datamemberName, TypeMedley value ) {
		if (  !setParameterAux( datamemberName, value )  ) {
			std::cerr << "Error:  class \"" << getClassName() << "\":  invalid parameter \"" + datamemberName + "\"" << std::endl;
		}
	}

private:
	bool setParameterAux( int datamemberHandle, const std::string &val ) {
		const std::string &datamemberName = getParameterName(datamemberHandle);
		if (datamemberName.empty()) {
			return false;
		}
		return setParameterAux(datamemberName, val);
	}

protected:
	virtual bool setParameterAux( const std::string &datamemberName, const std::string &value ) {
		return false;
	}

	virtual bool setParameterAux( const std::string &datamemberName, TypeMedley value ) {
		return false;
	}

	ParameterHandleValuePairSetSP createDatamemberHandleValuePairSet() {
		const StringVector &parameterNames = getAllParameterNames();
		StringVector::size_type numberOfParameters = parameterNames.size();

		ParameterHandleValuePairSetSP datamembers = ParameterHandleValuePairSetSP(RTI::ParameterSetFactory::create(numberOfParameters));
		for (StringVector::size_type i; i < numberOfParameters; i++) {
			std::string stringConversion = static_cast< std::string >( getParameter(parameterNames[i]) );
			datamembers->add( getParameterHandle(parameterNames[i]), stringConversion.c_str(), stringConversion.size() );
		}
		return datamembers;
	}

public:
	void sendInteraction( RTI::RTIambassador *rti, double time );
	void sendInteraction( RTI::RTIambassador *rti );

private:
	static std::string fedName;

public:
	static bool enablePubLog;
	static bool enableSubLog;
	static std::string pubLogLevel;
	static std::string subLogLevel;

public:
	static void enablePublishLog(const std::string &interaction, const std::string &fed, const std::string &thislevel, const std::string &globallevel);
	static void enableSubscribeLog(const std::string &interaction, const std::string &fed, const std::string &thislevel, const std::string &globallevel);

	void createLog( double time, bool isPub = true );
};

typedef InteractionRoot::SP InteractionRootSP;

static bool call_InteractionRoot_static_init = InteractionRoot::static_init();

std::ostream &operator<<( std::ostream &os, InteractionRoot::SP entitySP );
std::ostream &operator<<( std::ostream &os, const InteractionRoot &entity );

#endif
