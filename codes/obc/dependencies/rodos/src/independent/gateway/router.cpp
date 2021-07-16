
/**
 * @file router.cpp
 * @date 2013/03/27 17:46
 * @author Erik Dilger, Uni W�rzburg
 *
 *
 * @brief gateway for middleware
 *
 */

#include "gateway/router.h"
#include "rodos-debug.h"

namespace RODOS {

constexpr uint8_t not0(const void* a) { return (a) ? 1 : 0; }

Router::Router(bool forwardTopicReports_, Gateway* gateway1, Gateway* gateway2, Gateway* gateway3, Gateway* gateway4) :
    Subscriber(defaultRouterTopic,"Router"),
    localTopics(defaultGatewayTopic,*this,"Router") {

    forwardTopicReports=forwardTopicReports_;
    gateways[0]      = gateway1;
    gateways[1]      = gateway2;
    gateways[2]      = gateway3;
    gateways[3]      = gateway4;
    numberOfGateways   = static_cast<uint8_t>(
      not0(gateway1) + not0(gateway2) + not0(gateway3) + not0(gateway4));
}


uint32_t Router::put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, const NetMsgInfo& netMsgInfo) {
    routeMsg(*((NetworkMessage*)data),netMsgInfo.linkId);
    return 1;
}

bool Router::putGeneric(const uint32_t topicId, const size_t len,
                        const void* msg, const NetMsgInfo&) {

    protector.enter();
    prepareNetworkMessage(localMessage,topicId,msg,len);
    routeMsg(localMessage,RODOS_LOCAL_BROADCAST);
    protector.leave();

    return true;
}

void Router::routeMsg(NetworkMessage& msg,uint32_t linkid) {
    if(shouldRouteThisMsg(msg,linkid)) {
        msg.setCheckSum();

        for(uint8_t i=0; i<numberOfGateways; i++) {
            if(shouldRouteThisMsgToGateway(msg,linkid,gateways[i])) {
                gateways[i]->sendNetworkMessage(msg);
            }
        }
    }
}

bool Router::shouldRouteThisMsg(NetworkMessage& msg, [[gnu::unused]] uint32_t linkid) {
    if(msg.get_maxStepsToForward() <= 0)               return false;
    if(msg.get_topicId() == 0 && !forwardTopicReports) return false;
    return true;
}

bool Router::shouldRouteThisMsgToGateway(NetworkMessage& msg,uint32_t linkid, Gateway* gateway) {
    if(gateway->getLinkIdentifier()==linkid) return false;
    return gateway->shouldTopicForwarded(msg.get_topicId());
}

void Router::addGateway(Gateway* gateway) {
    gateways[numberOfGateways]= gateway;
    numberOfGateways++;

    static_assert(MAX_NUMBER_OF_GATEWAYS_PER_ROUTER < UINT8_MAX, "potential overflow in numberOfGatways!");
    if(numberOfGateways > MAX_NUMBER_OF_GATEWAYS_PER_ROUTER) {
        RODOS_ERROR("Too many Gateway added to a router\n");
        numberOfGateways--;
    }
}

}
