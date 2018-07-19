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

#ifndef _OBJECT_ROOT
#define _OBJECT_ROOT

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

#include "ObjectIf.hpp"

class ObjectRoot : public ObjectIf{
public:
	typedef std::map< std::string, StringSet * > NameStringSetPtrMap;
	typedef std::map< std::string, StringVector * > NameStringVectorPtrMap;
	typedef std::map< std::string, int > StringIntegerMap;
	typedef std::map< int, std::string > IntegerStringMap;

	typedef boost::shared_ptr< ObjectRoot > SP;
	typedef boost::shared_ptr< RTI::AttributeHandleValuePairSet > AttributeHandleValuePairSetSP;

	typedef SP (*FactoryFunctionPtr)( void );
	typedef std::map< std::string, FactoryFunctionPtr > ClassNameFactoryMap;

	typedef void (*PubsubFunctionPtr)( RTI::RTIambassador * );
	typedef std::map< std::string, PubsubFunctionPtr > ClassNamePubSubMap;
	typedef std::map< std::string, std::string> DatamemberTypeMap;

	typedef std::map< std::string, RTI::AttributeHandleSet * > ClassNameAttributesPtrMap;
	typedef std::map< int, SP > ObjectMap;

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

	static NameStringVectorPtrMap &getClassNamePublishAttributeNameVectorPtrMap( void ) {
		static NameStringVectorPtrMap classNamePublishAttributeNameVectorPtrMap;
		return classNamePublishAttributeNameVectorPtrMap;
	}
	static NameStringVectorPtrMap &getClassNameSubscribeAttributeNameVectorPtrMap( void ) {
		static NameStringVectorPtrMap classNameSubscribeAttributeNameVectorPtrMap;
		return classNameSubscribeAttributeNameVectorPtrMap;
	}

	static ClassNameAttributesPtrMap &getClassNamePublishedAttributesPtrMap( void ) {
		static ClassNameAttributesPtrMap classNamePublishedAttributesPtrMap;
		return classNamePublishedAttributesPtrMap;
	}
	static ClassNameAttributesPtrMap &getClassNameSubscribedAttributesPtrMap( void ) {
		static ClassNameAttributesPtrMap classNameSubscribedAttributesPtrMap;
		return classNameSubscribedAttributesPtrMap;
	}

	static ObjectMap &getObjectMap( void ) {
		static ObjectMap objectMap;
		return objectMap;
	}

	template< typename S, typename T >
	class Attribute {
	private:
		T _value;
		T _oldValue;
		bool _oldValueInit;
		double _time;

	public:
		Attribute( void ) : _value( 0 ), _oldValueInit( false ), _time( -1 ) { }

		T getValue( void ) const { return _value; }
		void setValue( T value ) { _value = value; }

		double getTime( void ) { return _time; }
		void setTime( double time ) { _time = time; }

		void setHasBeenUpdated( void ) {
			_oldValue = _value;
			_oldValueInit = true;
		}

		bool shouldBeUpdated( bool force ) { return force || !_oldValueInit || _oldValue != _value; }
	};

	template< typename S >
	class Attribute< S, std::string > {
	private:
		std::string _value;
		std::string _oldValue;
		bool _oldValueInit;
		double _time;

	public:
		Attribute( void ) : _oldValueInit( false ), _time( -1 ) { }

		const std::string &getValue( void ) const { return _value; }
		void setValue( const std::string &value ) { _value = value; }

		double getTime( void ) { return _time; }
		void setTime( double time ) { _time = time; }

		void setHasBeenUpdated( void ) {
			_oldValue = _value;
			_oldValueInit = true;
		}

		bool shouldBeUpdated( bool force ) { return force || !_oldValueInit || _oldValue != _value; }
	};

private:
	static int &getHandle( void ) {
		static int handle;
		return handle;
	}

public:
	static int get_handle( void ) { return getHandle(); }
	static std::string get_class_name( void ) { return "ObjectRoot"; }

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
	static RTI::AttributeHandleSet &getPublishedAttributeHandleSet_var( void ) {
		static RTI::AttributeHandleSet *publishedAttributeHandleSetPtr = RTI::AttributeHandleSetFactory::create( 0 );
		return *publishedAttributeHandleSetPtr;
	}
	static StringVector &getPublishAttributeNameVector( void ) {
		static StringVector publishAttributeNameVector;
		return publishAttributeNameVector;
	}

