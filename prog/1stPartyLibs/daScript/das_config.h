#pragma once
#include <EASTL/sort.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/set.h>
#include <EASTL/map.h>
#include <EASTL/unordered_set.h>
#include <EASTL/unordered_map.h>
#include <EASTL/string.h>
#include <EASTL/deque.h>
#include <EASTL/memory.h>
#include <EASTL/type_traits.h>
#include <EASTL/initializer_list.h>
#include <EASTL/functional.h>
#include <EASTL/algorithm.h>
#include <dag/dag_relocatable.h>
#include <generic/dag_smallTab.h>
#include <debug/dag_assert.h>
#include <debug/dag_fatal.h>
#include <debug/dag_log.h>
#include <cstddef>
#include <stddef.h>
#include <setjmp.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iosfwd>
#ifdef _MSC_VER
#include <malloc.h>
#endif

namespace das
{
template <typename T, typename Allocator = EASTLAllocatorType, bool init_constructing = dag::is_type_init_constructing<T>::value,
  typename Counter = uint32_t>
using vector = dag::Vector<T, Allocator, init_constructing, Counter>;
using namespace eastl;
using eastl::deque;
using eastl::max;
using eastl::min;
using std::atomic;
using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::nullptr_t;
using std::recursive_mutex;
using std::stringstream;
using std::thread;
using std::unique_lock;
namespace this_thread
{
using std::this_thread::yield;
}
namespace chrono
{
using std::chrono::milliseconds;
}
} // namespace das
#define DAS_USE_EASTL 1

#define DAS_GLOBAL_NEW 1

#define DAS_BIND_EXTERNAL 0

#define DAS_SMART_PTR_TRACKER 0

#define DAS_NO_GLOBAL_NEW_AND_DELETE 1

#define DAS_FAST_INTEGER_MOD 0 // fast-math does not work with integer mod

#define DAS_ASSERT    G_ASSERT
#define DAS_ASSERTF   G_ASSERTF
#define DAS_FATAL_LOG logerr
#define DAS_FATAL_ERROR(...) \
  {                          \
    DAG_FATAL(__VA_ARGS__);  \
  }
#if !_TARGET_PC
#define DAS_NO_FILEIO 1
#endif

#include <vecmath/dag_vecMathDecl.h>

#define DAS_ALIGNED_ALLOC 1
inline void *das_aligned_alloc16(uint32_t size) { return new char[size]; }
inline void das_aligned_free16(void *ptr) { delete[] (char *)ptr; }
inline size_t das_aligned_memsize(void *ptr) { return defaultmem->getSize(ptr); }

// if enabled, the generated interop will be marginally slower
// the upside is that it well generate significantly less templated code, thus reducing compile time (and binary size)
#ifndef DAS_SLOW_CALL_INTEROP
#define DAS_SLOW_CALL_INTEROP 0
#endif

#ifndef DAS_FUSION
#if _TARGET_64BIT && DAGOR_DBGLEVEL == 0
#define DAS_FUSION 0 // we switch off fusion engine, as in release we will rely on AOT. So that's basically optimizes executable size
#else
#define DAS_FUSION 0
#endif
#endif

#if DAS_SLOW_CALL_INTEROP
#undef DAS_FUSION
#define DAS_FUSION 0
#endif

#ifndef DAS_DEBUGGER
#if DAGOR_DBGLEVEL > 0
#define DAS_DEBUGGER 1
#endif
#endif

#ifndef DAS_MAX_FUNCTION_ARGUMENTS
#define DAS_MAX_FUNCTION_ARGUMENTS 64
#endif

#ifndef DAS_PRINT_VEC_SEPARATROR
#define DAS_PRINT_VEC_SEPARATROR ", "
#endif

#include <ska_hash_map/flat_hash_map2.hpp>
namespace das
{
template <typename K, typename V, typename H = das::hash<K>, typename E = das::equal_to<K>>
using das_map = ska::flat_hash_map<K, V, H, E>;
template <typename K, typename H = das::hash<K>, typename E = das::equal_to<K>>
using das_set = ska::flat_hash_set<K, H, E>;
template <typename K, typename V, typename H = das::hash<K>, typename E = das::equal_to<K>>
using das_hash_map = ska::flat_hash_map<K, V, H, E>;
template <typename K, typename H = das::hash<K>, typename E = das::equal_to<K>>
using das_hash_set = ska::flat_hash_set<K, H, E>;
template <typename K, typename V>
using das_safe_map = eastl::map<K, V>;
template <typename K, typename C = das::less<K>>
using das_safe_set = eastl::set<K, C>;
} // namespace das


#ifndef DAS_DEFAULT_STDOUT

#ifndef das_to_stdout_level_prefix_text
#define das_to_stdout_level_prefix_text(level, prefix, text)        \
  {                                                                 \
    (void)prefix;                                                   \
    logmessage_(level >= das::LogLevel::error     ? LOGLEVEL_ERR    \
                : level >= das::LogLevel::warning ? LOGLEVEL_WARN   \
                                                  : LOGLEVEL_DEBUG, \
      "%s", text);                                                  \
  }
#endif

#else // DAS_DEFAULT_STDOUT

#ifndef das_to_stdout_level_prefix_text
#define das_to_stdout_level_prefix_text(level, prefix, text) \
  {                                                          \
    if (level >= LogLevel::error)                            \
    {                                                        \
      fprintf(stderr, "%s", text);                           \
      fflush(stderr);                                        \
    }                                                        \
    else                                                     \
    {                                                        \
      fprintf(stdout, "%s%s", prefix, text);                 \
      fflush(stdout);                                        \
    }                                                        \
  }
#endif

#endif // DAS_DEFAULT_STDOUT