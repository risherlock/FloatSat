// The Publisher-Thread post every one second an ascending counter value,
// while the Subscriber-Thread simply displays the received integer value.

#include <rodos.h>

Topic<long> counter1(-1, "counter1");

class MyPublisher : public StaticThread<>
{
public:
    MyPublisher() : StaticThread("SenderSimple") {}
    void run()
    {
        long cnt = 0;
        TIME_LOOP(1 * SECONDS, 1 * SECONDS)
        {
            PRINTF("Published: %ld\n", ++cnt);
            counter1.publish(cnt);
        }
    }
} publisher;

class MySubscriber : public SubscriberReceiver<long>
{
public:
    MySubscriber() : SubscriberReceiver<long>(counter1) {}
    void put(long &data)
    {
        PRINTF("Received: %ld\n\n", data);
    }
} subscriber;