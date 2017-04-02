///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2017 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <iterator>
#include <trial/protocol/detail/lightweight_test.hpp>
#include <trial/protocol/dynamic/variable.hpp>

using namespace trial::protocol::dynamic;

//-----------------------------------------------------------------------------
// ForwardIterator concept
//-----------------------------------------------------------------------------

namespace concept_suite
{

void iterator_move_constructible()
{
    static_assert(std::is_move_constructible<variable::iterator>::value, "is_move_constructible");
}

void iterator_copy_constructible()
{
    static_assert(std::is_copy_constructible<variable::iterator>::value, "is_copy_constructible");
}

void iterator_copy_assignable()
{
    static_assert(std::is_copy_assignable<variable::iterator>::value, "is_copy_assignable");
}

void iterator_destructible()
{
    static_assert(std::is_destructible<variable::iterator>::value, "is_destructible");
}

void iterator_swappable()
{
    using std::swap;
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable::iterator b = data.end();
    swap(a, b);
    TRIAL_PROTOCOL_TEST(a == data.end());
    TRIAL_PROTOCOL_TEST(b == data.begin());
}

void iterator_traits()
{
    static_assert(std::is_same<std::iterator_traits<variable::iterator>::value_type, variable::iterator::value_type>::value, "value_type");
    static_assert(std::is_same<std::iterator_traits<variable::iterator>::difference_type, variable::iterator::difference_type>::value, "difference_type");
    static_assert(std::is_same<std::iterator_traits<variable::iterator>::reference, variable::iterator::reference>::value, "reference");
    static_assert(std::is_same<std::iterator_traits<variable::iterator>::pointer, variable::iterator::pointer>::value, "pointer");
    static_assert(std::is_same<std::iterator_traits<variable::iterator>::iterator_category, variable::iterator::iterator_category>::value, "iterator_category");
}

void iterator_dereferenceable()
{
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable result = *a;
    TRIAL_PROTOCOL_TEST(result == true);
}

void iterator_incrementable()
{
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable::iterator& b = ++a;
    TRIAL_PROTOCOL_TEST(b == a);
    TRIAL_PROTOCOL_TEST(b != data.begin());
}

void input_iterator_equality_comparable()
{
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable::iterator b = data.begin();
    variable::iterator c = data.begin();
    TRIAL_PROTOCOL_TEST(a == a);
    TRIAL_PROTOCOL_TEST(a == b);
    TRIAL_PROTOCOL_TEST(b == a);
    TRIAL_PROTOCOL_TEST(b == c);
    TRIAL_PROTOCOL_TEST(a == c);

    variable::iterator d = data.end();
    TRIAL_PROTOCOL_TEST(a != d);
}

void input_iterator_dereferenceable()
{
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    TRIAL_PROTOCOL_TEST_EQUAL((*a).size(), 1);
    TRIAL_PROTOCOL_TEST_EQUAL(a->size(), 1);
}

void input_iterator_post_incrementable()
{
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable::iterator b = a++;
    TRIAL_PROTOCOL_TEST(a != data.begin());
    TRIAL_PROTOCOL_TEST(b == data.begin());
}

void forward_iterator_default_constructible()
{
    static_assert(std::is_default_constructible<variable::iterator>::value, "default constructible");
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable::iterator b;
    TRIAL_PROTOCOL_TEST(a != b);
    TRIAL_PROTOCOL_TEST(b != a);
    b = a;
    TRIAL_PROTOCOL_TEST(a == b);
    TRIAL_PROTOCOL_TEST(b == a);
}

void forward_iterator_multipass()
{
    variable data = variable::array({ true, 2, 3.0, "alpha" });
    variable::iterator a = data.begin();
    variable::iterator b = a;
    TRIAL_PROTOCOL_TEST(a == b);
    ++a;
    ++b;
    TRIAL_PROTOCOL_TEST(a == b);
    TRIAL_PROTOCOL_TEST(*a == *b);
}

void run()
{
    // [iterator.iterators]
    iterator_move_constructible();
    iterator_copy_constructible();
    iterator_copy_assignable();
    iterator_destructible();
    iterator_swappable();
    iterator_traits();
    iterator_dereferenceable();
    iterator_incrementable();

    // [input.iterators]
    input_iterator_equality_comparable();
    input_iterator_dereferenceable();
    input_iterator_post_incrementable();

    // [forward.iterators]
    forward_iterator_default_constructible();
    forward_iterator_multipass();
}

} // namespace concept_suite

//-----------------------------------------------------------------------------
// std::begin
//-----------------------------------------------------------------------------

