#include <iostream>
#include <fstream>
#include <vector>

template<class T>
class AVLTree {
    struct Node {
        T data;
        int h;
        int balanceFactor;
        Node* parent = nullptr, * right = nullptr, * left = nullptr;
    };

    void _printTree(Node* root) {
        if (root != NULL)
        {
            std::cout << root->data << " ";
            _printTree(root->left);
            _printTree(root->right);
        }
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        if (y->left == nullptr)
            y->h = y->right->h + 1;
        else
            y->h = std::max(y->left->h, y->right->h) + 1;
        if (x->left == nullptr)
            x->h = x->right->h + 1;
        else
            x->h = std::max(x->left->h, x->right->h) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        if (x->right == nullptr)
            x->h = x->left->h + 1;
        else
            x->h = std::max(x->left->h, x->right->h) + 1;
        if (y->right == nullptr)
            y->h = y->left->h + 1;
        else
            y->h = std::max(y->left->h, y->right->h) + 1;
        return y;
    }

    int calcBalanceFactor(Node* N) {
        if (N == NULL || N->right == nullptr && N->left == nullptr)
            return 0;
        if (N->right == nullptr)
            return N->left->h;
        if (N->left == nullptr)
            return 0 - N->right->h;
        return (N->left->h - N->right->h);
    }

    Node* newNode(int data) {
        Node* node = new Node();
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        node->h = 1;
        return (node);
    }

    Node* insertNode(Node* node, T newData) {
        if (node == nullptr)
            return new Node{ newData, 0, 0, nullptr, nullptr, nullptr };
        if (newData < node->data)
            node->left = insertNode(node->left, newData);
        else if (newData > node->data)
            node->right = insertNode(node->right, newData);
        else // Equal keys are not allowed in BST
            return node;

        /* 2. Update height of this ancestor node */
        node->h =
            1 + std::max(node->left == nullptr ? 0 : node->left->h,
                node->right == nullptr ? 0 : node->right->h);

        int balance = calcBalanceFactor(node);

        if (balance > 1 && newData < node->left->data)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && newData > node->right->data)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && newData > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && newData < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void getLevelCount(Node* n, int count, int& max_count) const {
        if (n == nullptr)
            max_count = (max_count > count) ? max_count : count;
        else {
            count++;
            getLevelCount(n->left, count, max_count);
            getLevelCount(n->right, count, max_count);
        }
    }

public:
    Node* root;

    AVLTree(T rootData) {
        root = new Node{ rootData, 1, 0, nullptr, nullptr, nullptr };
    }

    void print() {
        _printTree(root);
    }

    int insert(T newData) {
        auto iter = root;

        while (true) {
            if (iter->data > newData)
                if (iter->left == nullptr) {
                    insertNode(iter, newData);
                    return 0;
                }
                else
                    iter = iter->left;
            else if (iter->data < newData)
                if (iter->right == nullptr) {
                    insertNode(iter, newData);
                    return 0;
                }
                else
                    iter = iter->right;
            else {
                return -1;
            }
        }
    }

    int getHeight() const {
        int ans = INT_MIN;
        getLevelCount(root, 0, ans);
        return ans;
    }
};



namespace std {
    string to_string(char c) {
        std::string res;
        res.push_back(c);
        return res;
    }

    string to_string(string s) {
        return s;
    }
}

template<class T>
class BinarySearchTree {
    struct Node {
        Node* parent = nullptr,
            * right = nullptr,
            * left = nullptr;
        T data;
    };

    Node* root = nullptr;

public:
    BinarySearchTree() = default;

    BinarySearchTree(const T& item) {
        root = new Node{ nullptr, nullptr, nullptr, item };
    }

