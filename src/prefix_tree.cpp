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



bool prefix_tree::remove_node( const char *key, unsigned int pos )
{
    if ( !key[ pos ] )
    {
        if ( !pos )
            return false;

        if ( is_finite_node() && next.empty() )
            return true;

        flag = NODE_FLAG::NO_FLAGS;
        return false;
    }

    auto it = next.find( key[ pos ] );
    if ( it == next.end() )
        return false;

    if ( !it->second->remove_node( key, ++pos ) )
        return false;

    if ( next.size() <= 1 && !is_finite_node() )
        return true;

    next.erase( it );
    return false;
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



prefix_tree::iterator::iterator( const prefix_tree *node_, bool finite_nodes_only_, const char *key )
: node( (prefix_tree*)node_ ), finite_nodes_only( finite_nodes_only_ ), symbols()
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

    if ( !node->next.empty() && forward )
        increment_via_next( node->next.begin(), node->next.end(), node );
    else
        node = nullptr;

    if ( !node )
    {
        node = cur;
        if ( forward )
            increment_via_parent();
        else
            decrement_via_parent();
    }
}


void prefix_tree::iterator::increment_via_next(
        next_nodes_container::iterator it,
        next_nodes_container::iterator it_end,
        prefix_tree *cur
)
{
    auto setup_node = [&] ()
    {
        symbols.push_back( static_cast<unsigned char>( it->first ) );
        node = it->second.get();
    };

    if ( !finite_nodes_only )
    {
        setup_node();
        return;
    }

    while ( it != it_end )
    {
        node = cur; // node may be setup to nullptr in previous iteration.

        setup_node();
        if ( it->second->is_finite_node() )
            return;

        if ( !it->second->next.empty() )
        {
            increment_via_next( it->second->next.begin(), it->second->next.end(), it->second.get() );

            if ( node )
                return;
            else if ( !symbols.empty() )
                symbols.pop_back();
        }
    }

    node = nullptr;
}



void prefix_tree::iterator::increment_via_parent()
{
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

    if ( next_it != next_it_end )
        increment_via_parent( next_it, next_it_end, cur );
    else
    {
        node = cur;
        increment_via_parent();
    }
}


void prefix_tree::iterator::increment_via_parent(
        next_nodes_container::iterator it,
        next_nodes_container::iterator it_end,
        prefix_tree *cur
)
{
    if ( !finite_nodes_only )
    {
        node = it != it_end ? it->second.get() : nullptr;
        symbols.push_back( static_cast<unsigned char>( it->first )   );
        return;
    }

    increment_via_next( it, it_end, cur );
    if ( !node )
    {
        node = cur;
        increment_via_parent();
    }
}



void prefix_tree::iterator::decrement_via_parent()
{
    if ( !node->parent )
    {
        node = nullptr;
        return;
    }

    prefix_tree *cur = node->parent;
    char c = static_cast<char>( symbols.back() );
    symbols.pop_back();

    next_nodes_container::iterator found_it = cur->next.find( c );

    if ( found_it == cur->next.begin() )
    {
        node = cur;
        if ( !finite_nodes_only || cur->is_finite_node() )
        {
            if ( symbols.empty() )
                node = nullptr;
            return;
        }
        decrement_via_parent();
    }
    else
    {
        decrement_via_parent(
                    next_nodes_container::reverse_iterator( found_it ),
                    cur->next.rend(),
                    cur
        );
    }
}


void prefix_tree::iterator::decrement_via_parent(
        next_nodes_container::reverse_iterator it,
        next_nodes_container::reverse_iterator it_end,
        prefix_tree *cur
)
{
    if ( !cur->next.empty() )
        decrement_via_next( it, it_end, cur );
    else
        node = nullptr;

    if ( !node )
    {
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
    while ( true )
    {
        if ( it->second->next.empty() )
        {
            if ( it->second->is_finite_node() || !finite_nodes_only )
            {
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
                return;
            }

            node = cur;
            symbols.pop_back();
        }

        if ( it == it_end )
        {
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


prefix_tree::iterator prefix_tree::find( const char *key, bool finite_node )
{
    const prefix_tree *found = find_node( key, finite_node );
    return found ?
                iterator( found, finite_node, key ) :
                iterator()
    ;
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
