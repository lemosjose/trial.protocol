#ifndef TRIAL_PROTOCOL_JSON_DETAIL_WRITER_IPP
#define TRIAL_PROTOCOL_JSON_DETAIL_WRITER_IPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <trial/protocol/json/error.hpp>

namespace trial
{
namespace protocol
{
namespace json
{

//-----------------------------------------------------------------------------
// writer::overloader
//-----------------------------------------------------------------------------

template <typename T, typename Enable>
struct writer::overloader
{
};

template <>
struct writer::overloader<token::null>
{
    inline static size_type value(writer& self)
    {
        return self.null_value();
    }
};

template <>
struct writer::overloader<token::begin_array>
{
    inline static size_type value(writer& self)
    {
        return self.begin_array_value();
    }
};

template <>
struct writer::overloader<token::end_array>
{
    inline static size_type value(writer& self)
    {
        return self.end_array_value();
    }
};

template <>
struct writer::overloader<token::begin_object>
{
    inline static size_type value(writer& self)
    {
        return self.begin_object_value();
    }
};

template <>
struct writer::overloader<token::end_object>
{
    inline static size_type value(writer& self)
    {
        return self.end_object_value();
    }
};

//-----------------------------------------------------------------------------
// writer
//-----------------------------------------------------------------------------

template <typename T>
writer::writer(T& buffer)
    : encoder(buffer)
{
    // Push outermost scope
    stack.push(frame(encoder, token::code::end_array));
}

inline boost::system::error_code writer::error() const BOOST_NOEXCEPT
{
    return make_error_code(last_error);
}

inline writer::size_type writer::level() const BOOST_NOEXCEPT
{
    return stack.size() - 1;
}

template <typename T>
writer::size_type writer::value()
{
    return overloader<T>::value(*this);
}

template <typename T>
writer::size_type writer::value(BOOST_FWD_REF(T) value)
{
    validate_scope();

    stack.top().write_separator();
    return encoder.value(boost::forward<T>(value));
}

inline writer::size_type writer::literal(const view_type& data) BOOST_NOEXCEPT
{
    return encoder.literal(data);
}

inline void writer::validate_scope()
{
    if (stack.empty())
    {
        last_error = unexpected_token;
        throw json::error(error());
    }
}

inline void writer::validate_scope(token::code::value code,
                                   enum json::errc e)
{
    if ((stack.size() < 2) || (stack.top().code != code))
    {
        last_error = e;
        throw json::error(error());
    }
}

inline writer::size_type writer::null_value()
{
    validate_scope();

    stack.top().write_separator();
    return encoder.template value<token::null>();
}

inline writer::size_type writer::begin_array_value()
{
    validate_scope();

    stack.top().write_separator();
    stack.push(frame(encoder, token::code::end_array));
    return encoder.template value<token::begin_array>();
}

inline writer::size_type writer::end_array_value()
{
    validate_scope(token::code::end_array, json::unexpected_token);

    size_type result = encoder.template value<token::end_array>();
    stack.pop();
    return result;
}

inline writer::size_type writer::begin_object_value()
{
    validate_scope();

    stack.top().write_separator();
    stack.push(frame(encoder, token::code::end_object));
    return encoder.template value<token::begin_object>();
}

inline writer::size_type writer::end_object_value()
{
    validate_scope(token::code::end_object, json::unexpected_token);

    size_type result = encoder.template value<token::end_object>();
    stack.pop();
    return result;
}

//-----------------------------------------------------------------------------
// frame
//-----------------------------------------------------------------------------

inline writer::frame::frame(encoder_type& encoder,
                            token::code::value code)
    : encoder(encoder),
      code(code),
      counter(0)
{
}

inline void writer::frame::write_separator()
{
    if (counter != 0)
    {
        switch (code)
        {
        case token::code::end_array:
            encoder.value<token::value_separator>();
            break;

        case token::code::end_object:
            if (counter % 2 == 0)
            {
                encoder.value<token::value_separator>();
            }
            else
            {
                encoder.value<token::name_separator>();
            }
            break;

        default:
            break;
        }
    }
    ++counter;
}

} // namespace json
} // namespace protocol
} // namespace trial

#endif // TRIAL_PROTOCOL_JSON_DETAIL_WRITER_IPP
