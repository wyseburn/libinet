/**
 *  Version:     @(#)Message.hxx   0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     Message form define
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

#ifndef __INET_MESSAGE_H__
#define __INET_MESSAGE_H__

#include "Compat.hxx"
#include <cassert>

namespace INet
{
    struct MsgHdr
    {
        static const UInt32 sSize = 8; // sizeof(MsgHdr)
        UInt32 mId;                    // Message handler ID   
        UInt32 mLength;                // Message length include header lenght. 
        MsgHdr(UInt32 id = 0) : mId(id), mLength(sSize) {}
    };

    class Message  // Support max message length is 64k
    {
    public:
        static const UInt32 sMaxSize = 1024 * 64;
        Message(UInt32 bodyLen = 1024) 
            : mInPos(MsgHdr::sSize), mOutPos(MsgHdr::sSize), mGCFlag(true) 
        {
            mSize = MsgHdr::sSize + bodyLen;
            mData = (UInt8 *)malloc(mSize);
            assert(mData);
            mHdr = new (mData) MsgHdr();
        }

        Message(UInt32 id, UInt32 bodyLen = 1024)
            : mInPos(MsgHdr::sSize), mOutPos(MsgHdr::sSize), mGCFlag(true) 
        {
            mSize = MsgHdr::sSize + bodyLen;
            mData = (UInt8 *)malloc(mSize);
            assert(mData);
            mHdr = new (mData) MsgHdr(id);
        }

        Message(void* data, UInt32 dataLen)
            : mInPos(MsgHdr::sSize), mOutPos(MsgHdr::sSize), mGCFlag(false) 
        {
            mSize = dataLen;
            mData = (UInt8 *)data;
            assert(mData);
            mHdr = (MsgHdr *)mData;
        }

        virtual ~Message() 
        {
            if (mGCFlag && mData) free(mData);
        }

        void setId(UInt32 id) { mHdr->mId = id; }

        void* get() const { return mData; }
        UInt32 getId() const { return mHdr->mId; }
        UInt32 getLen() const { return mHdr->mLength; }
        UInt32 getInPos() const { return mInPos; }
        UInt32 getOutPos() const { return mOutPos; }

    public:
        void read(void* dst, UInt32 len)
        {
            assert(mOutPos + len < mSize);
            memcpy(dst,  mData + mOutPos, len);
            mOutPos += len;
        }

        void read(void* dst, UInt32 pos /*begin from header*/, UInt32 len)
        {
            assert(pos + len < mSize);
            memcpy(dst, mData + pos, len);
        }

        void* read(UInt32 pos /*begin form header*/, UInt32 len = 0)
        {
            assert(mOutPos + len < mSize);
            return mData + mOutPos;
        }

        void append(const UInt8* src, UInt32 len)
        {
            if (len = 0) return;
            if (mInPos + len > mSize)
            {
                Int32 length = len > 10240 ? len : 10240; 
                realloc(mData, length); 
                assert(mData);
            }

            memcpy(mData + mInPos, src, len);
            mInPos += len;
            mHdr->mLength += len; 
        }

        Message& operator << (bool value) 
        { 
            append<Int8>((Int8)value);  
            return *this; 
        }

        Message& operator >> (bool& value) 
        { 
            value = read<Int8>() == 0 ? true : false; 
            return *this; 
        }

        Message& operator << (Int8 value) 
        { append<Int8>((Int8)value); 
            return *this; 
        }

        Message& operator >> (Int8& value) 
        { 
            value = read<Int8>(); 
            return *this; 
        }

        Message& operator << (UInt8 value) 
        { 
            append<UInt8>((UInt8)value); 
            return *this; 
        }
 
        Message& operator >> (UInt8& value) 
        { 
          value = read<UInt8>(); 
          return *this; 
        }

        Message& operator << (Int16 value) 
        { 
            append<Int16>((Int16)value); 
            return *this; 
        }

        Message& operator >> (Int16& value) 
        { 
            value = read<Int16>(); 
            return *this; 
        }

        Message& operator << (UInt16 value) 
        { 
          append<UInt16>((UInt16)value); 
          return *this; 
        }

        Message& operator >> (UInt16& value) 
        { 
           value = read<UInt16>(); 
           return *this; 
        }

        Message& operator << (Int32 value) 
        { 
            append<Int32>((Int32)value); 
            return *this; 
        }

        Message& operator >> (Int32& value) 
        { 
            value = read<Int32>(); 
            return *this; 
        }

        Message& operator << (UInt32 value) 
        { 
            append<UInt32>((UInt32)value); 
            return *this; 
        }

        Message& operator >> (UInt32& value) 
        { 
            value = read<UInt32>(); 
            return *this; 
        }

        Message& operator << (Int64 value) 
        { 
            append<Int64>((UInt32)value); 
            return *this; 
        }

        Message& operator >> (Int64& value) 
        { 
            value = read<Int64>(); 
            return *this; 
        }

        Message& operator << (UInt64 value) 
        { 
            append<UInt64>((UInt64)value); 
            return *this; 
        }

        Message& operator >> (UInt64& value) 
        { 
            value = read<UInt64>(); 
            return *this; 
        }

        Message& operator << (float value) 
        { 
            append<float>((float)value); 
            return *this; 
        }

        Message& operator >> (float& value) 
        { 
            value = read<float>(); 
            return *this; 
        }

        Message& operator << (double value) 
        { 
            append<double>((double)value); 
            return *this; 
        }

        Message& operator >> (double& value) 
        { 
            value = read<double>(); 
            return *this; 
        }

        Message& operator << (const std::string& value)
        {
            append((UInt8 const *)value.c_str(), (UInt32)value.length());
            append((UInt8)0); 
            return *this;
        }

        Message& operator << (const char* value)
        {
            append((UInt8 const *)value, value ? (UInt32)strlen(value) : 0);
            append((UInt8)0);
            return *this;
        }

        Message& operator >> (std::string& value)
        {
            value.clear();
            while (mOutPos < mSize)
            {
                Int8 c = read<Int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        Message& operator << (const wchar_t* value)
        {
            append((UInt8 const *)value, value ? (UInt32)wcslen(value)*2 : 0);
            append((UInt16)0);
            return *this;
        }

        Message& operator << (const std::wstring& value)
        {
            append((UInt8 const *)value.c_str(), (UInt32)value.length()*2);
            append((UInt16)0);
            return *this;
        }
 
        Message& operator >> (std::wstring& value)
        {
            value.clear();
            while (mOutPos < mSize)
            {
                UInt16 c = read<UInt16>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }
        
    private:
        template <typename _T_> 
        void append(_T_ value) { append((UInt8 *)&value, sizeof(value)); }

        template <typename _T_>
        _T_ read() 
        {
            assert(mOutPos < mSize);
            _T_ obj =  *((_T_ const *)(mData + mOutPos)); 
            mOutPos += sizeof(_T_);
            return obj;
        }
        
        MsgHdr*       mHdr;
        UInt8*        mData;
        UInt32        mSize;
        UInt32        mInPos;
        UInt32        mOutPos;
        bool          mGCFlag; 
    };
} // namespace

#endif // #ifndef __INET_MESSAGE_H__

