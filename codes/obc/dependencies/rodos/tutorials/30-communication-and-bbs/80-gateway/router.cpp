
#include "rodos.h"
#include "gateway.h"
#include "demo_topics.h"

extern Gateway gateway1;
extern Gateway gateway2;

Router router(false, &gateway1, &gateway2);

#ifdef USE_UART_CONN
static HAL_UART uart(UART_IDX0);
static LinkinterfaceUART linkinterfaceUart(&uart);
static Gateway gatewayUart(&linkinterfaceUart, false,false);
#endif


class GatewayInitiator : public Initiator {
    void init() {
        //router.addGateway(&gateway1);
        //router.addGateway(&gateway2);

#ifdef USE_UART_CONN
        gatewayUart.resetTopicsToForward();
        gatewayUart.addTopicsToForward(&counter1,&counter2);

        router.addGateway(&gatewayUart);
#endif

    }
} gatewayInitiator;



