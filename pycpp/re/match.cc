//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

// Include the source file so the implied declarations can interact.
#include <pycpp/re/regex.cc>
#include <pycpp/stl/tuple.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Implied base class for the regular expression match object.
 */
struct match_impl_t
{
    re2::RE2* re2 = nullptr;
    string_wrapper input;
    size_t pos;
    size_t endpos;
    size_t argc = 0;
    string_wrapper *groups = nullptr;
    ~match_impl_t();

    bool operator==(const match_impl_t&) const;
};


match_impl_t::~match_impl_t()
{
    delete groups;
}


bool match_impl_t::operator==(const match_impl_t& rhs) const
{
    return tie(re2, input, pos, endpos, argc, groups) == tie(rhs.re2, rhs.input, rhs.pos, rhs.endpos, rhs.argc, rhs.groups);
}


match_t::match_t(match_t&& rhs)
{
    swap(ptr_, rhs.ptr_);
}


match_t & match_t::operator=(match_t&& rhs)
{
    swap(ptr_, rhs.ptr_);
    return *this;
}


match_t::~match_t()
{}


const string_wrapper& match_t::group(size_t index) const
{
    assert(index < ptr_->argc);
    return ptr_->groups[index];
}


match_groups match_t::groups() const
{
    match_groups list;
    list.reserve(ptr_->argc - 1);
    for (size_t i = 1; i < ptr_->argc; ++i) {
        list.emplace_back(group(i));
    }

    return list;
}


match_groupdict match_t::groupdict() const
{
    const auto& names = ptr_->re2->CapturingGroupNames();
    match_groupdict dict;
    for (const auto& pair: names) {
        dict[group(pair.first)] = string_wrapper(pair.second);
    }

    return dict;
}


size_t match_t::start(size_t index) const
{
    const string_wrapper& view = group(index);
    return view.data() - ptr_->input.data();
}


size_t match_t::end(size_t index) const
{
    const string_wrapper& view = group(index);
    return view.data() + view.size() - ptr_->input.data();
}


pair<size_t, size_t> match_t::span(size_t index) const
{
    return make_pair(start(index), end(index));
}


size_t match_t::pos() const
{
    return ptr_->pos;
}


size_t match_t::endpos() const
{
    return ptr_->endpos;
}


size_t match_t::lastindex() const
{
    // always have extra arg for group0
    return ptr_->argc - 1;
}


string_wrapper match_t::lastgroup() const
{
    const auto& names = ptr_->re2->CapturingGroupNames();
    assert(!names.empty());
    return string_wrapper(names.rbegin()->second);
}


const string_wrapper& match_t::string() const
{
    return ptr_->input;
}


bool match_t::operator==(const match_t& rhs) const
{
    if (*this && rhs) {
        return *ptr_ == *rhs.ptr_;
    }
    return ptr_ == rhs.ptr_;
}


bool match_t::operator!=(const match_t& rhs) const
{
    return !operator==(rhs);
}


match_t::operator bool() const
{
    return bool(ptr_);
}


match_t::match_t()
{}


match_t::match_t(regexp_t& regex, const string_wrapper& view, size_t pos, size_t endpos):
    ptr_(make_unique<match_impl_t>())
{
    ptr_->re2 = &regex.ptr_->re2;
    ptr_->input = view;
    ptr_->pos = pos;
    ptr_->endpos = endpos;
    ptr_->argc = regex.ptr_->argc;
    ptr_->groups = new string_wrapper[ptr_->argc];
    for (size_t i = 0; i < ptr_->argc; ++i) {
        auto piece = regex.ptr_->piece[i];
        ptr_->groups[i] = string_wrapper(piece.data(), piece.size());
    }

    // reset our regular expression
    regex.ptr_->clear();
    regex.ptr_->initialize();
}


match_iterator_t::match_iterator_t()
{}


match_iterator_t::match_iterator_t(regexp_t& regex, const string_wrapper& str):
    match_(make_shared<match_t>(regex.search(str))),
    regex_(&regex),
    str_(str)
{}


match_iterator_t::~match_iterator_t()
{}


match_iterator_t::match_iterator_t(const self_t& rhs):
    match_(rhs.match_),
    regex_(rhs.regex_),
    str_(rhs.str_)
{}


auto match_iterator_t::operator=(const self_t& rhs) -> self_t&
{
    match_ = rhs.match_;
    regex_ = rhs.regex_;
    str_ = rhs.str_;
    return *this;
}


match_iterator_t::match_iterator_t(self_t&& rhs):
    match_(move(rhs.match_)),
    regex_(move(rhs.regex_)),
    str_(move(rhs.str_))
{}


auto match_iterator_t::operator=(self_t&& rhs) -> self_t&
{
    swap(rhs);
    return *this;
}


auto match_iterator_t::operator*() -> reference
{
    return *match_;
}


auto match_iterator_t::operator*() const -> const_reference
{
    return *match_;
}


auto match_iterator_t::operator->() -> pointer
{
    return &*match_;
}


auto match_iterator_t::operator->() const -> const_pointer
{
    return &*match_;
}


match_iterator_t& match_iterator_t::operator++()
{
    if (regex_ && match_ && *match_) {
        size_t pos = match_->end();
        match_ = make_shared<match_t>(regex_->search(str_, pos));
        // no more match, reset
        if (!*match_) {
            match_.reset();
            regex_ = nullptr;
            str_ = string_wrapper();
        }
    }

    return *this;
}


match_iterator_t match_iterator_t::operator++(int)
{
    match_iterator_t copy(*this);
    operator++();
    return copy;
}


bool match_iterator_t::operator==(const match_iterator_t& rhs) const
{
    if (match_ && rhs.match_) {
        return tie(*match_, regex_, str_) == tie(*rhs.match_, rhs.regex_, rhs.str_);
    }
    return match_ == rhs.match_;
}


bool match_iterator_t::operator!=(const match_iterator_t& rhs) const
{
    return !operator==(rhs);
}


void match_iterator_t::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(match_, rhs.match_);
    PYCPP_NAMESPACE::swap(regex_, rhs.regex_);
    PYCPP_NAMESPACE::swap(str_, rhs.str_);
}

PYCPP_END_NAMESPACE
