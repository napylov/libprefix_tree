/**
 * (c) 2021 Alexander Napylov
 * BSD 2-clause license.
 */

#include "prefix_tree/prefix_tree.h"


namespace prefix_tree
{


prefix_tree::prefix_tree() : next(), flag( NODE_FLAG::NO_FLAGS ), parent( nullptr )
{
}


prefix_tree::prefix_tree( prefix_tree *parent_ )
: next(), flag( NODE_FLAG::NO_FLAGS ), parent( parent_ )
{
}


prefix_tree *prefix_tree::new_node( prefix_tree *parent_ )
{
    return new prefix_tree( parent_ );
};


std::pair<prefix_tree&, bool> prefix_tree::append_node( const char *key )
{
    if ( !key  )
        return std::pair<prefix_tree&, bool>( *this, false );

    if ( !*key )
    {
        this->flag = NODE_FLAG::FINITE_NODE;
        return std::pair<prefix_tree&, bool>( *this, true );
    }
    
    auto it = next.find( *key );
    if ( it != next.end() && it->second )
        return it->second->append_node( key + 1 );
        
    auto appened_node = next.insert_or_assign( *key, ptr( new_node( this ) ) );
    if ( !appened_node.second )
        return std::pair<prefix_tree&, bool>( *this, false );

    return appened_node.first->second->append_node( key + 1 );
}



const prefix_tree* prefix_tree::find_node( const char *key, bool finite_node ) const
{
    if ( !key )
        return nullptr;
        
    if ( !*key )
    {
        if ( finite_node )
            return is_finite_node() ? this : nullptr;
        return this;
    }
    
    auto it = next.find( *key );
    if ( it != next.end() && it->second )
        return it->second->find_node( key + 1, finite_node );
        
    return nullptr;
}



prefix_tree::iterator::iterator( prefix_tree *node_, bool finite_nodes_only_, const char *key )
: node( node_ ), finite_nodes_only( finite_nodes_only_ ), symbols()
{
    if ( key )
    {
        int i = 0;
        while ( key[ i ] )
            symbols.push_back( static_cast<unsigned char>( key[ ++i ] ) );
    }
}


prefix_tree::iterator& prefix_tree::iterator::operator++()
{
    shift_iterator( true );
    return *this;
}


prefix_tree::iterator& prefix_tree::iterator::operator++( int unused )
{
    shift_iterator( true );
    return *this;
}



void prefix_tree::iterator::shift_iterator( bool forward )
{
    if ( !node )
        return;

    prefix_tree *cur = node;

    if ( !node->next.empty() )
    {
        if ( forward )
            increment_via_next( node->next.begin(), node->next.end(), node, forward );
        else
            increment_via_next( node->next.rbegin(), node->next.rend(), node, forward );
    }
    else
        node = nullptr;

    if ( !node )
    {
        node = cur;
        increment_via_parent( forward );
    }
}



void prefix_tree::iterator::increment_via_parent( bool forward )
{
    if ( !node->parent )
    {
        node = nullptr;
        return;
    }

    prefix_tree *cur = node->parent;
    char c = static_cast<char>( symbols.back() );
    symbols.pop_back();

    if ( forward )
    {
        auto next_it = cur->next.find( c );
        auto next_it_end = cur->next.end();

        ++next_it;

        increment_via_parent( next_it, next_it_end, cur, forward );
    }
    else
    {
        std::map<char, ptr>::reverse_iterator next_it( cur->next.find( c ) );
        auto next_it_end = cur->next.rend();

        ++next_it;

        increment_via_parent( next_it, next_it_end, cur, forward );
    }
}


prefix_tree::iterator& prefix_tree::iterator::operator--()
{
    shift_iterator( false );
    return *this;
}


prefix_tree::iterator& prefix_tree::iterator::operator--( int unused )
{
    shift_iterator( false );
    return *this;
}


prefix_tree* prefix_tree::iterator::operator->()
{
    return node;
}


bool prefix_tree::iterator::operator==( const iterator &right ) const
{
    return
            node == right.node                           &&
            (!node || finite_nodes_only == right.finite_nodes_only) &&
            symbols == right.symbols
    ;
}


std::string prefix_tree::iterator::get_key() const
{
    return std::string( symbols.begin(), symbols.end() );
}


prefix_tree::iterator prefix_tree::begin( bool finite_nodes_only )
{
    iterator it( this, finite_nodes_only, nullptr );
    ++it;

    return it;
}


prefix_tree::iterator prefix_tree::end()
{
    return iterator();
}



} // namespace prefix_tree
