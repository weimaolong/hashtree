/*
******************************************************************************
* Copyright (c) 2016 Tencent Inc. All rights reserved
* Author: markwei@tencent.com
* Date: 2016-10-30
* Description: 哈希树 - 质数分辨定理实现
******************************************************************************
*/

template <class KeyType, class ValueType, int SIZE = 32>
class HashTree
{
    struct Node
    {
        Node() : occupied_(false), child_({0}) {}
        KeyType key_;
        ValueType value_;
        bool occupied_;
        Node *child_[SIZE];
    };

public:
    HashTree()
    {
        max_level_ = 0;
        prime_ = {0};

        for (int i = 2; i <= SIZE; ++i)
        {
            if (IsPrime(i))
            {
                prime_[++max_level_] = i;
            }
        }
    }

    /**
     * 插入函数
     * @param key 键
     * @param value 值
     * @return 成功 true, 失败 false
     */
    bool Insert(const KeyType &key,  const ValueType &value)
    {
        return Insert(&root_, 1, key, value);
    }

    /**
     * 查找函数
     * @param key 键
     * @return 指向value的指针, 找不到时为NULL
     */
    const ValueType *Find(const KeyType &key)
    {
        return Find(&root_, 1, key);
    }

    /**
     * 删除函数
     * @param key 键
     * @return 任何情况下都返回true
     */
    bool Delete(const KeyType &key)
    {
        return Delete(&root_, 1, key);
    }

private:
    bool Insert(Node *node, int level, const KeyType &key, const ValueType &value)
    {
        if (!node->occupied_)
        {
            node->key_ = key;
            node->value_ = value;
            node->occupied_ = true;
            return true;
        }

        if (level > max_level_)
        {
            return false;
        }

        int idx = key % prime_[level];

        if (!node->child_[idx])
        {
            node->child_[idx] = new Node;
        }

        return Insert(node->child_[idx], ++level, key, value);
    }

    const ValueType *Find(Node *node, int level, const KeyType &key)
    {
        if (node->occupied_)
        {
            if (node->key_ == key)
            {
                return &node->value_;
            }
        }

        int idx = key % prime_[level];
        if (!node->child_[idx])
        {
            return 0;
        }

        return Find(node->child_[idx], ++level, key);
    }

    bool Delete(Node *node, int level, const KeyType &key)
    {
        if (node->occupied_)
        {
            if (node->key_ == key)
            {
                node->occupied_ = false;
                return true;
            }
        }

        int idx = key % prime_[level];
        if (!node->child_[idx])
        {
            return true;
        }

        return Delete(node->child_[idx], ++level, key);
    }
public:
    bool IsPrime(int n)
    {
        for (int i = 2; (i * i) < (n + 1); ++i)
        {
            if (!(n % i))
            {
                return false;
            }
        }
        return true;
    }

private:
    Node root_;
    int  max_level_;
    int  prime_[SIZE+1];
};


#include <iostream>

int main()
{
    HashTree<int, std::string> tree;

    tree.Insert(1111, "AAA");
    tree.Insert(2222, "BBB");

    const std::string *value;

    value = tree.Find(2222);
    std::cout << (value ? *value : "NULL") << std::endl;

    tree.Delete(2222);

    value = tree.Find(2222);
    std::cout << (value ? *value : "NULL") << std::endl;

    std::cout << tree.IsPrime(3);

    return 0;
}
