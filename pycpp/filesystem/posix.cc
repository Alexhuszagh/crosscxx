//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief POSIX-specific path normalization routines.
 */

#include <pycpp/config.h>
#include <pycpp/os.h>

#if defined(OS_POSIX)                           // POSIX & MACOS
#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
#include <pycpp/unicode.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <wordexp.h>
#include <algorithm>
#include <cstdlib>
#endif

PYCPP_BEGIN_NAMESPACE

#if defined(OS_POSIX)                           // POSIX & MACOS

// HELPERS
// -------


/**
 *  \brief Get path to home directory.
 */
template <typename Path, typename ToPath>
static Path home_impl(ToPath topath)
{
    char *dir = std::getenv("HOME");
    if (dir != nullptr) {
        return topath(dir);
    } else {
        // home not defined, root user
        return topath("/");
    }
}


/**
 *  \brief Get path to temporary directory.
 */
template <typename Path, typename ToPath>
static Path tmpdir_impl(ToPath topath)
{
    char *dir = std::getenv("TMPDIR");
    if (dir != nullptr) {
        return topath(dir);
    }

    dir = std::getenv("TEMP");
    if (dir != nullptr) {
        return topath(dir);
    }

    dir = std::getenv("TMP");
    if (dir != nullptr) {
        return topath(dir);
    } else {
        // temp directory not defined, return root
        return topath("/");
    }
}


/**
 *  \brief Get iterator where last directory separator occurs.
 */
template <typename Path>
static typename Path::const_iterator stem_impl(const Path& path)
{
    typedef typename Path::value_type char_type;

    return std::find_if(path.rbegin(), path.rend(), [](char_type c) {
        return path_separators.find(c) != path_separators.npos;
    }).base();
}


// RUNTIME

/** \brief Join POSIX-compliant paths to create path to full file.
 */
template <typename List, typename ToPath>
static typename List::value_type join_impl(const List &paths, ToPath topath)
{
    typename List::value_type path;
    for (auto &item: paths) {
        if (item.empty()) {
        } else if (path_separators.find(item[0]) != path_separators.npos) {
            path = item;
        } else {
            path += item;
        }
        path += topath(path_separator);
    }

    // clean up trailing separator
    if (path.size()) {
        path.erase(path.length() - 1);
    }

    return path;
}

// SPLIT

template <typename Path>
static std::deque<Path> split_impl(const Path& path)
{
    auto it = stem_impl(path);
    Path basename(it, path.cend());
    Path dir(path.cbegin(), it);
    if (dir.size() > 1 && path_separators.find(dir.back()) != path_separators.npos) {
        dir.erase(dir.length() - 1);
    }

    return {std::move(dir), std::move(basename)};
}


template <typename Path>
std::deque<Path> splitdrive_impl(const Path& path)
{
    return {Path(), path};
}


template <typename Path>
std::deque<Path> splitunc_impl(const Path& path)
{
    return {Path(), path};
}

// NORMALIZATION


template <typename Path>
static Path base_name_impl(const Path& path)
{
    return Path(stem_impl(path), path.cend());
}


template <typename Path>
static Path dir_name_impl(const Path& path)
{
    auto it = stem_impl(path);
    Path dir(path.cbegin(), it);
    if (dir.size() > 1 && path_separators.find(dir.back()) != path_separators.npos) {
        dir.erase(dir.length() - 1);
    }

    return dir;
}


template <typename Path, typename ToPath>
static Path expanduser_impl(const Path& path, ToPath topath)
{
    switch (path.size()) {
        case 0:
            return path;
        case 1:
            return path[0] == '~' ? home_impl<Path>(topath) : path;
        default: {
            if (path[0] == '~' && path_separators.find(path[1]) != path_separators.npos) {
                return home_impl<Path>(topath) + path.substr(1);
            }
            return path;
        }
    }
}


