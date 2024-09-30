#include "rodos.h"

/* ~~~~~ Topic definition ~~~~~ */

struct data_sensor
{
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t z = 0;
};

Topic<data_sensor> topic_sensor(-1, "topic_sensor");

/* ~~~~~ Transmitter thread ~~~~~ */

class transmitter_thread : public StaticThread<>
{
private:
  int period = 5000; // ms
  data_sensor data_tx;

public:
  transmitter_thread(const char *thread_name, const int priority) : StaticThread(thread_name, priority) {}

  void init() {}

  void run()
  {
    TIME_LOOP(0, period * MILLISECONDS)
    {
      data_tx.x += 1;
      data_tx.y += 2;
      data_tx.z += 3;
      topic_sensor.publish(data_tx);
      PRINTF("Transmitter: %d, %d, %d\n", data_tx.x, data_tx.y, data_tx.z);
    }
  }

} transmitter_thread_test("transmitter_thread", 100);

/* ~~~~~ Receiver thread ~~~~~ */

CommBuffer<data_sensor> cb_bob;
Subscriber name_not_important(topic_sensor, cb_bob);

class receiver_thread : public StaticThread<>
{
private:
  int period = 1000; // ms
  data_sensor data_rx;

public:
  receiver_thread(const char *thread_name, const int priority) : StaticThread(thread_name, priority) {}

  void init() {}

  void run()
  {
    TIME_LOOP(0, period * MILLISECONDS)
    {
      cb_bob.getOnlyIfNewData(data_rx);
      PRINTF("Receiver: %d, %d, %d\n", data_rx.x, data_rx.y, data_rx.z);
    }
  }

} receiver_thread_test("receiver_thread", 200);
