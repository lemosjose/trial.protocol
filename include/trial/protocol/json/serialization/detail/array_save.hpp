#ifndef TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_ARRAY_SAVE_HPP
#define TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_ARRAY_SAVE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/protocol/json/serialization/serialization.hpp>
#include <trial/protocol/serialization/array.hpp>

namespace trial
{
namespace protocol
{
namespace serialization
{

template <typename CharT, typename T, std::size_t N>
struct save_overloader< json::basic_oarchive<CharT>,
                        T[N] >
{
    static void save(json::basic_oarchive<CharT>& ar,
                     const T (&data)[N],
                     const unsigned int /* protocol_version */)
    {
        ar.template save<json::token::begin_array>();
        for (std::size_t i = 0; i < N; ++i)
        {
            ar.save_override(data[i]);
        }
        ar.template save<json::token::end_array>();
    }
};

} // namespace serialization
} // namespace protocol
} // namespace trial

#endif // TRIAL_PROTOCOL_JSON_SERIALIZATION_DETAIL_ARRAY_SAVE_HPP
