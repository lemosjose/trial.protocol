#ifndef TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_OARCHIVE_IPP
#define TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_OARCHIVE_IPP

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

template <typename CharT>
basic_oarchive<CharT>::basic_oarchive(buffer_type& buffer)
    : writer(buffer)
{
}

template <typename CharT>
template <typename T>
void basic_oarchive<CharT>::save(const T& data)
{
    writer.value(data);
}

template <typename CharT>
template<typename T>
void basic_oarchive<CharT>::save_override(const T& data)
{
    boost::archive::save(*this->This(), data);
}

template <typename CharT>
template<typename T>
void basic_oarchive<CharT>::save_override(const T& data, long /* version */)
{
    save_override(data);
}

template <typename CharT>
void basic_oarchive<CharT>::save_override(const char *data)
{
    save(data);
}

} // namespace json
} // namespace protocol
} // namespace trial

#endif // TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_OARCHIVE_IPP