    template<class IT>
    BinarySearchTree(IT begin, IT end) {
        root = new Node{ nullptr, nullptr, nullptr, *begin };
        ++begin;
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    int insert(T item) {
        auto iter = root;
        auto newNode = new Node{ root, nullptr, nullptr, item };

        while (true) {
            if (iter->data > item)
                if (iter->left == nullptr) {
                    iter->left = newNode;
                    newNode->parent = iter;
                    return 0;
                }
                else
                    iter = iter->left;
            else if (iter->data < item)
                if (iter->right == nullptr) {
                    iter->right = newNode;
                    newNode->parent = iter;
                    return 0;
                }
                else
                    iter = iter->right;
            else {
                delete newNode;
                return -1;
            }
        }
    }

    void remove(T item) {
        auto iter = root;

        while (true)
            if (iter->data > item)
                if (iter->left == nullptr)
                    return;
                else
                    iter = iter->left;
            else if (iter->data < item)
                if (iter->right == nullptr)
                    return;
                else
                    iter = iter->right;
            else { // contains item in tree

                if (iter->right == nullptr && iter->left == nullptr) {
                    // element to be removed has no children

                    if (iter->parent->right == iter)
                        iter->parent->right = nullptr;
                    else iter->parent->left = nullptr;

                    delete iter;
                    iter = nullptr;
                }
                else if (iter->right == nullptr) {
                    // element to be removed has only left child

                    if (iter->parent->right == iter)
                        iter->parent->right = iter->left;
                    else
                        iter->parent->left = iter->left;

                    iter->left->parent = iter->parent;

                    delete iter;
                    iter = nullptr;
                }
                else if (iter->left == nullptr) {
                    // element to be removed has only right child

                    if (iter->parent->right == iter)
                        iter->parent->right = iter->right;
                    else
                        iter->parent->left = iter->right;

                    iter->right->parent = iter->parent;

                    delete iter;
                    iter = nullptr;
                }
                else {
                    // element to be removed has 2 children

                    Node* succ = findPreSuccessorNode(root, nullptr, item);
                    iter->data = succ->data;

                    if (succ->parent->right == succ)
                        succ->parent->right = nullptr;
                    else succ->parent->left = nullptr;
                    delete succ;

                    succ = nullptr;
                }
                return;
            }

    }

    void bfsPrint() {
        int levelsCount = this->getHeight();
        for (int i = 1; i <= levelsCount; ++i) {
            printLevel(root, i);
            std::cout << "\n";
        }
    }

    T findInorderSuccessor(T item) {
        if (!this->contains(item))
            return item;

        Node* succ = findSuccessorNode(root, nullptr, item);

        return succ->data;
    }

    T findPreorderSuccessor(T item) {
        if (!this->contains(item))
            return item;

        Node* succ = findPreSuccessorNode(root, nullptr, item);

        return succ->data;
    }

    T getMin() {
        auto iter = root;
        while (iter->left != nullptr)
            iter = iter->left;
        return iter->data;
    }

    T getMax() {
        auto iter = root;
        while (iter->right != nullptr)
            iter = iter->right;
        return iter->data;
    }

    bool contains(T item) {
        auto Iter = root;

        while (true) {
            if (Iter->data > item)
                if (Iter->left == nullptr)
                    return false;
                else
                    Iter = Iter->left;
            else if (Iter->data < item)
                if (Iter->right == nullptr)
                    return false;
                else
                    Iter = Iter->right;
            else
                return true;

        }
    }

    void inorderPrint() {
        inorder(root);
        std::cout << "\n";
    }

    void preorderPrint() {
        preorder(root);
        std::cout << "\n";
    }

    void postorderPrint() {
        postorder(root);
        std::cout << "\n";
    }

    int getHeight() const {
        int ans = INT_MIN;
        getLevelCount(root, 0, ans);
        return ans;
    }

    void beautyPrint() {
        int maxWidth = getMaxElemWidth();
        maxWidth = maxWidth % 2 ? maxWidth : maxWidth + 1;
        for (int h = this->getHeight(), ws = 1; h > 0; --h) {

            auto lev = this->getLevel(h);
            std::cout << std::string(ws / 2, ' ');
            for (int i = 0; i < lev.size(); ++i) {
                int curWidth = lev[i].size();
                if (curWidth == 0) {
                    lev[i] = std::string(maxWidth, '*');
                    curWidth = maxWidth;
                }
                int l_ws = (maxWidth - curWidth) / 2;
                int r_ws = curWidth % 2 ? l_ws : l_ws + 1;

                printf("%*s%s%*s", l_ws, "", lev[i].c_str(), r_ws, "");
                std::cout << std::string(ws, ' ');
            }
            std::cout << "\n";
            ws = ws * 2 + 1 + (maxWidth / 2) * 2;
        }
    }

    std::vector<std::string> getLevel(int level) const {
        std::vector<std::string> res;
        getLevelRec(root, res, level);

        return res;
    }

    ~BinarySearchTree() {
        deleteTree(root);
    }


private:
    Node* findSuccessorNode(Node* n, Node* succ, int key) {
        if (n == nullptr)
            return succ;

        if (n->data == key) {
            if (n->right != nullptr)
                return findMinNode(n->right);
        }
        else if (key < n->data) {
            succ = n;
            return findSuccessorNode(n->left, succ, key);
        }
        else
            return findSuccessorNode(n->right, succ, key);

        return succ;
    }

    Node* findPreSuccessorNode(Node* n, Node* succ, int key) {
        if (n == nullptr)
            return succ;

        if (n->data == key) {
            if (n->left != nullptr)
                return findMaxNode(n->left);
        }
        else if (key > n->data) {
            succ = n;
            return findPreSuccessorNode(n->right, succ, key);
        }
        else
            return findPreSuccessorNode(n->left, succ, key);

        return succ;
    }

    Node* findMinNode(Node* n) {
        while (n->left)
            n = n->left;
        return n;
    }

    Node* findMaxNode(Node* n) {
        while (n->right)
            n = n->right;
        return n;
    }

    void printLevel(Node* n, int level) {
        if (n == nullptr)
            return;
        else if (level == 1)
            std::cout << n->data << " ";
        else if (level > 1) {
            printLevel(n->left, level - 1);
            printLevel(n->right, level - 1);
        }
    }

    void getLevelRec(Node* n, std::vector<std::string>& res, int level) const {
        if (n == nullptr && level == 1) {
            res.push_back("");
            return;
        }
        else if (n == nullptr) {
            for (int i = 0; i < level; ++i) {
                res.push_back("");
            }
            return;
        }
        else if (level == 1)
            res.push_back(std::to_string(n->data));
        else if (level > 1) {
            getLevelRec(n->left, res, level - 1);
            getLevelRec(n->right, res, level - 1);
        }
    }

    void getLevelCount(Node* n, int count, int& max_count) const {
        if (n == nullptr)
            max_count = (max_count > count) ? max_count : count;
        else {
            count++;
            getLevelCount(n->left, count, max_count);
            getLevelCount(n->right, count, max_count);
        }
    }

    void preorder(Node* n) {
        if (n != nullptr) {
            std::cout << n->data << " ";
            preorder(n->left);
            preorder(n->right);
        }
    }

    void inorder(Node* n) {
        if (n != nullptr) {
            inorder(n->left);

            std::cout << n->data << " ";

            inorder(n->right);
        }
    }

    void postorder(Node* n) {
        if (n != nullptr) {
            postorder(n->right);
            std::cout << n->data << " ";
            postorder(n->left);
        }
    }

    void deleteTree(Node* n) {
        if (n != nullptr) {
            deleteTree(n->right);
            deleteTree(n->left);
            delete n;
            n = nullptr;
        }
    }

    int getMaxElemWidth() {
        int maxWidth = 0;

        for (int h = this->getHeight(); h > 0; --h) {
            auto lev = this->getLevel(h);
            for (const auto& item : lev)
                if (item.size() > maxWidth)
                    maxWidth = item.size();

        }

        return maxWidth;
    }


};

template<class T>
class RBTree {
    struct Node {
        T data;
        Node* parent;
        Node* left;
        Node* right;
        int color;
    };

