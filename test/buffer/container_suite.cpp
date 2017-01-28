///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <vector>
#include <boost/array.hpp>
#include <trial/protocol/buffer/container.hpp>
#include <trial/protocol/detail/lightweight_test.hpp>

using namespace trial::protocol;

template <typename CharT>
class vector_buffer : buffer::basic_container< std::vector<CharT> >
{
    typedef buffer::basic_container< std::vector<CharT> > super;

public:
    typedef typename super::value_type value_type;
    typedef typename super::size_type size_type;
    typedef typename super::view_type view_type;

    vector_buffer(std::vector<CharT>& data)
        : super(data)
    {
    }

    virtual bool grow(size_type size)
    {
        return super::grow(size);
    }

    virtual void write(value_type value)
    {
        return super::write(value);
    }

    virtual void write(const view_type& view)
    {
        return super::write(view);
    }
};

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

namespace container_suite
{

void test_empty()
{
    std::vector<char> output;
    vector_buffer<char> container(output);

    boost::array<char, 0> expected;
    TRIAL_PROTOCOL_TEST_ALL_WITH(output.begin(), output.end(),
                                 expected.begin(), expected.end(),
                                 std::equal_to<char>());
}

void test_single()
{
    std::vector<char> output;
    vector_buffer<char> container(output);
    TRIAL_PROTOCOL_TEST_EQUAL(container.grow(1), true);
    TRIAL_PROTOCOL_TEST_NO_THROW(container.write('A'));

    char expected[] = { 'A' };
    TRIAL_PROTOCOL_TEST_ALL_WITH(output.begin(), output.end(),
                                 expected, expected + sizeof(expected),
                                 std::equal_to<char>());
}

void test_view()
{
    std::vector<char> output;
    vector_buffer<char> container(output);
    std::string input = "alpha";
    TRIAL_PROTOCOL_TEST_EQUAL(container.grow(input.size()), true);
    TRIAL_PROTOCOL_TEST_NO_THROW(container.write(input));

    TRIAL_PROTOCOL_TEST_ALL_WITH(output.begin(), output.end(),
                                 input.begin(), input.end(),
                                 std::equal_to<char>());
}

void test()
{
    test_empty();
    test_single();
    test_view();
}

} // namespace container_suite

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main()
{
    container_suite::test();

    return boost::report_errors();
}
