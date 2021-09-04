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
    static const std::string TEST_KEY3  = "abcdeg";
    static const std::string TEST_KEY4  = "abceee";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );
    ASSERT_TRUE( tree->append( TEST_KEY3 ) );
    ASSERT_TRUE( tree->append( TEST_KEY4 ) );

    auto it = tree->begin( true );

    ASSERT_EQ( TEST_KEY, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY2, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY3, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY4, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY1, it.get_key() );
    ++it;

    ASSERT_EQ( it, tree->end() );
}


TEST_F( test_prefix_tree, test_iterator_decrement )
{
    static const std::string TEST_KEY   = "abc";
    static const std::string TEST_KEY1  = "def";
    static const std::string TEST_KEY2  = "abcdef";
    static const std::string TEST_KEY3  = "abcdeg";
    static const std::string TEST_KEY4  = "abceee";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );
    ASSERT_TRUE( tree->append( TEST_KEY3 ) );
    ASSERT_TRUE( tree->append( TEST_KEY4 ) );

    auto it = tree->begin( true );

    ASSERT_EQ( TEST_KEY, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY2, it.get_key() );
    it++;
    ASSERT_EQ( TEST_KEY3, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY4, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY1, it.get_key() );

    --it;
    ASSERT_EQ( TEST_KEY4, it.get_key() );
    --it;
    ASSERT_EQ( TEST_KEY3, it.get_key() );
    it--;
    ASSERT_EQ( TEST_KEY2, it.get_key() );
    --it;
    ASSERT_EQ( TEST_KEY, it.get_key() );
}


TEST_F( test_prefix_tree, test_iterator_all_nodes )
{
    static const std::string TEST_KEY   = "abc";
    static const std::string TEST_KEY1  = "abcd";
    static const std::string TEST_KEY2  = "abce";
    static const std::string TEST_KEY3  = "def";
    static const std::string TEST_KEY4  = "de1";
    static const std::string TEST_KEY5  = "de2";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );
    ASSERT_TRUE( tree->append( TEST_KEY3 ) );
    ASSERT_TRUE( tree->append( TEST_KEY4 ) );
    ASSERT_TRUE( tree->append( TEST_KEY5 ) );

    auto it = tree->begin( false );
    ASSERT_EQ( "a", it.get_key() );
    ++it;
    ASSERT_EQ( "ab", it.get_key() );
    it++;
    ASSERT_EQ( "abc", it.get_key() );
    ++it;
    ASSERT_EQ( "abcd", it.get_key() );
    ++it;
    ASSERT_EQ( "abce", it.get_key() );
    ++it;
    ASSERT_EQ( "d", it.get_key() );
    ++it;
    ASSERT_EQ( "de", it.get_key() );
    ++it;
    ASSERT_EQ( "de1", it.get_key() );
    ++it;
    ASSERT_EQ( "de2", it.get_key() );
    ++it;
    ASSERT_EQ( "def", it.get_key() );
    --it;
    ASSERT_EQ( "de2", it.get_key() );
    --it;
    ASSERT_EQ( "de1", it.get_key() );
    --it;
    ASSERT_EQ( "de", it.get_key() );
    --it;
    ASSERT_EQ( "d", it.get_key() );
    --it;
    ASSERT_EQ( "abce", it.get_key() );
    --it;
    ASSERT_EQ( "abcd", it.get_key() );
    --it;
    ASSERT_EQ( "abc", it.get_key() );
    --it;
    ASSERT_EQ( "ab", it.get_key() );
    --it;
    ASSERT_EQ( "a", it.get_key() );
    --it;
    ASSERT_EQ( it, tree->end() );
}



TEST_F( test_prefix_tree, test_remove )
{
    static const std::string TEST_KEY   = "abc";
    static const std::string TEST_KEY1  = "abcdef";
    static const std::string TEST_KEY2  = "d";
    static const std::string TEST_KEY3  = "def";
    static const std::string TEST_KEY4  = "c";

    ASSERT_TRUE( tree->append( TEST_KEY ) );
    ASSERT_TRUE( tree->append( TEST_KEY1 ) );
    ASSERT_TRUE( tree->append( TEST_KEY2 ) );
    ASSERT_TRUE( tree->append( TEST_KEY3 ) );

    tree->remove( TEST_KEY1 );
    tree->remove( TEST_KEY2 );
    tree->remove( TEST_KEY4 );

    auto it = tree->begin( true );
    ASSERT_EQ( TEST_KEY, it.get_key() );
    ++it;
    ASSERT_EQ( TEST_KEY3, it.get_key() );
    ++it;
    ASSERT_EQ( it, tree->end() );

    auto it1 = tree->begin( false );
    ASSERT_EQ( "a", it1.get_key() );
    ++it1;
    ASSERT_EQ( "ab", it1.get_key() );
    ++it1;
    ASSERT_EQ( "abc", it1.get_key() );
    ++it1;
    ASSERT_EQ( "d", it1.get_key() );
    ++it1;
    ASSERT_EQ( "de", it1.get_key() );
    ++it1;
    ASSERT_EQ( "def", it1.get_key() );
    ++it1;
    ASSERT_EQ( it1, tree->end() );
}
