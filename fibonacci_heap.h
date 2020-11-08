#pragma once

#include <assert.h>
#include <vector>
#include <list>

namespace std {

    template <class _Ty, class _Pr = less<typename _Ty>, class _Alloc = allocator<_Ty>>
    class fibonacci_heap;

    

    /// <summary>
    /// Node type of double linked list
    /// </summary>
    /// <typeparam name="_Ty"> Data type </typeparam>
    template <class _Ty>
    class _DLlist_node {
    public:
        using _ptr_t = _DLlist_node<_Ty>*;
        _Ty data;
        _ptr_t pred;
        _ptr_t succ;

        _DLlist_node(const _Ty& _Val, _ptr_t pre) : data{ _Val }, pred{ pre }, succ{ pre->succ } {
            pre->succ = this;
            succ->pred = this;
        }
        _DLlist_node(const _Ty& _Val, _ptr_t const pre, _ptr_t const suc) : data{ _Val }, pred{ pre }, succ{suc} {
            pre->succ = this;
            suc->pred = this;
        }
        _DLlist_node(const _Ty& _Val) noexcept : data { _Val } {
            pred = this;
            succ = this;
        }
        ~_DLlist_node() {}

        _ptr_t add_to_left(const _Ty& _Val) {
            return new _DLlist_node<_Ty>(_Val, pred, this);
        }

        _ptr_t add_to_right(const _Ty& _Val) {
            return new _DLlist_node<_Ty>(_Val, this, succ);
        }

        void merge_to_right(_ptr_t l) {
            _ptr_t lpre = l->pred, suc = succ;
            l->pred = this;
            this->succ = l;
            lpre->succ = suc;
            suc->pred = lpre;
        }

        void merge_to_left(_ptr_t l) {
            _ptr_t lsuc = l->suc, pre = pred;
            l->suc = this;
            pred = l;
            lsuc->pre = pre;
            pre->succ = lsuc;
        }

        _ptr_t drop() {
            if (succ == this) return nullptr;
            succ->pred = pred;
            pred->succ = succ;
            return succ;
        }

        static _ptr_t remove(_ptr_t _Node) {
            _ptr_t ret = _Node->drop();
            delete _Node;
            return ret;
        }

        static void clear(_ptr_t _head) {
            _ptr_t pn, p = _head;
            do {
                pn = p->succ;
                delete p;
                p = pn;
            } while (p != _head);
        }
    };

    template <class _Ty, class _Pr = less<typename _Ty>>
    class _fibonacci_heap_node {
        friend fibonacci_heap<_Ty, _Pr>;
    private:
        using _ptr_t = _fibonacci_heap_node<_Ty, _Pr>*;
        _ptr_t    a_child;
        _ptr_t    parent;
        _ptr_t    left_sibling, right_sibling;
        _Ty     value;
        bool    marked = false;
        size_t  degree;

        _DLlist_node<_ptr_t>* root;
    public:
        /// <summary>
        /// Make a new node, add it to parent's children
        /// </summary>
        /// <param name="v"></param>
        /// <param name="parent"></param>
        _fibonacci_heap_node(const _Ty& _Val, _ptr_t _Father) : value{ _Val }, parent{ _Father }, a_child{ nullptr }, degree{ 0 }{
            parent->add_to_children(this);
        }
        /// <summary>
        /// Make a new root node
        /// </summary>
        /// <param name="v"></param>
        _fibonacci_heap_node(const _Ty& v) : value{ v }, parent{ nullptr }, root{ nullptr }, degree{ 0 }{
            left_sibling = this;
            right_sibling = this;
            a_child = nullptr;
        }
        
