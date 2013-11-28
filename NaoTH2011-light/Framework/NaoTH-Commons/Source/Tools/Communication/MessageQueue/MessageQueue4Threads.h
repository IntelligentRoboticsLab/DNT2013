/**
 * @file MessageQueue4Threads.h
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu, Yuan</a>
 * 
 * A message queue for communicating between threads
 */

#ifndef _MESSAGE_QUEUE_4_THREADS_H_
#define _MESSAGE_QUEUE_4_THREADS_H_

#include "MessageQueue.h"
#include <glib.h>

// This file has been touched by the documentation effort

/**
 * @brief This class extends MessageQueue in a thread-safe way.
 */
class MessageQueue4Threads: public MessageQueue
{
    public:
        /**
         * @brief Basic constructor.
         */
        MessageQueue4Threads();
        /**
         * @brief Basic destructor.
         */
        virtual ~MessageQueue4Threads();

        /**
         * @brief This function inserts a message in the queue.
         *
         * @param msg The message to be inserted.
         */
        virtual void write(const std::string& msg);

        /**
         * @brief This function checks whether the queue is empty.
         *
         * @return True if the queue is empty, false otherwise.
         */
        virtual bool empty();

        /**
         * @brief This function returns the top message in the queue, and removes it from the queue.
         *
         * @return The top message in the queue.
         */
        virtual std::string read();

        /**
         * @brief This function clears the queue and deletes any currently hold messages.
         */
        virtual void clear();

    private:
        GMutex* theMutex;
};

#endif // _MESSAGE_QUEUE_4_THREADS_H_

