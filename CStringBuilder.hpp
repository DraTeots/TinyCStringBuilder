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
#include <string.h>

#pragma diag_suppress  186

# define ETB_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    TypeName& operator=(const TypeName&) = delete

#if !defined( CSTRINGBUILDER_BASIC )
#define CSTRINGBUILDER_EXTENDED
#endif

template <typename CharType>
class BasicCStringBuilder {
    ETB_DISALLOW_COPY_AND_ASSIGN(BasicCStringBuilder);

private:
    CharType* _buffer;
    CharType _separator = (CharType)' ';
    std::size_t _bufferSize;
    std::size_t _cursor;

    void setStringEnd() {
        _buffer[_cursor] = '\0';
    }

    /* reverse:  reverse string s in place */
    void reverse(char s[])
    {
        int i, j;
        char c;

        for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
            c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }


    /** K&R int to string conversion */
    template <typename IntType>
    size_t IntTypeToS(IntType n)
    {
        size_t i;
        bool isNegative;
        char *s = &_buffer[_cursor];

        /* record sign */
        isNegative = n < 0;
        if ( isNegative) {
            _cursor++;       /* increase length fo 1 */
            n = -n;          /* make n positive */
        }

        i = 0;
        do {       /* generate digits in reverse order */
            s[i++] = n % 10 + '0';   /* get next digit */
            n /= 10;

            _cursor++;

        } while (n > 0 && _cursor < _bufferSize-1);     /* delete it */

        // Add negative sign!
        if (isNegative) {
            s[i++] = '-';
        }

        s[i] = '\0';
        reverse(s);
        return i;
    }

    /** Adds separator in the beginning if this is not the first addition*/
    template <typename IntType>
    size_t addIntValue(IntType n)
    {
        if(_cursor!=0) return addSeparator() + IntTypeToS(n);   // add separator (' ') by def. if not the first line
        return IntTypeToS(n);
    }

public:
    /**
     \rst
     Constructs a :class:`fmt::BasicArrayWriter` object for *array* of the
     given size.
     \endrst
     */
    BasicCStringBuilder(CharType *buffer, std::size_t bufferSize) {
        _buffer = buffer;
        _bufferSize = bufferSize;
        _cursor = 0;
        setStringEnd();
    }

    /**
     \rst
     Constructs a :class:`fmt::BasicArrayWriter` object for *array* of the
     size known at compile time.
     \endrst
     */
    template <std::size_t SIZE>
    explicit BasicCStringBuilder(CharType (&array)[SIZE])
    {
        _buffer = array;
        _bufferSize = SIZE;
        _cursor = 0;
        setStringEnd();
    }


    /** returns current size of the _buffer */
    size_t bufferSize(void) { return _bufferSize; }


    /** The actual size of the resulting string */
    size_t size(void) { return _cursor; }


    template<class CharConstPtr, typename = typename std::enable_if<std::is_same<CharConstPtr, CharType const*>::value>>
    size_t appendCStr(CharConstPtr array, std::size_t size) {
        if(_bufferSize - 1 == _cursor) return 0;
        size_t i;

        for(i=0; i<strlen(array) && i<size ; i++) {
            if(_bufferSize - 1 == _cursor) break;
            _buffer[_cursor] = array[i];
            _cursor++;
        }
        setStringEnd();
        return i;
    }

    size_t addCStr(const CharType *array, std::size_t size) {
        if(_cursor!=0) return addSeparator() + appendCStr(array, size);   // add separator (' ') by def. if not the first line
        return appendCStr(array, size);
    }

    template <std::size_t SIZE>
    size_t append(CharType (&array)[SIZE]){
        return appendCStr(array, SIZE);
    }

    template <std::size_t SIZE>
    size_t append(const CharType (&array)[SIZE]){
        return appendCStr(array, SIZE);
    }

    size_t append(int8_t value)   { return IntTypeToS<int8_t>(value); }     /// Converts int8_t to string and adds to the buffer
    size_t append(uint8_t value)  { return IntTypeToS<uint8_t>(value); }    /// Converts uint8_t to string and adds to the buffer
    size_t append(int16_t value)  { return IntTypeToS<int16_t>(value); }    /// Converts int16_t to string and adds to the buffer
    size_t append(uint16_t value) { return IntTypeToS<uint16_t>(value); }   /// Converts uint16_t to string and adds to the buffer
    size_t append(int32_t value)  { return IntTypeToS<int32_t>(value); }    /// Converts int32_t to string and adds to the buffer
    size_t append(uint32_t value) { return IntTypeToS<uint32_t>(value); }   /// Converts uint32_t to string and adds to the buffer
    size_t append(int64_t value)  { return IntTypeToS<int64_t>(value); }    /// Converts int64_t to string and adds to the buffer
    size_t append(uint64_t value) { return IntTypeToS<uint64_t>(value); }   /// Converts uint64_t to string and adds to the buffer

    template <std::size_t SIZE>
    size_t addValue(const CharType (&array)[SIZE]){
        return addCStr(array, SIZE) + addSeparator();
    }

    size_t addValue(int8_t value)   { return addIntValue<int8_t>(value)   ;}   /// Converts int8_t to string and adds to the buffer
    size_t addValue(uint8_t value)  { return addIntValue<uint8_t>(value)  ;}   /// Converts uint8_t to string and adds to the buffer
    size_t addValue(int16_t value)  { return addIntValue<int16_t>(value)  ;}   /// Converts int16_t to string and adds to the buffer
    size_t addValue(uint16_t value) { return addIntValue<uint16_t>(value) ;}   /// Converts uint16_t to string and adds to the buffer
    size_t addValue(int32_t value)  { return addIntValue<int32_t>(value)  ;}   /// Converts int32_t to string and adds to the buffer
    size_t addValue(uint32_t value) { return addIntValue<uint32_t>(value) ;}   /// Converts uint32_t to string and adds to the buffer
    size_t addValue(int64_t value)  { return addIntValue<int64_t>(value)  ;}   /// Converts int64_t to string and adds to the buffer
    size_t addValue(uint64_t value) { return addIntValue<uint64_t>(value) ;}   /// Converts uint64_t to string and adds to the buffer


    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], int8_t value  )  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<int8_t>(value)   ; }   /// Converts int8_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], uint8_t value )  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<uint8_t>(value)  ; }   /// Converts uint8_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], int16_t value )  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<int16_t>(value)  ; }   /// Converts int16_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], uint16_t value)  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<uint16_t>(value) ; }   /// Converts uint16_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], int32_t value )  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<int32_t>(value)  ; }   /// Converts int32_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], uint32_t value)  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<uint32_t>(value) ; }   /// Converts uint32_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], int64_t value )  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<int64_t>(value)  ; }   /// Converts int64_t to string and adds to the buffer
    template <std::size_t SIZE> size_t addValue(const CharType (&array)[SIZE], uint64_t value)  { return addCStr(array, SIZE) + addSeparator() + IntTypeToS<uint64_t>(value) ; }   /// Converts uint64_t to string and adds to the buffer
    size_t addSeparator(CharType sepCh)
    {
        if(_bufferSize - 1 == _cursor) return 0;
        _buffer[_cursor] = (CharType)sepCh;
        _cursor++;
        setStringEnd();
        return 1;
    }

    size_t addSeparator() { return addSeparator(_separator);}

    char * cStr() { return _buffer;}
};

typedef BasicCStringBuilder<char> CStringBuilder;
//typedef BasicArrayWriter<wchar_t> WArrayWriter;


#endif //TEMPLATE_CSTRINGBUILDER_HPP
