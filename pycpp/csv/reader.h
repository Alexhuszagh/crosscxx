//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief CSV base reader.
 */

#pragma once

#include <pycpp/csv/punct.h>
#include <pycpp/iterator/input_iterator_facade.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic reader for CSV file.
 *
 *  This reader is mostly analogous to Python's `csv.reader` object.
 *
 *  The punctation can be altered similar to an STL locale using `punctuation()`,
 *  the CSV reader takes ownership of the punct object.
 */
struct csv_stream_reader
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = csv_row;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = input_iterator_facade<csv_stream_reader>;

    // MEMBER FUNCTIONS
    // ----------------
    csv_stream_reader(csvpunct_impl* = nullptr);
    csv_stream_reader(const csv_stream_reader&) = delete;
    csv_stream_reader& operator=(const csv_stream_reader&) = delete;
    csv_stream_reader(csv_stream_reader&&);
    csv_stream_reader& operator=(csv_stream_reader&&);

    // STREAM
    csv_stream_reader(istream&, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(istream&, size_t skip = 0, csvpunct_impl* = nullptr);

    // PROPERTIES/MODIFIERS
    void punctuation(csvpunct_impl*);
    const csvpunct_impl* punctuation() const;
    void swap(csv_stream_reader&);

    // DATA
    value_type operator()();
    bool eof() const;
    explicit operator bool() const;

    // ITERATORS
    iterator begin();
    iterator end();

protected:
    friend struct csv_dict_file_reader;
    friend struct csv_dict_string_reader;
    istream* stream_ = nullptr;
    size_t row_length_ = 0;
    unique_ptr<csvpunct_impl> punct_;
};


/**
 *  \brief Reader for file-based document.
 */
struct csv_file_reader: csv_stream_reader
{
public:
    csv_file_reader(csvpunct_impl* = nullptr);
    csv_file_reader(const csv_file_reader&) = delete;
    csv_file_reader& operator=(const csv_file_reader&) = delete;
    csv_file_reader(csv_file_reader&&);
    csv_file_reader& operator=(csv_file_reader&&);

    // STREAM
    csv_file_reader(const string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    csv_file_reader(const wstring_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const wstring_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    csv_file_reader(const u16string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const u16string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
#endif                                          // WINDOWS

    // MODIFIERS
    void swap(csv_file_reader&);

private:
    ifstream file_;
};


/**
 *  \brief Reader for string-based document.
 */
struct csv_string_reader: csv_stream_reader
{
public:
    csv_string_reader(csvpunct_impl* = nullptr);
    csv_string_reader(const csv_string_reader&) = delete;
    csv_string_reader& operator=(const csv_string_reader&) = delete;
    csv_string_reader(csv_string_reader&&);
    csv_string_reader& operator=(csv_string_reader&&);

    // STREAM
    csv_string_reader(const string_wrapper& str, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const string_wrapper& str, size_t skip = 0, csvpunct_impl* = nullptr);

    // MODIFIERS
    void swap(csv_string_reader&);

private:
    istringstream sstream_;
};

PYCPP_END_NAMESPACE
