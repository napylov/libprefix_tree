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
    enum NODE_FLAG : uint8_t
    {
        NO_FLAGS = 0,
        FINITE_NODE
    };

protected:
    std::map<char, ptr>                     next;
    NODE_FLAG                               flag;

public:
    prefix_tree();
    virtual ~prefix_tree() = default;


    inline bool append( const char *key )
    {
        return append_node( key ).second;
    }
    
    
    inline bool append( const std::string &key )
    {
        return append_node( key ).second;
    }    


    inline bool exists( const char *key, bool finite_node = true ) const
    {
        return find_node( key ) != nullptr;
    }
    
    
    inline bool exists( const std::string &key, bool finite_node = true ) const
    {
        return find_node( key.c_str(), finite_node ) != nullptr;
    }

protected:
    virtual prefix_tree *new_node();

    std::pair<prefix_tree&, bool> append_node( const char *key );
    
    inline std::pair<prefix_tree&, bool> append_node( const std::string &key )
    {
        return append_node( key.c_str() );
    }

    const prefix_tree* find_node( const char *key, bool finite_node = true ) const;
};


} // namespace prefix_tree

#endif // PREFIX_TREE_H
