//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc library
// author:  lipengxiang
// crate at: 2014-04-22
// description: 
//

#ifndef ALLOC_HPP_2014_4_221
#define ALLOC_HPP_2014_4_221

#include "../../../container/memoryAlloc.h"

namespace hyc
{
namespace concurrent
{
  /**
   * allocate the memory.
   */
  template< typename T >
  class Allocator
  {
  public:
    static T*     allocate(size_t n)           { return (T*)hyc::MemoryAlloc::Malloc(n*sizeof(T)); }
	static void   deallocate(void* p, size_t)  { hyc::MemoryAlloc::Free(p); }
  };

} // end namespace concurrent
} // end namespace hyc

#endif