template <typename Path, typename FromPath, typename ToPath>
static Path expandvars_impl(const Path& path, FromPath frompath, ToPath topath)
{
    wordexp_t word;
    wordexp(frompath(path).data(), &word, 0);
    char** ptr = word.we_wordv;
    if (word.we_wordc == 0) {
        wordfree(&word);
        return path;
    } else {
        auto out = topath(ptr[0]);
        wordfree(&word);
        return out;
    }
}


template <typename Path>
bool isabs_impl(const Path& path)
{
    if (path.empty()) {
        return false;
    } else if (path_separators.find(path.front()) != path_separators.npos) {
        return true;
    }

    return false;
}

// MANIPULATION


/**
 *  \brief Use C FILE descriptors for high performance file copying.
 */
static bool copy_file_buffer(const path_t& src, const path_t& dst)
{
    static constexpr size_t length = 4096;

    FILE *in = fopen(src.data(), "rb");
    if (!in) {
        return false;
    }
    FILE *out = fopen(dst.data(), "wb");
    if (!out) {
        return false;
    }

    char* buf = new char[length];
    int read = 0;
    while ((read = fread(buf, 1, length, in)) == length) {
        fwrite(buf, 1, length, out);
    }
    fwrite(buf, 1, read, out);

    delete[] buf;
    fclose(out);
    fclose(in);

    return true;
}