	static RTI::AttributeHandleSet &getSubscribedAttributeHandleSet_var( void ) {
		static RTI::AttributeHandleSet *subscribedAttributeHandleSet = RTI::AttributeHandleSetFactory::create( 0 );
		return *subscribedAttributeHandleSet;
	}
	static StringVector &getSubscribeAttributeNameVector( void ) {
		static StringVector subscribeAttributeNameVector;
		return subscribeAttributeNameVector;
	}

private:
	static ObjectRoot::SP factory( void ) {
		return ObjectRoot::SP( new ObjectRoot() );
	}

	static std::string &getInitErrorMessage( void ) {
		static std::string initErrorMessage( "Error:  ObjectRoot:  could not initialize:  " );
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
		static std::string publishErrorMessage = "Error:  ObjectRoot:  could not publish:  ";
		return publishErrorMessage;
	}

public:
	static void publish( RTI::RTIambassador *rti );

private:
	static std::string &getUnpublishErrorMessage( void ) {
		static std::string unpublishErrorMessage = "Error:	ObjectRoot:	 could not unpublish:  ";
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
		static std::string subscribedErrorMessage = "Error:	 ObjectRoot:  could not subscribe:	";
		return subscribedErrorMessage;
	}

public:
	static void subscribe( RTI::RTIambassador *rti );

private:
	static std::string getUnsubscribeErrorMessage( void ) {
		static std::string unsubscribeErrorMessage = "Error:  ObjectRoot:  could not unsubscribe:  ";
		return unsubscribeErrorMessage;
	}

public:
	static void unsubscribe( RTI::RTIambassador *rti );

	static bool static_init( void );

	static bool match( int handle ) { return handle == get_handle(); }

	virtual int getClassHandle( void ) const { return get_handle(); }
	virtual std::string getClassName( void ) const { return get_class_name(); }
	virtual StringVector getAttributeNames( void ) const { return get_attribute_names(); }
	virtual StringVector getAllAttributeNames( void ) const { return get_all_attribute_names(); }

	virtual std::string getAttributeName( int datamemberHandle ) const {
		return std::string();
	}

	int getAttributeHandle( const std::string &datamemberName ) {
		return get_attribute_handle(getClassName(), datamemberName);
	}

	virtual void publishObject( RTI::RTIambassador *rti ) { publish( rti ); }
	virtual void subscribeObject( RTI::RTIambassador *rti ) { subscribe( rti ); }

	virtual ObjectRoot::SP clone( void ) {
		return ObjectRoot::SP(	new ObjectRoot( *this )	 );
	}

	virtual const RTI::AttributeHandleSet &getSubscribedAttributeHandleSet( void ) { return getSubscribedAttributeHandleSet_var(); }

public:
	static StringSet get_object_names( void ) { return getClassNameSet(); }
	static StringVector get_attribute_names( const std::string &className ) {
		NameStringVectorPtrMap::iterator nssItr = getDatamemberClassNameVectorPtrMap().find( className );
		return nssItr == getDatamemberClassNameVectorPtrMap().end() ? StringVector() : *nssItr->second;
	}

