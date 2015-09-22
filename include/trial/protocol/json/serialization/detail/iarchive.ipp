#ifndef TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_IARCHIVE_IPP
#define TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_IARCHIVE_IPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

namespace trial
{
namespace protocol
{
namespace json
{

inline iarchive::iarchive(const json::reader& reader)
    : reader(reader)
{
}

inline iarchive::iarchive(const json::reader::view_type& view)
    : reader(view)
{
}

template <typename Iterator>
iarchive::iarchive(Iterator begin, Iterator end)
    : reader(begin, end)
{
}

template <typename Tag>
void iarchive::load()
{
    next(Tag::code);
}

template <typename T>
void iarchive::load(T& value)
{
    value = reader.template value<T>();
    next();
}

template <typename Tag>
bool iarchive::at() const
{
    return (reader.code() == Tag::code);
}

inline token::code::value iarchive::code() const
{
    return reader.code();
}

inline void iarchive::next()
{
    if (!reader.next() && (reader.symbol() == token::symbol::error))
    {
        throw json::error(reader.error());
    }
}

inline void iarchive::next(token::code::value expect)
{
    if (!reader.next(expect) && (reader.symbol() == token::symbol::error))
    {
        throw json::error(reader.error());
    }
}

} // namespace json
} // namespace protocol
} // namespace trial

#endif // TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_IARCHIVE_IPP
