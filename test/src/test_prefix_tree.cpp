#include "test_prefix_tree.h"
#include "prefix_tree/prefix_tree.h"



void test_prefix_tree::SetUp()
{
    tree.reset( new prefix_tree::prefix_tree() );
}


void test_prefix_tree::TearDown()
{
    tree.reset();
}


TEST_F( test_prefix_tree, test_positive )
{
    static const std::string TEST_KEY   = "abc";
    static const char*       TEST_KEY1  = "def";
    static const std::string TEST_KEY2  = "abcdef";
    static const std::string TEST_KEY3  = "abcdee";
    static const std::string TEST_KEY4  = "abcd";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );
    ASSERT_TRUE( tree->append( TEST_KEY3 ) );
    ASSERT_TRUE( tree->append( TEST_KEY4 ) );

    ASSERT_TRUE( tree->exists( TEST_KEY, true ) );
    ASSERT_TRUE( tree->exists( TEST_KEY1, true ) );
    ASSERT_TRUE( tree->exists( TEST_KEY2, true ) );
    ASSERT_TRUE( tree->exists( TEST_KEY3, true ) );
    ASSERT_TRUE( tree->exists( TEST_KEY4, true ) );

    ASSERT_TRUE( tree->exists( TEST_KEY.substr( 0, 1 ), false ) );
    ASSERT_TRUE( tree->exists( TEST_KEY2.substr( 0, 3 ), false ) );
}


TEST_F( test_prefix_tree, test_iterator )
{
    static const std::string TEST_KEY   = "abc";
    static const std::string TEST_KEY1  = "def";
    static const std::string TEST_KEY2  = "abcdef";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );

    auto it = tree->begin( true );

    ASSERT_EQ( TEST_KEY, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY2, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY1, it.get_key() );
    ++it;

    ASSERT_EQ( it, tree->end() );
}

/*
TEST_F( test_prefix_tree, test_iterator_decrement )
{
    static const std::string TEST_KEY   = "abc";
    static const std::string TEST_KEY1  = "def";
    static const std::string TEST_KEY2  = "abcdef";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );

    auto it = tree->begin( true );

    ASSERT_EQ( TEST_KEY, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY2, it.get_key() );
    it++;
    ASSERT_EQ( TEST_KEY1, it.get_key() );

    std::cout << "--------------\n";

    it--;
    ASSERT_EQ( TEST_KEY2, it.get_key() );
    --it;
    ASSERT_EQ( TEST_KEY, it.get_key() );
}
*/
