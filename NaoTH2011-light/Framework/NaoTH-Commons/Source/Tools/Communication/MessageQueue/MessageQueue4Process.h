/**
 * @file MessageQueue4Process.h
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu, Yuan</a>
 * 
 * A message queue for communicating between processes
 */
 
#ifndef _MESSAGE_QUEUE_4_PROCESS_H_
#define _MESSAGE_QUEUE_4_PROCESS_H_

#include "MessageQueue.h"
#include <glib.h>
#include <gio/gio.h>
#include "Tools/Communication/SocketStream/SocketStream.h"

// This file has been touched by the documentation effort

/**
* @brief This class manages socket streaming and message sending.
*
* ToDo: So many things can go wrong it's not even funny. No status of the sockets is ever checked.
*/
class MessageQueue4Process: public MessageQueue
{
public:
/**
* @brief This constructor creates a socket address in /tmp/ with the specified name.
*
* @param name The name of the new socket address.
*/
  MessageQueue4Process(const std::string& name);

/**
* @brief Basic destructor. Clears all held sockets.
*
* This destructor does not clear the socket. Hopefully since it is in /tmp/ 
* it will be cleared by the OS and not clutter stuff. Hopefully.
*/
  virtual ~MessageQueue4Process();
  
/**
* @brief This function initializes the writeSocket and sends a message to it.
*
* This function returns with nothing done if the connection failed.
*
* @param msg The message to send.
*/
  virtual void write(const std::string& msg);
  
/**
* @brief This function tries to read incoming messages and stores them in the queue.
*
* @return True if no messages are in the queue, false otherwise.
*/
  virtual bool empty();

/**
* @brief This function sets a reader for the queue.
*
* This function sets up a new reader socket attached to
* addr, and sets up theReadStream to read from it.
*
* @param reader A MessageReader.
*/
  virtual void setReader(MessageReader* reader);

/**
* @brief This function sets a writer for the queue.
*
* This function sets up a server socket and binds it
* to addr, so that we can listen to incoming requests.
*
* @param writer
*/
  virtual void setWriter(MessageWriter* writer);

protected:
/**
* @brief This function connects the readSocket to the address and links theReadStream to it.
*/
  void connect();

/**
* @brief THIS FUNCTION IS NOT DEFINED. Probably defined not used because it exists in the PrefixedSocketStream.
*
*/
  bool isFixedLengthDataAvailable(unsigned int len);
  
private:
/**
* @brief Name of the socket address.
*/
  std::string theName;
/**
* @brief Pointer to the socket address.
*/
  GSocketAddress* addr;

/**
* @brief Server socket. Listens for incoming connections.
*/
  GSocket* serverSocket;

/**
* @brief Reader socket. Reads data from the connection.
*/
  GSocket* readSocket;
/**
* @brief Writer socket. Gets a connection from the server and writes to it.
*/
  GSocket* writeSocket;
/**
* @brief The stream used to read data from the connection.
*/
  PrefixedSocketStream theReadStream;
/**
* @brief The stream used to write data to the connection.
*/
  PrefixedSocketStream theWriteStream;
};

#endif // _MESSAGE_QUEUE_4_PROCESS_H_
