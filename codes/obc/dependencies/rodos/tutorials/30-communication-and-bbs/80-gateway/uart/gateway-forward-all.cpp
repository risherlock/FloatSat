
#include "rodos.h"
#include "demo_topics.h"

static HAL_UART uart0(UART_IDX0);
static int init_dummy = uart0.init(115200);

static LinkinterfaceUART linkinterfaceUART0(&uart0);
static Gateway gateway3(&linkinterfaceUART0, true);

