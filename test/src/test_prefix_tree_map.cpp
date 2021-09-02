#include "test_prefix_tree_map.h"
#include "prefix_tree/prefix_tree_map.h"



void test_prefix_tree_map::SetUp()
{
    tree.reset( new prefix_tree::prefix_tree_map<int>() );
}


void test_prefix_tree_map::TearDown()
{
    tree.reset();
}


TEST_F( test_prefix_tree_map, test_append )
{
    static const std::string TEST_KEY    = "abc";
    static const char*       TEST_KEY1   = "def";
    static const std::string TEST_KEY2   = "abcd";
    static const std::string TEST_KEY3   = "abcdef";
    static const std::string TEST_KEY4   = "de1";

    static const int         TEST_VALUE  = 100;
    static const int         TEST_VALUE1 = 200;
    static const int         TEST_VALUE2 = 300;
    static const int         TEST_VALUE3 = 400;
    static const int         TEST_VALUE4 = 500;

    ASSERT_TRUE( tree->append( TEST_KEY, TEST_VALUE ) );
    ASSERT_TRUE( tree->append( TEST_KEY1, TEST_VALUE1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2, TEST_VALUE2 ) );
    ASSERT_TRUE( tree->append( TEST_KEY3, TEST_VALUE3 ) );
    ASSERT_TRUE( tree->append( TEST_KEY4, TEST_VALUE4 ) );

    auto it = tree->find( TEST_KEY );
    ASSERT_NE( it, tree->end() );
    ASSERT_EQ( it.get_value(), TEST_VALUE );

    it = tree->find( TEST_KEY1 );
    ASSERT_NE( it, tree->end() );
    ASSERT_EQ( it.get_value(), TEST_VALUE1 );

    it = tree->find( TEST_KEY2 );
    ASSERT_NE( it, tree->end() );
    ASSERT_EQ( it.get_value(), TEST_VALUE2 );

    it = tree->find( TEST_KEY3 );
    ASSERT_NE( it, tree->end() );
    ASSERT_EQ( it.get_value(), TEST_VALUE3 );

    it = tree->find( TEST_KEY4 );
    ASSERT_NE( it, tree->end() );
    ASSERT_EQ( it.get_value(), TEST_VALUE4 );
}
