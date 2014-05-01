#ifndef XK_TYPES_H
#define XK_TYPES_H

namespace Xk {

typedef struct _MSG_PACKET{
    int     nID;
    void*   pBuf;
    int     nLength;
    unsigned long long guid;
}MSG_PACKET;

//��140��ʼ��159�������ڲ��õ�19����ϢID
#define XK_INTERNAL_MSGID_BEGIN 140
//��160��ʼ�������û��Զ���ʹ��
#define XK_USER_MSGID_BEGIN 160

enum{
    MSGID_INTER_LOG = XK_INTERNAL_MSGID_BEGIN,
    MSGID_INTER_GC
};

}//namespace Xk

#endif