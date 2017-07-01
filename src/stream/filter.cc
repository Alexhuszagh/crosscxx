//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <stream/filter.h>
#include <algorithm>
#include <cstring>

// FUNCTIONS
// ---------


/**
 *  \brief Empty callback, copy src to dst.
 */
static void null_callback(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = std::min(srclen, dstlen) * char_size;
    const char* src_ = reinterpret_cast<const char*>(src);
    char* dst_ = reinterpret_cast<char*>(dst);

    // copy bytes
    while (bytes--) {
        *dst_++ = *src_++;
    }

    // reassign to buffer
    src = (const void*) src_;
    dst = (void*) dst_;
}

// OBJECTS
// -------

// STREAMBUF

filter_streambuf::filter_streambuf(std::streambuf* f, filter_callback c):
    filebuf(f),
    callback(null_callback)
{
    in_buffer = new char_type[buffer_size];
    out_buffer = new char_type[buffer_size];
    setg(0, 0, 0);
    setp(out_buffer, out_buffer + buffer_size);
    set_callback(c);
}


filter_streambuf::~filter_streambuf()
{
    sync();
    delete[] in_buffer;
    delete[] out_buffer;
}


filter_streambuf::filter_streambuf(filter_streambuf&& other)
{
    swap(other);
}


filter_streambuf& filter_streambuf::operator=(filter_streambuf&& other)
{
    swap(other);
    return *this;
}


void filter_streambuf::swap(filter_streambuf& other)
{
    std::streambuf::swap(other);
    std::swap(filebuf, other.filebuf);
    std::swap(in_buffer, other.in_buffer);
    std::swap(in_first, other.in_first);
    std::swap(in_last, other.in_last);
    std::swap(out_buffer, other.out_buffer);
}


auto filter_streambuf::underflow() -> int_type
{
    size_t distance;
    std::streamsize read, converted;

    if (filebuf) {
        if (in_first == 0) {
            read = filebuf->sgetn(in_buffer, buffer_size);
            in_first = in_buffer;
            in_last = in_buffer + read;
        }

        // convert bytes
        distance = std::distance(in_first, in_last);
        const void* src = (const void*) in_first;
        void* dst = (void*) out_buffer;
        callback(src, distance, dst, buffer_size, sizeof(char_type));
        read = std::distance(in_first, (char*)src);
        converted = std::distance(out_buffer, (char*)dst);

        // store state
        if (read < distance) {
            // overflow in bytes written to dst, store state
            in_first += read;
        } else {
            // fully converted
            in_first = nullptr;
            in_last = nullptr;
        }

        // set get input pointer
        if (!converted) {
            return traits_type::eof();
        }
        setg(out_buffer, out_buffer, out_buffer + converted);

        return traits_type::to_int_type(*gptr());
    }
    return traits_type::eof();
}


auto filter_streambuf::overflow(int_type c) -> int_type
{
    if (filebuf) {
        int write = pptr() - pbase();
        if (write) {
            if (filebuf->sputn(out_buffer, write) != write)  {
                return traits_type::eof();
            }
        }

        // write to output buffer
        setp(out_buffer, out_buffer + buffer_size);
        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            sputc(c);
        }
        return traits_type::not_eof(c);
    }

    return traits_type::eof();
}


int filter_streambuf::sync()
{
    auto result = overflow(traits_type::eof());
    if (filebuf) {
        filebuf->pubsync();
    }

    if (traits_type::eq_int_type(result, traits_type::eof())) {
        return -1;
    }
    return 0;
}


auto filter_streambuf::seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode mode) -> pos_type
{
    pos_type out;
    if (filebuf) {
        out = filebuf->pubseekoff(off, way, mode);
        in_first = in_last = nullptr;
    } else {
        out = pos_type(off_type(-1));
    }

    return out;
}


auto filter_streambuf::seekpos(pos_type pos, std::ios_base::openmode mode) -> pos_type
{
    pos_type out;
    if (filebuf) {
        out = filebuf->pubseekpos(pos, mode);
        in_first = in_last = nullptr;
    } else {
        out = pos_type(off_type(-1));
    }

    return out;
}


void filter_streambuf::set_filebuf(std::streambuf* f)
{
    filebuf = f;
}


void filter_streambuf::set_callback(filter_callback c)
{
    if (c) {
        callback = c;
    } else {
        callback = null_callback;
    }
}


// ISTREAM


filter_istream::filter_istream(filter_callback c):
    buffer(nullptr, c),
    std::istream(&buffer)
{}


filter_istream::~filter_istream()
{}


filter_istream::filter_istream(std::istream& stream, filter_callback c):
    buffer(nullptr, c),
    std::istream(&buffer)
{
    open(stream, c);
}


