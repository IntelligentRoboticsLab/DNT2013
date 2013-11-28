/**
 * @file MessageQueue.h
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu, Yuan</a>
 * 
 * A message queue for communicating
 */

#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include <string>
#include <queue>

// This file has been touched by the documentation effort

class MessageReader;
class MessageWriter;

/**
 * @brief This class represents a message queue.
 *
 * This class contains a message queue. Each message is
 * represented by a std::string.
 *
 * It is important to note that instances of this class
 * cannot be used directly, as their methods are protected.
 * 
 * \sa MessageReader and MessageWriter.
 */
class MessageQueue
{
    public:
        friend class MessageReader;
        friend class MessageWriter;

        MessageQueue();

        virtual ~MessageQueue();
    protected:
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

        /**
         * @brief This function checks whether an existing reader exists, and sets it otherwise.
         *
         * @param reader A MessageReader.
         */
        virtual void setReader(MessageReader* reader);

        /**
         * @brief This function checks whether an existing writer exists, and sets it otherwise.
         *
         * @param writer A MessageWriter.
         */
        virtual void setWriter(MessageWriter* writer);

    protected:
        std::queue<std::string> theMsg;

        MessageReader* theReader;
        MessageWriter* theWriter;
};

/**
 * @brief This class defines a read-only interface over a MessageQueue.
 */
class MessageReader
{
    public:
        /**
         * @brief This constructor sets the MessageQueue referenced by the class.
         *
         * @param msgQueue The MessageQueue referenced by the class.
         */
        MessageReader(MessageQueue* msgQueue);

        /**
         * @brief This destructor DOES NOT delete the referenced MessageQueue.
         */
        virtual ~MessageReader();

        /**
         * @brief This function reads a message from the referenced MessageQueue.
         *
         * @return A string containing the message.
         */
        std::string read() { return theMsgQueue->read(); }

        /**
         * @brief This function checks whether the referenced MessageQueue is empty().
         *
         * @return True if the referenced MessageQueue is empty, false otherwise.
         */
        bool empty() const { return theMsgQueue->empty(); }

    private:
        MessageQueue* theMsgQueue;
};

/**
 * @brief This class defines a write-only interface over a MessageQueue.
 */
class MessageWriter
{
    public:
        /**
         * @brief This constructor sets the MessageQueue referenced by the class.
         *
         * @param msgQueue The MessageQueue referenced by the class.
         */
        MessageWriter(MessageQueue* msgQueue);

        /**
         * @brief This destructor DOES NOT delete the referenced MessageQueue.
         */
        virtual ~MessageWriter();

        /**
         * @brief This function inserts a message into the referenced MessageQueue.
         *
         * @param msg The message to be inserted.
         */
        void write(const std::string& msg) { theMsgQueue->write(msg); }

    private:
        MessageQueue* theMsgQueue;
};

#endif
