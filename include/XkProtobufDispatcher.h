#ifndef XK_PROTOBUF_DISPATCHER_H
#define XK_PROTOBUF_DISPATCHER_H

#include <iostream>
#include <functional>
#include <map>
#include "XkNonCopyAble.h"
#include <google/protobuf/message.h>
#include <Poco/SharedPtr.h>
#include <XkDebugStr.h>

namespace Xk {

class Callback : NonCopyAble
{
public:
    virtual ~Callback() {}
    virtual void onMessage(Poco::UInt64 guid, google::protobuf::Message* message) const = 0;
};

template <typename T>
class CallbackT : public Callback
{
public:
    typedef std::function<void (Poco::UInt64 guid, T* message)> ProtobufMessageCallback;

    CallbackT(const ProtobufMessageCallback& callback)
        : m_callback(callback)
    {
    }

    ~CallbackT()
    {
        std::cout<<"~CallbackT"<<std::endl;
    }

    virtual void onMessage(Poco::UInt64 guid, google::protobuf::Message* message) const
    {
        T* t = dynamic_cast<T*>(message);
        assert(t != NULL);
        m_callback(guid, t);
    }

private:
    ProtobufMessageCallback m_callback;
};

namespace {
    void discardProtobufMessage(Poco::UInt64 guid, google::protobuf::Message* message)
    {
        (guid);
        (message);
        XkDebugString("DiscardMessage!!! guid: %u, message: %s", guid, message->GetTypeName().c_str());
        //没有处理的消息，打印后释放内存
        delete message;
    }
}

class ProtobufDispatcher
{
public:
    ProtobufDispatcher()
        : m_defaultCallback(discardProtobufMessage)
    {
        
    }

    void onMessage(Poco::UInt64 guid, google::protobuf::Message* message) const
    {
        CallbackMap::const_iterator it = m_map.find(message->GetDescriptor());
        if(it != m_map.end()) {
            it->second->onMessage(guid, message);
        }else {
            m_defaultCallback(guid, message);
        }
    }

    template<typename T>
    void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageCallback& callback)
    {
        Poco::SharedPtr<CallbackT<T> > pd(new CallbackT<T>(callback));
        m_map[T::descriptor()] = pd;
    }

    typedef std::map<const google::protobuf::Descriptor*, Poco::SharedPtr<Callback> > CallbackMap;

    CallbackMap m_map;

    std::function<void (Poco::UInt64 guid, google::protobuf::Message* message)> m_defaultCallback;
};

}//namespace Xk

#define XKBIND2(func, object) std::bind(&func, &object, std::placeholders::_1, std::placeholders::_2)

#endif