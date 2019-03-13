//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  UTF 转换器实现; reference to ConvertUTF.h
//

#include "../preheader.h"

#include "../../include/utils/stringUtils.h"

#if CN_PLATFORM == CN_PLATFORM_LINUX
#include <ctype.h>
#endif

 

//-----------------------------------------------
// the content of ConvertUTF
//-----------------------------------------------

/*
* Copyright 2001-2004 Unicode, Inc.
* 
* Disclaimer
* 
* This source code is provided as is by Unicode, Inc. No claims are
* made as to fitness for any particular purpose. No warranties of any
* kind are expressed or implied. The recipient agrees to determine
* applicability of information provided. If this file has been
* purchased on magnetic or optical media from Unicode, Inc., the
* sole remedy for any claim will be exchange of defective media
* within 90 days of receipt.
* 
* Limitations on Rights to Redistribute This Code
* 
* Unicode, Inc. hereby grants the right to freely use the information
* supplied in this file in the creation of products supporting the
* Unicode Standard, and to make copies of this file in any form
* for internal or external distribution as long as this notice
* remains attached.
*/

/* ---------------------------------------------------------------------

Conversions between UTF32, UTF-16, and UTF-8.  Header file.

Several funtions are included here, forming a complete set of
conversions between the three formats.  UTF-7 is not included
here, but is handled in a separate source file.

Each of these routines takes pointers to input buffers and output
buffers.  The input buffers are const.

Each routine converts the text between *sourceStart and sourceEnd,
putting the result into the buffer between *targetStart and
targetEnd. Note: the end pointers are *after* the last item: e.g. 
*(sourceEnd - 1) is the last item.

The return result indicates whether the conversion was successful,
and if not, whether the problem was in the source or target buffers.
(Only the first encountered problem is indicated.)

After the conversion, *sourceStart and *targetStart are both
updated to point to the end of last text successfully converted in
the respective buffers.

Input parameters:
sourceStart - pointer to a pointer to the source buffer.
The contents of this are modified on return so that
it points at the next thing to be converted.
targetStart - similarly, pointer to pointer to the target buffer.
sourceEnd, targetEnd - respectively pointers to the ends of the
two buffers, for overflow checking only.

These conversion functions take a ConversionFlags argument. When this
flag is set to strict, both irregular sequences and isolated surrogates
will cause an error.  When the flag is set to lenient, both irregular
sequences and isolated surrogates are converted.

Whether the flag is strict or lenient, all illegal sequences will cause
an error return. This includes sequences such as: <F4 90 80 80>, <C0 80>,
or <A0> in UTF-8, and values above 0x10FFFF in UTF-32. Conformant code
must check for illegal sequences.

When the flag is set to lenient, characters over 0x10FFFF are converted
to the replacement character; otherwise (when the flag is set to strict)
they constitute an error.

Output parameters:
The value "sourceIllegal" is returned from some routines if the input
sequence is malformed.  When "sourceIllegal" is returned, the source
value will point to the illegal value that caused the problem. E.g.,
in UTF-8 when a sequence is malformed, it points to the start of the
malformed sequence.  

Author: Mark E. Davis, 1994.
Rev History: Rick McGowan, fixes & updates May 2001.
Fixes & updates, Sept 2001.

------------------------------------------------------------------------ */

/* ---------------------------------------------------------------------
The following 4 definitions are compiler-specific.
The C standard does not guarantee that wchar_t has at least
16 bits, so wchar_t is no less portable than unsigned short!
All should be unsigned values to avoid sign extension during
bit mask & shift operations.
------------------------------------------------------------------------ */

typedef unsigned int	UTF32;	/* at least 32 bits */
typedef unsigned short	UTF16;	/* at least 16 bits */
typedef unsigned char	UTF8;	/* typically 8 bits */
typedef unsigned char	Boolean; /* 0 or 1 */

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

typedef enum {
    conversionOK, 		/* conversion successful */
    sourceExhausted,	/* partial character in source, but hit end */
    targetExhausted,	/* insuff. room in target for conversion */
    sourceIllegal		/* source sequence is illegal/malformed */
} ConversionResult;

typedef enum {
    strictConversion = 0,
    lenientConversion
} ConversionFlags;

/* This is for C++ and does no harm in C */
#ifdef __cplusplus
extern "C" {
#endif

    ConversionResult ConvertUTF8toUTF16 (
        const UTF8** sourceStart, const UTF8* sourceEnd, 
        UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags);

    ConversionResult ConvertUTF16toUTF8 (
        const UTF16** sourceStart, const UTF16* sourceEnd, 
        UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags);

    ConversionResult ConvertUTF8toUTF32 (
        const UTF8** sourceStart, const UTF8* sourceEnd, 
        UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags);

    ConversionResult ConvertUTF32toUTF8 (
        const UTF32** sourceStart, const UTF32* sourceEnd, 
        UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags);

    ConversionResult ConvertUTF16toUTF32 (
        const UTF16** sourceStart, const UTF16* sourceEnd, 
        UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags);

    ConversionResult ConvertUTF32toUTF16 (
        const UTF32** sourceStart, const UTF32* sourceEnd, 
        UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags);

    Boolean isLegalUTF8Sequence(const UTF8 *source, const UTF8 *sourceEnd);

#ifdef __cplusplus
}
#endif

/* --------------------------------------------------------------------- */
#ifdef CVTUTF_DEBUG
#include <stdio.h>
#endif

