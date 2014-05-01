#ifndef XK_NONCOPYABLE_H
#define XK_NONCOPYABLE_H

namespace Xk {

class NonCopyAble
{
protected:
    NonCopyAble() {}
    ~NonCopyAble() {}

private:
    NonCopyAble(const NonCopyAble& );
    const NonCopyAble& operator = (const NonCopyAble& );
};

}//namespace Xk

#endif