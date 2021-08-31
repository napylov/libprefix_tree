/**
 * (c) 2021 Alexander Napylov
 * BSD 2-clause license.
 */

#include <iostream>
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
            increment_via_next( node->next.begin(), node->next.end(), node );
        else
            decrement_via_next( node->next.rbegin(), node->next.rend(), node );
    }
    else
        node = nullptr;

    if ( !node )
    {
        std::cout << "DEBUG: shift_iterator() !node\n";
        node = cur;
        if ( forward )
            increment_via_parent();
        else
            decrement_via_parent();
    }
}



void prefix_tree::iterator::increment_via_parent()
{
    std::cout << "DEBUG: increment_via_parent(): key = " << get_key() << "\n";

    if ( !node->parent )
    {
        node = nullptr;
        return;
    }

    prefix_tree *cur = node->parent;
    char c = static_cast<char>( symbols.back() );
    symbols.pop_back();

    auto next_it = cur->next.find( c );
    auto next_it_end = cur->next.end();

    ++next_it;

    increment_via_parent( next_it, next_it_end, cur );
}


void prefix_tree::iterator::decrement_via_parent()
{
    std::cout << "DEBUG: decrement_via_parent(): key = " << get_key() << "\n";

    if ( !node->parent )
    {
        node = nullptr;
        return;
    }

    prefix_tree *cur = node->parent;
    char c = static_cast<char>( symbols.back() );
    symbols.pop_back();

    next_nodes_container::reverse_iterator next_it( cur->next.find( c ) );
    next_nodes_container::reverse_iterator next_it_end = cur->next.rend();

    next_nodes_container::reverse_iterator tmp = next_it;

    ++next_it;

    if ( next_it == tmp )
    {
        node = cur;
        if ( !finite_nodes_only || cur->is_finite_node() )
            return;
        decrement_via_parent();
    }
    else
        decrement_via_parent( next_it, next_it_end, cur );
}


void prefix_tree::iterator::decrement_via_parent(
        next_nodes_container::reverse_iterator it,
        next_nodes_container::reverse_iterator it_end,
        prefix_tree *cur
)
{
    std::cout << "DEBUG: first char " << it->first << " finite_nodes_only " << finite_nodes_only << "\n";

    if ( !cur->next.empty() )
        decrement_via_next( it, it_end, cur );
    else
        node = nullptr;

    if ( !node )
    {
        std::cout << "DEBUG: decrement_via_parent(...) !node\n";
        if ( !finite_nodes_only || cur->is_finite_node() )
            node = cur;
        else
            decrement_via_parent();
    }

}



prefix_tree::iterator& prefix_tree::iterator::operator--()
{
    shift_iterator( false );
    return *this;
}


void prefix_tree::iterator::decrement_via_next(
        next_nodes_container::reverse_iterator it,
        next_nodes_container::reverse_iterator it_end,
        prefix_tree *cur
)
{
    std::cout << "DEBUG: decrement_via_next()\n";

    while ( true )
    {
        std::cout << "DEBUG: char " << it->first << "\n";
        if ( it->second->next.empty() )
        {
            std::cout << "DEBUG: it->second->next.empty()\n";
            if ( it->second->is_finite_node() || !finite_nodes_only )
            {
                std::cout << "DEBUG: return it->second\n";
                node = it->second.get();
                symbols.push_back( static_cast<unsigned char>( it->first ) );
                return;
            }
            else
                node = nullptr;
        }
        else
        {
            symbols.push_back( static_cast<unsigned char>( it->first ) );
            decrement_via_next( it->second->next.rbegin(), it->second->next.rend(), it->second.get() );

            if ( node )
            {
                std::cout << "DEBUG: decrement_via_next() node return\n";
                return;
            }

            node = cur;
            symbols.pop_back();
        }

        if ( it == it_end )
        {
            std::cout << "DEBUG: it == it_end\n";
            node = nullptr;
            return;
        }

        ++it;
    }
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
