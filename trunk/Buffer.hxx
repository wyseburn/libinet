/**
 *  Version:     @(#)Buffer.hxx    0.0.2 01/06/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     A Wrapper class for data operation 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty 
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 *  the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
 *  Boston, MA 02111-1307  USA
 */

#ifndef __INET_BUFFER_H__
#define __INET_BUFFER_H__

#include <cassert>
#include <string>

#include "Compat.hxx"

namespace INet
{
    struct Buffer 
    {
        UInt32            mReadPos;
        UInt32            mWritePos;
        char*             mData;
        UInt32            mSize;

        Buffer(void* data, UInt32 dataLen) 
            : mData((char *)data), mSize(dataLen), mReadPos(0), mWritePos(0) {}
        ~Buffer() {}

        UInt32 blankLen() { return mSize - mWritePos; }
        Int32 remainDataLen() { return mWritePos - mReadPos; }

        void append(const UInt8* src, UInt32 len)
        {
            assert(mWritePos + len <= mSize);
            memcpy(mData + mWritePos, src, len);
            mWritePos += len;
        }

        template <typename _T_> 
        void append(_T_ value) { append((UInt8 *)&value, sizeof(value)); }

        template <typename _T_>
        _T_ read() 
        {
            assert(mReadPos < mSize);
            _T_ obj =  *((_T_ const *)(mData + mReadPos)); 
            mReadPos += sizeof(_T_);
            return obj;
        }

        Buffer& operator << (bool value) 
        { 
            append<Int8>((Int8)value);  
            return *this; 
        }

        Buffer& operator >> (bool& value) 
        { 
            value = read<Int8>() == 0 ? true : false; 
            return *this; 
        }

        Buffer& operator << (char value) 
        { append<Int8>((char)value); 
            return *this; 
        }

        Buffer& operator >> (char& value) 
        { 
            value = read<char>(); 
            return *this; 
        }
        
        Buffer& operator << (Int8 value) 
        { append<Int8>((Int8)value); 
            return *this; 
        }

        Buffer& operator >> (Int8& value) 
        { 
            value = read<Int8>(); 
            return *this; 
        }

        Buffer& operator << (UInt8 value) 
        { 
            append<UInt8>((UInt8)value); 
            return *this; 
        }
 
        Buffer& operator >> (UInt8& value) 
        {
          value = read<UInt8>(); 
          return *this; 
        }

        Buffer& operator << (Int16 value) 
        { 
            append<Int16>((Int16)value); 
            return *this; 
        }

        Buffer& operator >> (Int16& value) 
        { 
            value = read<Int16>(); 
            return *this; 
        }

        Buffer& operator << (UInt16 value) 
        { 
          append<UInt16>((UInt16)value); 
          return *this; 
        }

        Buffer& operator >> (UInt16& value) 
        { 
           value = read<UInt16>(); 
           return *this; 
        }

        Buffer& operator << (long value) 
        { 
            append<Int32>((long)value); 
            return *this; 
        }

        Buffer& operator >> (long& value)
        { 
            value = read<long>(); 
            return *this; 
        }

        Buffer& operator << (unsigned long value) 
        { 
            append<Int32>((unsigned long)value); 
            return *this; 
        }

        Buffer& operator >> (unsigned long& value)
        { 
            value = read<unsigned long>(); 
            return *this; 
        }

        Buffer& operator << (Int32 value) 
        { 
            append<Int32>((Int32)value); 
            return *this; 
        }

        Buffer& operator >> (Int32& value)
        { 
            value = read<Int32>(); 
            return *this; 
        }

        Buffer& operator << (UInt32 value) 
        { 
            append<UInt32>((UInt32)value); 
            return *this; 
        }

        Buffer& operator >> (UInt32& value)
        { 
            value = read<UInt32>(); 
            return *this; 
        }

        Buffer& operator << (Int64 value)
        { 
            append<Int64>((UInt32)value); 
            return *this; 
        }

        Buffer& operator >> (Int64& value) 
        { 
            value = read<Int64>(); 
            return *this; 
        }

        Buffer& operator << (UInt64 value) 
        { 
            append<UInt64>((UInt64)value); 
            return *this; 
        }

        Buffer& operator >> (UInt64& value) 
        { 
            value = read<UInt64>(); 
            return *this; 
        }

        Buffer& operator << (float value) 
        { 
            append<float>((float)value); 
            return *this; 
        }

        Buffer& operator >> (float& value) 
        { 
            value = read<float>(); 
            return *this; 
        }

        Buffer& operator << (double value) 
        { 
            append<double>((double)value); 
            return *this; 
        }

        Buffer& operator >> (double& value) 
        { 
            value = read<double>(); 
            return *this; 
        }

        Buffer& operator << (const std::string& value)
        {
            append((UInt8 const *)value.c_str(), (UInt32)value.length());
            append((UInt8)0); 
            return *this;
        }

        Buffer& operator << (const char* value)
        {
            append((UInt8 const *)value, value ? (UInt32)strlen(value) : 0);
            append((UInt8)0);
            return *this;
        }

        Buffer& operator >> (std::string& value)
        {
            value.clear();
            while (mReadPos < mSize)
            {
                Int8 c = read<Int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        Buffer& operator << (const wchar_t* value)
        {
            append((UInt8 const *)value, value ? (UInt32)wcslen(value)*2 : 0);
            append((UInt16)0);
            return *this;
        }

        Buffer& operator << (const std::wstring& value)
        {
            append((UInt8 const *)value.c_str(), (UInt32)value.length()*2);
            append((UInt16)0);
            return *this;
        }
 
        Buffer& operator >> (std::wstring& value)
        {
            value.clear();
            while (mReadPos < mSize)
            {
                UInt16 c = read<UInt16>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }
    };
} // namespace

#endif // __INET_BUFFER_H__
