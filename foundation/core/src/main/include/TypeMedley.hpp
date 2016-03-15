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
 */

#ifndef _TYPEMEDLEY
#define _TYPEMEDLEY

#include <typeinfo>
#include <ctype.h>

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

class TMBase {
public:
	typedef boost::shared_ptr< TMBase > SP;

protected:
	template< typename TOTYPE, typename FROMTYPE, typename T = void >
	class Converter {
	public:
		typedef TOTYPE ToType;
		typedef FROMTYPE FromType;

		static ToType get( const FromType &from ) {
			return boost::lexical_cast< ToType >( from );
		}
	};

	template< typename T >
	class Converter< std::string, bool, T > {
	public:
		static std::string get( const bool &from ) {
			return from ? "true" : "false";
		}
	};

	template< typename T >
	class Converter< bool, std::string, T > {
	public:
		static bool get( const std::string &from ) {
			if ( from.empty() ) {
				return false;
			}

			std::string modFrom(   ::boost::to_lower_copy(  ::boost::trim_copy( from )  )   );
			if ( modFrom == "false" ) {
				return false;
			}

			std::string::size_type pos = modFrom.find_first_not_of( "0" );
			if ( pos == std::string::npos ) {
				return true;
			}
			if ( modFrom[ pos ] != '.' ) {
				return false;
			}

			pos = modFrom.find_first_not_of( "0", pos + 1 );
			return pos == std::string::npos;
		}
	};

public:

#define CONVERSION_OPERATOR( x ) \
	virtual operator x( void ) const = 0;

	CONVERSION_OPERATOR( bool )
	CONVERSION_OPERATOR( char )
	CONVERSION_OPERATOR( short )
	CONVERSION_OPERATOR( int )
	CONVERSION_OPERATOR( long )
	CONVERSION_OPERATOR( float )
	CONVERSION_OPERATOR( double )
	CONVERSION_OPERATOR( std::string )

#undef CONVERSION_OPERATOR

	virtual std::string getTypeName( void ) const = 0;

};

template< typename TYPE >
class TMTemplate : public TMBase {
public:
	typedef TYPE Type;
	typedef TMBase Super;

private:
	const Type _value;

public:
	TMTemplate( Type value ) : _value( value ) { }

#define CONVERSION_OPERATOR( x ) \
	virtual operator x( void ) const {\
		return Super::Converter< x, Type >::get( _value );\
	}

	CONVERSION_OPERATOR( bool )
	CONVERSION_OPERATOR( char )
	CONVERSION_OPERATOR( short )
	CONVERSION_OPERATOR( int )
	CONVERSION_OPERATOR( long )
	CONVERSION_OPERATOR( float )
	CONVERSION_OPERATOR( double )
	CONVERSION_OPERATOR( std::string )

#undef CONVERSION_OPERATOR

#define TYPE_NAME( x ) \
	if (  typeid( x ) == typeid( Type )  ) {\
		return #x;\
	}

	virtual std::string getTypeName( void ) const {
		TYPE_NAME( bool )
		TYPE_NAME( char )
		TYPE_NAME( short )
		TYPE_NAME( int )
		TYPE_NAME( long )
		TYPE_NAME( float )
		TYPE_NAME( double )
		TYPE_NAME( std::string )
		return "unknown";
	}

#undef TYPE_NAME
};

class TypeMedley {
private:
	TMBase::SP _tmBaseSP;

public:
	template< typename TYPE >
	TypeMedley( const TYPE &value ) : _tmBaseSP(  new TMTemplate< TYPE >( value )  ) { }

#define CONVERSION_OPERATOR( x ) \
	virtual operator x( void ) const {\
		return static_cast< x >( *_tmBaseSP );\
	}

	CONVERSION_OPERATOR( bool )
	CONVERSION_OPERATOR( char )
	CONVERSION_OPERATOR( short )
	CONVERSION_OPERATOR( int )
	CONVERSION_OPERATOR( long )
	CONVERSION_OPERATOR( float )
	CONVERSION_OPERATOR( double )
	CONVERSION_OPERATOR( std::string )

#undef CONVERSION_OPERATOR

	std::string getTypeName( void ) {
		return _tmBaseSP->getTypeName();
	}
};

#endif
