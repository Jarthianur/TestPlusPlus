/*
    Copyright (C) 2017 Jarthianur

    This file is part of TestPlusPlus (Test++).

    TestPlusPlus (Test++) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TestPlusPlus (Test++) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TestPlusPlus (Test++).  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TPP_TEST_STREAMBUF_PROXY_HPP
#define TPP_TEST_STREAMBUF_PROXY_HPP

#include <ostream>
#include <sstream>
#include <streambuf>

namespace tpp
{
namespace intern
{
namespace test
{
/**
 * Streambuffer proxy to capture everything sent to a stream.
 */
class streambuf_proxy : public std::streambuf
{
public:
    streambuf_proxy(streambuf_proxy const&)     = delete;
    streambuf_proxy(streambuf_proxy&&) noexcept = delete;
    auto
    operator=(streambuf_proxy const&) -> streambuf_proxy& = delete;
    auto
    operator=(streambuf_proxy&&) noexcept -> streambuf_proxy& = delete;

    /**
     * Replace the underlying buffer of the stream.
     * As long as this object lives, everything sent to be stream is captured.
     *
     * @param stream_ is the stream to capture from.
     */
    explicit streambuf_proxy(std::ostream& stream_) : m_orig_buf(stream_.rdbuf(this)), m_orig_stream(stream_) {}

    /**
     * Restore the original buffer of the stream.
     * After that, the stream is in its original state.
     */
    ~streambuf_proxy() noexcept override {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    /**
     * Get the current buffer content.
     */
    auto
    str() const -> std::string {
        return m_buffer.str();
    }

    /**
     * Clear the buffer.
     */
    void
    clear() {
        m_buffer.str("");
    }

private:
    auto
    overflow(int_type c_) -> int_type override {
        return m_buffer.sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    auto
    xsputn(char const* s_, std::streamsize n_) -> std::streamsize override {
        return m_buffer.sputn(s_, n_);
    }

    std::streambuf* m_orig_buf;     ///< Target streams original buffer.
    std::ostream&   m_orig_stream;  ///< Target stream
    std::stringbuf  m_buffer;       ///< Buffer to store captured output.
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_STREAMBUF_PROXY_HPP
