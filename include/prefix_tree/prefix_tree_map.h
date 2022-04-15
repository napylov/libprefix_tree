/**
 * Prefix tree library.
 * 
 * (c) 2021 Alexander Napylov
 * BSD 2-clause license.
 */

#ifndef PREFIX_TREE_MAP_H
#define PREFIX_TREE_MAP_H

#include "prefix_tree.h"


namespace prefix_tree
{

/**
 * @brief prefix_tree_map       key => value container
 *                              implemented as prefix tree.
 */
template <typename value_type>
class prefix_tree_map : protected prefix_tree
{
private:
    value_type                              value;


public:
    class iterator : public prefix_tree::iterator
    {
    public:
        iterator() : prefix_tree::iterator() {}
        iterator( prefix_tree_map *node_, const char *key )
            : prefix_tree::iterator( node_, true, key ) {}
        iterator( const iterator & ) = default;

        ~iterator() = default;

        inline iterator& operator=( const iterator& _ )
        {
            prefix_tree::iterator::operator=( _ );
            return *this;
        }


        /**
         * @brief get_value
         * @return              Value of node.
         */
        inline const value_type& get_value()
        {
            return static_cast<prefix_tree_map*>( node )->value;
        }

    protected:
        prefix_tree_map* operator->()
        {
            return static_cast<prefix_tree_map*>( prefix_tree::iterator::operator->() );
        }
    };

    

protected:
    prefix_tree_map( prefix_tree *parent_ ) : prefix_tree( parent_ ), value() {}

public:
    prefix_tree_map() : prefix_tree(), value() {}
    virtual ~prefix_tree_map() = default;


    /**
     * @brief append    Append node to the tree.
     * @param key       Key.
     * @param value_    Value.
     * @return          true if success
     */
    inline bool append( const char *key, value_type &&value_ )
    {
        auto appended = append_node( key );
        if ( !appended.second )
            return false;

        static_cast<prefix_tree_map&>( appended.first ).value = std::move( value_ );
        return true;
    }


    /**
     * @brief append    Append node to the tree.
     * @param key       Key.
     * @param value_    Value.
     * @return          true if success
     */
    inline bool append( const std::string &key, value_type &&value_ )
    {
        return append( key.c_str(), value_ );
    }


    /**
     * @brief append    Append node to the tree.
     * @param key       Key.
     * @param value_    Value.
     * @return          true if success
     */
    inline bool append( const char *key, const value_type &value_ )
    {
        auto appended = append_node( key );
        if ( !appended.second )
            return false;

        static_cast<prefix_tree_map&>( appended.first ).value = value_;
        return true;
    }


    /**
     * @brief append    Append node to the tree.
     * @param key       Key.
     * @param value_    Value.
     * @return          true if success
     */
    inline bool append( const std::string &key, const value_type &value_ )
    {
        return append( key.c_str(), value_ );
    }


    /**
     * @brief find      Find node by key.
     * @param key       Key.
     * @return          Iterator to found node or iterator equal
     *                  iterator returned by end() if not found.
     */
    iterator find( const char *key )
    {
        prefix_tree *found = (prefix_tree*)( find_node( key ) );
        return found ?
                    iterator( static_cast<prefix_tree_map*>( found ), key ) :
                    iterator()
        ;
    }


    /**
     * @brief find      Find node by key.
     * @param key       Key.
     * @return          Iterator to found node or iterator equal
     *                  iterator returned by end() if not found.
     */
    inline iterator find( const std::string &key )
    {
        return find( key.c_str() );
    }


    /**
     * @brief exists        Check key or prefix is exist.
     * @param key           Key ot prefix.
     * @param finite_node   If true looking for finite node only else prefix or finite node.
     * @return              true if key or prefix is exist.
     */
    inline bool exists( const char *key, bool finite_node = true ) const
    {
        return prefix_tree::exists(key, finite_node);
    }


    /**
     * @brief exists        Check key or prefix is exist.
     * @param key           Key ot prefix.
     * @param finite_node   If true looking for finite node only else prefix or finite node.
     * @return              true if key or prefix is exist.
     */
    inline bool exists( const std::string &key, bool finite_node = true ) const
    {
        return prefix_tree::exists(key, finite_node);
    }


    /**
     * @brief begin
     * @return          Iterator to first node.
     */
    iterator begin()
    {
        iterator it( this, nullptr );
        ++it;

        return it;
    }


    /**
     * @brief end
     * @return          Invalid iterator to use in loop as end marker.
     */
    iterator end() { return iterator(); }

protected:
    virtual prefix_tree *new_node( prefix_tree *parent_ ) override
    {
        return new prefix_tree_map<value_type>( parent_ );
    }
};


} // namespace prefix_tree


#endif // PREFIX_TREE_H