static const int halfShift  = 10; /* used for shifting by 10 bits */

static const UTF32 halfBase = 0x0010000UL;
static const UTF32 halfMask = 0x3FFUL;

#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF
#define false	   0
#define true	    1

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF16 (
                                      const UTF32** sourceStart, const UTF32* sourceEnd, 
                                      UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags) 
{
    ConversionResult result = conversionOK;
    const UTF32* source = *sourceStart;
    UTF16* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch;
        if (target >= targetEnd) {
            result = targetExhausted; break;
        }
        ch = *source++;
        if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
            /* UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = (UTF16)ch; /* normal case */
            }
        } else if (ch > UNI_MAX_LEGAL_UTF32) {
            if (flags == strictConversion) {
                result = sourceIllegal;
            } else {
                *target++ = UNI_REPLACEMENT_CHAR;
            }
        } else {
            /* target is a character in range 0xFFFF - 0x10FFFF. */
            if (target + 1 >= targetEnd) {
                --source; /* Back up source pointer! */
                result = targetExhausted; break;
            }
            ch -= halfBase;
            *target++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
            *target++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF16toUTF32 (
                                      const UTF16** sourceStart, const UTF16* sourceEnd, 
                                      UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags) 
{
    ConversionResult result = conversionOK;
    const UTF16* source = *sourceStart;
    UTF32* target = *targetStart;
    UTF32 ch, ch2;
    while (source < sourceEnd) {
        const UTF16* oldSource = source; /*  In case we have to back up because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
            /* If the 16 bits following the high surrogate are in the source buffer... */
            if (source < sourceEnd) {
                ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
                    ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
                        + (ch2 - UNI_SUR_LOW_START) + halfBase;
                    ++source;
                } else if (flags == strictConversion) { /* it's an unpaired high surrogate */
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            } else { /* We don't have the 16 bits following the high surrogate. */
                --source; /* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        } else if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        if (target >= targetEnd) {
            source = oldSource; /* Back up source pointer! */
            result = targetExhausted; break;
        }
        *target++ = ch;
    }
    *sourceStart = source;
    *targetStart = target;
#ifdef CVTUTF_DEBUG
    if (result == sourceIllegal) {
        fprintf(stderr, "ConvertUTF16toUTF32 illegal seq 0x%04x,%04x\n", ch, ch2);
        fflush(stderr);
    }
#endif
    return result;
}

/* --------------------------------------------------------------------- */

/*
* Index into the table below with the first Byte of a UTF-8 sequence to
* get the number of trailing bytes that are supposed to follow it.
* Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
* left as-is for anyone who may want to do such conversion, which was
* allowed in earlier algorithms.
*/
static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/*
* Magic values subtracted from a buffer value during UTF8 conversion.
* This table contains as many values as there might be trailing bytes
* in a UTF-8 sequence.
*/
static const UTF32 offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 
0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/*
* Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
* into the first Byte, depending on how many bytes follow.  There are
* as many entries in this table as there are UTF-8 sequence types.
* (I.e., one Byte sequence, two Byte... etc.). Remember that sequencs
* for *legal* UTF-8 will be 4 or fewer bytes total.
*/
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

/* --------------------------------------------------------------------- */

/* The interface converts a whole buffer to avoid function-call overhead.
* Constants have been gathered. Loops & conditionals have been removed as
* much as possible for efficiency, in favor of drop-through switches.
* (See "Note A" at the bottom of the file for equivalent code.)
* If your compiler supports it, the "isLegalUTF8" call can be turned 
* into an inline function.
*/

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF16toUTF8 (
                                     const UTF16** sourceStart, const UTF16* sourceEnd, 
                                     UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags) 
{
    ConversionResult result = conversionOK;
    const UTF16* source = *sourceStart;
    UTF8* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80; 
        const UTF16* oldSource = source; /* In case we have to back up because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
            /* If the 16 bits following the high surrogate are in the source buffer... */
            if (source < sourceEnd) {
                UTF32 ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
                    ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
                        + (ch2 - UNI_SUR_LOW_START) + halfBase;
                    ++source;
                } else if (flags == strictConversion) { /* it's an unpaired high surrogate */
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            } else { /* We don't have the 16 bits following the high surrogate. */
                --source; /* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        } else if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /* Figure out how many bytes the result will require */
        if (ch < (UTF32)0x80) {	     bytesToWrite = 1;
        } else if (ch < (UTF32)0x800) {     bytesToWrite = 2;
        } else if (ch < (UTF32)0x10000) {   bytesToWrite = 3;
        } else if (ch < (UTF32)0x110000) {  bytesToWrite = 4;
        } else {			    bytesToWrite = 3;
        ch = UNI_REPLACEMENT_CHAR;
        }

        target += bytesToWrite;
        if (target > targetEnd) {
            source = oldSource; /* Back up source pointer! */
            target -= bytesToWrite; result = targetExhausted; break;
        }
        switch (bytesToWrite) { /* note: everything falls through. */
        case 4: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
        case 3: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
        case 2: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
        case 1: *--target =  (UTF8)(ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}



/* --------------------------------------------------------------------- */

/*
* Utility routine to tell whether a sequence of bytes is legal UTF-8.
* This must be called with the length pre-determined by the first Byte.
* If not calling this from ConvertUTF8to*, then the length can be set by:
*  length = trailingBytesForUTF8[*source]+1;
* and the sequence is illegal right away if there aren't that many bytes
* available.
* If presented with a length > 4, this returns false.  The Unicode
* definition of UTF-8 goes up to 4-Byte sequences.
*/

static Boolean isLegalUTF8(const UTF8 *source, int length) {
    UTF8 a;
    const UTF8 *srcptr = source+length;
    switch (length) {
    default: return false;
        /* Everything else falls through when "true"... */
    case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 2: if ((a = (*--srcptr)) > 0xBF) return false;

        switch (*source) {
            /* no fall-through in this inner switch */
    case 0xE0: if (a < 0xA0) return false; break;
    case 0xED: if (a > 0x9F) return false; break;
    case 0xF0: if (a < 0x90) return false; break;
    case 0xF4: if (a > 0x8F) return false; break;
    default:   if (a < 0x80) return false;
        }

    case 1: if (*source >= 0x80 && *source < 0xC2) return false;
    }
    if (*source > 0xF4) return false;
    return true;
}

/* --------------------------------------------------------------------- */

/*
* Exported function to return whether a UTF-8 sequence is legal or not.
* This is not used here; it's just exported.
*/
Boolean isLegalUTF8Sequence(const UTF8 *source, const UTF8 *sourceEnd) {
    int length = trailingBytesForUTF8[*source]+1;
    if (source+length > sourceEnd) {
        return false;
    }
    return isLegalUTF8(source, length);
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF16 (
                                     const UTF8** sourceStart, const UTF8* sourceEnd, 
                                     UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags) 
{
    ConversionResult result = conversionOK;
    const UTF8* source = *sourceStart;
    UTF16* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (source + extraBytesToRead >= sourceEnd) {
            result = sourceExhausted; break;
        }
        /* Do this check whether lenient or strict */
        if (! isLegalUTF8(source, extraBytesToRead+1)) {
            result = sourceIllegal;
            break;
        }
        /*
        * The cases all fall through. See "Note A" below.
        */
        switch (extraBytesToRead) {
        case 5: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
        case 4: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
        case 3: ch += *source++; ch <<= 6;
        case 2: ch += *source++; ch <<= 6;
        case 1: ch += *source++; ch <<= 6;
        case 0: ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];

        if (target >= targetEnd) {
            source -= (extraBytesToRead+1); /* Back up source pointer! */
            result = targetExhausted; break;
        }
        if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    source -= (extraBytesToRead+1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = (UTF16)ch; /* normal case */
            }
        } else if (ch > UNI_MAX_UTF16) {
            if (flags == strictConversion) {
                result = sourceIllegal;
                source -= (extraBytesToRead+1); /* return to the start */
                break; /* Bail out; shouldn't continue */
            } else {
                *target++ = UNI_REPLACEMENT_CHAR;
            }
        } else {
            /* target is a character in range 0xFFFF - 0x10FFFF. */
            if (target + 1 >= targetEnd) {
                source -= (extraBytesToRead+1); /* Back up source pointer! */
                result = targetExhausted; break;
            }
            ch -= halfBase;
            *target++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
            *target++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF8 (
                                     const UTF32** sourceStart, const UTF32* sourceEnd, 
                                     UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags) 
{
    ConversionResult result = conversionOK;
    const UTF32* source = *sourceStart;
    UTF8* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80; 
        ch = *source++;
        if (flags == strictConversion ) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /*
        * Figure out how many bytes the result will require. Turn any
        * illegally large UTF32 things (> Plane 17) into replacement chars.
        */
        if (ch < (UTF32)0x80) {	     bytesToWrite = 1;
        } else if (ch < (UTF32)0x800) {     bytesToWrite = 2;
        } else if (ch < (UTF32)0x10000) {   bytesToWrite = 3;
        } else if (ch <= UNI_MAX_LEGAL_UTF32) {  bytesToWrite = 4;
        } else {			    bytesToWrite = 3;
        ch = UNI_REPLACEMENT_CHAR;
        result = sourceIllegal;
        }

        target += bytesToWrite;
        if (target > targetEnd) {
            --source; /* Back up source pointer! */
            target -= bytesToWrite; result = targetExhausted; break;
        }
        switch (bytesToWrite) { /* note: everything falls through. */
        case 4: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
        case 3: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
        case 2: *--target = (UTF8)((ch | byteMark) & byteMask); ch >>= 6;
        case 1: *--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF32 (
                                     const UTF8** sourceStart, const UTF8* sourceEnd, 
                                     UTF32** targetStart, UTF32* targetEnd, ConversionFlags flags)
{
    ConversionResult result = conversionOK;
    const UTF8* source = *sourceStart;
    UTF32* target = *targetStart;
    while (source < sourceEnd) {
        UTF32 ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (source + extraBytesToRead >= sourceEnd) {
            result = sourceExhausted; break;
        }
        /* Do this check whether lenient or strict */
        if (! isLegalUTF8(source, extraBytesToRead+1)) {
            result = sourceIllegal;
            break;
        }
        /*
        * The cases all fall through. See "Note A" below.
        */
        switch (extraBytesToRead) {
        case 5: ch += *source++; ch <<= 6;
        case 4: ch += *source++; ch <<= 6;
        case 3: ch += *source++; ch <<= 6;
        case 2: ch += *source++; ch <<= 6;
        case 1: ch += *source++; ch <<= 6;
        case 0: ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];

        if (target >= targetEnd) {
            source -= (extraBytesToRead+1); /* Back up the source pointer! */
            result = targetExhausted; break;
        }
        if (ch <= UNI_MAX_LEGAL_UTF32) {
            /*
            * UTF-16 surrogate values are illegal in UTF-32, and anything
            * over Plane 17 (> 0x10FFFF) is illegal.
            */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    source -= (extraBytesToRead+1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = ch;
            }
        } else { /* i.e., ch > UNI_MAX_LEGAL_UTF32 */
            result = sourceIllegal;
            *target++ = UNI_REPLACEMENT_CHAR;
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* ---------------------------------------------------------------------

Note A.
The fall-through switches in UTF-8 reading code save a
temp variable, some decrements & conditionals.  The switches
are equivalent to the following loop:
{
int tmpBytesToRead = extraBytesToRead+1;
do {
ch += *source++;
--tmpBytesToRead;
if (tmpBytesToRead) ch <<= 6;
} while (tmpBytesToRead > 0);
}
In UTF-8 writing code, the switches on "bytesToWrite" are
similarly unrolled loops.

--------------------------------------------------------------------- */

/*===================================
//函数名：  IsUTF8File
//作者：    guandiqun 
//日期：    2011-12-01
//功能：    判断文件是否是utf-8
//输入参数：TCHAR *strFile    
//返回值：  int
-2：表示文件错误
-1：表示打开文件错误
1：是UTF-8（有bom）
2：是UTF-8（无bom）
0：表示不是utf-8
//===================================*/
int isUTF8String(const UTF8 *source, int length)
{
    hyc::Byte* bytes = (hyc::Byte*)source;
    if ((bytes[0] == 0x44&& bytes[1] == 0x46 && bytes[2] == 0x41))// 有bom.
    {
        return 1;
    }
    if( (bytes[0] == 0xEF&& bytes[1] == 0xBB && bytes[2] == 0xBF) )// 无bom.
    {
        return 2;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------
// the content of ConvertUTF
//----------------------------------------------------------------------------------------------------


#include "../../include/utils/encodingUtils.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	EncodingUtils
    //----------------------------------------------------------------------------------------------------
     

    //-----------------------------------------------
    bool EncodingUtils::IsLegalUTF8( const utf8* pCode, u32 length )
    {
        return 0 != isLegalUTF8(pCode, length);
    }

    //-----------------------------------------------
    bool EncodingUtils::IsLegalUTF8File( const utf8* pCode, u32 length )
    {
        return 0 != isUTF8String(pCode, length);
    }
    
    //-----------------------------------------------
    u32 EncodingUtils::UTF8toUTF16( const utf8* pSrcStream, u32 srcLength, OUT utf16* pDestStream, u32 destLength, ConversionFlag flags /*= CF_strict */ )
    {
        utf16* pDestStreamSrc = pDestStream;

        const utf8** ppSource = &pSrcStream;
        utf16** ppDest = &pDestStream;

        if (conversionOK == ConvertUTF8toUTF16(
            ppSource, pSrcStream+srcLength, 
            ppDest, pDestStream+destLength,
            ((CF_strict==flags) ? strictConversion : lenientConversion)
        ))
        {
            return (u32)(pDestStream-pDestStreamSrc);
        }
        else
        {
            return 0;
        }

    }

    //-----------------------------------------------
    u32 EncodingUtils::UTF8toUTF32( const utf8* pSrcStream, u32 srcLength, OUT utf32* pDestStream, u32 destLength, ConversionFlag flags /*= CF_strict */ )
    {
        utf32* pDestStreamSrc = pDestStream;

        const utf8** ppSource = &pSrcStream;
        utf32** ppDest = &pDestStream;

        if (conversionOK == ConvertUTF8toUTF32(
            ppSource, pSrcStream+srcLength, 
            ppDest, pDestStream+destLength,
            ((CF_strict==flags) ? strictConversion : lenientConversion)
            ))
        {
            return (u32)(pDestStream-pDestStreamSrc);
        }
        else
        {
            return 0;
        }
    }

    //-----------------------------------------------
    u32 EncodingUtils::UTF16toUTF8( const utf16* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength, ConversionFlag flags /*= CF_strict */ )
    {
        utf8* pDestStreamSrc = pDestStream;

        const utf16** ppSource = &pSrcStream;
        utf8** ppDest = &pDestStream;

        if (conversionOK == ConvertUTF16toUTF8(
            ppSource, pSrcStream+srcLength, 
            ppDest, pDestStream+destLength,
            ((CF_strict==flags) ? strictConversion : lenientConversion)
            ))
        {
            return (u32)(pDestStream-pDestStreamSrc);
        }
        else
        {
            return 0;
        }
    }

    //-----------------------------------------------
    u32 EncodingUtils::UTF16toUTF32( const utf16* pSrcStream, u32 srcLength, OUT utf32* pDestStream, u32 destLength, ConversionFlag flags /*= CF_strict */ )
    {
        utf32* pDestStreamSrc = pDestStream;

        const utf16** ppSource = &pSrcStream;
        utf32** ppDest = &pDestStream;

        if (conversionOK == ConvertUTF16toUTF32(
            ppSource, pSrcStream+srcLength, 
            ppDest, pDestStream+destLength,
            ((CF_strict==flags) ? strictConversion : lenientConversion)
            ))
        {
            return (u32)(pDestStream-pDestStreamSrc);
        }
        else
        {
            return 0;
        }
    }

    //-----------------------------------------------
    u32 EncodingUtils::UTF32toUTF8( const utf32* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength, ConversionFlag flags /*= CF_strict */ )
    {
        utf8* pDestStreamSrc = pDestStream;

        const utf32** ppSource = &pSrcStream;
        utf8** ppDest = &pDestStream;

        if (conversionOK == ConvertUTF32toUTF8(
            ppSource, pSrcStream+srcLength, 
            ppDest, pDestStream+destLength,
            ((CF_strict==flags) ? strictConversion : lenientConversion)
            ))
        {
            return (u32)(pDestStream-pDestStreamSrc);
        }
        else
        {
            return 0;
        }
    }

    //-----------------------------------------------
    u32 EncodingUtils::UTF32toUTF16( const utf32* pSrcStream, u32 srcLength, OUT utf16* pDestStream, u32 destLength, ConversionFlag flags /*= CF_strict */ )
    {
        utf16* pDestStreamSrc = pDestStream;

        const utf32** ppSource = &pSrcStream;
        utf16** ppDest = &pDestStream;

        if (conversionOK == ConvertUTF32toUTF16(
            ppSource, pSrcStream+srcLength, 
            ppDest, pDestStream+destLength,
            ((CF_strict==flags) ? strictConversion : lenientConversion)
            ))
        {
            return (u32)(pDestStream-pDestStreamSrc);
        }
        else
        {
            return 0;
        }
    }

    //-----------------------------------------------
    //  UTF转换表; 只支持含最多3个字节表示的字符; 不支持少数的4字节字符
//     static const u16 g_UTF8TABLE[] = { 0x007F, 0x07FF, 0xFFFF, 0x10FFFF };

    // 获得字节的指定位
#define BIT_GET_WORD( v, index ) (((v)>>(index))&0x1)
#define BIT_GET_BYTE( v, index ) (((v)>>(index))&0x1)


    //-----------------------------------------------
    u32 EncodingUtils::StrToUTF8( const char* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength )
    {
        if (!pDestStream || !pSrcStream)
        {
            return 0;
        }

        if (CN_IS_UTF8_CSTRING)
        {
            u32 l = Math::Min(destLength, srcLength);
            memcpy(pDestStream, pSrcStream, l);
            return l;
        }
        else
        {
            wString str;
            StringUtils::Narrow2wide( pSrcStream, srcLength, str );

            return StrToUTF8(str.c_str(), str.size(), pDestStream, destLength);
        }
    }

    //-----------------------------------------------
    u32 EncodingUtils::StrToUTF8( const wchar_t* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength )
    {
        if (!pDestStream || !pSrcStream)
        {
            return 0;
        }

        // unicode16.
        if (sizeof(wchar_t) == 2)
        {
            return UTF16toUTF8((utf16*)pSrcStream, srcLength, pDestStream, destLength, CF_lenient);
        }
        // unicode32.
        else
        {
            return UTF32toUTF8((utf32*)pSrcStream, srcLength, pDestStream, destLength, CF_lenient);
        }
    }

    //-----------------------------------------------
    bool EncodingUtils::UTF16ToUStr( const utf16* pSrcStream, u32 srcLength, OUT String& uStr )
    {
        tchar buf[5] = {0};
        uStr.clear();
        uStr.reserve(srcLength*6);

        for (u32 i = 0; i < srcLength; i++)
        {
            utf16 c = pSrcStream[i];
            c = ByteUtils::Hton(c);

            uStr += _T("\\u");
            EncodingUtils::ByteToHex((Byte*)&c, 2, buf, 4);
            uStr += buf;
        } // for.

        return true;
    }

    //-----------------------------------------------
    void EncodingUtils::UStrToStr( const char* pStr, u32 srcLength, OUT String& str )
    {
        str.clear();

        if (!pStr || srcLength == 0)
            return;

        str.reserve(srcLength);

        char buf[5] = {0};
        utf8 ccBuf[5] = {0};
        tchar strBuf[4] = {0};
        bool haveMark = false;

        u32 i = 0;
        for (; i < srcLength; i++)
        {
            char c = pStr[i];
            
            // enter utf8 encoding.
            if (haveMark && (c == 'u' || c == 'U'))
            {
                haveMark = false;
                i++;

                if (i+4 <= srcLength)
                {
                    buf[0] = pStr[i];
                    buf[1] = pStr[i+1];
                    buf[2] = pStr[i+2];
                    buf[3] = pStr[i+3];
                    u16 cc; 
                    EncodingUtils::HexToByte(buf, 4, (Byte*)&cc, 2);
                    cc = ByteUtils::Ntoh(cc);
                    ZeroMemory(ccBuf, sizeof(ccBuf));
                    u32 l = EncodingUtils::UTF16toUTF8(&cc, 1, ccBuf, 5);
                    ZeroMemory(strBuf, sizeof(strBuf));
                    EncodingUtils::UTF8ToStr(ccBuf, l, strBuf, 4);
                    str += strBuf;

                    i += 3;
                }
                else
                {
                    break;
                }
            }
            else
            {
                // append pre-char.
                if (haveMark)
                {
                    haveMark = false;
                    str += _T('\\');
                }
                
                // append other.
                if (c == '\\')
                {
                    haveMark = true;
                    continue;
                }

#if defined(_UNICODE) || defined(UNICODE)
                wchar_t wc;
                uinteger wcSize = 1;
                StringUtils::Narrow2wide(&c, 1, &wc, wcSize);
                str += wc;
#else
                str += c;
#endif
            }

        } // for.

        // supplement a char.
        if (haveMark && i < srcLength)
        {
            str += _T('\\');
        }
    }

    //-----------------------------------------------
    void EncodingUtils::UStrToStr( const wchar_t* pStr, u32 srcLength, OUT String& str )
    {
        str.clear();

        if (!pStr || srcLength == 0)
            return;

        str.reserve(srcLength);

        wchar_t buf[5] = {0};
        utf8 ccBuf[5] = {0};
        tchar strBuf[4] = {0};
        bool haveMark = false;

        u32 i = 0;
        for (; i < srcLength; i++)
        {
            wchar_t c = pStr[i];

            // enter utf8 encoding.
            if (haveMark && (c == L'u' || c == L'U'))
            {
                haveMark = false;
                i++;

                if (i+4 <= srcLength)
                {
                    buf[0] = pStr[i];
                    buf[1] = pStr[i+1];
                    buf[2] = pStr[i+2];
                    buf[3] = pStr[i+3];
                    u16 cc; 
                    EncodingUtils::HexToByte(buf, 4, (Byte*)&cc, 2);
                    cc = ByteUtils::Ntoh(cc);
                    ZeroMemory(ccBuf, sizeof(ccBuf));
                    u32 l = EncodingUtils::UTF16toUTF8(&cc, 1, ccBuf, 5);
                    ZeroMemory(strBuf, sizeof(strBuf));
                    EncodingUtils::UTF8ToStr(ccBuf, l, strBuf, 4);
                    str += strBuf;

                    i += 3;
                }
                else
                {
                    break;
                }
            }
            else
            {
                // append pre-char.
                if (haveMark)
                {
                    haveMark = false;
                    str += _T('\\');
                }

                // append other.
                if (c == L'\\')
                {
                    haveMark = true;
                    continue;
                }

#if defined(_UNICODE) || defined(UNICODE)
                str += c;
#else
                char nc[3] = {0};
                uinteger ncSize = 2;
                StringUtils::Wide2narrow(&c, 1, nc, ncSize);
                str += nc;
#endif
            }

        } // for.

        // supplement a char.
        if (haveMark && i < srcLength)
        {
            str += _T('\\');
        }
    }

    //-----------------------------------------------
    u32 EncodingUtils::UTF8ToStr( const utf8* pSrcStream, u32 srcLength, OUT char* pDestStream, u32 destLength )
    {
        if (!pDestStream || !pSrcStream)
        {
            return 0;
        }

        if (CN_IS_UTF8_CSTRING)
        {
            u32 l = Math::Min(destLength, srcLength);
            memcpy(pDestStream, pSrcStream, l);
            return l;
        }
        else
        {
            u32 ll = destLength+1;
            wchar_t* pBuf = (wchar_t*)MemoryAlloc::Malloc(ll*sizeof(wchar_t));
            u32 pos = UTF8ToStr(pSrcStream, srcLength, pBuf, ll);
            if (pos == 0)
            {
                MemoryAlloc::Free(pBuf);
                return 0;
            }

            pBuf[pos] = L'\0';

            // to multibyte
            uinteger dl = destLength;
            StringUtils::Wide2narrow( pBuf, (uinteger)pos, (char*)pDestStream, dl );
            MemoryAlloc::Free(pBuf);

            return (u32)dl;
        }
    }
    
    //-----------------------------------------------
    u32 EncodingUtils::UTF8ToStr( const utf8* pSrcStream, u32 srcLength, OUT wchar_t* pDestStream, u32 destLength )
    {
        if (!pDestStream || !pSrcStream)
        {
            return 0;
        }

        // unicode16.
        if (sizeof(wchar_t) == 2)
        {
            return UTF8toUTF16((utf8*)pSrcStream, srcLength, (utf16*)pDestStream, destLength);
        }
        // unicode32.
        else
        {
            return UTF8toUTF32((utf8*)pSrcStream, srcLength, (utf32*)pDestStream, destLength);
        }

//         if (!pDestStream || !pSrcStream)
//         {
//             return 0;
//         }
// 
//         wchar_t* pBuf = pDestStream;
//         u32 ll = destLength;
// 
// 
//         // 转换
//         u32 pos = 0;
//         u32 i = 0;
//         if (pSrcStream[0] == 0xEF
//             && pSrcStream[1] == 0xBB
//             && pSrcStream[2] == 0xBF)
//         {
//             i = 3;
//         }
// 
//         u32 maxlen = Math::Min(srcLength, destLength);
// 
//         for ( ; i < maxlen; ++i )
//         {
//             utf8 t = pSrcStream[i];
// 
//             if ( 0 == (t&0x80) ) // 单字节
//             {
//                 if ( pos == ll )    break;
// 
//                 pBuf[pos++] = (wchar_t)t;
//             }
//             else if ( 1 == BIT_GET_BYTE(t,7) && 1 == BIT_GET_BYTE(t,6) && 0 == BIT_GET_BYTE(t,5) )  // 双字节
//             {
//                 if ( pos == ll || i == srcLength-1 )    break;
// 
//                 wchar_t& t16 = pBuf[pos++];
//                 utf8 t1 = pSrcStream[i+1];
// 
//                 t16 = (wchar_t)t;
//                 t16 <<= 8+3;  t16 >>= 5;
// 
//                 t1 <<= 2;   t1 >>= 2;
//                 t16 |= (wchar_t)t1;
// 
//                 ++i;
//             }
//             else if ( 1 == BIT_GET_BYTE(t,7) && 1 == BIT_GET_BYTE(t,6) && 1 == BIT_GET_BYTE(t,5) && 0 == BIT_GET_BYTE(t,4) )  // 三字节
//             {
//                 if ( pos == ll || i == srcLength-2 )    break;
// 
//                 wchar_t& t16 = pBuf[pos++];
//                 utf16 t1 = (utf16)pSrcStream[i+1];
//                 utf8 t2 = pSrcStream[i+2];
// 
//                 t16 = (wchar_t)t;
//                 t16 <<= 8+4;
// 
//                 t1 <<= 8+2;   t1 >>= 4;
//                 t16 |= t1;
// 
//                 t2 <<= 2;   t2 >>= 2;
//                 t16 |= (wchar_t)t2;
// 
//                 i += 2;
//             } // if..else
//         } // for 
// 
//         return pos;
    }

    //-----------------------------------------------
    void EncodingUtils::ByteToHex( const Byte* pVale, u32 cLenData, OUT cString& str )
    {
        static const char HEX[16] = {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f'
        };

        // bytes to hex string
        str.clear();

        if ( !pVale || cLenData <= 0 )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        str.reserve(cLenData*2);

        for ( u32 i = 0; i < cLenData; ++i ) {
            integer t = pVale[i];
            integer a = t / 16;
            integer b = t % 16;
            str.append(1, HEX[a]);
            str.append(1, HEX[b]);
        }
    }

    //-----------------------------------------------
    void EncodingUtils::ByteToHex( const Byte* pVale, u32 cLenData, OUT wString& str )
    {
        static const wchar_t HEX[16] = {
            L'0', L'1', L'2', L'3',
            L'4', L'5', L'6', L'7',
            L'8', L'9', L'a', L'b',
            L'c', L'd', L'e', L'f'
        };

        // bytes to hex string
        str.clear();

        if ( !pVale || cLenData <= 0 )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        str.reserve(cLenData*2);

        for ( u32 i = 0; i < cLenData; ++i ) {
            integer t = pVale[i];
            integer a = t / 16;
            integer b = t % 16;
            str.append(1, HEX[a]);
            str.append(1, HEX[b]);
        }
    }

    //-----------------------------------------------
    void EncodingUtils::ByteToHex( const Byte* pVale, u32 cLenData, OUT char* pStr, u32 cLenStr )
    {
        static const char HEX[16] = {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f'
        };

        // bytes to hex string
        if ( !pVale || cLenData <= 0 || !pStr || cLenStr <= 0 )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        u32 j = 0;
        for ( u32 i = 0; i < cLenData && j+1 < cLenStr; ++i ) {
            integer t = pVale[i];
            integer a = t / 16;
            integer b = t % 16;
            pStr[j++] = HEX[a];
            pStr[j++] = HEX[b];
        }
    }

    //-----------------------------------------------
    void EncodingUtils::ByteToHex( const Byte* pVale, u32 cLenData, OUT wchar_t* pStr, u32 cLenStr )
    {
        static const wchar_t HEX[16] = {
            L'0', L'1', L'2', L'3',
            L'4', L'5', L'6', L'7',
            L'8', L'9', L'a', L'b',
            L'c', L'd', L'e', L'f'
        };

        // bytes to hex string
        if ( !pVale || cLenData <= 0 || !pStr || cLenStr <= 0 )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        u32 j = 0;
        for ( u32 i = 0; i < cLenData && j+1 < cLenStr; ++i ) {
            integer t = pVale[i];
            integer a = t / 16;
            integer b = t % 16;
            pStr[j++] = HEX[a];
            pStr[j++] = HEX[b];
        }
    }

    //-----------------------------------------------
    void EncodingUtils::HexToByte( const char* pHexStr, u32 cLenStr, OUT Byte* pValue, u32 cLenData )
    {
        if ( !pHexStr || !pValue || cLenStr <= 0 || cLenData <= 0 )  {
            throw ExceptionType_ErrorParam;        
            return;
        }
        memset( pValue, 0, cLenData );

        Byte ch = 0;

        if (cLenStr == 1)
        {
            ch = pHexStr[0];
            ch -= '0';

            if (ch >= 'a'-'0')
            {
                ch -= 'a'-'9'-1;
            }
            else if (ch >= 'A'-'0')
            {

                ch -= 'A'-'9'-1;
            }
            pValue[0] = ch;
            return;
        }

        u32 j = 0;
        for ( u32 i = 0; i+1 < cLenStr && j < cLenData; ++i ) {

            ch = pHexStr[i];
            ch -= '0';

            if (ch >= 'a'-'0')
            {
                ch -= 'a'-'9'-1;
            }
            else if (ch >= 'A'-'0')
            {

                ch -= 'A'-'9'-1;
            }
            pValue[j] = ch;

            ++i;

            ch = pHexStr[i];
            ch -= '0';

            if (ch >= 'a'-'0')
            {
                ch -= 'a'-'9'-1;
            }
            else if (ch >= 'A'-'0')
            {
                ch -= 'A'-'9'-1;
            }

            pValue[j] = (pValue[j]<<4) | ch;
            ++j;
        }
    }

    //-----------------------------------------------
    void EncodingUtils::HexToByte( const wchar_t* pHexStr, u32 cLenStr, OUT Byte* pValue, u32 cLenData )
    {
        cString str;
        StringUtils::Wide2narrow( pHexStr, cLenStr, str );

        HexToByte( str.c_str(), (u32)str.size(), pValue, cLenData );

    }

    //-----------------------------------------------
    void EncodingUtils::Base64_Encode( const Byte* pData, u32 cLenData, OUT cString& strBase64, bool needReturn )
    {
        strBase64.clear();
        if ( NULL == pData || 0 >= cLenData )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        // 编码表
        static const s8 EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        
        // 返回值
        u8 Tmp[4] = {0};
        integer LineLength = 0;
        u32 len = (cLenData / 3);

        for( u32 i = 0; i < len; ++i )
        {
            Tmp[1] = *pData++;
            Tmp[2] = *pData++;
            Tmp[3] = *pData++;

            strBase64 += EncodeTable[Tmp[1] >> 2];
            strBase64 += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
            strBase64 += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
            strBase64 += EncodeTable[Tmp[3] & 0x3F];
            if (needReturn) {
                if ( LineLength += 4, LineLength == 76 ) { strBase64 += "\r\n"; LineLength = 0; }
            }
        }

        // 对剩余数据进行编码
        integer Mod = cLenData % 3;
        if ( 1 == Mod )
        {
            Tmp[1] = *pData++;
            strBase64 += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strBase64 += EncodeTable[((Tmp[1] & 0x03) << 4)];
            strBase64 += "==";
        }
        else if( 2 == Mod )
        {
            Tmp[1] = *pData++;
            Tmp[2] = *pData++;
            strBase64 += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strBase64 += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
            strBase64  += EncodeTable[((Tmp[2] & 0x0F) << 2)];
            strBase64 += "=";
        }
    }

    //-----------------------------------------------
    void EncodingUtils::Base64_Decode( const char* pStrBase64, u32 cLenStr, OUT ByteStream& stream )
    {
        stream.clear();
        if ( !pStrBase64 || 0 == cLenStr )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        // 解码表
        static const s8 DecodeTable[] =
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            62, // '+'
            0, 0, 0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
            13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0,
            26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
        };

        char* pData = (char*)pStrBase64;

        // 返回值
        integer nValue;
        u32 i = 0;
        while ( i < cLenStr )
        {
            if ( *pData != '\r' && *pData != '\n' )
            {
                nValue = DecodeTable[(s32)*pData++] << 18;
                nValue += DecodeTable[(s32)*pData++] << 12;
                stream.push_back( (nValue & 0x00FF0000) >> 16 );
                if (*pData != '=')
                {
                    nValue += DecodeTable[(s32)*pData++] << 6;
                    stream.push_back( (nValue & 0x0000FF00) >> 8 );
                    if (*pData != '=')
                    {
                        nValue += DecodeTable[(s32)*pData++];
                        stream.push_back( nValue & 0x000000FF );
                    }
                }
                i += 4;
            }
            else// 回车换行,跳过
            {
                pData++;
                i++;
            }
        } // while
    }

    //-----------------------------------------------
    bool EncodingUtils::IsBase64( const char& c )
    {
        if(c >= 'A' && c <= 'Z')
        {
            return true;
        }

        if(c >= 'a' && c <= 'z')
        {
            return true;
        }

        if(c >= '0' && c <= '9')
        {
            return true;
        }

        if(c == '+')
        {
            return true;
        }

        if(c == '/')
        {
            return true;
        }

        if(c == '=')
        {
            return true;
        }

        return false;
    }

    //-----------------------------------------------
    void EncodingUtils::Url_Encode( const char* pStream, u32 cLenData, OUT cString& strUrlEncode )
    {
        if ( !pStream || 0 >= cLenData )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        strUrlEncode.clear();
        Byte buf[4] = {0};

        for( u32 ix = 0; ix < cLenData; ++ix )
        {
            memset( buf, 0, 4 );
            if( '0' <= pStream[ix] && pStream[ix] <= '9' )
            {
                buf[0] = pStream[ix];
            }
            else if ('a' <= pStream[ix] && pStream[ix] <= 'z')
            {
                buf[0] = pStream[ix];
            }
            else if ('A' <= pStream[ix] && pStream[ix] <= 'Z')
            {
                buf[0] = pStream[ix];
            }
            else if ('.' == pStream[ix] || '-' == pStream[ix] || '_' == pStream[ix])
            {
                buf[0] = pStream[ix];
            }
            else if (' ' == pStream[ix])
            {
                buf[0] = '+';
            }
            else
            {
                buf[0] = '%';
                ByteToHex( (Byte*)(pStream+ix), 1, (char*)(buf+1), 2 );
                buf[1] = toupper(buf[1]);
                buf[2] = toupper(buf[2]);
            }
            strUrlEncode += (char*)buf;
        }
    }

    //-----------------------------------------------
    void EncodingUtils::Url_Decode( const char* pUrlEncode, u32 cLenData, OUT cString& stream )
    {
        if ( !pUrlEncode || 0 >= cLenData )  {
            throw ExceptionType_ErrorParam;        
            return;
        }

        stream.clear();
        Byte buf;

        for( u32 ix = 0; ix < cLenData; ++ix )
        {
            buf = 0;
            if( '%' == pUrlEncode[ix] )
            {
                if ( ix+2 < cLenData )
                {
                    HexToByte( pUrlEncode+ix+1, 2, &buf, 1 );
                    ix += 2;
                }
                else
                {
                    throw ExceptionType_ErrorParam;        
                    return;
                }
            }
            else if ( '+' == pUrlEncode[ix] )
            {
                buf = ' ';
            }
            else
            {
                buf = pUrlEncode[ix];
            }
            stream += (char)buf;
        }
    }




} // end namespace hyc
