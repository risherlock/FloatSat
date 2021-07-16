/*
 *
 * @file router.h
 *  Created on: 27.03.2013
 *      Author: Dilger
 */


#pragma once

#include "gateway/gateway.h"
#include "putter.h"
#include "subscriber.h"
#include "topic.h"


namespace RODOS {

constexpr uint8_t MAX_NUMBER_OF_GATEWAYS_PER_ROUTER = 8;

class Router : public Subscriber,Putter  {
    Subscriber localTopics;
    NetworkMessage localMessage;

protected:
    Gateway* gateways[MAX_NUMBER_OF_GATEWAYS_PER_ROUTER];
    uint8_t numberOfGateways;
    bool forwardTopicReports;

public:
    Router(bool forwardTopicReports_ = false, Gateway* gateway1=0, Gateway* gateway2=0, Gateway* gateway3=0, Gateway* gateway4=0); 
    virtual ~Router() {}

    /*
     * Networkmessages from all Gateway come in here
     */
    uint32_t put(const uint32_t topicId, const size_t len, void* data, const NetMsgInfo& netMsgInfo) override;

    /*
     * Local Topics to send out come in here
     */
    bool putGeneric(const uint32_t topicId, const size_t len, const void* msg, const NetMsgInfo& netMsgInfo) override;


    /**
     * Gets called for every packet that enters this Router from local or from network.
     * Default implementation make routing descision with below functions for every Gateway added to this router
     */
    virtual void routeMsg(NetworkMessage &msg, uint32_t linkid);


    /*
     * Should this Message be routet in general?
     *
     */
    virtual bool shouldRouteThisMsg(NetworkMessage &msg, uint32_t linkid);

    /**
     * Shoould this message be routet to this Gateway?
     */
    virtual bool shouldRouteThisMsgToGateway(NetworkMessage &msg, uint32_t linkid,Gateway* gateway);

    virtual void addGateway(Gateway* gateway);
};


}

