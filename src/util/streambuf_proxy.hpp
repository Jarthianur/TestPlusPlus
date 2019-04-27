/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef SCTF_SRC_UTIL_STREAMBUF_PROXY_HPP_
#define SCTF_SRC_UTIL_STREAMBUF_PROXY_HPP_

#include <ostream>
#include <sstream>
#include <streambuf>

namespace sctf
{
namespace util
{
/**
 * @brief The mt_streambuf class
 */
class streambuf_proxy : public std::streambuf
{
public:
    streambuf_proxy(std::ostream& stream) : m_orig_buf(stream.rdbuf(this)), m_orig_stream(stream) {}

    virtual ~streambuf_proxy() noexcept override
    {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    std::string str() const
    {
        return m_buffer.str();
    }

    void clear()
    {
        m_buffer.str("");
    }

protected:
    virtual int_type overflow(int_type c) override
    {
        return m_buffer.sputc(std::stringbuf::traits_type::to_char_type(c));
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        return m_buffer.sputn(s, n);
    }

    // private:
    std::streambuf* m_orig_buf;
    std::ostream&   m_orig_stream;
    std::stringbuf  m_buffer;
};

}  // namespace util
}  // namespace sctf

#endif  // SCTF_SRC_UTIL_STREAMBUF_PROXY_HPP_
