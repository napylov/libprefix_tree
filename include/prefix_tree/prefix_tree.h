/**
 * Prefix tree library.
 * 
 * (c) 2021 Alexander Napylov
 * BSD 2-clause license.
 */

#ifndef PREFIX_TREE_H
#define PREFIX_TREE_H

#include <memory>
#include <map>


namespace prefix_tree
{


class prefix_tree
{
public:
    typedef std::unique_ptr<prefix_tree>    ptr;
  
protected:
    std::map<char, ptr>                     next;

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


    inline bool exists( const char *key ) const
    {
        return find_node( key ) != nullptr;
    }
    
    
    inline bool exists( const std::string &key ) const
    {
        return find_node( key.c_str() ) != nullptr;
    }

protected:
    virtual prefix_tree *new_node();

    std::pair<prefix_tree&, bool> append_node( const char *key );
    
    inline std::pair<prefix_tree&, bool> append_node( const std::string &key )
    {
        return append_node( key.c_str() );
    }

    const prefix_tree* find_node( const char *key ) const;
};


} // namespace prefix_tree

#endif // PREFIX_TREE_H
