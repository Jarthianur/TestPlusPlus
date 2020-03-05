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

#ifndef SCTF_COMMON_STREAMBUF_PROXY_HPP
#define SCTF_COMMON_STREAMBUF_PROXY_HPP

#include <ostream>
#include <sstream>
#include <streambuf>

namespace sctf
{
namespace private_
{
/**
 * Streambuffer proxy to capture everything sent to a stream.
 */
class streambuf_proxy : public std::streambuf
{
public:
    /**
     * Replace the underlying buffer of the stream.
     * As long as this object lives, everything sent to be stream is captured.
     */
    streambuf_proxy(std::ostream& stream_)
        : m_orig_buf(stream_.rdbuf(this)), m_orig_stream(stream_) {}

    /**
     * Restore the original buffer of the stream.
     * After that, the stream is in its original state.
     */
    virtual ~streambuf_proxy() noexcept override {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    /**
     * Get the current buffer content.
     */
    std::string str() const {
        return m_buffer.str();
    }

    /**
     * Clear the buffer.
     */
    void clear() {
        m_buffer.str("");
    }

protected:
    virtual int_type overflow(int_type c_) override {
        return m_buffer.sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    virtual std::streamsize xsputn(char const* s_, std::streamsize n_) override {
        return m_buffer.sputn(s_, n_);
    }

    std::streambuf* m_orig_buf;
    std::ostream&   m_orig_stream;
    std::stringbuf  m_buffer;
};
}  // namespace private_
}  // namespace sctf

#endif  // SCTF_COMMON_STREAMBUF_PROXY_HPP
