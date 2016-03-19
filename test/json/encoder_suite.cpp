///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <limits>
#include <trial/protocol/buffer/array.hpp>
#include <trial/protocol/buffer/ostream.hpp>
#include <trial/protocol/buffer/vector.hpp>
#include <trial/protocol/buffer/string.hpp>
#include <trial/protocol/json/detail/encoder.hpp>
#include <trial/protocol/detail/lightweight_test.hpp>

using namespace trial::protocol;
namespace token = json::token;

typedef json::detail::basic_encoder<char> encoder_type;

//-----------------------------------------------------------------------------
// Buffer
//-----------------------------------------------------------------------------

namespace buffer_suite
{

void test_boost_array()
{
    boost::array<char, 2> result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(42), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(std::string(result.begin(), result.size()), "42");
}

void test_ostringstream()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(42), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "42");
}

void test_vector()
{
    std::vector<char> result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(42), 2);
    std::string expected("42");
    TRIAL_PROTOCOL_TEST_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                          expected.begin(), expected.end());
}

void test_string()
{
    std::string result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(42), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result, "42");
}

void run()
{
    test_boost_array();
    test_ostringstream();
    test_vector();
    test_string();
}

} // namespace buffer_suite

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

namespace basic_suite
{

void test_comma()
{
    boost::array<char, 1> buffer;
    encoder_type encoder(buffer);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(std::string(buffer.begin(), buffer.size()), ",");
}

void fail_comma()
{
    boost::array<char, 0> buffer;
    encoder_type encoder(buffer);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 0);
}

void test_colon()
{
    boost::array<char, 1> buffer;
    encoder_type encoder(buffer);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::name_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(std::string(buffer.begin(), buffer.size()), ":");
}

void fail_colon()
{
    boost::array<char, 0> buffer;
    encoder_type encoder(buffer);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::name_separator>(), 0);
}

void test_null()
{
    boost::array<char, 4> buffer;
    encoder_type encoder(buffer);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::null>(), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(std::string(buffer.begin(), buffer.size()), "null");
}

void fail_null()
{
    boost::array<char, 3> buffer;
    encoder_type encoder(buffer);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::null>(), 0);
}

void test_null_null()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::null>(), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::null>(), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null,null");
}

void test_true()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(true), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "true");
}

void test_true_true()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(true), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(true), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "true,true");
}

void test_false()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "false");
}

void test_false_false()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "false,false");
}

void run()
{
    test_comma();
    fail_comma();
    test_colon();
    fail_colon();
    test_null();
    fail_null();
    test_null_null();
    test_true();
    test_true_true();
    test_false();
    test_false_false();
}

} // namespace basic_suite

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

namespace integer_suite
{

void test_zero()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(0)), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "0");
}

void test_zero_zero()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(0)), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(0)), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "0,0");
}

void test_one()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(1)), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1");
}

void test_minus_one()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(-1)), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-1");
}

void test_nine()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(9)), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "9");
}

void test_minus_nine()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(-9)), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-9");
}

void test_ten()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(10)), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "10");
}

void test_minus_ten()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(-10)), 3);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-10");
}

void test_almost_hundred()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(99)), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "99");
}

void test_hundred()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(100)), 3);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "100");
}

void test_almost_minus_hundred()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(-99)), 3);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-99");
}

void test_minus_hundred()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(boost::int64_t(-100)), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-100");
}

void test_max()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<boost::int64_t>::max()), 19);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "9223372036854775807");
}

void test_almost_min()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<boost::int64_t>::min() + 1), 20);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-9223372036854775807");
}

void test_min()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<boost::int64_t>::min()), 20);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-9223372036854775808");
}

void regress_digits10()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(1561718168), 10);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1561718168");
}

void run()
{
    test_zero();
    test_zero_zero();
    test_one();
    test_minus_one();
    test_nine();
    test_minus_nine();
    test_ten();
    test_minus_ten();
    test_almost_hundred();
    test_hundred();
    test_almost_minus_hundred();
    test_minus_hundred();
    test_max();
    test_almost_min();
    test_min();
    regress_digits10();
}

} // namespace integer_suite

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

namespace floating_suite
{

void test_float_zero()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(0.0f), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "0");
}

void test_double_zero()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(0.0), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "0");
}

void test_double_one()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(1.0), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1");
}

void test_double_minus_one()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(-1.0), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-1");
}

void test_double_half()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(0.5), 3);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "0.5");
}

void test_double_minus_half()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(-0.5), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "-0.5");
}

