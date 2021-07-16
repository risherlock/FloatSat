
/**
* @file subscriber.h
* @date 2008/09/01 7:07
* @author Sergio Montenegro
*
*
* @brief subscribers for middleware
*
*/

#pragma once

#include <stdint.h>

#include "rodos-debug.h"
#include "rodos-semaphore.h"
#include "topic.h"


namespace RODOS {

class Putter;

/**
 * Data structure containing information about topic messages sent via network (gateway).
 */
class NetMsgInfo {
  public:
    int32_t  senderNode;     ///< Node ID of sending instance of RODOS
    int64_t  sentTime;       ///< Time in localTime units
    uint32_t senderThreadId; ///< The ID of the sending thread
    uint32_t linkId;         ///< The ID of the %Linkinterface from which the message was received.
};



/**
* @class Subscriber
* @brief Subscriber to receive topic messages
*
*  The middleware communication is based on the publisher/subscriber protocol.
*  Publishers make messages public under a given topic.
*  Subscribers to a given topic get all messages which are published
*  under the given topic.
*  To establish a transfer path, both:
*  publisher and subscriber have to share the same topic.
*
* Each subscriber has a reference to the associated topic and a putter
* to store messages.
* All subscribers are registered in a list.
* Each time a messages is published the list of all subscriber will be search and
* for each subscriber where the topic matches the associated putter will be called
* to store a copy of the published message.
*/
class Subscriber : public ListElement {

    friend class TopicInterface;
    friend class TopicReporter;
    friend void initSystem();

public:
    /// Reference to a topic to which the subscriber listens.
    TopicInterface& topicInterface;

    /// Reference to the associated handler of a message received.
    Putter* receiver;

protected:
    /// Flag to indicate if the subscriber broadcasts a message internally or externally
    bool isAGateway;
    /// Flag to activate or deactivate the subscriber
    bool isEnabled;
    /// List of all subscribers in the system
    static List subscriberList;

    Semaphore protector;
    // DEPRECATED! DO not use anymore!
    //virtual long put(const long topicId, const long len, const void* data, long linkId);

    /// Default function: forward the message and invoke the receiver (putter). It locks the semaphore protector
    virtual uint32_t put(const uint32_t topicId, const size_t len, void* data, const NetMsgInfo& netMsgInfo);

    /// do not lock any semaphore. Do not call any thread function
    /// default function resumes the associated thread (if defined) if it is waiting for it
    virtual void putFromInterrupt(const uint32_t topicId, const void* any, size_t len = 0);


public:

    /** At creation time a connection to the corresponding topic
     * and putter (receiver) has to be done.
     * Subscriber my be "Normal" message receiver or may be Gateways.
     * Gateways are interfaces to other memory spaces and nodes, e.g.
     * going through a network. Gateways just forward messages to other nodes.
     *
     * The parameter name is only used for debug purposes, to see which subscriber is
     * attached to which topic. Using names you can recognise present subscribers in listings.
     *
     * @param topic reference to the topic to which the owner will be subscribed
     * @param putter reference to a putter receiving messages from the topic subscribed
     * @param name name is used only for debug purposes, sing it one can recognise Threads.
     * @param isAGatewayParam : gateways get from all topics and they decide to forward or not
     */
    Subscriber(TopicInterface &topic, Putter& receiver, const char* name = "anonymSubscriber", bool isAGatewayParam=false);

    /**
      * Simple subscriber has no associated receiver (nor thread nor putter), they just (have to)
      * redefine the put method
      */
    Subscriber(TopicInterface &topic, const char* name = "anonymThreadSubscriber");

    /// Destructor, should not be called in nominal operation.
    virtual ~Subscriber() {
        if(isShuttingDown) return;
        RODOS_ERROR("Subscriber deleted");
    }

    /** returns true if InputPort is enabled and wants to receive messages for
    *   given topicId
    */
    // virtual bool wants(const long topicId) const;

    /**
     * @return enabled == true -> receive all messages to the given ServiceId,
    *    else ignore them
    */
    void enable(bool onOff = true);

    /** Check whether the current subscriber is a gateway,
     * hence connects to the network.
     * @return if true -> message is sent to the network, false -> is send locally
     */
    bool isGateway() const;

};


/**
 * Simple Subscriber interface for users.
 * @tparam Type The data type of the topic message that shall be received by the %SubscriberReceiver.
 */
template <class Type>
class SubscriberReceiver : public Subscriber {
    void (*receiverFunc)(Type &msg); ///< a function to be called for each message (optional)

public:
    /**
     * This constructor is usually used when developers inherit from this class in order to implement message handling
     * in the overridden put() method.
     * @param topic A reference to the topic that shall be subscribed.
     * @param name The name of the subscriber.
     */
    SubscriberReceiver(TopicInterface &topic, const char* name = "anonymSubscriber") :
        Subscriber(topic, name) {
        receiverFunc = 0;
    }

    /**
     * This constructor is usually used when developers just create an instance in order to call the passed message
     * handling function.
     * @param topic A reference to the topic that shall be subscribed.
     * @param funcPtr The pointer to the function that is called every time a topic message arrives.
     * @param name The name of the subscriber.
     */
    SubscriberReceiver(TopicInterface &topic,  void (*funcPtr)(Type&), const char* name = "anonymSubscriber") :
        Subscriber(topic, name) {
        receiverFunc = funcPtr;
    }

    /**
     * This method is called every time a topic message arrives.
     * @param msg The message that was published to the topic.
     */
    virtual void put(Type &msg) {
        if(receiverFunc) (*receiverFunc)(msg);
    }

    /**
     * This method is called every time a topic message arrives.
     * @param msg The message that was published to the topic.
     * @param netMsgInfo Meta information about the message (path).
     */
    virtual void put(Type &msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        put(msg);
    }

    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, const NetMsgInfo& netMsgInfo) override {
        put(*(Type*)data,netMsgInfo);
        return 1;
    }
};


/** a filter which may modify the content of published messages
 ** WARNING: Please be very careful when using filters
 ** do not abuse it. It is intended for voters, Kalman filter, etc
 **/
class TopicFilter {
public:
    virtual ~TopicFilter() = default;
    /// will  be called before all subscribers
    virtual void prolog ([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, [[gnu::unused]] void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) { }
    /// will  be called after all subscribers
    virtual void epilog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, [[gnu::unused]] void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) { }
};



}  // namespace

