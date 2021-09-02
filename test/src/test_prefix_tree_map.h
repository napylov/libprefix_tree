#ifndef TEST_PREFIX_TREE_MAP_H
#define TEST_PREFIX_TREE_MAP_H

#include <gtest/gtest.h>
#include "prefix_tree/prefix_tree_map.h"

class test_prefix_tree_map : public testing::Test
{
public:
    std::unique_ptr<prefix_tree::prefix_tree_map<int> >   tree;

public:
    test_prefix_tree_map() = default;

    virtual void SetUp() override;
    virtual void TearDown() override;
};

#endif // TEST_PREFIX_TREE_H
