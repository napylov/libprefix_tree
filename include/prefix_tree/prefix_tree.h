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
    /// Parent node.
    prefix_tree                             *parent;


public:
    class iterator
    {
    private:
        prefix_tree                 *node;
        bool                        finite_nodes_only;
        std::deque<unsigned char>   symbols;

    public:
        iterator() : node( nullptr ), finite_nodes_only( false), symbols() {}
        iterator( prefix_tree *node_, bool finite_nodes_only_, const char *key );

        ~iterator() {}

        iterator& operator++();
        iterator& operator--();
        prefix_tree* operator->();
    private:
        void shift_iterator( bool forward );

        template <typename iterator_type>
        void increment_via_next(
                iterator_type it,
                iterator_type it_end,
                prefix_tree *cur,
                bool forward
        )
        {
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
                        if ( forward )
                            increment_via_next( it->second->next.begin(), it->second->next.end(), it->second.get(), forward );
                        else
                            increment_via_next( it->second->next.rbegin(), it->second->next.rend(), it->second.get(), forward );

                        if ( node )
                            return;
                        else if ( !symbols.empty() )
                            symbols.pop_back();
                    }

                    ++it;
                }

                node = nullptr;
            }
        }

        void increment_via_parent( bool forward );

        template <typename iterator_type>
        void increment_via_parent( iterator_type it, iterator_type it_end, prefix_tree *cur, bool forward )
        {
            if ( !finite_nodes_only )
            {
                node = it != it_end ? it->second.get() : nullptr;
                return;
            }

            increment_via_next( it, it_end, cur, forward );
            if ( !node )
            {
                node = cur;
                increment_via_parent( forward );
            }
        }
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


    inline bool is_finite_node() const
    {
        return flag == NODE_FLAG::FINITE_NODE;
    }


    iterator begin( bool finite_nodes_only );
    iterator end();


protected:
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
     * @brief find_node     Find node by key.
     * @param key           Key ot prefix.
     * @param finite_node   If true looking for finite node only else prefix or finite node.
     * @return              Raw const pointer to found node or nullptr.
     */
    const prefix_tree* find_node( const char *key, bool finite_node = true ) const;
};


} // namespace prefix_tree

#endif // PREFIX_TREE_H
