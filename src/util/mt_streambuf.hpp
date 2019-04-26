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

#ifndef SCTF_SRC_UTIL_MT_STREAMBUF_HPP_
#define SCTF_SRC_UTIL_MT_STREAMBUF_HPP_

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

#ifdef _OPENMP
#    include <omp.h>
#endif

namespace sctf
{
namespace util
{
/**
 * @brief The mt_streambuf class
 */
class mt_streambuf final : public std::streambuf
{
public:
    mt_streambuf(const mt_streambuf&) = delete;
    mt_streambuf& operator=(const mt_streambuf&) = delete;

    mt_streambuf(std::ostream& stream)
        : m_orig_buf(stream.rdbuf(this)), m_orig_stream(stream), m_streams(omp_get_max_threads())
    {}

    virtual ~mt_streambuf() noexcept
    {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    std::string get_buf()
    {
        std::cerr << __FUNCTION__ << " called from " << omp_get_thread_num() << std::endl;
        std::stringbuf& buf = m_streams.at(omp_get_thread_num());
        buf.pubsync();
        return buf.str();
    }

protected:
    virtual int overflow(int c) override
    {
        std::cerr << __FUNCTION__ << " called from " << omp_get_thread_num() << std::endl;
        std::stringbuf& buf = m_streams.at(omp_get_thread_num());
        return buf.sputc(c);
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        std::cerr << __FUNCTION__ << " called from " << omp_get_thread_num() << std::endl;
        std::stringbuf& buf = m_streams.at(omp_get_thread_num());
        return buf.sputn(s, n);
    }

private:
    std::streambuf*             m_orig_buf;
    std::ostream&               m_orig_stream;
    std::vector<std::stringbuf> m_streams;
};

}  // namespace util
}  // namespace sctf

#endif  // SCTF_SRC_UTIL_MT_STREAMBUF_HPP_
