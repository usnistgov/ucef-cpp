/*
 * C2WException.hpp
 *
 *  Created on: Jul 17, 2011
 *      Author: c2wt
 */

#ifndef C2WEXCEPTION_HPP_
#define C2WEXCEPTION_HPP_

#include <string>
#include <typeinfo>

class C2WException : public std::exception {
private:
	std::string _message;

public:
	C2WException( const std::string &message ) : _message( message ) { }
	virtual ~C2WException( void ) throw() { }

	virtual const char *what() const throw() {
		return _message.c_str();
	}
};

#endif /* C2WEXCEPTION_HPP_ */
