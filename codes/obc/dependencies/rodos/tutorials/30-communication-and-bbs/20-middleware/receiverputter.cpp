#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("ReciverPutter", 1100);

class JustPrint : public Putter {

	/// Implements the complete generic interface of putter
        bool putGeneric(const uint32_t topicId, const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
		PRINTF(NB "%lu" MD "%ld" TID "%lu\n",
			static_cast<unsigned long>(msgLen),
			*static_cast<const long*>(msg),
			static_cast<unsigned long>(topicId));
		return true;
	}
} justPrint;

/** Two subscriber doint the same.... */
static Subscriber nameNotImportant01(counter1, justPrint, "justprint01");
static Subscriber nameNotImportant02(counter2, justPrint, "justprint02");
//static Subscriber nameNotImportant03(position, justPrint, "justprint03");
