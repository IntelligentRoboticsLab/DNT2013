/**
 * @file SocketStream.h
 *
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu Yuan</a>
 * @brief Encapsulate the socket I/O as a stream
 * 
 */

#ifndef SOCKET_STREAM_H
#define SOCKET_STREAM_H

#include <gio/gio.h>

#include <errno.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stdexcept>

// This file has been touched by the documentation effort

const int default_recv_buffer_size = 1024;

/**
* @brief This class tries to behave as stream over a socket connection.
*/
class SocketStream
{
public:

/**
* @brief Default constructor, initializes socket buffer.
*/
  SocketStream();

/**
* @brief Default destructors, clears socket and socket buffer.
*/
  ~SocketStream();

/**
* @brief This function initializes the internal socket.
*
* In addition it increases the reference count to it. I assume
* the socket library has some sort of shared pointer stuff going
* on which must be manually managed. This function takes care of that.
*
* @param s The socket that has to be managed.
*/
  void init(GSocket* s);

/**
* @brief This function sends a message through the socket.
*
* This function assumes that the socket has been set through
* the init function. If not, this function does nothing.
*
* @param msg The message to send through the socket.
*
* @throw std::runtime_error
*/
  void send(const std::string& msg) throw(std::runtime_error);

/**
* @brief This function sends the currently stored stream message through the socket.
*
* @return The SocketStream instance.
*/
  SocketStream& send();

/**
* @brief This function receives messages from the socket.
*
* This function assumes that the socket has been set through
* the init function. If not, this function returns -1.
*
* @param msg The message received.
*
* @return An error code.
*
* @throw std::runtime_error
*/
  int recv(std::string& msg) throw(std::runtime_error);

/**
* @brief This function inserts data in the internal stream.
*
* @tparam T The type of the data to insert.
* @param msg The data to insert.
*
* @return The SocketStream instance.
*/
  template <class T>
  SocketStream & operator <<(const T& msg)
  {
    mSendStr << msg;
    return *this;
  }

/**
* @brief This function applies a manipulator on the SocketStream.
*
* For more information see documentation of the same operator for std::stringstream.
*
* @param pf The manipulator that needs to be applied on the stream.
*
* @return The SocketStream instance.
*/
  SocketStream & operator <<(SocketStream& (*pf) (SocketStream&))
  {
    return pf(*this);
  }

/**
* @brief This function extracts a message from the socket.
*
* @param msg The message to extract.
*
* @return The SocketStream instance.
*/
  SocketStream & operator >>(std::string& msg)
  {
    recv(msg);
    return *this;
  }

protected:

/**
* @brief This function sends the currently stored stream message through the socket, prefixed by its length.
*/
  void prefixedSend();

/**
* @brief This function reads the socket connection and buffers at least len bytes of data.
*
* @param len The length of the data requested.
*
* @return True if the data is available, false otherwise.
*
* @throw std::runtime_error
*/
  bool isFixedLengthDataAvailable(unsigned int len) throw(std::runtime_error);

/**
* @brief This function reads a prefixed message from the socket.
*
* @param msg The message received.
*
* @return 0 if reading failed, the length of the message otherwise.
*/
  int prefixedRecv(std::string& msg);

/**
* @brief This function reallocates the buffer to be size long.
*
* ToDo: THIS FUNCTION CAN BUFFER OVERFLOW!!
*
* @param size The new size of the buffer.
*/
  void reallocRecvBuffer(unsigned int size);

/**
* @brief Adds a message to the stringstream.
*
* This function exists only to help extend children.
*
* @tparam T The type of the data.
* @param msg The message to insert.
*/
  template <class T>
  void addSendMsg(const T& msg)
  {
    mSendStr << msg;
  }



private:
/**
* @brief Stringstream to provide a stream interface.
*/
  std::stringstream mSendStr;
/**
* @brief Buffer for incoming messages.
*/
  char* mRecvBuf;
/**
* @brief Size of incoming message buffer.
*/
  unsigned int mRecvBufSize;
/**
* @brief Size of available incoming buffer, received from isFixedLengthDataAvailable().
*
* This value gets automatically reduced after a prefixedRecv() call.
*/
  unsigned int mRecvdLen;

/**
* @brief Pointer to socket object for communication.
*/
  GSocket* socket;
};


/**
* @brief This function provides a global way to use send.
*
* This is good for a number of reasons, mostly because its
* extendable to classes with a different interface, so we 
* can normalize usage.
*
* @tparam T Any type that supports sending.
* @param o The object that needs to send.
*
* @return The same object.
*/
template<class T>
T& send(T& o)
{
  return o.send();
}




















/**
* @brief This class reads and writes from a socket using only prefixed messages.
*/
class PrefixedSocketStream : public SocketStream
{
public:

/**
* @brief Basic constructor.
*/
  PrefixedSocketStream()
  {
  }

/**
* @brief Basic destructor.
*/
  ~PrefixedSocketStream()
  {
  }

/**
* @brief This function sends the currently stored stream message through the socket, prefixed by its length.
*
* @return The PrefixedSocketStream instance.
*/
  PrefixedSocketStream& send()
  {
    prefixedSend();
    return *this;
  }

/**
* @brief This function inserts data in the internal stream.
*
* @tparam T The type of the data to insert.
* @param msg The data to insert.
*
* @return The PrefixedSocketStream instance.
*/
  template <class T>
  PrefixedSocketStream & operator <<(const T& msg)
  {
    addSendMsg(msg);
    return *this;
  }

/**
* @brief This function applies a manipulator on the PrefixedSocketStream.
*
* For more information see documentation of the same operator for std::stringstream.
*
* @param pf The manipulator that needs to be applied on the stream.
*
* @return The PrefixedSocketStream instance.
*/
  PrefixedSocketStream & operator <<(PrefixedSocketStream& (*pf) (PrefixedSocketStream&))
  {
    return pf(*this);
  }

/**
* @brief This function reads a prefixed message from the socket.
*
* @param msg The message received.
*
* @return The PrefixedSocketStream instance.
*/
  PrefixedSocketStream & operator >>(std::string& msg)
  {
    prefixedRecv(msg);
    return *this;
  }
};

#endif /* SOCKET_STREAM_HPP */
