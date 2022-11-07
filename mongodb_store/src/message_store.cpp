#include "mongodb_store/message_store.h"

namespace mongodb_store {

	const orion::BSONObj MessageStoreProxy::EMPTY_BSON_OBJ =  orion::BSONObj();

	mongodb_store_msgs::StringPair makePair(const std::string & _first, const std::string & _second) {
	    mongodb_store_msgs::StringPair pair;
	    pair.first = _first;
	    pair.second = _second;
	    return pair;
	}

}