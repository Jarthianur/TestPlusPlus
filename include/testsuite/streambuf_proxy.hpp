/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SCTF_TESTSUITE_STREAMBUF_PROXY_HPP
#define SCTF_TESTSUITE_STREAMBUF_PROXY_HPP

#include <ostream>
#include <sstream>
#include <streambuf>

namespace sctf
{
namespace intern
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
     *
     * @param stream_ is the stream to capture from.
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

    std::streambuf* m_orig_buf;     ///< Target streams original buffer.
    std::ostream&   m_orig_stream;  ///< Target stream
    std::stringbuf  m_buffer;       ///< Buffer to store captured output.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TESTSUITE_STREAMBUF_PROXY_HPP
