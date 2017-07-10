//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Authentification for requests.
 */

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Authorization string for requests.
 */
struct authentication_t
{
    std::string username;
    std::string password;

    authentication_t() = default;
    authentication_t(const authentication_t&) = default;
    authentication_t & operator=(const authentication_t&) = default;
    authentication_t(authentication_t&&) = default;
    authentication_t & operator=(authentication_t&&) = default;

    authentication_t(std::string &&username, std::string &&password);
    authentication_t(const std::string &username, const std::string &password);

    const std::string string() const noexcept;
    explicit operator bool() const;
};

PYCPP_END_NAMESPACE