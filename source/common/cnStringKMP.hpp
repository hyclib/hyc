//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc
// author:       lipengxiang
// description:  
//

#ifndef __BP_CNSTRINGKMP_HPP_20140609112930__
#define __BP_CNSTRINGKMP_HPP_20140609112930__

namespace hyc
{
    inline u32 KMP(const char *A, const char *B)  
    {
        // make b struct.
        int i, j = 0;
        int len2 = (int)strlen(B);
        int* P = (int*)MemoryAlloc::Malloc(len2*sizeof(int));
        P[0] = 0;  
        P[1] = 0;
        for(i=2; i<len2; ++i)  
        {  
            while(j>0 && B[j]!=B[i-1])
                j = P[j];                  
            if(B[j] == B[i-1])    
                ++j;  
            P[i] = j;   
        }

        // find.
        int len1 = (int)strlen(A);
        j = 0;                 //j代表目前B串中已与A串匹配了的字符的个数.  
        for(i=0; i<len1; ++i)  
        {  
            while(j>0 && A[i]!=B[j])    //0...j-1,已匹配了j个字符,sub[j]是sub的第j+1的字符,因为下标从0开始.
                j = P[j];  

            if(A[i] == B[j])   
                ++j;  
            if(j == len2)              //当j(B中已匹配了的字符串的个数)与B串本身长度相等时,说明匹配完毕.
            {
                MemoryAlloc::Free(P);
                return i-j+1;        //此时可计算出指针位置.
            }
        }

        MemoryAlloc::Free(P);
        return -1;  
    }

    inline u32 KMPr(const char *A, const char *B, int offset)  
    {
        // make b struct.
        int len2 = (int)strlen(B);
        int i, j = len2-1;
        int* P = (int*)MemoryAlloc::Malloc(len2*sizeof(int));
        P[len2-1] = j;  
        P[len2-2] = j;
        for(i=len2-3; i>=0; --i)  
        {  
            while(j<len2-1 && B[j]!=B[i+1])
                j = P[j];
            if(B[j] == B[i+1])    
                --j;
            P[i] = j;   
        }

        // find.
        int len1 = offset+1;  
        j = len2-1;                 //j代表目前B串中已与A串匹配了的字符的个数.  
        for(i=len1-1; i>=0; --i)  
        {  
            while(j<len2-1 && A[i]!=B[j])    //0...j-1,已匹配了j个字符,sub[j]是sub的第j+1的字符,因为下标从0开始.
                j = P[j];  

            if(A[i] == B[j])   
                --j;  
            if(j == -1)              //当j(B中已匹配了的字符串的个数)与B串本身长度相等时,说明匹配完毕.
            {
                MemoryAlloc::Free(P);
                return i+len2-1;        //此时可计算出指针位置.
            }
        }

        MemoryAlloc::Free(P);
        return -1;  
    }

    inline u32 KMPw(const wchar_t *A, const wchar_t *B)  
    {
        // make b struct.
        int i, j = 0, len2 = (int)wcslen(B);
        int* P = (int*)MemoryAlloc::Malloc(len2*sizeof(int));
        P[0] = 0;  
        P[1] = 0;
        for(i=2; i<len2; ++i)  
        {  
            while(j>0 && B[j]!=B[i-1])
                j = P[j];                  
            if(B[j] == B[i-1])    
                ++j;  
            P[i] = j;   
        }

        // find.
        int len1 = (int)wcslen(A);
        j = 0;                 //j代表目前B串中已与A串匹配了的字符的个数.  
        for(i=0; i<len1; ++i)  
        {  
            while(j>0 && A[i]!=B[j])    //0...j-1,已匹配了j个字符,sub[j]是sub的第j+1的字符,因为下标从0开始.
                j = P[j];  

            if(A[i] == B[j])   
                ++j;  
            if(j == len2)              //当j(B中已匹配了的字符串的个数)与B串本身长度相等时,说明匹配完毕.
            {
                MemoryAlloc::Free(P);
                return i-j+1;        //此时可计算出指针位置.
            }
        }

        MemoryAlloc::Free(P);
        return -1;  
    }

    inline u32 KMPwr(const wchar_t *A, const wchar_t *B, int offset)  
    {
        // make b struct.
        int len2 = (int)wcslen(B);  
        int i, j = len2-1;
        int* P = (int*)MemoryAlloc::Malloc(len2*sizeof(int));
        P[len2-1] = j;  
        P[len2-2] = j;
        for(i=len2-3; i>=0; --i)  
        {  
            while(j<len2-1 && B[j]!=B[i+1])
                j = P[j];
            if(B[j] == B[i+1])    
                --j;
            P[i] = j;   
        }

        // find.
        int len1 = offset+1;  
        j = len2-1;                 //j代表目前B串中已与A串匹配了的字符的个数.  
        for(i=len1-1; i>=0; --i)  
        {  
            while(j<len2-1 && A[i]!=B[j])    //0...j-1,已匹配了j个字符,sub[j]是sub的第j+1的字符,因为下标从0开始.
                j = P[j];  

            if(A[i] == B[j])   
                --j;  
            if(j == -1)              //当j(B中已匹配了的字符串的个数)与B串本身长度相等时,说明匹配完毕.
            {
                MemoryAlloc::Free(P);
                return i+len2-1;        //此时可计算出指针位置.
            }
        }

        MemoryAlloc::Free(P);
        return -1;  
    }


} // end namespace hyc

#endif // end of define __BP_CNSTRINGKMP_HPP_20140609112930__