void test_double_e_100()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(1e100), 6);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1e+100");
}

void test_double_e_minus_100()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(1e-100), 6);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1e-100");
}

void test_float_max()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<float>::max()), 14);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "3.40282347e+38");
}

void test_double_max()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<double>::max()), 23);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1.7976931348623157e+308");
}

void test_float_min()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<float>::min()), 14);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "1.17549435e-38");
}

void test_double_min()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<double>::min()), 23);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "2.2250738585072014e-308");
}

void test_float_infinity()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<float>::infinity()), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null");
}

void test_double_infinity()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<double>::infinity()), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null");
}

void test_float_minus_infinity()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(-std::numeric_limits<float>::infinity()), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null");
}

void test_double_minus_infinity()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(-std::numeric_limits<double>::infinity()), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null");
}

void test_float_nan()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<float>::quiet_NaN()), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null");
}

void test_double_nan()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(std::numeric_limits<double>::quiet_NaN()), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "null");
}

void run()
{
    test_float_zero();
    test_double_zero();
    test_double_one();
    test_double_minus_one();
    test_double_half();
    test_double_minus_half();
    test_double_e_100();
    test_double_e_minus_100();
    test_float_max();
    test_double_max();
    test_float_min();
    test_double_min();
    test_float_infinity();
    test_double_infinity();
    test_float_minus_infinity();
    test_double_minus_infinity();
    test_float_nan();
    test_double_nan();
}

} // namespace floating_suite

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

namespace string_suite
{

void test_literal_empty()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(""), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\"");
}

void test_empty()
{
    std::ostringstream result;
    std::string text("");
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(text), 2);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\"");
}

void test_space()
{
    std::ostringstream result;
    std::string text(" ");
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(text), 3);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\" \"");
}

void test_alpha()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("alpha"), 7);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"alpha\"");
}

void test_alpha_bravo()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("alpha bravo"), 13);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"alpha bravo\"");
}

void test_escape_quote()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\""), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\\"\"");
}

void test_escape_reverse_solidus()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\\"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\\\\"");
}

void test_escape_solidus()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("/"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\/\"");
}

void test_escape_backspace()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\b"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\b\"");
}

void test_escape_formfeed()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\f"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\f\"");
}

void test_escape_newline()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\n"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\n\"");
}

void test_escape_carriage_return()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\r"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\r\"");
}

void test_escape_tab()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("\t"), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"\\t\"");
}

void run()
{
    test_literal_empty();
    test_empty();
    test_space();
    test_alpha();
    test_alpha_bravo();
    test_escape_quote();
    test_escape_reverse_solidus();
    test_escape_solidus();
    test_escape_backspace();
    test_escape_formfeed();
    test_escape_newline();
    test_escape_carriage_return();
    test_escape_tab();
}

} // namespace string_suite

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

namespace container_suite
{

void test_array_begin()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "[");
}

void test_array_end()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "]");
}

void test_array_empty()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "[]");
}

void test_array_bool_one()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "[false]");
}

void test_array_bool_two()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(true), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_array>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "[false,true]");
}

void test_object_begin()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "{");
}

void test_object_end()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "}");
}

void test_object_empty()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "{}");
}

void test_object_bool_one()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("false"), 7);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::name_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "{\"false\":false}");
}

void test_object_bool_two()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::begin_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("false"), 7);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::name_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(false), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::value_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value("true"), 6);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::name_separator>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value(true), 4);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.value<token::end_object>(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "{\"false\":false,\"true\":true}");
}

void run()
{
    test_array_begin();
    test_array_end();
    test_array_empty();
    test_array_bool_one();
    test_array_bool_two();

    test_object_begin();
    test_object_end();
    test_object_empty();
    test_object_bool_one();
    test_object_bool_two();
}

} // namespace container_suite

//-----------------------------------------------------------------------------
// Literal
//-----------------------------------------------------------------------------

namespace literal_suite
{

void test_string()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.literal("alpha"), 5);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "alpha");
}

void test_quote()
{
    std::ostringstream result;
    encoder_type encoder(result);
    TRIAL_PROTOCOL_TEST_EQUAL(encoder.literal("\""), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(result.str(), "\"");
}

void run()
{
    test_string();
    test_quote();
}

} // namespace literal_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    buffer_suite::run();
    basic_suite::run();
    integer_suite::run();
    floating_suite::run();
    string_suite::run();
    container_suite::run();
    literal_suite::run();

    return boost::report_errors();
}
