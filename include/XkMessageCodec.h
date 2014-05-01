#ifndef XK_MESSAGE_CODEC_H
#define XK_MESSAGE_CODEC_H

#include <google/protobuf/message.h>

namespace Xk {

class MessageCodec
{
public:
    static google::protobuf::Message* createMessage(const std::string& type_name);
    static std::string encode(const google::protobuf::Message& message);
    static google::protobuf::Message* decode(const std::string& buf);
    static std::string getTypeName(void* buf, unsigned int size);
};

}//namespace Xk

#endif