namespace begin_suite
{

void test_null()
{
    variable data;
    TRIAL_PROTOCOL_TEST(data.begin() == data.end());
}

void test_const_null()
{
    const variable data;
    TRIAL_PROTOCOL_TEST(data.begin() == data.end());
}

void test_boolean()
{
    variable data(true);
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_const_boolean()
{
    const variable data(true);
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_integer()
{
    variable data(0);
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_const_integer()
{
    const variable data(0);
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_number()
{
    variable data(0.0);
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_const_number()
{
    const variable data(0.0);
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_string()
{
    variable data("alpha");
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_const_string()
{
    const variable data("alpha");
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_array()
{
    variable data = variable::array({ true, 1, 2.0, "alpha" });
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_const_array()
{
    const variable data = variable::array({ true, 1, 2.0, "alpha" });
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_map()
{
    variable data = variable::map(
        {
            {"key", "alpha"}
        });
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void test_const_map()
{
    const variable data = variable::map(
        {
            {"key", "alpha"}
        });
    TRIAL_PROTOCOL_TEST(data.begin() != data.end());
}

void convert_to_const()
{
    variable data = variable::array({ true, 1, 2.0, "alpha" });
    // Conversion from iterator to const_iterator
    variable::const_iterator begin = data.begin();
    variable::const_iterator end = data.end();
    TRIAL_PROTOCOL_TEST(begin != end);
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(begin, end), 4);
}

void run()
{
    test_null();
    test_const_null();
    test_boolean();
    test_const_boolean();
    test_integer();
    test_const_integer();
    test_number();
    test_const_number();
    test_string();
    test_const_string();
    test_array();
    test_const_array();
    test_map();
    test_const_map();
    convert_to_const();
}

} // namespace begin_suite

//-----------------------------------------------------------------------------
// operator++()
//-----------------------------------------------------------------------------

namespace pre_increment_suite
{

void test_array()
{
    variable data = variable::array({ true, 1, 2.0, "alpha" });
    variable::iterator where = data.begin();
    TRIAL_PROTOCOL_TEST(*where == variable(true));
    ++where;
    TRIAL_PROTOCOL_TEST(where != data.end());
    TRIAL_PROTOCOL_TEST(*where == variable(1));
    ++where;
    TRIAL_PROTOCOL_TEST(where != data.end());
    TRIAL_PROTOCOL_TEST(*where == variable(2.0));
    ++where;
    TRIAL_PROTOCOL_TEST(where != data.end());
    TRIAL_PROTOCOL_TEST(*where == variable("alpha"));
    ++where;
    TRIAL_PROTOCOL_TEST(where == data.end());
}

void run()
{
    test_array();
}

} // namespace pre_increment_suite

//-----------------------------------------------------------------------------
// operator++(int)
//-----------------------------------------------------------------------------

namespace post_increment_suite
{

void test_array()
{
    variable data = variable::array({ true, 1, 2.0, "alpha" });
    variable::iterator where = data.begin();
    TRIAL_PROTOCOL_TEST(*where == variable(true));
    variable::iterator result = where++;
    TRIAL_PROTOCOL_TEST(*result == variable(true));
    TRIAL_PROTOCOL_TEST(where != data.end());
    TRIAL_PROTOCOL_TEST(*where == variable(1));
    result = where++;
    TRIAL_PROTOCOL_TEST(*result == variable(1));
    TRIAL_PROTOCOL_TEST(where != data.end());
    TRIAL_PROTOCOL_TEST(*where == variable(2.0));
    result = where++;
    TRIAL_PROTOCOL_TEST(*result == variable(2.0));
    TRIAL_PROTOCOL_TEST(where != data.end());
    TRIAL_PROTOCOL_TEST(*where == variable("alpha"));
    result = where++;
    TRIAL_PROTOCOL_TEST(*result == variable("alpha"));
    TRIAL_PROTOCOL_TEST(where == data.end());
}

void run()
{
    test_array();
}

} // namespace post_increment_suite

//-----------------------------------------------------------------------------
// std::distance
//-----------------------------------------------------------------------------

namespace distance_suite
{

void test_null()
{
    variable data;
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 0);
}

void test_boolean()
{
    variable data(true);
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 1);
}

void test_integer()
{
    variable data(0);
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 1);
}

void test_number()
{
    variable data(0.0);
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 1);
}

void test_string()
{
    variable data("alpha");
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 1);
}

void test_array()
{
    variable data = variable::array({ true, 1, 2.0, "alpha" });
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 4);
}

void test_array_nulls()
{
    variable data = variable::array({ variable::null, variable::null, variable::null });
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 3);
}

void test_map()
{
    variable data = variable::map(
        {
            {"alpha", "hydrogen"},
            {"bravo", "helium"},
            {"charlie", "lithium"}
        });
    TRIAL_PROTOCOL_TEST_EQUAL(std::distance(data.begin(), data.end()), 3);
}

void run()
{
    test_null();
    test_boolean();
    test_integer();
    test_number();
    test_string();
    test_array();
    test_array_nulls();
    test_map();
}

} // namespace distance_suite

//-----------------------------------------------------------------------------
// Range-based for loop
//-----------------------------------------------------------------------------

namespace range_for_suite
{

void test_null()
{
    variable data;
    variable::size_type size = 0;
    for (auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 0);
}

void test_boolean()
{
    variable data(true);
    variable::size_type size = 0;
    for (auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 1);
}

void test_integer()
{
    variable data(0);
    variable::size_type size = 0;
    for (auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 1);
}

void test_number()
{
    variable data(0.0);
    variable::size_type size = 0;
    for (auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 1);
}

void test_string()
{
    variable data("alpha");
    variable::size_type size = 0;
    for (const auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 5);
}

void test_array()
{
    variable data = variable::array({ true, 1, 2.0, "alpha" });
    variable::size_type size = 0;
    for (const auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 1 + 1 + 1 + 5);
}

void test_array_nulls()
{
    variable data = variable::array({ variable::null, variable::null, variable::null });
    variable::size_type size = 0;
    for (const auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 0 + 0 + 0);
}

void test_map()
{
    variable data = variable::map(
        {
            {"alpha", "hydrogen"},
            {"bravo", "helium"},
            {"charlie", "lithium"}
        });
    variable::size_type size = 0;
    for (const auto& value : data)
    {
        size += value.size();
    }
    TRIAL_PROTOCOL_TEST_EQUAL(size, 8 + 6 + 7);
}

void run()
{
    test_null();
    test_boolean();
    test_integer();
    test_number();
    test_string();
    test_array();
    test_array_nulls();
}

} // namespace range_for_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    concept_suite::run();
    begin_suite::run();
    pre_increment_suite::run();
    post_increment_suite::run();
    distance_suite::run();
    range_for_suite::run();

    return boost::report_errors();
}
