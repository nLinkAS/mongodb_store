#ifndef __MONGODB_STORE_UTIL_H
#define __MONGODB_STORE_UTIL_H

#include <ros/ros.h>


//include to get BSON. There's probably a much smaller of set of headers we could get away with
// #include "mongo/client/dbclient.h"
#include "mongoDriver/BSONObjBuilder.h"
#include "mongoDriver/BSONArrayBuilder.h"


/*
if ctime is not None:


needs to be in native datetime format for language
    meta['inserted_at'] = datetime.utcfromtimestamp(ctime)
OR
    meta['inserted_at'] = datetime.utcfromtimestamp(rospy.get_rostime().to_sec())

could allow addition meta if desirable
doc["_meta"]=meta

    #  also store type information

add stored class:
    doc["_meta"]["stored_class"] = msg.__module__ + "." + msg.__class__.__name__
	e.g. "nav_msgs.msg._OccupancyGrid.OccupancyGrid"

and type:
    doc["_meta"]["stored_type"] = msg._type
	e.g. "nav_msgs/OccupancyGrid"

*/

namespace mongodb_store {

template<typename MsgType> 
void add_meta_for_msg(const typename MsgType::ConstPtr & _msg,  orion::BSONObjBuilder & _builder) {

  	orion::BSONObjBuilder meta;

  	ros::Time now = ros::Time::now();  	
  	orion::BSONDate nowDate((now.sec * 1000.0) + (now.nsec / 1000000.0));	
	meta.appendDate("inserted_at", nowDate);

  	std::string type(ros::message_traits::DataType<MsgType>::value());
	meta.append("stored_type", type);

	size_t slashIndex = type.find('/');
	std::string package = type.substr(0, slashIndex);
	std::string name = type.substr(slashIndex + 1, type.length() - slashIndex - 1);
	std::string pythonName = package + ".msg._" + name + "." + name;
	meta.append("stored_class", pythonName);

	_builder.append("_meta", meta.obj());

}


// template <class T>
// inline BSONObjBuilder& BSONObjBuilder::append(const StringData& fieldName,
//                                               const std::vector<T>& vals) {
//     BSONObjBuilder arrBuilder;
//     for (unsigned int i = 0; i < vals.size(); ++i)
//         arrBuilder.append(numStr(i), vals[i]);
//     appendArray(fieldName, arrBuilder.done());
//     return *this;
// }

void appendVector(orion::BSONObjBuilder& builder, const std::string& fieldName,
                                              const std::vector<orion::BSONObj>& vals) {
    orion::BSONArrayBuilder arrBuilder;
    for (unsigned int i = 0; i < vals.size(); ++i)
        arrBuilder.append(vals[i]);
	const orion::BSONArray array = arrBuilder.arr();
    builder.append(fieldName, array);
}

}

#endif