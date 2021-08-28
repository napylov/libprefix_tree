#ifndef TEST_PREFIX_TREE_H
#define TEST_PREFIX_TREE_H

#include <gtest/gtest.h>
#include "prefix_tree/prefix_tree.h"

class test_prefix_tree : public testing::Test
{
public:
    std::unique_ptr<prefix_tree::prefix_tree>   tree;

public:
    test_prefix_tree() = default;

    virtual void SetUp() override;
    virtual void TearDown() override;
};

#endif // TEST_PREFIX_TREE_H
