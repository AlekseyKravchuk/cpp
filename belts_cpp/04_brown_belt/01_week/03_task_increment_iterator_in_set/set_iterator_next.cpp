#include "test_runner.h"
#include <cassert>
#include <deque>
#include <iostream>

using namespace std;

struct Node {
    Node(int v, Node* p)
        : value(v),
          parent(p) {}

    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent;
};

class NodeBuilder {
  public:
    Node* CreateRoot(int value) {
        _nodes.emplace_back(value, nullptr);
        return &_nodes.back();
    }

    Node* CreateLeftSon(Node* me, int value) {
        assert(me->left == nullptr);

        _nodes.emplace_back(value, me);
        me->left = &_nodes.back();
        return me->left;
    }

    Node* CreateRightSon(Node* me, int value) {
        assert(me->right == nullptr);

        _nodes.emplace_back(value, me);
        me->right = &_nodes.back();
        return me->right;
    }

  private:
    std::deque<Node> _nodes;
};

Node* GetLeftMost(Node* me) {
    assert(me);
    while (me->left) {
        me = me->left;
    }
    return me;
}

Node* Next(Node* me) {
    // всегда проверяем, а не nullptr нам передали в качестве указателя
    assert(me);

    // если справа есть узел или поддерево
    if (me->right) {
        return GetLeftMost(me->right);
    }

    // если текущий узел является правым ребенком, поднимаемся вверх по линии правых детей
    while (me->parent && me->parent->right == me) {
        me = me->parent;
    }

    return me->parent;
}

void Test1() {
    NodeBuilder nb;

    Node* root = nb.CreateRoot(50);
    ASSERT_EQUAL(root->value, 50);

    Node* l = nb.CreateLeftSon(root, 2);
    Node* min = nb.CreateLeftSon(l, 1);
    Node* r = nb.CreateRightSon(l, 4);
    ASSERT_EQUAL(min->value, 1);
    ASSERT_EQUAL(r->parent->value, 2);

    nb.CreateLeftSon(r, 3);
    nb.CreateRightSon(r, 5);

    r = nb.CreateRightSon(root, 100);
    l = nb.CreateLeftSon(r, 90);
    nb.CreateRightSon(r, 101);

    nb.CreateLeftSon(l, 89);
    r = nb.CreateRightSon(l, 91);

    ASSERT_EQUAL(Next(l)->value, 91);
    ASSERT_EQUAL(Next(root)->value, 89);
    ASSERT_EQUAL(Next(min)->value, 2);
    ASSERT_EQUAL(Next(r)->value, 100);

    while (min) {
        std::cout << min->value << '\n';
        min = Next(min);
    }
}

void TestRootOnly() {
    NodeBuilder nb;
    Node* root = nb.CreateRoot(42);
    ASSERT(Next(root) == nullptr);
};

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, TestRootOnly);
    return 0;
}