        /// <summary>
        /// Add node to its children
        /// </summary>
        /// <param name="node"></param>
        void add_to_children(_ptr_t _Node) {
            _Node->parent = this;
            _Node->root = root;
            if (a_child) {
                _Node->left_sibling = a_child->left_sibling;
                _Node->right_sibling = a_child;
                _Node->right_sibling->left_sibling = _Node;
                _Node->left_sibling->right_sibling = _Node;
            }
            else {
                a_child = _Node;
                _Node->left_sibling = _Node->right_sibling = _Node;
            }
            degree++;
        }

        /// <summary>
        /// cut this from its parent. (remain parent pointer, marked not changed)
        /// </summary>
        void cut() {
            if (!parent) return;
            if (right_sibling == this) {
                parent->a_child = nullptr;
            }
            else {
                left_sibling->right_sibling = right_sibling;
                right_sibling->left_sibling = left_sibling;
                left_sibling = right_sibling = this;
                parent->a_child = right_sibling;
            }
            parent->degree--;
        }

        /// <summary>
        /// clear all
        /// </summary>
        /// <param name="_head"></param>
        static void clear(_ptr_t rt) {
            _ptr_t _head = rt->a_child;
            delete rt;
            if (!_head) return;
            _ptr_t pn, p = _head;
            do {
                pn = p->right_sibling;
                clear(p);
                p = pn;
            } while (p != _head);
        }
    };
    
    /*
    template <class _Ty, class _Pr, class _Alloc>
    class fibonacci_heap_iterator {
    public:
        using _Alty = _Rebind_alloc_t<_Alloc, _Ty>;
        using _Alty_traits = allocator_traits<_Alty>;
        using reference = _Ty&;
        using pointer   = _Ty*;

        _fibonacci_heap_node<_Ty, _Pr>* _Ptr;

        _NODISCARD reference operator*() const {
            return const_cast<reference>(_Mybase::operator*());
        }
    public:
        _NODISCARD pointer operator->() const {
            return *_Ptr->value;
        }
    };
    */
    
    
    
    
    

    template <class _Ty, class _Pr, class _Alloc>
    class fibonacci_heap {
    private:
        using _Alty = _Rebind_alloc_t<_Alloc, _Ty>;
        using _Alty_traits = allocator_traits<_Alty>;

        
    public:
        using value_type        = _Ty;
        using value_compare     = _Pr;
        using reference         = _Ty&;
        using allocator_type    = _Alloc;
        using const_reference   = const _Ty&;
        using pointer           = typename _Alty_traits::pointer;
        using const_pointer     = typename _Alty_traits::const_pointer;
        using size_type         = typename _Alty_traits::size_type;
        using difference_type   = typename _Alty_traits::difference_type;

        //using iterator = fibonacci_heap_iterator<_Ty, _Pr, _Alloc>;
    private:
        using heap_node_t       = _fibonacci_heap_node<_Ty, _Pr>;
        using heap_ptr_t        = _fibonacci_heap_node<_Ty, _Pr>*;
        using root_list_node_t  = _DLlist_node<_fibonacci_heap_node<_Ty, _Pr>*>;
        using root_list_ptr_t   = _DLlist_node<_fibonacci_heap_node<_Ty, _Pr>*>*;

    private:
        root_list_ptr_t _min_pointer;
        size_type       _size;

        /// <summary>
        /// Add _Node to root list, compare and update min.
        /// <para> _Node will modified(_Node->parent, _Node->root, etc.) </para>
        /// </summary>
        /// <param name="_Node"></param>
        void add_to_root_list(heap_ptr_t _Node) {
            static _Pr comp;
            _Node->parent = nullptr;
            if (_min_pointer) {
                _Node->root = _min_pointer->add_to_left(_Node);
                if (comp(_Node->value, _min_pointer->data->value)) {
                    _min_pointer = _min_pointer->pred;
                }
            }
            else {
                _Node->root = _min_pointer = new root_list_node_t(_Node);
            }
        }

