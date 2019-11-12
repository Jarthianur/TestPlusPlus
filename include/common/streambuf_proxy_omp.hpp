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

#ifndef SCTF_COMMON_STREAMBUF_PROXY_OMP_HPP
#define SCTF_COMMON_STREAMBUF_PROXY_OMP_HPP

#include <ostream>
#include <sstream>
#include <streambuf>
#include <vector>

#ifdef _OPENMP
#    include <omp.h>
#else
#    define omp_get_max_threads() 1
#    define omp_get_thread_num() 0
#endif

namespace sctf
{
namespace _
{
/**
 * @brief Streambuffer proxy to capture everything sent to a stream in a multithreaded context with
 * OpenMP.
 */
class streambuf_proxy_omp : public std::streambuf
{
#define CURRENT_THREAD_BUFFER() (m_thd_buffers.at(static_cast<std::size_t>(omp_get_thread_num())))

public:
    /**
     * @brief Switch the underlying buffer on construction.
     * @param stream The stream to capture
     */
    streambuf_proxy_omp(std::ostream& stream_)
        : m_orig_buf(stream_.rdbuf(this)),
          m_orig_stream(stream_),
          m_thd_buffers(static_cast<std::size_t>(omp_get_max_threads()))
    {}

    /**
     * @brief Restore the original buffer on destruction.
     */
    virtual ~streambuf_proxy_omp() noexcept override
    {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    /**
     * @brief Get the current buffer content for the executing thread.
     * @return The the buffer content as string
     */
    std::string str() const
    {
        return CURRENT_THREAD_BUFFER().str();
    }

    /**
     * @brief Clear the buffer for the executing thread.
     */
    void clear()
    {
        CURRENT_THREAD_BUFFER().str("");
    }

protected:
    /**
     * @brief Write a single character into the buffer for the executing thread.
     * @param c The character to write
     * @return the character on success, else EOF
     */
    virtual int_type overflow(int_type c_) override
    {
        return CURRENT_THREAD_BUFFER().sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    /**
     * @brief Write a character sequence into the buffer for the executing thread.
     * @param s The character sequence
     * @param n The length of the sequence
     * @return The number of characters written
     */
    virtual std::streamsize xsputn(char const* s_, std::streamsize n_) override
    {
        return CURRENT_THREAD_BUFFER().sputn(s_, n_);
    }

    /// @brief The original underlying buffer of the captured stream
    std::streambuf* m_orig_buf;

    /// @brief The captured stream
    std::ostream& m_orig_stream;

    /// @brief The internal buffers per thread
    std::vector<std::stringbuf> m_thd_buffers;
};
}  // namespace _
}  // namespace sctf

#ifndef _OPENMP
#    undef omp_get_max_threads
#    undef omp_get_thread_num
#endif
#undef CURRENT_THREAD_BUFFER

#endif  // SCTF_COMMON_STREAMBUF_PROXY_OMP_HPP
