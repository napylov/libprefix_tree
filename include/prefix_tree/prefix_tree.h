/**
 * Prefix tree.
 * Header-only C++-library.
 * 
 * (c) 2021 Alexander Napylov
 * BSD 2-clause license.
 */

#ifndef PREFIX_TREE_H
#define PREFIX_TREE_H

#include <map>

template <typename value_type>
class prefix_tree
{
public:
    std::unique_ptr<prefix_tree<value_type> >   ptr;
  
protected:
    std::map<char, ptr>                         next;

public:
    prefix_tree() = default;
    ~prefix_tree() = default;


    inline void append( const char *key )
    {
        append_node( key );
    }
    
    
    inline void append( const std::string &key )
    {
        append_node( key );
    }    

protected:
    virtual prefix_tree *new_node()
    {
        return new prefix_tree;
    };


    prefix_tree& append_node( const char *key )
    {
        if ( !key || !*key )
            return *this;
        
        auto it = next.find( *key );
        if ( it != next.end() && it.second )
            return it.second->append_node( key + 1 );
            
        it = next.insert_or_update( *key, ptr( new_node() ) );
        return it.second->append_node( key + 1 );
    }
    
    
    prefix_tree& append_node( const std::string &key )
    {
        return append_node( key.c_str() );
    }


    const prefix_tree* find_node( const char *key ) const
    {
        if ( !key )
            return nullptr;
            
        if ( !*key )
            return this;
        
        auto it = next.find( *key );
        if ( it != next.end() && it.second )
            return it.second->find_node( key + 1 );
            
        return nullptr;
    }

public:
    inline bool exists( const char *key ) const
    {
        return find_node( key ) != nullptr;
    }
    
    
    inline bool exists( const std::string &key ) const
    {
        return find_node( key.c_str() ) != nullptr;
    }
};

#endif // PREFIX_TREE_H
