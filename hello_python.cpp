#include "rodos.h"
#include "gateway.h"

/* ~~~~~ Topic definition ~~~~~ */

const uint32_t topic_id_python_to_rodos = 1002;
const uint32_t topic_id_rodos_to_python = 1003;

// Python Middleware to STM32
struct data_sensor
{
  char name[20];
  int32_t idx;
  double x, y, z;
};

// STM32 to Python Middleware
struct __attribute__((packed)) data_time
{
  int64_t time_sec;
  int32_t msg_idx;
};

Topic<data_sensor> topic_sensor(topic_id_python_to_rodos, "topic_sensor");
Topic<data_time> topic_time(topic_id_rodos_to_python, "topic_time");

/* ~~~~~ Set UART as gateway ~~~~~ */

static HAL_UART bluetooth(UART_IDX2); // Tx: PD5, Rx: PD6
static LinkinterfaceUART link_name_not_imp(&bluetooth, 115200, 3, 10);
static Gateway gw_name_not_imp(&link_name_not_imp, true);

/* ~~~~~ Transmitter thread ~~~~~ */

class transmitter : public StaticThread<>
{
private:
  data_time my_time;
  int32_t count = 0;

public:
  transmitter() : StaticThread("STM32 transmitter") {}

  void init()
  {
    bluetooth.init(115200);
  }

  void run()
  {
    TIME_LOOP(0, 2000 * MILLISECONDS)
    {
      // Publish count and time
      my_time.msg_idx = count++;
      my_time.time_sec = NOW() / SECONDS;
      topic_time.publish(my_time);

      // Print in console
      MW_PRINTF("RODOS sends index: %ld and time (s): %lld with regards!\n", my_time.msg_idx, my_time.time_sec);
    }
  }

} transmitter_thread;

/* ~~~~~ Receiver thread ~~~~~ */

struct receiver : public Subscriber
{
  receiver() : Subscriber(topic_sensor, "STM32 receiver") {}

  uint32_t put(const uint32_t topic_id, const size_t len, void *msg, const NetMsgInfo &) override
  {
    data_sensor *sensor = (data_sensor *)msg;

    // Print the data received from Python Middleware`
    PRINTF("Python sends ID: %d, Name: %s, Index: %d, X: %f, Y: %f, Z: %f\n",
           (int)topic_id, sensor->name, (int)(sensor->idx), sensor->x, sensor->y, sensor->z);

    return true;
  }
} receiver_thread;