void filter_istream::open(std::istream& stream, filter_callback c)
{
    this->stream = &stream;
    buffer.set_filebuf(stream.rdbuf());
    buffer.set_callback(c);
}


filter_istream::filter_istream(filter_istream&& other):
    std::istream(std::move(other)),
    stream(std::move(other.stream)),
    buffer(std::move(other.buffer))
{
    std::ios::rdbuf(&buffer);
}


filter_istream & filter_istream::operator=(filter_istream&& other)
{
    swap(other);
    return *this;
}


filter_streambuf* filter_istream::rdbuf() const
{
    return const_cast<filter_streambuf*>(&buffer);
}


void filter_istream::rdbuf(filter_streambuf *buffer)
{
    std::ios::rdbuf(buffer);
}


void filter_istream::swap(filter_istream &other)
{
    // swap
    std::istream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(stream, other.stream);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// OSTREAM

filter_ostream::filter_ostream(filter_callback c):
    buffer(nullptr, c),
    std::ostream(&buffer)
{}


filter_ostream::~filter_ostream()
{}


filter_ostream::filter_ostream(std::ostream& stream, filter_callback c):
    buffer(nullptr, c),
    std::ostream(&buffer)
{
    open(stream, c);
}


void filter_ostream::open(std::ostream& stream, filter_callback c)
{
    this->stream = &stream;
    buffer.set_filebuf(stream.rdbuf());
    buffer.set_callback(c);
}


filter_ostream::filter_ostream(filter_ostream&& other):
    std::ostream(std::move(other)),
    stream(std::move(other.stream)),
    buffer(std::move(other.buffer))
{
    std::ios::rdbuf(&buffer);
}


filter_ostream & filter_ostream::operator=(filter_ostream&& other)
{
    swap(other);
    return *this;
}


filter_streambuf* filter_ostream::rdbuf() const
{
    return const_cast<filter_streambuf*>(&buffer);
}


void filter_ostream::rdbuf(filter_streambuf *buffer)
{
    std::ios::rdbuf(buffer);
}


void filter_ostream::swap(filter_ostream &other)
{
    // swap
    std::ostream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(stream, other.stream);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// IFSTREAM

filter_ifstream::filter_ifstream(filter_callback c):
    filter_istream(file, c)
{}


filter_ifstream::~filter_ifstream()
{
    close();
}


filter_ifstream::filter_ifstream(filter_ifstream &&other):
    filter_istream(file)
{
    swap(other);
}


filter_ifstream & filter_ifstream::operator=(filter_ifstream &&other)
{
    swap(other);
    return *this;
}


filter_ifstream::filter_ifstream(const std::string &name, std::ios_base::openmode mode, filter_callback c):
    filter_istream(file, c)
{
    open(name, mode, c);
}


void filter_ifstream::open(const std::string &name, std::ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#ifdef HAVE_WFOPEN

filter_ifstream::filter_ifstream(const std::wstring &name, std::ios_base::openmode mode, filter_callback c):
    filter_istream(file, c)
{
    open(name, mode, c);
}

void filter_ifstream::open(const std::wstring &name, std::ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#endif


bool filter_ifstream::is_open() const
{
    return file.is_open();
}


void filter_ifstream::close()
{
    file.close();
}


void filter_ifstream::swap(filter_ifstream &other)
{
    filter_istream::swap(other);
    std::swap(file, other.file);
}

// OFSTREAM

filter_ofstream::filter_ofstream(filter_callback c):
    filter_ostream(file, c)
{}


filter_ofstream::~filter_ofstream()
{
    close();
}


filter_ofstream::filter_ofstream(filter_ofstream &&other):
    filter_ostream(file)
{
    swap(other);
}


filter_ofstream & filter_ofstream::operator=(filter_ofstream &&other)
{
    swap(other);
    return *this;
}


filter_ofstream::filter_ofstream(const std::string &name, std::ios_base::openmode mode, filter_callback c):
    filter_ostream(file, c)
{
    open(name, mode, c);
}

void filter_ofstream::open(const std::string &name, std::ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#ifdef HAVE_WFOPEN

filter_ofstream::filter_ofstream(const std::wstring &name, std::ios_base::openmode mode, filter_callback c):
    filter_ostream(file, c)
{
    open(name, mode, c);
}

void filter_ofstream::open(const std::wstring &name, std::ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#endif


bool filter_ofstream::is_open() const
{
    return file.is_open();
}


void filter_ofstream::close()
{
    file.close();
}


void filter_ofstream::swap(filter_ofstream &other)
{
    filter_ostream::swap(other);
    std::swap(file, other.file);
}