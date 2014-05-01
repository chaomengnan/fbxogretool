#ifndef XK_GC_H
#define XK_GC_H

#include "XkConfig.h"
#include <typeinfo>

namespace GC {

#define XK_GC_THIS __FILE__, __LINE__
#define XK_GC_FUNC __FUNCTION__

#if XK_DEBUG_MEMORY == 1
void gc_debugger_push_ptr(void* ptr, unsigned int size, const char* type, const char* file, unsigned int line);
void gc_debugger_pop_ptr(void* ptr, const char* file, unsigned int line);
#endif

template<typename T, typename AX> inline T* safe_malloc(unsigned int bytes, const char* file, unsigned line)
{
	T* mem = (T*)AX::allocateBytes(bytes);
#if XK_DEBUG_MEMORY == 1
    gc_debugger_push_ptr(mem, bytes, typeid(T).name(), file, line);
#endif
	return mem;
}

template<typename T, typename AX> inline T* safe_malloc(unsigned int bytes, const char* type, const char* file, unsigned line)
{
	T* mem = (T*) AX::allocateBytes(bytes);
#if XK_DEBUG_MEMORY == 1
    gc_debugger_push_ptr(mem, bytes, type, file, line);
#endif
	return mem;
}

template<typename T, typename AX> inline void safe_free(T* mem, const char* file, unsigned line)
{
	if(mem == 0)
		return;
#if XK_DEBUG_MEMORY == 1
    gc_debugger_pop_ptr(mem, file, line);
#endif
	AX::deallocateBytes(mem);
}

template<typename T> inline T* safe_new0(const char* file, int line)
{
	T* ptr = new T();
#if XK_DEBUG_MEMORY == 1
    gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
	return ptr;
}

template<typename T, typename AX> inline T* safe_allocated_new0(const char* file, int line)
{
	void* mem = AX::allocateBytes(sizeof(T));
	T* ptr = new(mem) T();
#if XK_DEBUG_MEMORY == 1
    gc_debugger_push_ptr(mem, sizeof(T), typeid(T).name(), file, line);
#endif
	return ptr;
}

template<typename T, typename A1> inline T* safe_new1(const A1& v1, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1);
#if XK_DEBUG_MEMORY == 1
    gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1> inline T* safe_allocated_new1(const A1& v1, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1);
#if XK_DEBUG_MEMORY == 1
    gc_debugger_push_ptr(mem, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2> inline T* safe_new2(const A1& v1, const A2& v2, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2> inline T* safe_allocated_new2(const A1& v1, const A2& v2, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(mem, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2, typename A3> inline T* safe_new3(const A1& v1, const A2& v2, const A3& v3, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2, v3);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2, typename A3> inline T* safe_allocated_new3(const A1& v1, const A2& v2, const A3& v3, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2, v3);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2, typename A3, typename A4> inline T* safe_new4(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2, v3, v4);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2, typename A3, typename A4> inline T* safe_allocated_new4(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2, v3, v4);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5> inline T* safe_new5(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2, v3, v4, v5);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2, typename A3, typename A4, typename A5> inline T* safe_allocated_new5(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2, v3, v4, v5);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline T* safe_new6(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const A6& v6, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2, v3, v4, v5, v6);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline T* safe_allocated_new6(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const A6& v6, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2, v3, v4, v5, v6);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7> inline T* safe_new7(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const A6& v6, const A7& v7, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2, v3, v4, v5, v6, v7);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7> inline T* safe_allocated_new7(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const A6& v6, const A7& v7, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2, v3, v4, v5, v6, v7);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>  inline T* safe_new8(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const A6& v6, const A7& v7, const A8& v8, const char* file = 0, int line = 0)
{
 T* ptr = new T(v1, v2, v3, v4, v5, v6, v7, v8);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T, typename AX, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>  inline T* safe_allocated_new8(const A1& v1, const A2& v2, const A3& v3, const A4& v4, const A5& v5, const A6& v6, const A7& v7, const A8& v8, const char* file = 0, int line = 0)
{
 void* mem = AX::allocateBytes(sizeof(T));
 T* ptr = new(mem) T(v1, v2, v3, v4, v5, v6, v7, v8);
#if XK_DEBUG_MEMORY == 1
 gc_debugger_push_ptr(ptr, sizeof(T), typeid(T).name(), file, line);
#endif
 return ptr;
};

template<typename T> void inline safe_delete(T* ptr, const char* file, int line)
{
 if (ptr == 0)
  return;
#if XK_DEBUG_MEMORY == 1
 gc_debugger_pop_ptr(ptr, file, line);
#endif
 delete ptr;
}

template<typename T, typename AX> void inline safe_allocated_delete(T* ptr, const char* file, int line)
{
 if (ptr == 0)
  return;
 ptr->~T();
#if XK_DEBUG_MEMORY == 1
 gc_debugger_pop_ptr(ptr, file, line);
#endif
 AX::deallocateBytes(ptr);
}

};//namespace GC

#endif
