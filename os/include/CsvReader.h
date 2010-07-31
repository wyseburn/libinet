/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_CSVREADER_H
#define IGAME_OS_CSVREADER_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include "os/include/Compat.h"

namespace IGame
{
    class CsvReader
    {
    public:
        CsvReader(std::istream& input, std::string sep = ",") : m_InputStream(input), m_FieldSep(sep) {}
        ~CsvReader(){}

        bool GetLine(std::string& line)
        {
            if(!m_InputStream.good())
                return false;
            char c;
            for (m_Line = ""; m_InputStream.get(c) && !EndOfLine(c); )
                m_Line += c;
            Split();
            line = m_Line;
            return !m_InputStream.eof();
        }

        bool ReadLine()
        {
            if(!m_InputStream.good() || m_InputStream.eof())
                return false;
            char c;
            for (m_Line = ""; m_InputStream.get(c) && !EndOfLine(c); )
                m_Line += c;

            if(Split() == 0)
                return false;
            return true;
        }

        bool InitTitle()
        {
            if(!ReadLine()) return false;
            if(m_Fields.size() == 0) return false;
            for(int i = 0; i < m_Fields.size(); i++)
            {
                std::string titleName = m_Fields[i];
                StrTrim(titleName);
                std::transform(titleName.begin(), titleName.end(), titleName.begin(), tolower);
                mColumns.insert(std::make_pair(titleName, i));
            }
            return true;
        }

        bool Bind(const std::string& title, std::string& val)
        {
            std::string titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), tolower);
            Columns::const_iterator cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }
            val = m_Fields[cit->second];
            StrTrim(val);
            return true;
        }
        
        template <typename Type>
        bool Bind(const std::string& title, Type& val)
        {
            val = 0;
            std::string titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), tolower);
            Columns::const_iterator cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }
            std::string strVal = m_Fields[cit->second];
            StrTrim(strVal);

            if (strVal.empty())
            {
                return false;
            }
            val = Atoi<Type>(strVal.c_str());
            return true;
        }

        bool Bind(const std::string& title, bool& val)
        {
            int intVal;
            if (!Bind(title, intVal))
            {
                return false;
            }
            val = intVal == 0 ? false : true;
            return true;
        }

        std::string& GetField(_UInt32 index)
        {
            return m_Fields[index];
        }

        _UInt32 GetFieldNumber() const { return m_FieldNumber; }

    private:
        _UInt32 Split()
        {
            std::string fld;
            _Int32 i, j;
            m_FieldNumber = 0;
            if (m_Line.length() == 0)
                return 0;
            i = 0;
            do {
                if (i < m_Line.length() && m_Line[i] == '"')
                    j = AdvQuoted(m_Line, fld, ++i);	// skip quote
                else
                    j = AdvPlain(m_Line, fld, i);
                if (m_FieldNumber >= m_Fields.size())
                    m_Fields.push_back(fld);
                else
                    m_Fields[m_FieldNumber] = fld;
                m_FieldNumber++;
                i = j + 1;
            } while (j < m_Line.length());

            return m_FieldNumber;
        }

        char EndOfLine(char c)
        {
            char eol;
            eol = (c=='\r' || c=='\n');
            if (c == '\r') 
            {
                m_InputStream.get(c);
                if (!m_InputStream.eof() && c != '\n')
                    m_InputStream.putback(c);	// read too far
            }
            return eol;
        }

        _Int32 AdvPlain(const std::string& line, std::string& fld, _Int32 i)
        {    
            _Int32 j;
            j = line.find_first_of(m_FieldSep, i); // look for separator
            if (j > line.length()) // none found
                j = line.length();
            fld = std::string(line, i, j-i);
            return j;
        }
        _Int32 AdvQuoted(const std::string& line, std::string& fld, _Int32 i)
        {
            _Int32 j;
            fld = "";
            for (j = i; j < line.length(); j++) 
            {
                if (line[j] == '"' && line[++j] != '"') 
                {
                    int k = (int)line.find_first_of(m_FieldSep, j);
                    if (k > (int)line.length())	// no separator found
                        k = (int)line.length();
                    for (k -= j; k-- > 0; )
                        fld += line[j++];
                    break;
                }
                fld += line[j];
            }
            return j;
        }

        std::istream& m_InputStream;            // input file pointer
        std::string m_Line;                     // input m_Line
        std::vector<std::string> m_Fields;      // m_Fields strings
        typedef std::map<std::string, int> Columns;
        Columns mColumns;
        _UInt32 m_FieldNumber;                  // number of fields
        std::string m_FieldSep;                 // separator characters
    };
} // namespace IGame

#endif	// IGAME_OS_CSVREADER_H