        /// <summary>
        /// Add _Node to root list, do not compare.
        /// _Node will be modified (_Node->parent, _Node->root, etc.)
        /// </summary>
        /// <param name="_Node"></param>
        void add_to_root_list_no_comp(heap_ptr_t _Node) {
            _Node->parent = nullptr;
            if (_min_pointer) 
                _Node->root = _min_pointer->add_to_left(_Node);
            else 
                _Node->root = _min_pointer = new root_list_node_t(_Node);
        }

        void consolidate() {
            static _Pr comp;

            size_t table_size = 2 * (size_t(log(_size)) + 1);
            root_list_ptr_t* degree_table = new root_list_ptr_t[table_size];
            memset(degree_table, 0, sizeof(root_list_ptr_t) * table_size);

            root_list_ptr_t pn, p = _min_pointer;
            do {
                pn = p->succ;
                root_list_ptr_t conflict = degree_table[p->data->degree];
                if (conflict) {
                    degree_table[p->data->degree] = nullptr;
                    if (comp(p->data->value, conflict->data->value)) {
                        p->data->add_to_children(conflict->data);
                        root_list_node_t::remove(conflict);
                        degree_table[p->data->degree] = p;
                    }
                    else {
                        conflict->data->add_to_children(p->data);
                        root_list_node_t::remove(p);
                        degree_table[conflict->data->degree] = conflict;
                    }

                }
                else {
                    degree_table[p->data->degree] = p;
                }
                p = pn;
            } while (p != _min_pointer);
            delete[] degree_table;
        }

        void find_min() {
            static _Pr comp;
            root_list_ptr_t _head = _min_pointer;
            if (!_head) return;
            root_list_ptr_t p = _head;
            do {
                if (comp(p->data->value, _min_pointer->data->value))
                    _min_pointer = p;
                p = p->succ;
            } while (p != _head);
        }

        /// <summary>
        /// Cut & Cascading Cut in while loops.
        /// </summary>
        /// <param name="_Node"></param>
        void cascading_cut(heap_ptr_t _Node) {
            heap_ptr_t temp_fa;
            while (_Node->parent && _Node->parent->marked) {
                temp_fa = _Node->parent;
                _Node->cut();
                _Node->marked = false;
                add_to_root_list_no_comp(_Node);
                _Node = temp_fa;

            }
            if (_Node->parent)
                _Node->parent->marked = true;
        }
    public:
        /// <summary>
        /// Make an empty heap
        /// </summary>
        fibonacci_heap() : _min_pointer{ nullptr }, _size{ 0 } {

        }

        /// <summary>
        /// Make a heap from std::initializer_list
        /// </summary>
        fibonacci_heap(const initializer_list<_Ty>& _Al) {
            for (_Ty it : _Al)
                add_to_root_list(new heap_node_t(it));
            _size = _Al.size();
        }

        /// <summary>
        /// Make a heap from std::vector
        /// </summary>
        explicit fibonacci_heap(const vector<_Ty>& _Al) {
            for (_Ty it : _Al)
                add_to_root_list(new heap_node_t(it));
            _size = _Al.size();
        }

        /// <summary>
        /// Make a heap from std::list
        /// </summary>
        explicit fibonacci_heap(const list<_Ty>& _Al) {
            for (_Ty it : _Al)
                add_to_root_list(new heap_node_t(it));
            _size = _Al.size();
        }

        ~fibonacci_heap() {
            if (!_min_pointer) return;
            root_list_ptr_t pn, p = _min_pointer;
            do {
                pn = p->succ;
                heap_node_t::clear(p->data);
                delete p;
                p = pn;
            } while (p != _min_pointer);
        }

        fibonacci_heap(const fibonacci_heap<_Ty, _Pr, _Alloc>& _Right) {

        }

