#include "fast_mem_equals.h"
#include <emmintrin.h>

namespace fast_mem_equals_private {

    bool fast_mem_equals0(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t)
    {
        return true;
    }

    bool fast_mem_equals1(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t)
    {
        return *pStr1 == *pStr2;
    }

    bool fast_mem_equals2(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t)
    {
        return *(uint16_t*)pStr1 == *(uint16_t*)pStr2;
    }

    bool fast_mem_equals3(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t)
    {
        return *(uint16_t*)pStr1 == *(uint16_t*)pStr2
            && (*(pStr1 + 2) == *(pStr2 + 2));
    }

    bool fast_mem_equals4(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t)
    {
        return *(uint32_t*)pStr1 == *(uint32_t*)pStr2;
    }

    bool fast_mem_equals5_7(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t count)
    {
        return (*(uint32_t*)pStr1 == *(uint32_t*)pStr2)
            && (*(uint32_t*)(pStr1 + count - 4) == *(uint32_t*)(pStr2 + count - 4));
    }

    bool fast_mem_equals8(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t)
    {
        return *(uint64_t*)pStr1 == *(uint64_t*)pStr2;
    }

    bool fast_mem_equals9_15(const uint8_t * pStr1, const uint8_t *pStr2, std::size_t count)
    {
        return (*(uint64_t*)pStr1 == *(uint64_t*)pStr2)
            && (*(uint64_t*)(pStr1 + count - 8) == *(uint64_t*)(pStr2 + count - 8));
    }

    typedef bool(*fast_mem_equals_type) (const uint8_t *, const uint8_t *, std::size_t);

    static const fast_mem_equals_type FAST_MEM_EQUALS[] = {
        fast_mem_equals0,
        fast_mem_equals1,
        fast_mem_equals2,
        fast_mem_equals3,
        fast_mem_equals4,
        fast_mem_equals5_7,
        fast_mem_equals5_7,
        fast_mem_equals5_7,
        fast_mem_equals8,
        fast_mem_equals9_15,
        fast_mem_equals9_15,
        fast_mem_equals9_15,
        fast_mem_equals9_15,
        fast_mem_equals9_15,
        fast_mem_equals9_15,
        fast_mem_equals9_15
    };
}

bool fast_mem_equals(const uint8_t* pStr1, const uint8_t *pStr2, std::size_t count)
{
    if (count >= 16)
    {
        std::size_t i = 0;

        for (std::size_t count16 = ((count >> 4) << 4); i < count16; i += 16, pStr1 += 16, pStr2 += 16)
        {
            __m128i xor = _mm_xor_si128(_mm_loadu_si128((__m128i*)pStr1), _mm_loadu_si128((__m128i*)pStr2));

            if (!_mm_testz_si128(xor, xor))
                return false;
        }

        if (i == count)
            return true;

        __m128i xor = _mm_xor_si128(_mm_loadu_si128((__m128i*)(pStr1 - 16 + count - i)), _mm_loadu_si128((__m128i*)(pStr2 - 16 + count - i)));

        return _mm_testz_si128(xor, xor) ? true : false;
    }
    else // count < 16
    {
        return fast_mem_equals_private::FAST_MEM_EQUALS[count](pStr1, pStr2, count);
    }
}
