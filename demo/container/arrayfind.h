
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_ARRAYFIND_H_20130917105221__
#define __BP_ARRAYFIND_H_20130917105221__


inline int array_sort(const void* e1, const void* e2)
{
    return *(s32*)e1 > *(s32*)e2 ? 1 : -1;
}

inline int array_find_q_compare( const void* ap, const void* bp ) {
    const s32* a = (s32*)ap;
    const s32* b = (s32*)bp;
    if (*a == *b)       return 0;
    else if (*a < *b)   return 1;
    else                return -1;
}

void container_arrayfind()
{
    ArrayFind<s32>  a;
    a.sort_insert(3, array_sort);
    a.sort_insert(2, array_sort);
    a.sort_insert(2, array_sort);
    a.sort_insert(4, array_sort);
    a.sort_insert(1, array_sort);
    a.sort_insert(5, array_sort);

    hy_assertTrue(a.size() == 6);
    
    s32 e = s32_INFINITY;
    for (u32 i = 0; i < a.size(); i++)
    {
        hy_assertTrue(a[i] <= e);
        e = a[i];
    }

    hy_assertTrue(a.find_q(2, array_find_q_compare) >= 0);
}

#endif // end of define __BP_ARRAYFIND_H_20130917105221__