    Node* root;
    Node* TNULL;

    void insertFix(Node* k) {
        Node* u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    lRotate(k->parent->parent);
                }
            }
            else {
                u = k->parent->parent->right;

                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        lRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    void _print(Node* root, string indent, bool last) {
        if (root != TNULL) {
            cout << indent;
            if (last) {
                cout << "R    ";
                indent += "   ";
            }
            else {
                cout << "L    ";
                indent += "|  ";
            }

            string sColor = root->color ? "R" : "B";
            cout << root->data << "(" << sColor << ")" << endl;
            _print(root->left, indent, false);
            _print(root->right, indent, true);
        }
    }

    void getLevelCount(Node* n, int count, int& max_count) const {
        if (n == nullptr)
            max_count = (max_count > count) ? max_count : count;
        else {
            count++;
            getLevelCount(n->left, count, max_count);
            getLevelCount(n->right, count, max_count);
        }
    }
public:
    RBTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    int getHeight() const {
        int ans = INT_MIN;
        getLevelCount(root, 0, ans);
        return ans;
    }

    void lRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void insert(T key) {
        Node* node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;

        Node* y = nullptr;
        Node* x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        }
        else if (node->data < y->data) {
            y->left = node;
        }
        else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = 0;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        insertFix(node);
    }

    void printTree() {
        if (root) {
            _print(this->root, "", true);
        }
    }
};

#include <chrono>

