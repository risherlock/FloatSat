
#pragma once

struct Position  { double x, y, z; };

/// Warning this is not really safe, no atomarity between reader and writer
template <class Type>
class SubsCopyToObj : public SubscriberReceiver<Type> {
public:
    Type* target;
    SubsCopyToObj(Type* tp, TopicInterface &topic, const char *name) : SubscriberReceiver<Type>(topic, name) { target = tp; }
    void put(Type &data) { PRIORITY_CEILER_IN_SCOPE(); *target = data; }
};

extern Topic<Position>  position;


