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
#include <stack>

#include <iostream>

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

    typedef std::map<char, ptr>             next_nodes_container;

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
    /// Parent node.
    prefix_tree                             *parent;


public:
    class iterator
    {
    protected:
        prefix_tree                 *node;
    private:
        bool                        finite_nodes_only;
        std::deque<unsigned char>   symbols;

    public:
        /// @brief iterator      Default constructor
        iterator() : node( nullptr ), finite_nodes_only( false), symbols() {}

        /**
         * @brief iterator              Constructor.
         * @param node_                 Current node of prefix tree.
         * @param finite_nodes_only_    Iterate via finite nodes only.
         * @param key                   Key of node_.
         */
        iterator( const prefix_tree *node_, bool finite_nodes_only_, const char *key );

        iterator( const iterator &_ ) = default;

        ~iterator() = default;

        iterator& operator=( const iterator &_ ) = default;
        iterator& operator++();
        iterator& operator--();
        iterator& operator++( int unused );
        iterator& operator--( int unused );
        bool operator==( const iterator &right ) const;
        bool operator!=( const iterator &right ) const { return !operator==(right); };

        operator bool() const { return node != nullptr; }

        /**
         * @brief get_key
         * @return          Key of current node.
         */
        std::string get_key() const;
    protected:
        prefix_tree* operator->();
    private:
        void shift_iterator( bool forward );

        void increment_via_next(
                next_nodes_container::iterator it,
                next_nodes_container::iterator it_end,
                prefix_tree *cur
        );

        void increment_via_parent();
        void decrement_via_parent();

        void increment_via_parent(
                next_nodes_container::iterator it,
                next_nodes_container::iterator it_end,
                prefix_tree *cur
        );


        void decrement_via_next(
                next_nodes_container::reverse_iterator it,
                next_nodes_container::reverse_iterator it_end,
                prefix_tree *cur
        );

        void decrement_via_parent(
                next_nodes_container::reverse_iterator it,
                next_nodes_container::reverse_iterator it_end,
                prefix_tree *cur
        );

    };


protected:
    prefix_tree( prefix_tree *parent_ );

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
     * @brief remove        Remove key from the prefix tree.
     * @param key           Key of node. If a node of key if not finite
     *                      the node will not be removed.
     */
    inline void remove( const std::string &key )
    {
        remove( key.c_str() );
    }


    /**
     * @brief remove        Remove key from the prefix tree.
     * @param key           Key of node. If a node of key if not finite
     *                      the node will not be removed.
     */
    inline void remove( const char *key )
    {
        if ( !key )
            return;

        if ( remove_node( key, 0 ) )
            next.erase( *key );
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


    /**
     * @brief find          Find key in tree.
     * @param key           Key whose looking for.
     * @param finite_node   If true find finite node only.
     * @return              Iterator of found node. If not found return iterator equal to
     *                      iterator returned by end().
     */
    iterator find( const char *key, bool finite_node );


    /**
     * @brief find          Find key in tree.
     * @param key           Key whose looking for.
     * @param finite_node   If true find finite node only.
     * @return              Iterator of found node. If not found return iterator equal to
     *                      iterator returned by end().
     */
    inline iterator find( const std::string &key )
    {
        return find( key.c_str() );
    }


    /**
     * @brief begin                 Get iterator to first node.
     * @param finite_nodes_only     Iterate finite nodes only.
     * @return                      Iterator to first node.
     */
    iterator begin( bool finite_nodes_only );


    /**
     * @brief end
     * @return      Invalid iterator to use in loop as marker of looping end.
     */
    iterator end();

protected:
    inline bool is_finite_node() const
    {
        return flag == NODE_FLAG::FINITE_NODE;
    }


    /**
     * @brief new_node      Factory method to create new child node.
     *                      NOTE! The function must be overload in derived class
     *                      because all nodes in the tree have to equal type.
     * @return              Raw pointer to new object of prefix_tree type or derived.
     */
    virtual prefix_tree *new_node( prefix_tree *parent_ );


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
     * @brief remove_node   Remove node.
     * @param key           Key of node.
     * @param pos           Current symbol position in key.
     * @return              true if node must be removed.
     *                      If returned false node marked as non finite but
     *                      not removed.
     */
    bool remove_node( const char *key, unsigned int pos );


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
