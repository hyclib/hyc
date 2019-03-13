//
//  identifierUtils.hpp
//  hyc
//
//  Created by brainpoint on 14-3-22.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#ifndef HEADER_IDENTIFIERUTiLS_HPP_df211124535
#define HEADER_IDENTIFIERUTiLS_HPP_df211124535

#if CN_PLATFORM == CN_PLATFORM_LINUX
#include <uuid/uuid.h>

namespace hyc
{
    //-----------------------------------------------
    String IdentifierUtils::generateUUID()
    {
    	uuid_t a_id;
    	uuid_generate(a_id);

    	char uuidString[36 + 1];
    	uuid_unparse(a_id, uuidString);
    	uuidString[36] = '\0';

    	return uuidString;
    }
}

#else

//
// Initialize the pid.
//
class __PidInitializer
{
public:
    __PidInitializer()
    {
        pid_t p = getpid();
        myPid[0] = (p >> 8) & 0x7F;
        myPid[1] = p & 0xFF;
    }
    char myPid[2];
};

namespace hyc
{
    //-----------------------------------------------
    String IdentifierUtils::generateUUID()
    {
        __PidInitializer pidInitializer;
        
        struct UUID
        {
            unsigned char timeLow[4];
            unsigned char timeMid[2];
            unsigned char timeHighAndVersion[2];
            unsigned char clockSeqHiAndReserved;
            unsigned char clockSeqLow;
            unsigned char node[6];
        };
        UUID uuid;
        
        assert(sizeof(UUID) == 16);
        
        //
        // Get a random sequence of bytes. Instead of using 122 random
        // bits that could be duplicated (because of a bug with some Linux
        // kernels and potentially other Unix platforms -- see comment in
        // Random.cpp), we replace the last 15 bits of all "random"
        // Randoms by the last 15 bits of the process id.
        //
        char* buffer = reinterpret_cast<char*>(&uuid);
        Math::SRand((u32)time(0));
        s32 cNum = static_cast<int>(sizeof(UUID));
        for (s32 i = 0; i < cNum; ++i)
        {
            buffer[i] = (char)Math::RandRange(0.0f, 255.0f);
        }
        
        //
        // Adjust the bits that say "version 4" UUID
        //
        uuid.timeHighAndVersion[0] &= 0x0F;
        uuid.timeHighAndVersion[0] |= (4 << 4);
        uuid.clockSeqHiAndReserved &= 0x3F;
        uuid.clockSeqHiAndReserved |= 0x80;
        
        //
        // Replace the end of the node by myPid (15 bits)
        //
        uuid.node[4] = (uuid.node[4] & 0x80) | pidInitializer.myPid[0];
        uuid.node[5] = pidInitializer.myPid[1];
        
        //
        // Convert to a UUID string
        //
        char uuidString[16 * 2 + 4 + 1]; // 16 bytes, 4 '-' and a final '\0'
        char* uuidIndex = uuidString;
        
        s32 len = sizeof(uuidString);
        
        EncodingUtils::ByteToHex( uuid.timeLow, sizeof(uuid.timeLow), uuidIndex, len );
        uuidIndex += sizeof(uuid.timeLow)/2;
        *uuidIndex++ = '-';
        len -= sizeof(uuid.timeLow)/2+1;
        
        EncodingUtils::ByteToHex(uuid.timeMid, sizeof(uuid.timeMid), uuidIndex, len );
        uuidIndex += sizeof(uuid.timeMid)/2;
        *uuidIndex++ = '-';
        len -= sizeof(uuid.timeMid)/2+1;
        
        EncodingUtils::ByteToHex(uuid.timeHighAndVersion, sizeof(uuid.timeHighAndVersion), uuidIndex, len );
        uuidIndex += sizeof(uuid.timeHighAndVersion)/2;
        *uuidIndex++ = '-';
        len -= sizeof(uuid.timeHighAndVersion)/2+1;
        
        EncodingUtils::ByteToHex(&uuid.clockSeqHiAndReserved, sizeof(uuid.clockSeqHiAndReserved), uuidIndex, len );
        uuidIndex += sizeof(uuid.timeHighAndVersion)/2;
        len -= sizeof(uuid.timeHighAndVersion)/2;
        
        EncodingUtils::ByteToHex(&uuid.clockSeqLow, sizeof(uuid.clockSeqLow), uuidIndex, len );
        uuidIndex += sizeof(uuid.clockSeqLow)/2;
        *uuidIndex++ = '-';
        len -= sizeof(uuid.clockSeqLow)/2+1;
        
        EncodingUtils::ByteToHex(uuid.node, sizeof(uuid.node), uuidIndex, len );
        uuidIndex += sizeof(uuid.node)/2;
        *uuidIndex = '\0';
        
        return uuidString;
    }
}
#endif


#endif // HEADER_IDENTIFIERUTiLS_HPP_df211124535
