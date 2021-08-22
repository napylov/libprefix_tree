/**
 * (c) 2021 Alexander Napylov
 * BSD 2-clause license.
 */

#include "prefix_tree/prefix_tree.h"


namespace prefix_tree
{



prefix_tree *prefix_tree::new_node()
{
    return new prefix_tree;
};


std::pair<prefix_tree&, bool> prefix_tree::append_node( const char *key )
{
    if ( !key  )
        return std::pair<prefix_tree&, bool>( *this, false );

    if ( !*key )
        return std::pair<prefix_tree&, bool>( *this, true );
    
    auto it = next.find( *key );
    if ( it != next.end() && it->second )
        return it->second->append_node( key + 1 );
        
    auto appened_node = next.insert_or_assign( *key, ptr( new_node() ) );
    if ( !appened_node.second )
        return std::pair<prefix_tree&, bool>( *this, false );

    return appened_node.first->second->append_node( key + 1 );
}



const prefix_tree* prefix_tree::find_node( const char *key ) const
{
    if ( !key )
        return nullptr;
        
    if ( !*key )
        return this;
    
    auto it = next.find( *key );
    if ( it != next.end() && it->second )
        return it->second->find_node( key + 1 );
        
    return nullptr;
}


} // namespace prefix_tree