template <typename Path, typename MoveFile>
static bool move_file_impl(const Path& src, const Path& dst, bool replace, MoveFile move)
{
    auto dst_dir = dir_name(dst);

    // ensure we have a file and a dest directory
    auto src_stat = stat(src);
    auto dst_stat = stat(dst_dir);
    if (!isfile(src)) {
        throw filesystem_error(filesystem_not_a_file);
    } else if (!exists(dst_stat)) {
        throw filesystem_error(filesystem_no_such_directory);
    }

    // POSIX rename doesn't work accross filesystems
    // make sure stat data looks like the file was moved
    if (src_stat.st_dev != dst_stat.st_dev) {
        if (copy_file(src, dst, replace)) {
            return copystat(src, dst);
        }
        return false;
    }

    if (exists(dst)) {
        if (replace) {
            remove_file(dst);
        } else {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    return move(src, dst);
}


template <typename Path>
static bool is_empty_directory_impl(const Path& path)
{
    directory_iterator it(path);
    return it == directory_iterator();
}


template <typename Path>
static bool move_dir_impl(const Path& src, const Path& dst, bool replace)
{
    // get stats
    auto src_stat = stat(src);
    auto dst_stat = stat(dst);

    // check if we can move
    if (!isdir(src)) {
        throw filesystem_error(filesystem_no_such_directory);
    } else if (exists(dst_stat)) {
        // destination exists, can we overwrite?
        if (replace) {
            if (!remove_path(dst)) {
                return false;
            }
        } else if (!isdir(dst_stat) || !is_empty_directory_impl(dst)) {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    // check to see if they're on the same device
    if (src_stat.st_dev == dst_stat.st_dev) {
        // same filesystem, guaranteed atomicity
        return rename(src.data(), dst.data()) == 0;
    } else {
        // different filesystems, remove and copy
        if (!copy_dir(src, dst, true)) {
            throw filesystem_error(filesystem_unexpected_error);
        }
        if (!remove_dir(src)) {
            throw filesystem_error(filesystem_unexpected_error);
        }
    }

    return true;
}


template <typename Path, typename Mklink>
static bool mklink_impl(const Path& target, const Path& dst, bool replace, Mklink linker)
{
    if (replace && exists(dst)) {
        remove_file(dst);
    }

    return linker(target, dst);
}


template <typename Path, typename CopyFile>
static bool copy_file_impl(const Path& src, const Path& dst, bool replace, CopyFile copy)
{
    auto dst_dir = dir_name(dst);

    // ensure we have a file and a dest directory
    auto src_stat = stat(src);
    auto dst_stat = stat(dst_dir);
    if (!isfile(src)) {
        throw filesystem_error(filesystem_not_a_file);
    } else if (!exists(dst_stat)) {
        throw filesystem_error(filesystem_no_such_directory);
    }

    if (exists(dst)) {
        if (replace) {
            remove_file(dst);
        } else {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    return copy(src, dst);
}


template <typename Path>
static bool copy_dir_shallow_impl(const Path&src, const Path& dst)
{
    auto src_stat = stat(src);
    if (!exists(src_stat)) {
        throw filesystem_error(filesystem_no_such_directory);
    }

    return mkdir(dst, src_stat.st_mode);
}


template <typename Path>
static bool copy_dir_recursive_impl(const Path&src, const Path& dst)
{
    if (!copy_dir_shallow_impl(src, dst)) {
        return false;
    }

    directory_iterator first(src);
    directory_iterator last;
    for (; first != last; ++first) {
        path_list_t dst_list = {dst, first->basename()};
        if (first->isfile()) {
            if (!copy_file(first->path(), join(dst_list))) {
                return false;
            }
        } else if (first->islink()) {
            if (!copy_link(first->path(), join(dst_list))) {
                return false;
            }
        } else if (first->isdir()) {
            if (!copy_dir_recursive_impl(first->path(), join(dst_list))) {
                return false;
            }
        }
    }

    return true;
}


template <typename Path>
static bool copy_dir_impl(const Path&src, const Path& dst, bool recursive, bool replace)
{
    if (replace && exists(dst)) {
        if (!remove_path(dst)) {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    if (recursive) {
        return copy_dir_recursive_impl(src, dst);
    } else {
        return copy_dir_shallow_impl(src, dst);
    }
}

// FILE UTILS


static int convert_openmode(std::ios_base::openmode mode)
{
    int flags = 0;

    // read/write
    if ((mode & std::ios_base::in) && (mode & std::ios_base::out)) {
        flags |= O_RDWR;
    } else if (mode & std::ios_base::in) {
        flags |= O_WRONLY;
    } else if (mode & std::ios_base::out) {
        flags |= O_RDONLY;
    }

    // create file
    if (mode & std::ios_base::out) {
        flags |= O_CREAT;
    }

    // append
    if (mode & std::ios_base::app) {
        flags |= O_APPEND;
    }

    // truncate
    if (mode & std::ios_base::trunc) {
        flags |= O_TRUNC;
    }

    return flags;
}


template <typename Path>
static bool file_allocate_impl(const Path& path, size_t size)
{
    fd_t fd = file_open(path, std::ios_base::out);
    if (fd < 0) {
        return false;
    }
    bool status = file_allocate(fd, size);
    file_close(fd);

    return status;
}


template <typename Path>
static bool file_truncate_impl(const Path& path, size_t size)
{
    fd_t fd = file_open(path, std::ios_base::out);
    if (fd < 0) {
        return false;
    }
    bool status = file_truncate(fd, size);
    file_close(fd);

    return status;
}

// FUNCTIONS
// ---------

// RUNTIME

path_t getcwd()
{
    char* buf = new char[PATH_MAX];
    if (!::getcwd(buf, PATH_MAX)) {
        throw filesystem_error(filesystem_unexpected_error);
    }

    path_t output(buf);
    delete[] buf;
    return output;
}


path_t join(const path_list_t &paths)
{
    return join_impl(paths, [](char c) {
        return c;
    });
}

// SPLIT

path_list_t split(const path_t& path)
{
    return split_impl(path);
}


path_list_t splitdrive(const path_t& path)
{
    return splitdrive_impl(path);
}


path_list_t splitunc(const path_t& path)
{
    return splitunc_impl(path);
}

// NORMALIZATION


bool isabs(const path_t& path)
{
    return isabs_impl(path);
}


path_t base_name(const path_t& path)
{
    return base_name_impl(path);
}


path_t dir_name(const path_t& path)
{
    return dir_name_impl(path);
}


path_t expanduser(const path_t& path)
{
    return expanduser_impl(path, [](const path_t& p) {
        return p;
    });
}


path_t expandvars(const path_t& path)
{
    auto frompath = [](const path_t& p) {
        return p;
    };
    auto topath = [](const char* p) {
        return p;
    };

    return expandvars_impl(path, frompath, topath);
}


path_t normcase(const path_t& path)
{
    return path;
}

// MANIPULATION


bool move_link(const path_t& src, const path_t& dst, bool replace)
{
    // POSIX rename works identically on files.
    return move_file(src, dst, replace);
}


bool move_file(const path_t& src, const path_t& dst, bool replace)
{
    return move_file_impl(src, dst, replace, [](const path_t& src, const path_t& dst) {
        return rename(src.data(), dst.data()) == 0;
    });
}


bool move_dir(const path_t& src, const path_t& dst, bool replace)
{
    return move_dir_impl(src, dst, replace);
}


bool mklink(const path_t& target, const path_t& dst, bool replace)
{
    return mklink_impl(target, dst, replace, [](const path_t& t, const path_t& d) {
        return symlink(d.data(), t.data()) == 0;
    });
}


bool copy_file(const path_t& src, const path_t& dst, bool replace)
{
    return copy_file_impl(src, dst, replace, [](const path_t& src, const path_t& dst) {
        return copy_file_buffer(src, dst);
    });
}


bool remove_link(const path_t& path)
{
    // same as remove_file on POSIX systems
    return remove_file(path);
}


bool remove_file(const path_t& path)
{
    return unlink(path.data()) == 0;
}


static bool remove_dir_shallow_impl(const path_t& path)
{
    return rmdir(path.data()) == 0;
}


static bool remove_dir_recursive_impl(const path_t& path)
{
    directory_iterator first(path);
    directory_iterator last;
    for (; first != last; ++first) {
        if (first->isfile()) {
            if (!remove_file(first->path())) {
                return false;
            }
        } else if (first->islink()) {
            if (!remove_link(first->path())) {
                return false;
            }
        } else if (first->isdir()) {
            if (!remove_dir_recursive_impl(first->path())) {
                return false;
            }
        }
    }

    return true;
}


bool remove_dir(const path_t& path, bool recursive)
{
    if (recursive) {
        return remove_dir_recursive_impl(path);
    } else {
        return remove_dir_shallow_impl(path);
    }
}


bool copy_dir(const path_t& src, const path_t& dst, bool recursive, bool replace)
{
    return copy_dir_impl(src, dst, recursive, replace);
}


bool mkdir(const path_t& path, int mode)
{
    return ::mkdir(path.data(), static_cast<mode_t>(mode)) == 0;
}


bool makedirs(const path_t& path, int mode)
{
    if (!exists(path)) {
        makedirs(dir_name(path), mode);
        return mkdir(path, mode);
    }

    return false;
}

// FILE UTILS


fd_t file_open(const path_t& path, std::ios_base::openmode mode)
{
    return open(path.data(), convert_openmode(mode));
}


int64_t file_read(fd_t fd, void* buf, size_t count)
{
    return read(fd, buf, count);
}


void file_close(fd_t fd)
{
    close(fd);
}


#if !defined(OS_MACOS)

bool file_allocate(fd_t fd, size_t size)
{
    return posix_fallocate(fd, 0, size) == 0;
}

#endif              // MACOS

bool file_allocate(const path_t& path, size_t size)
{
    return file_allocate_impl(path, size);
}


bool file_truncate(fd_t fd, size_t size)
{
    return ftruncate(fd, size) == 0;
}


bool file_truncate(const path_t& path, size_t size)
{
    return file_truncate_impl(path, size);
}

#endif

PYCPP_END_NAMESPACE
