/*  MIT License

Copyright (c) 2016 Dmitry Romanov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */


#ifndef HEADERLOCK_CSTRINGBUILDER_HPP
#define HEADERLOCK_CSTRINGBUILDER_HPP

#include <cstddef>
#include <cstring>
#include <cmath>
#include <type_traits>


#pragma diag_suppress  186

// Allows to switch on/off floating point support.
// Using floating point operations is ON by default
#if !defined( TCSB_USE_FP )
    #define TCSB_USE_FP (1)
#endif

// Keil UVision and some other controller compilers happen to have problems with namespaces (and nested namespaces)
// TCSB_USE_NAMESPACE allows to exclude tcsb namespace from the class. Namespace is used by default
#if !defined( TCSB_NO_NAMESPACE )
namespace tcsb {
#endif

#if TCSB_USE_FP
typedef struct TCSB_Fp {
    uint64_t frac;
    int exp;
} TCSB_Fp;
#endif  //#if TCSB_USE_FP


class CStringBuilder {
    CStringBuilder(const CStringBuilder&) = delete;
    CStringBuilder& operator=(const CStringBuilder&) = delete;

private:
    char* _buffer;
    char _separator = ' ';
    std::size_t _bufferSize;
    std::size_t _cursor;
    bool _isOverflow;

    void setStringEnd() {
        _buffer[_cursor] = '\0';
    }

