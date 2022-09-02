#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include <stdlib.h>

namespace danikk_framework
{
    size_t getOctetSize(char32_t chatacher)
    {
        uint8 mask = (uint8)(0xff & (uint8)chatacher);
        if (mask == 0) 				return 0;
        else if (mask < 128)		return 1;
        else if ((mask >> 5) == 6)	return 2;
        else if ((mask >> 4) == 14)	return 3;
        else if ((mask >> 3) == 30)	return 4;
                                    return 0;
    }

    Utf8Iterator::Utf8Iterator(char* ptr)
    {
        charPtr = ptr;
        octetSize = 0;
    }

    bool Utf8Iterator::operator!=(const Utf8Iterator& other) const
    {
        return charPtr != other.charPtr;
    }

    bool Utf8Iterator::operator==(const Utf8Iterator& other) const
    {
        return charPtr == other.charPtr;
    }

    size_t  Utf8Iterator::operator-(const Utf8Iterator& other) const
    {
        return (size_t)(charPtr - other.charPtr);
    }

    Utf8Iterator::operator char*()
    {
        return (char*)charPtr;
    }

    Utf8Iterator::operator void*()
    {
        return charPtr;
    }

    void Utf8Iterator::operator++()
    {
        charPtr += octetSize;
        octetSize = getOctetSize(*charPtr);
    }

    const Utf8Iterator Utf8Iterator::operator++(int)
    {
        Utf8Iterator iterator(charPtr);
        charPtr += octetSize;
        octetSize = getOctetSize(*charPtr);
        return iterator;
    }

    unicodeChar Utf8Iterator::operator*() const
    {
        union
        {
            uint32 numberValue;
            uint8 arrayValue[4];
        };
        numberValue = *((uint32*)charPtr);
        switch(octetSize)
        {
            case 1:
            {
                return arrayValue[0];
            }
            break;

            case 2:
            {

            }
            break;

            case 3:
            {

            }
            break;

            case 4:
            {

            }
            break;
        }
        return 0;
    }
}
