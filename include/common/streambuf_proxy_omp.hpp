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
 * Streambuffer proxy to capture everything sent to a stream in a multithreaded context with
 * OpenMP.
 */
class streambuf_proxy_omp : public std::streambuf
{
#define CURRENT_THREAD_BUFFER() (m_thd_buffers.at(static_cast<std::size_t>(omp_get_thread_num())))

public:
    /**
     * Replace the underlying buffer of the stream.
     * As long as this object lives, everything sent to be stream is captured.
     */
    streambuf_proxy_omp(std::ostream& stream_)
        : m_orig_buf(stream_.rdbuf(this)),
          m_orig_stream(stream_),
          m_thd_buffers(static_cast<std::size_t>(omp_get_max_threads()))
    {}

    /**
     * Restore the original buffer of the stream.
     * After that, the stream is in its original state.
     */
    virtual ~streambuf_proxy_omp() noexcept override
    {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    /**
     * Get the current buffer content for the executing thread.
     */
    std::string str() const
    {
        return CURRENT_THREAD_BUFFER().str();
    }

    /**
     * Clear the buffer for the executing thread.
     */
    void clear()
    {
        CURRENT_THREAD_BUFFER().str("");
    }

protected:
    virtual int_type overflow(int_type c_) override
    {
        return CURRENT_THREAD_BUFFER().sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    virtual std::streamsize xsputn(char const* s_, std::streamsize n_) override
    {
        return CURRENT_THREAD_BUFFER().sputn(s_, n_);
    }

    std::streambuf*             m_orig_buf;
    std::ostream&               m_orig_stream;
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
