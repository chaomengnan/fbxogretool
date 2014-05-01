#ifndef XK_TYPES_H
#define XK_TYPES_H

namespace Xk {

typedef struct _MSG_PACKET{
    int     nID;
    void*   pBuf;
    int     nLength;
    unsigned long long guid;
}MSG_PACKET;

//从140开始到159，这是内部用的19个消息ID
#define XK_INTERNAL_MSGID_BEGIN 140
//从160开始往后，由用户自定义使用
#define XK_USER_MSGID_BEGIN 160

enum{
    MSGID_INTER_LOG = XK_INTERNAL_MSGID_BEGIN,
    MSGID_INTER_GC
};

}//namespace Xk

#endif