/*****************************************************************************
* Copyright (c) 2010-2012, Sergey Radionov <rsatom_gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the <organization> nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "hexcodec.h"

////////////////////////////////////////////////////////////////////////
//HexEncode
////////////////////////////////////////////////////////////////////////
template<typename CharT>
void HexEncodeTmpl(const void* in, size_t in_len, CharT* out)
{
    const unsigned char* _in = reinterpret_cast<const unsigned char*>(in);

    //для возможности inplace кодирования, выполняем кодирование с конца
    for( ; in_len>0 ; --in_len ) {
        unsigned char _char   = _in[in_len-1] & 0x0F;
        out[ (in_len-1)*2+1 ] = (_char<10) ? '0'+_char : _char-10+'a';
        _char                 = _in[in_len-1] >> 4;
        out[ (in_len - 1)*2 ] = (_char<10) ? '0'+_char : _char-10+'a';
    }
}
void HexEncode(const void* in, size_t in_len, char* out)
{
    HexEncodeTmpl(in, in_len, out);
}

void HexEncode(const void* in, size_t in_len, wchar_t* out)
{
    HexEncodeTmpl(in, in_len, out);
}

////////////////////////////////////////////////////////////////////////
//HexDecode
////////////////////////////////////////////////////////////////////////
template<typename CharT>
inline bool HexDecodeTmpl(const CharT* in, size_t in_len, void* out)
{
    unsigned char* _out=reinterpret_cast<unsigned char*>(out);

    if( in_len&1 ) {//нечетное количество символов
        --in_len;//делаем четным (т.е. последний символ считаем ошибочным)
    }

    for( ; in_len ; --in_len, ++in ) {
        CharT in_ch = (*in);
        unsigned char out_ch = 0;
        if( in_ch>='0' && in_ch<='9' ) out_ch = in_ch-'0';
        else if( in_ch>='a' && in_ch<='f' ) out_ch = static_cast<unsigned char>(in_ch-'a')+10;
        else if( in_ch>='A' && in_ch<='F' ) out_ch = static_cast<unsigned char>(in_ch-'A')+10;
        else return false;

        if( 0 == ( in_len&1 ) )//первый символ HEX пары
            ( *_out ) = ( out_ch<<4 );//выставляем старшие 4 бита
        else{ //второй символ HEX пары
            ( *_out ) |= out_ch;//выставляем младшие 4 бита
            ++_out;//передвигаемся к следующему символу
        }
    }
    return true;
}

bool HexDecode(const char* in, size_t in_len, void* out)
{
    return HexDecodeTmpl(in, in_len, out);
}

bool HexDecode(const wchar_t* in, size_t in_len, void* out)
{
    return HexDecodeTmpl(in, in_len, out);
}

////////////////////////////////////////////////////////////////////////
//IsHex
////////////////////////////////////////////////////////////////////////
bool IsHex(const void* in, size_t in_len)
{
    if( !in_len ) return false;

    const unsigned char* _in = reinterpret_cast<const unsigned char*>(in);

    for( ; in_len > 0 ; --in_len ){
        unsigned char _char = _in[in_len-1];
        if( !( ( _char>='0' && _char<='9' ) ||
               ( _char>='a' && _char<='f' ) ||
               ( _char>='A' && _char<='F' ) ) )
        {
            return false;
        }
    }
    return true;
}