std::pair<std::vector<long long>, std::vector<long long>> bstTest() {
    std::fstream in("../test_data/10.txt", std::ios_base::in);

    if (!in.is_open())
        throw;

    std::vector<long long> bstTimes, bstHeights;
    BinarySearchTree<int> bsTree(1);

    auto start = std::chrono::high_resolution_clock::now();
    int data;
    while (in >> data) {
        bsTree.insert(data);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());
    bstHeights.push_back(bsTree.getHeight());

    in.close();
    in.open("../test_data/100.txt", std::ios_base::in);
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree1(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree1.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());
    bstHeights.push_back(bsTree1.getHeight());

    in.close();
    in.open("../test_data/1000.txt");
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree2(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree2.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());
    bstHeights.push_back(bsTree2.getHeight());

    in.close();
    in.open("../test_data/3000.txt");
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree3(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree3.insert(data);
    }
    in.close();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());

    bstHeights.push_back(bsTree3.getHeight());

    in.close();
    in.open("../test_data/5000.txt");
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree4(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree4.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());

    bstHeights.push_back(bsTree4.getHeight());


    in.close();
    in.open("../test_data/20000.txt");
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree5(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree5.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());

    bstHeights.push_back(bsTree5.getHeight());


    in.close();
    in.open("../test_data/60000.txt");
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree7(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree7.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());

    bstHeights.push_back(bsTree7.getHeight());

    in.close();
    in.open("../test_data/100000.txt");
    if (!in.is_open())
        throw;

    BinarySearchTree<int> bsTree6(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        bsTree6.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    bstTimes.push_back(duration.count());

    bstHeights.push_back(bsTree6.getHeight());


    in.close();

    return { bstTimes, bstHeights };
}


std::pair<std::vector<long long>, std::vector<long long>> rbTest() {
    std::fstream in("../test_data/10.txt", std::ios_base::in);

    if (!in.is_open())
        throw;

    std::vector<long long> rbTimes, rbHeights;
    RBTree<int> rbTree;

    auto start = std::chrono::high_resolution_clock::now();
    int data;
    while (in >> data) {
        rbTree.insert(data);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());
    rbHeights.push_back(rbTree.getHeight());
    in.close();
    in.open("../test_data/100.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree1;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree1.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());
    rbHeights.push_back(rbTree1.getHeight());

    in.close();
    in.open("../test_data/1000.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree2;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree2.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());

    rbHeights.push_back(rbTree2.getHeight());

    in.close();
    in.open("../test_data/3000.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree3;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree3.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());
    rbHeights.push_back(rbTree3.getHeight());

    in.close();
    in.open("../test_data/5000.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree4;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree4.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());
    rbHeights.push_back(rbTree4.getHeight());


    in.close();
    in.open("../test_data/20000.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree5;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree5.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());

    rbHeights.push_back(rbTree5.getHeight());

    in.close();
    in.open("../test_data/60000.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree7;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree7.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());

    rbHeights.push_back(rbTree7.getHeight());

    in.close();
    in.open("../test_data/100000.txt");
    if (!in.is_open())
        throw;

    RBTree<int> rbTree6;

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        rbTree6.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    rbTimes.push_back(duration.count());

    rbHeights.push_back(rbTree6.getHeight());


    return { rbTimes, rbHeights };
}



std::pair<std::vector<long long>, std::vector<long long>> avlTest() {
    std::fstream in("../test_data/10.txt", std::ios_base::in);

    if (!in.is_open())
        throw;

    std::vector<long long> avlTimes, avlHeights;
    AVLTree<int> avlTree(1);

    auto start = std::chrono::high_resolution_clock::now();
    int data;
    while (in >> data) {
        avlTree.insert(data);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());

    in.close();
    in.open("../test_data/100.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree1(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree1.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());

    in.close();
    in.open("../test_data/1000.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree2(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree2.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());

    in.close();
    in.open("../test_data/3000.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree3(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree3.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());

    in.close();
    in.open("../test_data/5000.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree4(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree4.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());


    in.close();
    in.open("../test_data/20000.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree5(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree5.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());


    in.close();
    in.open("../test_data/60000.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree7(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree7.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());
    in.close();
    in.open("../test_data/100000.txt");
    if (!in.is_open())
        throw;

    AVLTree<int> avlTree6(1);

    start = std::chrono::high_resolution_clock::now();
    while (in >> data) {
        avlTree6.insert(data);
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    avlTimes.push_back(duration.count());


    return { avlTimes, avlHeights };
}