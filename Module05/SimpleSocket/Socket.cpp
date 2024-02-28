//
// File:     Socket.cpp
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Implementation file for the main portion of our C++ Sockets interface.  This
// code is based on code from an 2002 article in Linux Gazette.
//
#include "Socket.h"
#include "string.h"
#include <string.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
//
// Socket::Socket()
// Construct a new instance of our class.
// Pre-condition:
// NONE.
//
// Post-condition:
// A cleared space for the address instance variable has been created.
//
Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );

}
//
// Socket::~Socket()
// Destory an existing instance of our class.
// Pre-condition:
// A valide socket is available
//
// Post-condition:
// The socket is closed as needed.
//
Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}
//
// Socket::create()
// Create a new socket and associate it with our instance.
// Pre-condition:
// Instance has been created.
//
// Post-condition:
// A reference to a valid socket can be found in our private variable.
//
bool Socket::create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );

  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;
  return true;

}
//
// Socket::bind()
// Create a new socket and associate it with our instance.
// Pre-condition:
// We have a valid socket
//
// Post-condition:
// That socket is bound to the IP and port.
//
bool Socket::bind ( const int port )
{

  if ( ! is_valid() )
    {
      return false;
    }
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );
  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );
  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}
//
// Socket::listen()
// Create a new socket and associate it with our instance.
// Pre-condition:
// We have a valid socket
//
// Post-condition:
// We are listening on a valid socket.
//
bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}
//
// Socket::accept()
// Accpet a connection on this socket.
// Pre-condition:
// A valid and correct socket is passed to us for connection purposes
//
// Post-condition:
// We are accepting connections on that socket
//
bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}
//
// Socket::send(const std::string)
// Send data out over our socket.
// Pre-condition:
// A valid and correct socket is passed to us for connection purposes
// Data is passed to us in the method parameter.
//
// Post-condition:
// The data gets sent over the socket.
//
bool Socket::send ( const std::string s ) const
{
  int status = ::send( m_sock, s.c_str(), s.size(), 0);
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}
//
// Socket::recv(std::string&)
// Get data from our socket.
// Pre-condition:
// A valid and correct socket is passed to us for connection purposes
// We have a valid string to put data into
//
// Post-condition:
// The data gets read from the socket.
//
int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}
//
// Socket::connect(const std::string, const int)
// Connect a socket to a host and port
// Pre-condition:
// A valid and correct socket is passed to us for connection purposes
// We have a valid host and port
//
// Post-condition:
// Our socket is connected to the host and port.
//
bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}
//
// Socket::set_non_blocking(bool)
// The socket state is set to non-blocking.
// Pre-condition:
// A valid and correct socket is passed to us for connection purposes
//
// Post-condition:
// The state of the socket has been changed.
//
void Socket::set_non_blocking ( const bool b )
{
  int opts;
  opts = fcntl ( m_sock,
		 F_GETFL );
  if ( opts < 0 )
    {
      return;
    }
  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );
  fcntl ( m_sock,
	  F_SETFL,opts );
}