        /// <summary>
        /// Merge two heap. 
        /// Time complexity : O(1).
        /// </summary>
        /// <param name="_Right"></param>
        void merge(fibonacci_heap<_Ty, _Pr>& _Right) {
            static _Pr comp;
            if (_Right._min_pointer) {
                _min_pointer->merge_to_right(_Right._min_pointer);
                if (comp(_Right._min_pointer->data->value, _min_pointer->data->value)) {
                    _min_pointer = _Right._min_pointer;
                }
                _Right._min_pointer = nullptr;
            }
            _size += _Right._size;
            _Right._size = 0;
        }
        
        /// <summary>
        /// Merge _Left and _Right into _Dest
        /// </summary>
        /// <param name="_Dest"></param>
        /// <param name="_Left"></param>
        /// <param name="_Right"></param>
        /// <returns></returns>
        static fibonacci_heap<_Ty, _Pr> merge(fibonacci_heap<_Ty, _Pr>& _Dest, fibonacci_heap<_Ty, _Pr>& _Left, fibonacci_heap<_Ty, _Pr>& _Right) {
            _Dest.merge(_Left);
            _Dest.merge(_Right);
        }
        
       

        /// <summary>
        /// Insert one element to heap
        /// Time complexity : O(1)
        /// </summary>
        /// <param name="_Val"></param>
        /// <returns></returns>
        const heap_ptr_t insert(const _Ty& _Val) {
            static _Pr comp;
            heap_ptr_t ret;
            add_to_root_list(ret = new _fibonacci_heap_node<_Ty, _Pr>(_Val));
            _size++;
            return ret;
        }

        /// <summary>
        /// Return top min
        /// </summary>
        /// <returns></returns>
        reference top() const{
            return _min_pointer->data->value;
        }

        /// <summary>
        /// Delete min:
        /// <para> 1. Delete min pointer and min root </para>
        /// <para> 2. Move chilren to root list </para>
        /// <para> 3. Consolidate </para>
        /// </summary>
        void pop() {
            assert(_min_pointer);
            _size--;
            heap_ptr_t rt = _min_pointer->data;
            _min_pointer = root_list_node_t::remove(_min_pointer);
            
            if (rt->a_child) {
                heap_ptr_t _head = rt->a_child;
                if (!_head) return; 
                heap_ptr_t pn, p = _head;
                do {
                    pn = p->right_sibling;
                    add_to_root_list_no_comp(p);
                    p = pn;
                } while (p != _head);
            }
            delete rt;
            if (!_min_pointer)
                return;
            find_min();
            consolidate();
        }

        

        void remove(heap_ptr_t _Node) {
            if (_Node->parent)
                cascading_cut(_Node);
            _min_pointer = _Node->root;
            pop();
        }

        /// <summary>
        /// Modify key of _Node to _Val
        /// </summary>
        /// <param name="_Node"></param>
        /// <param name="_Val"></param>
        void modify_key(heap_ptr_t _Node, const_reference _Val) {
            static _Pr comp;
            // Decrease
            if (comp(_Val, _Node->value)) {
                _Node->value = _Val;
                if (_Node->parent && comp(_Val, _Node->parent->value)) 
                    cascading_cut(_Node);

                if (comp(_Val, _min_pointer->data->value)) 
                    _min_pointer = _Node->root;
            }
            // Increase
            if (comp(_Node->value, _Val)) {
                _Node->value = _Val;

                heap_ptr_t _head = _Node->a_child;
                if (!_head) return;
                heap_ptr_t pn, p = _head;
                do {
                    pn = p->right_sibling;
                    add_to_root_list_no_comp(p);
                    p = pn;
                } while (p != _head);
                _Node->a_child = nullptr;
                cascading_cut(_Node);
            }
        }

        /// <summary>
        /// Swap components of two heap
        /// </summary>
        /// <param name="_Right"></param>
        void swap(fibonacci_heap<_Ty, _Pr>& _Right) {
            std::swap(_min_pointer, _Right._min_pointer);
            std::swap(_size, _Right._size);
        }

        size_type size() noexcept{
            return _size;
        }

        bool empty() noexcept {
            return _size == 0;
        }
    };
}
