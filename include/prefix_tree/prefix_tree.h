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


/**
 * @brief The prefix_tree class
 */
class prefix_tree
{
public:
    /// @brief ptr          Pointer to prefix_tree (unique_ptr).
    typedef std::unique_ptr<prefix_tree>    ptr;

protected:
    /// @brief NODE_FLAG    Enumeration for describe kind of node (finite or note).
    enum NODE_FLAG : uint8_t
    {
        NO_FLAGS = 0,
        FINITE_NODE
    };

protected:
    /// @brief next         Pointers to next nodes.
    std::map<char, ptr>                     next;
    /// @brief flag         Kind of node.
    NODE_FLAG                               flag;

public:
    prefix_tree();
    virtual ~prefix_tree() = default;


    /**
     * @brief append        Append new chain to prefix tree.
     * @param key           Key to append.
     * @return              true if append is successful.
     */
    inline bool append( const char *key )
    {
        return append_node( key ).second;
    }
    
    
    /**
     * @brief append        Append new chain to prefix tree.
     * @param key           Key to append.
     * @return              true if append is successful.
     */
    inline bool append( const std::string &key )
    {
        return append_node( key ).second;
    }    


    /**
     * @brief exists        Check key or prefix is exist.
     * @param key           Key ot prefix.
     * @param finite_node   If true looking for finite node only else prefix or finite node.
     * @return              true if key or prefix is exist.
     */
    inline bool exists( const char *key, bool finite_node = true ) const
    {
        return find_node( key ) != nullptr;
    }
    
    
    /**
     * @brief exists        Check key or prefix is exist.
     * @param key           Key ot prefix.
     * @param finite_node   If true looking for finite node only else prefix or finite node.
     * @return              true if key or prefix is exist.
     */
    inline bool exists( const std::string &key, bool finite_node = true ) const
    {
        return find_node( key.c_str(), finite_node ) != nullptr;
    }

protected:
    /**
     * @brief new_node      Factory method to create new child node.
     *                      NOTE! The function must be overload in derived class
     *                      because all nodes in the tree have to equal type.
     * @return              Raw pointer to new object of prefix_tree type or derived.
     */
    virtual prefix_tree *new_node();


    /**
     * @brief append_node   Append node to prefix tree.
     * @param key           Key.
     * @return              Pair where first is reference to new node second is
     *                      flag append has been successful.
     */
    std::pair<prefix_tree&, bool> append_node( const char *key );
    

    /**
     * @brief append_node   Append node to prefix tree.
     * @param key           Key.
     * @return              Pair where first is reference to new node second is
     *                      flag append has been successful.
     */
    inline std::pair<prefix_tree&, bool> append_node( const std::string &key )
    {
        return append_node( key.c_str() );
    }


    /**
     * @brief find_node     Find node by key.
     * @param key           Key ot prefix.
     * @param finite_node   If true looking for finite node only else prefix or finite node.
     * @return              Raw const pointer to found node or nullptr.
     */
    const prefix_tree* find_node( const char *key, bool finite_node = true ) const;
};


} // namespace prefix_tree

#endif // PREFIX_TREE_H