    /* reverse:  reverse string s in place */
    void reverse(char s[])
    {
        size_t i, j;
        char c;

        for (i = 0, j = strlen((const char*)s)-1; i<j; i++, j--) {
            c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }


    /** K&R int to string conversion */
    template <typename IntType>
    size_t IntTypeToS(IntType n)
    {
        if (_bufferSize - 1 <= _cursor) return 0;
        size_t i;
        bool isNegative;
        char *s = &_buffer[_cursor];

        /* record sign */
        isNegative = n < 0;
        if (isNegative) {
            _cursor++;       /* increase length fo 1 */
            n = -n;          /* make n positive */

            // Check buffer overflow
            if (_bufferSize - 1 <= _cursor) {
                setStringEnd();
                return 1;
            }
        }

        i = 0;
        do {       /* generate digits in reverse order */
            s[i++] = n % 10 + '0';   /* get next digit */
            n /= 10;

            _cursor++;

        } while (n > 0 && _cursor < _bufferSize-1);     /* delete it */

        // Check buffer overflow
        if (_bufferSize - 1 <= _cursor) {
            setStringEnd();
            return 1;
        }

        // Add negative sign!
        if (isNegative) {
            s[i++] = '-';
        }

        s[i] = '\0';
        reverse(s);
        return i;
    }

public:
    /**
     \rst
     Constructs a :class:`fmt::BasicArrayWriter` object for *array* of the
     given size.
     \endrst
     */
    CStringBuilder(char *buffer, std::size_t bufferSize) {
        _buffer = buffer;
        _bufferSize = bufferSize;
        _cursor = 0;
        _isOverflow = false;
        setStringEnd();

    }

    /**
     \rst
     Constructs a :class:`fmt::BasicArrayWriter` object for *array* of the
     size known at compile time.
     \endrst
     */
    template <std::size_t SIZE>
        explicit CStringBuilder(char(&array)[SIZE])
    {
        _buffer = array;
        _bufferSize = SIZE;
        _cursor = 0;
        _isOverflow = false;
        setStringEnd();
    }


    /** returns current size of the _buffer */
    size_t bufferSize(void) { return _bufferSize; }


    /** The actual size of the resulting string */
    size_t size(void) { return _cursor; }

    char * cStr() { return _buffer;}


    /** Append by C string */
    template<class CharConstPtr>
    size_t add(CharConstPtr array, std::size_t size) {
        if(_bufferSize - 1 == _cursor) return 0;
        size_t i;

        for(i=0; array[i] != (char)'\0' && i<size ; i++) {
            if(_bufferSize - 1 == _cursor) break;
            _buffer[_cursor] = array[i];
            _cursor++;
        }
        setStringEnd();
        return i;
    }

    //template <std::size_t SIZE>
    //size_t add(char (&array)[SIZE]){
      //  return add(array, SIZE);
    //}

    template <std::size_t SIZE>
    size_t add(const char (&array)[SIZE]){
        return add(&array[0], SIZE);
    }

    size_t add(int8_t value)   { return IntTypeToS<int8_t>(value); }     /// Converts int8_t to string and adds to the buffer
    size_t add(uint8_t value)  { return IntTypeToS<uint8_t>(value); }    /// Converts uint8_t to string and adds to the buffer
    size_t add(int16_t value)  { return IntTypeToS<int16_t>(value); }    /// Converts int16_t to string and adds to the buffer
    size_t add(uint16_t value) { return IntTypeToS<uint16_t>(value); }   /// Converts uint16_t to string and adds to the buffer
    size_t add(int32_t value)  { return IntTypeToS<int32_t>(value); }    /// Converts int32_t to string and adds to the buffer
    size_t add(uint32_t value) { return IntTypeToS<uint32_t>(value); }   /// Converts uint32_t to string and adds to the buffer
    size_t add(int64_t value)  { return IntTypeToS<int64_t>(value); }    /// Converts int64_t to string and adds to the buffer
    size_t add(uint64_t value) { return IntTypeToS<uint64_t>(value); }   /// Converts uint64_t to string and adds to the buffer

    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, int8_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], int8_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], int8_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, uint8_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], uint8_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], uint8_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, int16_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], int16_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], int16_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, uint16_t value) { return add(array, size) + add(value); }

    //template <std::size_t SIZE>
    //size_t dadd(char (&array)[SIZE], uint16_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], uint16_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, int32_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], int32_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], int32_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, uint32_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], uint32_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], uint32_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, int64_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], int64_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], int64_t value){ return add(array) + add(value); }


    template<class CharConstPtr>
    size_t dadd(CharConstPtr array, std::size_t size, uint64_t value) { return add(array, size) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(char (&array)[SIZE], uint64_t value){ return add(array) + add(value); }

    template <std::size_t SIZE>
    size_t dadd(const char (&array)[SIZE], uint64_t value){ return add(array) + add(value); }



#if TCSB_USE_FP

    /*----------------- FLOATING POINT -----------------------*/
    /* The next code is taken from fpconv library https://github.com/night-shift/fpconv
     * fpconv is distributed by MIT license as this library
     **/

private:


    #define TCSB_npowers     87
    #define TCSB_steppowers  8
    #define TCSB_firstpower -348 /* 10 ^ -348 */

    #define TCSB_expmax     -32
    #define TCSB_expmin     -60

    static constexpr const TCSB_Fp powers_ten[TCSB_npowers] = {
            { 18054884314459144840U, -1220 }, { 13451937075301367670U, -1193 },
            { 10022474136428063862U, -1166 }, { 14934650266808366570U, -1140 },
            { 11127181549972568877U, -1113 }, { 16580792590934885855U, -1087 },
            { 12353653155963782858U, -1060 }, { 18408377700990114895U, -1034 },
            { 13715310171984221708U, -1007 }, { 10218702384817765436U, -980 },
            { 15227053142812498563U, -954 },  { 11345038669416679861U, -927 },
            { 16905424996341287883U, -901 },  { 12595523146049147757U, -874 },
            { 9384396036005875287U,  -847 },  { 13983839803942852151U, -821 },
            { 10418772551374772303U, -794 },  { 15525180923007089351U, -768 },
            { 11567161174868858868U, -741 },  { 17236413322193710309U, -715 },
            { 12842128665889583758U, -688 },  { 9568131466127621947U,  -661 },
            { 14257626930069360058U, -635 },  { 10622759856335341974U, -608 },
            { 15829145694278690180U, -582 },  { 11793632577567316726U, -555 },
            { 17573882009934360870U, -529 },  { 13093562431584567480U, -502 },
            { 9755464219737475723U,  -475 },  { 14536774485912137811U, -449 },
            { 10830740992659433045U, -422 },  { 16139061738043178685U, -396 },
            { 12024538023802026127U, -369 },  { 17917957937422433684U, -343 },
            { 13349918974505688015U, -316 },  { 9946464728195732843U,  -289 },
            { 14821387422376473014U, -263 },  { 11042794154864902060U, -236 },
            { 16455045573212060422U, -210 },  { 12259964326927110867U, -183 },
            { 18268770466636286478U, -157 },  { 13611294676837538539U, -130 },
            { 10141204801825835212U, -103 },  { 15111572745182864684U, -77 },
            { 11258999068426240000U, -50 },   { 16777216000000000000U, -24 },
            { 12500000000000000000U,   3 },   { 9313225746154785156U,   30 },
            { 13877787807814456755U,  56 },   { 10339757656912845936U,  83 },
            { 15407439555097886824U, 109 },   { 11479437019748901445U, 136 },
            { 17105694144590052135U, 162 },   { 12744735289059618216U, 189 },
            { 9495567745759798747U,  216 },   { 14149498560666738074U, 242 },
            { 10542197943230523224U, 269 },   { 15709099088952724970U, 295 },
            { 11704190886730495818U, 322 },   { 17440603504673385349U, 348 },
            { 12994262207056124023U, 375 },   { 9681479787123295682U,  402 },
            { 14426529090290212157U, 428 },   { 10748601772107342003U, 455 },
            { 16016664761464807395U, 481 },   { 11933345169920330789U, 508 },
            { 17782069995880619868U, 534 },   { 13248674568444952270U, 561 },
            { 9871031767461413346U,  588 },   { 14708983551653345445U, 614 },
            { 10959046745042015199U, 641 },   { 16330252207878254650U, 667 },
            { 12166986024289022870U, 694 },   { 18130221999122236476U, 720 },
            { 13508068024458167312U, 747 },   { 10064294952495520794U, 774 },
            { 14996968138956309548U, 800 },   { 11173611982879273257U, 827 },
            { 16649979327439178909U, 853 },   { 12405201291620119593U, 880 },
            { 9242595204427927429U,  907 },   { 13772540099066387757U, 933 },
            { 10261342003245940623U, 960 },   { 15290591125556738113U, 986 },
            { 11392378155556871081U, 1013 },  { 16975966327722178521U, 1039 },
            { 12648080533535911531U, 1066 }
    };

    static TCSB_Fp find_cachedpow10(int exp, int* k)
    {
        const double one_log_ten = 0.30102999566398114;

        int approx = -(exp + TCSB_npowers) * one_log_ten;
        int idx = (approx - TCSB_firstpower) / TCSB_steppowers;

        while(1) {
            int current = exp + powers_ten[idx].exp + 64;

            if(current < TCSB_expmin) {
                idx++;
                continue;
            }

            if(current > TCSB_expmax) {
                idx--;
                continue;
            }

            *k = (TCSB_firstpower + idx * TCSB_steppowers);

            return powers_ten[idx];
        }
    }

#define TCSB_fracmask  0x000FFFFFFFFFFFFFU
#define TCSB_expmask   0x7FF0000000000000U
#define TCSB_hiddenbit 0x0010000000000000U
#define TCSB_signmask  0x8000000000000000U
#define TCSB_expbias   (1023 + 52)
#define TCSB_tens_len (20)

#define TCDB_absv(n) ((n) < 0 ? -(n) : (n))
#define TCSB_minv(a, b) ((a) < (b) ? (a) : (b))

    static constexpr const uint64_t tens[TCSB_tens_len] = {
            10000000000000000000U, 1000000000000000000U, 100000000000000000U,
            10000000000000000U, 1000000000000000U, 100000000000000U,
            10000000000000U, 1000000000000U, 100000000000U,
            10000000000U, 1000000000U, 100000000U,
            10000000U, 1000000U, 100000U,
            10000U, 1000U, 100U,
            10U, 1U
    };

    static inline uint64_t get_dbits(double d)
    {
        union {
            double   dbl;
            uint64_t i;
        } dbl_bits = { d };

        return dbl_bits.i;
    }

    TCSB_Fp build_fp(double d)
    {
        uint64_t bits = get_dbits(d);

        TCSB_Fp fp;
        fp.frac = bits & TCSB_fracmask;
        fp.exp = (bits & TCSB_expmask) >> 52;

        if(fp.exp) {
            fp.frac += TCSB_hiddenbit;
            fp.exp -= TCSB_expbias;

        } else {
            fp.exp = -TCSB_expbias + 1;
        }

        return fp;
    }

    void normalize(TCSB_Fp* fp)
    {
        while ((fp->frac & TCSB_hiddenbit) == 0) {
            fp->frac <<= 1;
            fp->exp--;
        }

        int shift = 64 - 52 - 1;
        fp->frac <<= shift;
        fp->exp -= shift;
    }

    void get_normalized_boundaries(TCSB_Fp* fp, TCSB_Fp* lower, TCSB_Fp* upper)
    {
        upper->frac = (fp->frac << 1) + 1;
        upper->exp  = fp->exp - 1;

        while ((upper->frac & (TCSB_hiddenbit << 1)) == 0) {
            upper->frac <<= 1;
            upper->exp--;
        }

        int u_shift = 64 - 52 - 2;

        upper->frac <<= u_shift;
        upper->exp = upper->exp - u_shift;


        int l_shift = fp->frac == TCSB_hiddenbit ? 2 : 1;

        lower->frac = (fp->frac << l_shift) - 1;
        lower->exp = fp->exp - l_shift;


        lower->frac <<= lower->exp - upper->exp;
        lower->exp = upper->exp;
    }

    TCSB_Fp multiply(TCSB_Fp* a, TCSB_Fp* b)
    {
        const uint64_t lomask = 0x00000000FFFFFFFF;

        uint64_t ah_bl = (a->frac >> 32)    * (b->frac & lomask);
        uint64_t al_bh = (a->frac & lomask) * (b->frac >> 32);
        uint64_t al_bl = (a->frac & lomask) * (b->frac & lomask);
        uint64_t ah_bh = (a->frac >> 32)    * (b->frac >> 32);

        uint64_t tmp = (ah_bl & lomask) + (al_bh & lomask) + (al_bl >> 32);
        /* round up */
        tmp += 1U << 31;

        TCSB_Fp fp = {
                ah_bh + (ah_bl >> 32) + (al_bh >> 32) + (tmp >> 32),
                a->exp + b->exp + 64
        };

        return fp;
    }

    void round_digit(char* digits, int ndigits, uint64_t delta, uint64_t rem, uint64_t kappa, uint64_t frac)
    {
        while (rem < frac && delta - rem >= kappa &&
               (rem + kappa < frac || frac - rem > rem + kappa - frac)) {

            digits[ndigits - 1]--;
            rem += kappa;
        }
    }

    int generate_digits(TCSB_Fp* fp, TCSB_Fp* upper, TCSB_Fp* lower, char* digits, int* K)
    {
        uint64_t wfrac = upper->frac - fp->frac;
        uint64_t delta = upper->frac - lower->frac;

        TCSB_Fp one;
        one.frac = 1ULL << -upper->exp;
        one.exp  = upper->exp;

        uint64_t part1 = upper->frac >> -one.exp;
        uint64_t part2 = upper->frac & (one.frac - 1);

        int idx = 0, kappa = 10;
        uint64_t* divp;
        /* 1000000000 */

        for(divp = (uint64_t*)tens + 10; kappa > 0; divp++) {

            uint64_t div = *divp;
            unsigned digit = part1 / div;

            if (digit || idx) {
                digits[idx++] = digit + '0';
            }

            part1 -= digit * div;
            kappa--;

            uint64_t tmp = (part1 <<-one.exp) + part2;
            if (tmp <= delta) {
                *K += kappa;
                round_digit(digits, idx, delta, tmp, div << -one.exp, wfrac);

                return idx;
            }
        }

        /* 10 */
        uint64_t* unit = (uint64_t*)tens + 18;

        while(true) {
            part2 *= 10;
            delta *= 10;
            kappa--;

            unsigned digit = part2 >> -one.exp;
            if (digit || idx) {
                digits[idx++] = digit + '0';
            }

            part2 &= one.frac - 1;
            if (part2 < delta) {
                *K += kappa;
                round_digit(digits, idx, delta, part2, one.frac, wfrac * *unit);
                return idx;
            }

            unit--;
        }
    }

    int grisu2(double d, char* digits, int* K)
    {
        TCSB_Fp w = build_fp(d);

        TCSB_Fp lower, upper;
        get_normalized_boundaries(&w, &lower, &upper);

        normalize(&w);

        int k;
        TCSB_Fp cp = find_cachedpow10(upper.exp, &k);

        w     = multiply(&w,     &cp);
        upper = multiply(&upper, &cp);
        lower = multiply(&lower, &cp);

        lower.frac++;
        upper.frac--;

        *K = -k;

        return generate_digits(&w, &upper, &lower, digits, K);
    }

    int emit_digits(char* digits, int ndigits, char* dest, int K, bool neg)
    {
        int exp = TCDB_absv(K + ndigits - 1);

        /* write plain integer */
        if(K >= 0 && (exp < (ndigits + 7))) {
            memcpy(dest, digits, ndigits);
            memset(dest + ndigits, '0', K);

            return ndigits + K;
        }

        /* write decimal w/o scientific notation */
        if(K < 0 && (K > -7 || exp < 4)) {
            int offset = ndigits - TCDB_absv(K);
            /* fp < 1.0 -> write leading zero */
            if(offset <= 0) {
                offset = -offset;
                dest[0] = '0';
                dest[1] = '.';
                memset(dest + 2, '0', offset);
                memcpy(dest + offset + 2, digits, ndigits);

                return ndigits + 2 + offset;

                /* fp > 1.0 */
            } else {
                memcpy(dest, digits, offset);
                dest[offset] = '.';
                memcpy(dest + offset + 1, digits + offset, ndigits - offset);

                return ndigits + 1;
            }
        }

        /* write decimal w/ scientific notation */
        ndigits = TCSB_minv(ndigits, 18 - neg);

        int idx = 0;
        dest[idx++] = digits[0];

        if(ndigits > 1) {
            dest[idx++] = '.';
            memcpy(dest + idx, digits + 1, ndigits - 1);
            idx += ndigits - 1;
        }

        dest[idx++] = 'e';

        char sign = K + ndigits - 1 < 0 ? '-' : '+';
        dest[idx++] = sign;

        int cent = 0;

        if(exp > 99) {
            cent = exp / 100;
            dest[idx++] = cent + '0';
            exp -= cent * 100;
        }
        if(exp > 9) {
            int dec = exp / 10;
            dest[idx++] = dec + '0';
            exp -= dec * 10;

        } else if(cent) {
            dest[idx++] = '0';
        }

        dest[idx++] = exp % 10 + '0';

        return idx;
    }

    int filter_special(double fp, char* dest)
    {
        if(fp == 0.0) {
            dest[0] = '0';
            return 1;
        }

        uint64_t bits = get_dbits(fp);

        bool nan = (bits & TCSB_expmask) == TCSB_expmask;

        if(!nan) {
            return 0;
        }

        if(bits & TCSB_fracmask) {
            dest[0] = 'n'; dest[1] = 'a'; dest[2] = 'n';
        } else {
            dest[0] = 'i'; dest[1] = 'n'; dest[2] = 'f';
        }

        return 3;
    }

    size_t fpconv_dtoa(double d, char dest[24])
    {
        char digits[18];

        size_t str_len = 0;
        bool neg = false;

        if(get_dbits(d) & TCSB_signmask) {
            dest[0] = '-';
            str_len++;
            neg = true;
        }

        int spec = filter_special(d, dest + str_len);

        if(spec) {
            return str_len + spec;
        }

        int K = 0;
        int ndigits = grisu2(d, digits, &K);

        str_len += emit_digits(digits, ndigits, dest + str_len, K, neg);

        return str_len;
    }

public:

    size_t addf(double value)
    {
        // TODO now buffer have to have at least 24 bytes left while in most of the cases it should be less
        if(_bufferSize - _cursor < 24) {
            _isOverflow = true;
            return 0;
        }

        size_t size = fpconv_dtoa(value, &_buffer[_cursor]);
        _cursor += size;
        setStringEnd();
        return size;
    }


    template<class CharConstPtr>
    size_t daddf(CharConstPtr array, std::size_t size, double value) { return add(array, size) + addf(value); }

    template <std::size_t SIZE>
    size_t daddf(char (&array)[SIZE], double value){ return add(array) + addf(value); }

    template <std::size_t SIZE>
    size_t daddf(const char (&array)[SIZE], double value){ return add(array) + addf(value); }


#endif //#if TCSB_USE_FP

#if TCSB_USE_SEPARATOR
    /** Adds separator in the beginning if this is not the first addition*/
    template <typename IntType>
    size_t addIntValue(IntType n)
    {
        if(_cursor!=0) return addSeparator() + IntTypeToS(n);   // add separator (' ') by def. if not the first line
        return IntTypeToS(n);
    }


    template <std::size_t SIZE>
    size_t addValue(const char (&array)[SIZE]){
        return sadd(array, SIZE) + addSeparator();
    }


    template<class CharConstPtr, typename = typename std::enable_if<std::is_same<CharConstPtr, char const*>::value>>
    size_t sadd(CharConstPtr array, std::size_t size) {
        if(_cursor!=0) return addSeparator() + add(array, size);   // add separator (' ') by def. if not the first line
        return add(array, size);
    }


    size_t addValue(int8_t value)   { return addIntValue<int8_t>(value)   ;}   /// Converts int8_t to string and adds to the buffer
    size_t addValue(uint8_t value)  { return addIntValue<uint8_t>(value)  ;}   /// Converts uint8_t to string and adds to the buffer
    size_t addValue(int16_t value)  { return addIntValue<int16_t>(value)  ;}   /// Converts int16_t to string and adds to the buffer
    size_t addValue(uint16_t value) { return addIntValue<uint16_t>(value) ;}   /// Converts uint16_t to string and adds to the buffer
    size_t addValue(int32_t value)  { return addIntValue<int32_t>(value)  ;}   /// Converts int32_t to string and adds to the buffer
    size_t addValue(uint32_t value) { return addIntValue<uint32_t>(value) ;}   /// Converts uint32_t to string and adds to the buffer
    size_t addValue(int64_t value)  { return addIntValue<int64_t>(value)  ;}   /// Converts int64_t to string and adds to the buffer
    size_t addValue(uint64_t value) { return addIntValue<uint64_t>(value) ;}   /// Converts uint64_t to string and adds to the buffer


    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], int8_t value  )  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<int8_t>(value)   ; }   /// Converts int8_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], uint8_t value )  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<uint8_t>(value)  ; }   /// Converts uint8_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], int16_t value )  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<int16_t>(value)  ; }   /// Converts int16_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], uint16_t value)  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<uint16_t>(value) ; }   /// Converts uint16_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], int32_t value )  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<int32_t>(value)  ; }   /// Converts int32_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], uint32_t value)  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<uint32_t>(value) ; }   /// Converts uint32_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], int64_t value )  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<int64_t>(value)  ; }   /// Converts int64_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const char (&array)[SIZE], uint64_t value)  { return
                sadd(array, SIZE) + addSeparator() + IntTypeToS<uint64_t>(value) ; }   /// Converts uint64_t to string and adds to the buffer

    size_t addSeparator(char sepCh)
    {
        if(_bufferSize - 1 == _cursor) return 0;
        _buffer[_cursor] = (char)sepCh;
        _cursor++;
        setStringEnd();
        return 1;
    }

    size_t addSeparator() { return addSeparator(_separator);}


#endif  // #if TCSB_USE_SEPARATOR
};

    //If class includes floating point, we have to define this two static arrays
#if TCSB_USE_FP
    
    constexpr const TCSB_Fp CStringBuilder::powers_ten[TCSB_npowers];

    
    constexpr const uint64_t CStringBuilder::tens[TCSB_tens_len];
#endif //#if TCSB_USE_FP


//typedef BasicArrayWriter<wchar_t> WArrayWriter;

#if !defined( TCSB_NO_NAMESPACE )
}   //namespace tcsb {
#endif

#endif //TEMPLATE_CSTRINGBUILDER_HPP