	static StringVector get_all_attribute_names( const std::string &className ) {
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

	static std::string get_attribute_name( int datamemberHandle ) {
		IntegerStringMap::iterator ismItr = getDatamemberHandleNameMap().find( datamemberHandle );
		return ismItr == getDatamemberHandleNameMap().end() ? std::string() : ismItr->second;
	}

	static int get_attribute_handle( const std::string &className, const std::string &datamemberName ) {
		StringIntegerMap::iterator simItr = getDatamemberNameHandleMap().find( className + "." + datamemberName );
		if ( simItr == getDatamemberNameHandleMap().end() ) {
			std::cerr << "Bad attribute \"" << datamemberName << "\" for class \"" << className << "\" on get_attribute_handle." << std::endl;
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

	static void publish( const std::string &className, const std::string &attributeName );

	static void unpublish( const std::string &className, const std::string &attributeName ) {
		NameStringVectorPtrMap::iterator camItr = getClassNamePublishAttributeNameVectorPtrMap().find( className );
		if ( camItr == getClassNamePublishAttributeNameVectorPtrMap().end() ) {
			std::cerr << "ERROR:  ObjectRoot.unpublish:	 could not unpublish class \"" << className << "\" \"" << attributeName << "\" attribute." << std::endl;
			return;
		}
		camItr->second->erase( std::remove( camItr->second->begin(), camItr->second->end(), attributeName ), camItr->second->end() );
	}
	static void subscribe( const std::string &className, const std::string &attributeName );

	static void unsubscribe( const std::string &className, const std::string &attributeName ) {
		NameStringVectorPtrMap::iterator camItr = getClassNameSubscribeAttributeNameVectorPtrMap().find( className );
		if ( camItr == getClassNameSubscribeAttributeNameVectorPtrMap().end() ) {
			std::cerr << "ERROR:  ObjectRoot.unsubscribe:  could not unsubscribe class \"" << className << "\" \"" << attributeName << "\" attribute." << std::endl;
			return;
		}
		camItr->second->erase( std::remove( camItr->second->begin(), camItr->second->end(), attributeName ), camItr->second->end() );
	}

public:
	static SP create_object( const std::string &className ) {
		ClassNameFactoryMap::iterator cfmItr = getClassNameFactoryMap().find( className );
		return cfmItr == getClassNameFactoryMap().end() ? SP( (ObjectRoot *)0 ) : (*cfmItr->second)();
	}

	static SP create_object( const std::string &className, const RTIfedTime &dlc13FedTime ) {
		ClassNameFactoryMap::iterator cfmItr = getClassNameFactoryMap().find( className );
		if ( cfmItr == getClassNameFactoryMap().end() ) {
			return SP( (ObjectRoot *)0 );
		}

		SP sp = (*cfmItr->second)();
		sp->setTime( dlc13FedTime.getTime() );
		return sp;
	}

	static SP create_object( int classHandle ) {
		IntegerStringMap::iterator ismItr = getClassHandleNameMap().find( classHandle );
		return ismItr == getClassHandleNameMap().end() ? SP( (ObjectRoot *)0 ) : create_object( ismItr->second );
	}

	static SP create_object( int classHandle, const RTIfedTime &dlc13FedTime ) {
		IntegerStringMap::iterator ismItr = getClassHandleNameMap().find( classHandle );
		return ismItr == getClassHandleNameMap().end() ? SP( (ObjectRoot *)0 ) : create_object( ismItr->second, dlc13FedTime );
	}

	static SP create_object( int classHandle, const RTI::AttributeHandleValuePairSet &datamemberMap );
	static SP create_object( int classHandle, const RTI::AttributeHandleValuePairSet &datamemberMap, const RTIfedTime &dlc13FedTime );

	static StringVector get_attribute_names() {
		return StringVector();
	}

	static StringVector get_all_attribute_names() {
		return StringVector();
	}

	static SP discover( int class_handle, int object_handle ) {

		SP sp = create_object( class_handle );
		if ( sp != 0 ) {
			getObjectMap().insert(	std::make_pair( object_handle, sp )	 );
		}
		return sp;
	}

	static SP reflect( int object_handle, const RTI::AttributeHandleValuePairSet &datamemberMap ) {
		ObjectMap::iterator obmItr = getObjectMap().find( object_handle );
		if ( obmItr == getObjectMap().end() ) {
			return SP( (ObjectRoot *)0 );
		}

		SP sp = obmItr->second;
		sp->setTime( -1 );
		sp->setAttributes( datamemberMap );
		return sp;
	}

	static SP reflect(
	 int object_handle,
	 const RTI::AttributeHandleValuePairSet &datamemberMap,
	 double theTime
	) {
		ObjectMap::iterator obmItr = getObjectMap().find( object_handle );
		if ( obmItr == getObjectMap().end() ) {
			return SP( (ObjectRoot *)0 );
		}

		SP sp = obmItr->second;
		sp->setTime( theTime );
		sp->setAttributes( datamemberMap );
		return sp;
	}

	void requestUpdate( RTI::RTIambassador *rti );

	static SP getObject( int object_handle ) {
		ObjectMap::iterator obmItr = getObjectMap().find( object_handle );
		return obmItr == getObjectMap().end() ? SP( (ObjectRoot *)0 ) : obmItr->second;
	}

	static SP removeObject( int object_handle ) {
		ObjectMap::iterator obmItr = getObjectMap().find( object_handle );
		if ( obmItr == getObjectMap().end() ) {
			return SP( (ObjectRoot *)0 );
		}

		SP sp = obmItr->second;
		getObjectMap().erase( obmItr );
		return sp;
	}

private:
	int _object_handle;

	void setObjectHandle( int object_handle ) {
		getObjectMap().erase( object_handle );
		_object_handle = object_handle;
		getObjectMap().insert(	std::make_pair( object_handle, this )  );
	}

public:
	int getObjectHandle( void ) const { return _object_handle; }

private:
	double _time;
	static C2WLogger* _logger;

public:
	double getTime( void ) { return _time; }

	void setTime( double time ) { _time = time; }
	void setTime( const RTIfedTime &dlc13FedTime ) {
		setTime( dlc13FedTime.getTime() );
	}

	ObjectRoot( void ) : _time( -1 ), _isRegistered( false ) { }

	ObjectRoot( const RTI::AttributeHandleValuePairSet &datamemberMap ) : _time( -1 ), _isRegistered( false ) {
		setAttributes( datamemberMap );
	}

	ObjectRoot( const RTI::AttributeHandleValuePairSet &datamemberMap, const RTIfedTime &dlc13FedTime ) : _time( dlc13FedTime.getTime() ), _isRegistered( false ) {
		setAttributes( datamemberMap );
	}

	static SP create( void ) { return SP( new ObjectRoot ); }
	static SP create( const RTI::AttributeHandleValuePairSet &datamemberMap ) { return SP(	new ObjectRoot( datamemberMap )	 ); }
	static SP create( const RTI::AttributeHandleValuePairSet &datamemberMap, const RTIfedTime &dlc13FedTime ) {
		return SP(	new ObjectRoot( datamemberMap, dlc13FedTime )  );
	}


	virtual TypeMedley getAttribute( const std::string &datamemberName ) const {
		throw std::invalid_argument( getClassName() + " class has no attribute named \"" + datamemberName + "\"" );
	}

	virtual TypeMedley getAttribute( int datamemberHandle ) const {
		const std::string &datamemberName = getAttributeName(datamemberHandle);
		if (datamemberName.empty()) {
			throw std::invalid_argument(
				getClassName() + " class has no attribute with handle (" + boost::lexical_cast< std::string >( datamemberHandle ) + ")"
			);
		}
		return getAttribute(datamemberName);
	}

	void setAttributes( const RTI::AttributeHandleValuePairSet &datamemberMap );

private:
	void setAttribute( RTI::Handle handle, const std::string &val ) {
		if (  !setAttributeAux( handle, val )  ) {
			std::cerr << "set:	bad attribute handle in class \"" + getClassName() + "\"" << std::endl;
		}
	}

public:
	void setAttribute( const std::string &datamemberName, TypeMedley value ) {
		if (  !setAttributeAux( datamemberName, value )	 ) {
			std::cerr << "Error:  class \"" << getClassName() << "\":  invalid attribute \"" + datamemberName + "\"" << std::endl;
		}
	}

private:
	virtual bool setAttributeAux( int datamemberHandle, const std::string &val ) {
		const std::string &datamemberName = getAttributeName(datamemberHandle);
		if (datamemberName.empty()) {
			return false;
		}
		return setAttributeAux(datamemberName, val);
	}

protected:
	virtual bool setAttributeAux( const std::string &datamemberName, const std::string &value ) {
		return false;
	}

	virtual bool setAttributeAux( const std::string &datamemberName, TypeMedley value ) {
		return false;
	}

	virtual AttributeHandleValuePairSetSP createDatamemberHandleValuePairSet( RTI::ULong count, bool force ) {
		return AttributeHandleValuePairSetSP(  RTI::AttributeSetFactory::create( count )  );
	}

	virtual AttributeHandleValuePairSetSP createDatamemberHandleValuePairSet( bool force ) {
		return createDatamemberHandleValuePairSet( force );
	}

private:
	bool _isRegistered;

public:
	void registerObject( RTI::RTIambassador *rti );
	void registerObject( RTI::RTIambassador *rti, const std::string &name ) throw ( RTI::ObjectAlreadyRegistered );
	void unregisterObject( RTI::RTIambassador *rti );

	void updateAttributeValues( RTI::RTIambassador *rti, double time, bool force );

	void updateAttributeValues( RTI::RTIambassador *rti, double time ) {
		updateAttributeValues( rti, time, false );
	}

	void updateAttributeValues( RTI::RTIambassador *rti, bool force );

	void updateAttributeValues( RTI::RTIambassador *rti ) {
		updateAttributeValues( rti, false );
	}

private:
	static std::string fedName;

public:
	static void enablePublishLog(const std::string &object, const std::string &attribute, const std::string &fed, const std::string &thislevel, const std::string &globallevel);
	static void enableSubscribeLog(const std::string &object, const std::string &attribute, const std::string &fed, const std::string &thislevel, const std::string &globallevel);

	static std::map<std::string, std::string> &getPubAttributeLogMap( void) {
		static std::map<std::string, std::string> pubAttributeLogMap;
		return pubAttributeLogMap;
	}

	static std::map<std::string, std::string> &getSubAttributeLogMap( void) {
		static std::map<std::string, std::string> subAttributeLogMap;
		return subAttributeLogMap;
	}

	void createLog( double time, bool isPub = true );
};

typedef ObjectRoot::SP ObjectRootSP;

static bool call_ObjectRoot_static_init = ObjectRoot::static_init();

std::ostream &operator<<( std::ostream &os, ObjectRoot::SP entitySP );
std::ostream &operator<<( std::ostream &os, const ObjectRoot &entity );

#endif
