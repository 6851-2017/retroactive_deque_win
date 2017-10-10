bool f_ck_yeah = true;

/* Code written by Kliment Serafimov */

#include <fstream>
#include <iomanip>
#include <iostream>

#include <cmath>
#include <queue>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#define f first
#define s second
#define pb push_back
#define mp make_pair
#define MAX 2139062143
#define inf MAX/2
#define MAX_63  1061109567
#define MAXll 9187201950435737471
#define rand(a, b) ((rand()%(b-a+1))+a)
#define MEM(a, b) memset(a, b, sizeof(a))
#define sort_v(a) sort(a.begin(), a.end())
#define rev_v(a)  reverse(a.begin(), a.end())

///for file reads/writes
//#define fin cin
//#define fout cout
using namespace std;
//ifstream fin(".in");
//ofstream fout(".out");

#define maxB 8
///NODE of A-B Tree

#define NUM_TESTS 10000
#define AUTO_TEST 1
#define random_seed 0

#define test_stack 0
#define max_n 128
#define max_constant max_n*10
#define max_time max_n*10
#define print_wrong_input 1
#define print_input 0

#define print_queries (1-AUTO_TEST)
#define deep_print print_queries

///type, description
///0, pop R
///1, push R
///2, pop L
///3, push L
///4, query R
///5, query L
///6, delete
///queries are in the form of (time, type, key_value) if type == 1 or type == 3; Or (type, type) otherwise

string int_to_string(int n)
{
    if(n == -inf)
    {
        return "/";
    }
    if(n == 0)
    {
        return "0";
    }
    string ret;
    while(n>0)
    {
        ret+=(n%10)+'0';
        n/=10;
    }
    rev_v(ret);
    return ret;
}

class one_side
{
public:
    int total_adds;
    int total_removes;

    int removes;
    int adds;
    int val;

    void init()
    {
        removes = 0;
        adds = 0;
        val = -inf;
        total_adds = 0;
        total_removes = 0;
    }
    one_side()
    {
        init();
    }
    one_side(one_side* to_copy)
    {
        removes = to_copy->removes;
        adds = to_copy->adds;
        val = to_copy->val;
        total_adds = to_copy->total_adds;
        total_removes = to_copy->total_removes;
    }
    bool is_pop(int type)
    {
        return (type%2 == 0);
    }
    bool is_push(int type)
    {
        return (type%2 == 1);
    }
    void update(int type, int _val)
    {
        assert(0<=type && type <=4);
        if(is_pop(type))
        {
            removes = 1;
            total_removes = 1;
            val = -inf;
        }
        else if(is_push(type))
        {
            adds = 1;
            total_adds = 1;
            val = _val;
        }
        else
        {
            assert(0);
        }
    }
    void merge(one_side *right_operand)
    {
        total_adds+=right_operand->total_adds;
        total_removes+=right_operand->total_removes;
        removes = removes-min(0, adds-right_operand->removes);
        adds = max(0, adds-right_operand->removes) + right_operand->adds;
        if(adds>0)
        {
            if(right_operand->val != -inf)
            {
                val = right_operand->val;
            }
        }
    }
    string print(one_side *p)
    {
        return "val=" + int_to_string(p->val) + ", [max delta: -=" + int_to_string(p->removes) + ", +=" +int_to_string(p->adds) +"],[total: +="+int_to_string(p->total_adds) +", t-="+int_to_string(p->total_removes)+ "]";
    }
};

class element
{
public:
    one_side *one[2];
    void init()
    {
        one[0] = new one_side();
        one[1] = new one_side();
    }
    element()
    {
        init();
    }
    element(element* to_copy)
    {
        one[0] = new one_side(to_copy->one[0]);
        one[1] = new one_side(to_copy->one[1]);
    }
    bool is_R(int type)
    {
        return (0<=type && type<2);
    }
    bool is_L(int type)
    {
        return (type>=2 && type<4);
    }
    element(int type, int _val)
    {
        init();
        if(is_R(type))
        {
            one[0]->update(type, _val);
        }
        else if(is_L(type))
        {
            one[1]->update(type, _val);
        }
        else
        {
            assert(0);
        }
    }
    void merge(element *right_operand)
    {
        one[0]->merge(right_operand->one[0]);
        one[1]->merge(right_operand->one[1]);
    }
    int get(int side)
    {
        return one[side]->val;
    }
    int get_prefix(int side)
    {
        return one[side]->total_adds-one[side]->total_removes;
    }
    string print()
    {
        return "L = {"+one[1]->print(one[1]) +"}, R = {"+ one[0]->print(one[0]) +"}";
    }
};
class node
{
public:
    int a, b;

    int num_children;
    node* child[maxB+1];
    int left_bound;
    int right_bound;

    element* operation;

    node(int _a, int _b, int moment, int _right_bound, element* operand)
    {
        a = _a;
        b = _b;
        left_bound = moment;
        right_bound = _right_bound;
        num_children = 0;
        operation = operand;
    }
    node(int _a, int _b)
    {
        a = _a;
        b = _b;
    }

    void update_bounds()
    {
        if(num_children>0)
        {
            left_bound = child[0]->left_bound;
            right_bound = child[num_children-1]->right_bound;
        }
    }
    void update_operand()
    {
        if(num_children>0)
        {
            operation = new element();
            for(int i = 0; i<num_children; i++)
            {
                if(i!=num_children-1)
                {
                    child[i]->right_bound = child[i+1]->left_bound;
                }
                operation->merge(child[i]->operation);
            }
            update_bounds();
        }
    }
    node* create_leaf(int moment, int type, int val)
    {
        if(moment>=left_bound)
        {
            assert(moment<right_bound);
            int old_rb = right_bound;
            right_bound = min(right_bound, moment);
            node* ret_node = new node(a, b, moment, old_rb, new element(type, val));
            return ret_node;
        }
        else
        {
            node *new_node = new node(a, b, left_bound, right_bound, operation);
            right_bound = left_bound;
            left_bound = moment;
            operation = new element(type, val);
            return new_node;
        }
    }
    node* insert_node(int moment, int type, int val)
    {
        //assert(left_bound<=moment && moment<right_bound);
        node* new_node = NULL;
        bool do_break = false;
        for(int i = 0; i<num_children&&!do_break; i++)
        {
            bool is_smallest = (i == 0 && moment < left_bound);
            if(is_smallest || (child[i]->left_bound <=moment && moment < child[i]->right_bound))
            {
                new_node = child[i]->insert(moment, type, val);
                if(new_node != NULL)
                {
                    for(int j = num_children; j>i; j--)
                    {
                        child[j+1] = child[j];
                    }
                    int new_child_at = i+1;
                    child[new_child_at] = new_node;
                    num_children++;
                }
                else
                {
                    return NULL;
                }
                do_break = true;
            }
        }
        return new_node;
    }
    node* split_node()
    {
        node* ret_node = NULL;
        if(num_children == b+1)
        {
            ret_node = new node(a, b);
            int aug_num_c = (b+1)/2;
            ret_node->num_children = 0;
            num_children = num_children - aug_num_c;
            for(int i = b, at = aug_num_c-1; i>=num_children; i--, at--)
            {
                ret_node->child[at] = child[i];
                ret_node->num_children++;
            }
            assert(ret_node->num_children == aug_num_c);
            update_operand();
            ret_node->update_operand();
        }
        else
        {
            update_operand();
        }
        return ret_node;
    }
    node* insert(int moment, int type, int val)
    {
        if(num_children == 0)
        {
            return create_leaf(moment, type, val);
        }
        insert_node(moment, type, val);
        node* ret_node = split_node();
        return ret_node;
    }
    bool delete_at(int moment)
    {
        assert(left_bound<=moment && moment<right_bound);
        if(num_children == 0)
        {
            return true;
        }
        for(int i = 0; i<num_children; i++)
        {
            if(child[i]->left_bound<=moment && moment<child[i]->right_bound)
            {
                if(child[i]->delete_at(moment))
                {
                    update_bounds();
                    if(i!=num_children-1)
                    {
                        int add = 1;
                        if(child[i]->num_children == 0)
                        {
                            add = 0;
                        }
                        else
                        {
                            child[i]->update_right_bound(child[i+1]->left_bound);
                        }
                        node* new_node = child[i]->combine(child[i+1]);
                        if(new_node == NULL)
                        {
                            for(int j = i+add; j<num_children-1; j++)
                            {
                                child[j] = child[j+1];
                            }
                            int new_at = i+add;
                            if(new_at !=0 && new_at!= num_children-1)
                            {
                                child[new_at-1]->update_right_bound(child[new_at]->left_bound);
                            }
                            num_children--;
                        }
                        else
                        {
                            child[i+1] = new_node;
                        }
                    }
                    else
                    {
                        child[i-1]->update_right_bound(child[i]->left_bound);
                        node* new_node = child[i-1]->combine(child[i]);
                        if(new_node == NULL)
                        {
                            child[i-1]->update_right_bound(child[i]->right_bound);
                            right_bound = child[i]->right_bound;
                            num_children--;
                        }
                        else
                        {
                            child[i] = new_node;
                        }
                    }
                }
                if(i!=0 && num_children != i)
                {
                    child[i-1]->update_right_bound(child[i]->left_bound);
                }
                break;
            }
        }
        update_operand();
        if(num_children<a)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    element *partial_operation;
    element *delta_query(int moment)
    {
        if(moment<left_bound)
        {
            return new element();
        }
        if(num_children == 0)
        {
            return partial_operation = operation;
        }
        element* ret = new element();
        for(int i = 0; i<num_children; i++)
        {
            if(child[i]->left_bound<=moment && moment<child[i]->right_bound)
            {
                ret->merge(child[i]->delta_query(moment));
                break;
            }
            else
            {
                ret->merge(child[i]->operation);
            }
        }
        return partial_operation = ret;
    }
    void print_e(element *ret)
    {
        cout << ret->print() << endl;
    }
    element *wrap_query(element *ret, int moment)
    {
        //cout << "wrap: " <<endl;print_e(ret);
        if(moment<left_bound)
        {
            return ret;
        }
        if(num_children == 0)
        {
            if(deep_print)
            {
                cout << "preprint:" <<endl;
                print_e(ret);
                cout << "merge with:" << endl;
                cout << operation->print() <<endl;
            }
            ret->merge(operation);
            if(deep_print)
            {
                cout <<"at "<< left_bound <<" "<< right_bound <<  "leaf:" <<endl;
                print_e(ret);
            }
            return ret;
        }
        for(int i = 0; i<num_children; i++)
        {
            if(child[i]->left_bound<=moment && moment<child[i]->right_bound)
            {
                if(deep_print)
                {
                    cout <<"at "<< left_bound <<" "<< right_bound << "Walk to child at i = " << i << "(prewalk state:)"<<endl;
                    print_e(ret);
                }
                ret = child[i]->wrap_query(ret, moment);
                break;
            }
            else
            {
                ret->merge(child[i]->operation);
                if(deep_print)
                {
                    cout <<"at "<< left_bound <<" "<< right_bound << " after merge with i = " <<i <<endl;
                    print_e(ret);
                }
            }
        }
        if(deep_print)
        {
            cout <<"return to parent: "<<endl;
            print_e(ret);
        }

        return ret;
    }
    node* combine(node *right_node)
    {
        if(num_children == 0)
        {
            assert(right_node->num_children == 0);
            return NULL;
        }
        assert(right_node->left_bound == right_bound);
        for(int i = num_children, j = 0; j<right_node->num_children; j++, i++)
        {
            child[i] = right_node->child[j];
        }
        right_bound = right_node->right_bound;
        num_children = num_children+right_node->num_children;
        return split_node();
    }
    void update_right_bound(int new_right_bound)
    {
        if(right_bound!=new_right_bound)
        {
            if(num_children != 0)
            {
                child[num_children-1]->update_right_bound(new_right_bound);
            }
            right_bound = new_right_bound;
        }
    }
    int query_value_at_order(int moment, element *cumulative, int order, int side)
    {
        if(num_children == 0)
        {
            for(int side_i = 0; side_i<2;side_i++)
            {
                int prev_prefix = cumulative->get_prefix(side_i);
                int current_prefix = cumulative->get_prefix(side_i)+operation->one[side_i]->adds;
                int side_order = order;
                if(side_i != side)
                {
                    side_order = -order+1;
                }
                if(deep_print)cout << "prev, current: " << prev_prefix <<" "<< current_prefix << " side_order = "<< side_order << endl;
                if(prev_prefix<side_order && side_order <=current_prefix)
                {
                    if(deep_print)cout << "return" <<endl;
                    cumulative->merge(operation);
                    return operation->get(side_i);
                }
            }
            cumulative->merge(operation);
            return -inf;
        }
        pair<int, element *>query_at, up_to_next_query[2] = {mp(0, new element(cumulative)), mp(0, new element(cumulative))};
        for(int i = 0; i<num_children && child[i]->left_bound<=moment; i++)
        {
            element *child_operation = child[i]->operation;
            if(child[i]->right_bound>moment)
            {
                child_operation = child[i]->partial_operation;//delta_query(moment);
            }
            for(int side_i = 0; side_i<2;side_i++)
            {
                int low_order = cumulative->get_prefix(side_i)-child_operation->one[side_i]->removes;
                int high_order = cumulative->get_prefix(side_i)+child_operation->one[side_i]->adds;
                int side_order = order;
                if(side_i!=side)
                {
                    side_order = -order+1;
                }
                if(deep_print)cout << "find order = " << side_order << " between low high at side: " << side_i <<" :: " << low_order <<" "<< high_order <<endl;
                if((low_order<side_order && side_order<=high_order))
                {
                    up_to_next_query[side_i].s = new element(cumulative);
                    if(deep_print)cout << "take side_i = " << side_i << endl;
                    up_to_next_query[side_i].f = i;
                }
            }
            cumulative->merge(child_operation);
            if(deep_print)
            {
                cout << "cumulative at i = " << i << " of "<< left_bound <<" "<< right_bound << endl;
                cout << "after merge: "<<endl;
                print_e(cumulative);
            }
        }
        query_at = max(up_to_next_query[0], up_to_next_query[1]);
        if(deep_print)cout << "expand at: "  << query_at.f <<endl;
        return child[query_at.f]->query_value_at_order(moment, query_at.s, order, side);
    }
    string print_rb()
    {
        if(right_bound == inf) return "inf";
        else return int_to_string(right_bound);
    }
    void print(int c, int d)
    {
        for(int i = 0; i<c; i++) cout << " ";
        cout << "operation: " << operation->print() <<" | t = [" << left_bound <<", "<< print_rb() << ") " ;

        if(num_children > 0)
        {
            for(int i = 0; i<c; i++) cout << " ";
            cout << "num_children = " << num_children << " :: ";
        }
        cout << endl;
        for(int i = 0; i<num_children; i++)
        {
            child[i]->print(c+d, d);
        }
    }
};

class a_b_tree
{
public:
    node *root;
    int a, b;
    a_b_tree(int _a, int _b)
    {
        a = _a;
        b = _b;
        root = NULL;
    }
    element *query(int moment)
    {
        if(root == NULL)
        {
            return new element();
        }
        return root->wrap_query(new element(), moment);
    }
    int query_value_at_order(int moment, element* cumulative, int order, int side)
    {
        root->delta_query(moment);
        return root->query_value_at_order(moment, cumulative, order, side);
    }
    void insert(int moment, int type, int val)
    {
        if(root == NULL)
        {
            if(deep_print)cout << "new root!" <<endl;
            root = new node(a, b, moment, inf, new element(type, val));
            root->update_bounds();
        }
        else
        {
            node* augment_node = root->insert(moment, type, val);
            if(augment_node != NULL)
            {
                if(deep_print)cout << "new root!" <<endl;
                node* prev_root = root;
                root = new node(a, b);
                root->child[0] = prev_root;
                root->child[1] = augment_node;
                root->num_children = 2;
                root->update_operand();
            }
        }
    }
    void delete_at(int moment)
    {
        if(root == NULL)
        {

        }
        else if(moment<root->left_bound)
        {

        }
        else
        {
            root->delete_at(moment);
            if(root->num_children == 1)
            {
                root = root->child[0];
            }
            else if(root->num_children == 0)
            {
                root = NULL;
            }
        }
    }
    void print()
    {
        if(root == NULL)
        {

        }
        else
        {
            root->print(0, 4);
            cout << endl;
        }
    }
    void print_e(element *ret)
    {
        root->print_e(ret);
    }
};


class retroactive_deque
{
public:
    a_b_tree* retro_deque;
    retroactive_deque()
    {
        retro_deque = new a_b_tree(2, 3);
    }
    int query(int moment, int side)
    {
        element* find_order = retro_deque->query(moment);
        if(find_order->get(side) == -inf && find_order->get(1-side) == -inf)
        {
            return -inf;
        }
        if(print_queries)
        {
            cout << "ORDER COUNT" <<endl;retro_deque->print_e(find_order);
        }
        int order = find_order->get_prefix(side);
        if(print_queries)cout << "Find Order = " << order << endl;
        return retro_deque->query_value_at_order(moment, new element(), order,  side);
    }
    void delete_at(int moment)
    {
        retro_deque->delete_at(moment);
    }
    void insert(int moment, int type, int val)
    {
        retro_deque->insert(moment, type, val);
    }
    void print()
    {
        retro_deque->print();
    }
};

vector<pair<pair<int, int>, int> >  in;
vector<int> out;

int work_a_b_tree()
{
    retroactive_deque* retro_deque = new retroactive_deque();
    int q;
    if(AUTO_TEST) q = in.size();
    else cin >> q;
    for(int i = 0; i<q; i++)
    {
        int t, type, val = -inf;
        if(AUTO_TEST)
        {
            t = in[i].f.f, type = in[i].f.s, val = in[i].s;
        }
        else cin >> t >> type;

        if(type == 4 || type == 5)
        {
            if(print_queries)cout <<"query: "<< t <<" "<< type <<endl;
            int rezultat = retro_deque->query(t, type%2);
            if(AUTO_TEST)out.pb(rezultat);
            if(print_queries) cout <<"result at: " << t <<" :: "<< rezultat <<endl;
        }
        else if(type == 6)
        {
            if(print_queries)cout <<"query: "<< t <<" "<< type <<endl;
            val = -1;
            retro_deque->delete_at(t);
        }
        else
        {
            if(type%2 == 1)if(!AUTO_TEST)cin >> val;
            if(print_queries)
            {
                cout <<"query: "<< t <<" "<< type;
                if(type%2 == 1) cout <<" "<< val;
                cout <<endl;
            }
            retro_deque->insert(t, type, val);
        }
        if(print_queries)retro_deque->print();
    }
    return 0;
}

vector<int> oracle;
bool time_line[max_time+1];
bool constant[max_constant+1];
class test
{
public:
    bool illegal;
    int n;
    test(int _n)
    {
        n = _n;
        in.clear();
        out.clear();
        oracle.clear();
        MEM(time_line, 0);
        MEM(constant, 0);
        int q_size = 0;
        if(print_input)cout << n <<endl;
        //bool test_stack = ;
        for(int i = 0; i<n; i++)
        {
            int t, type, val;
            if(i<max(1, (int)pow((log(n)/log(2)+1), 1.5)))
            {
                t = i;
                time_line[t] = 1;
                type = 3;
                val = rand(1, max_constant);
            }
            else
            {
                t = rand(1, max_time);
                while(time_line[t] == 1)
                {
                    t+=rand(1, max_time-1);
                    t%=max_time;
                    t++;
                }
                time_line[t] = 1;
                type = rand(0, 6);
                if(test_stack)
                {
                    if(type == 0 || type == 1)
                    {
                        type+=2;
                    }
                    if(type == 4)
                    {
                        type = 5;
                    }
                }

                val = rand(1, max_constant);
                while(constant[val] == 1)
                {
                    val+=rand(1, max_constant);
                    val%=(max_constant+1);
                    val++;
                }
            }
            q_size+=(type == 1 || type == 3);
            q_size-=(type == 0 || type == 2);
            if(q_size == 0 && (type == 4 || type == 5))
            {
                if(test_stack)type = 3;
                else type = 2*rand(0, 1)+1;
                q_size++;
            }
            if(q_size<0)
            {
                if(test_stack)type = 3;
                else type = 2*rand(0, 1)+1;
                q_size+=2;
            }
            in.pb(mp(mp(t, type), val));
            if(type == 1 || type == 3)
            {
                constant[val] = 1;
            }
            if(print_input)
            {
                cout << t <<" "<< type;
                if(type == 1 || type == 3)cout<< " "<< val;
                cout <<endl;
            }
        }
        O_n_solve();
    }
    void O_n_solve()
    {
        vector<pair<pair<int, int>, int> > partial;
        illegal = false;
        for(int i = 0; i<=n; i++)
        {
            deque<int> q;
            bool entered = false;
            for(int j = 0; j<partial.size(); j++)
            {
                if(partial[j].f.s == 0 && q.size()>0)
                {
                    q.pop_front();
                }
                else if(partial[j].f.s == 1)
                {
                    q.push_front(partial[j].s);
                }
                else if(partial[j].f.s == 2 && q.size()>0)
                {
                    q.pop_back();
                }
                else if(partial[j].f.s == 3)
                {
                    q.push_back(partial[j].s);
                }
                else if(q.size()>0)
                {
                    cout << partial[j].f.s <<endl;
                    assert(0);
                }
                else
                {
                    illegal = true;
                    assert(q.size()==0);
                }
                if(i < n && (in[i].f.s == 4 || in[i].f.s == 5) && ((j == partial.size()-1 || partial[j+1].f.f > in[i].f.f) && partial[j].f.f <= in[i].f.f) && !entered)
                {
                    entered = true;
                    if(q.size()>0)
                    {
                        if(in[i].f.s == 4)
                        {
                            oracle.pb(q.front());
                        }
                        else if(in[i].f.s == 5)
                        {
                            oracle.pb(q.back());
                        }
                        else
                        {
                            assert(0);
                        }
                    }
                    else
                    {
                        illegal = true;
                        oracle.pb(-inf);
                    }
                }
            }
            if(entered)
            {

            }
            else if(i < n && in[i].f.s == 6)
            {
                int j = 0;
                for(j = 0; j<(partial.size()) && partial[j].f.f<=in[i].f.f;)
                {
                    j++;
                }
                if(partial.size()>=1 && in[i].f.f >= partial[0].f.f)
                {
                    partial.erase(partial.begin()+j-1, partial.begin()+j);
                }
            }
            else if(i<n)
            {
                if(!(in[i].f.s == 4 || in[i].f.s == 5))
                {
                    partial.pb(in[i]);
                    sort_v(partial);
                }
                else
                {
                    illegal = true;
                    oracle.pb(-inf);
                }
            }
        }
        if(print_input)
        {
            cout << "ORACLE COMPLETE" <<endl;
            cout << "IS ILLEGAL: " << illegal <<endl;
        }
    }
    string check()
    {
        if(oracle.size() == out.size())
        {
            for(int i = 0; i<oracle.size(); i++)
            {
                if(oracle[i] != out[i])
                {
                    if(illegal)
                    {
                        return "ILLEGAL";
                    }
                    if(print_wrong_input)
                    {
                        cout << "----------------------" <<endl;
                        cout <<"missmatch here: i = "<< i <<" : Oracle: "<< oracle[i] <<" core: "<< out[i] <<endl;
                        cout << in.size()<<endl;
                        for(int j = 0; j<in.size(); j++)
                        {
                            cout << in[j].f.f <<" "<< in[j].f.s<<" ";
                            if(in[j].f.s == 1 || in[j].f.s == 3)cout << in[j].s;
                            cout <<endl;
                        }
                        cout<< "RESULT: "<<endl;
                        if(!illegal)cout << "LEGAL" <<endl;
                    }
                    return "NOT";
                }
            }
            return "OK";
        }
        else
        {
            if(print_wrong_input)cout << "num query missmatch" <<endl;
            return "NOT";
        }
    }
};

int main()
{
    if(random_seed)srand (time(NULL));
    int T = NUM_TESTS;
    int n = max_n;
    if(!AUTO_TEST) T = 1;
    double correct = 0;
    double total = 0;
    int illegal = 0;
    while(T--)
    {
        test *test_case;
        if(AUTO_TEST)
        {
            test_case = new test(n);
            if(!test_case->illegal)
            {
                work_a_b_tree();
                string rez = test_case->check();
                correct+=(rez == "OK");
                if(rez != "ILLEGAL")
                {
                    total++;
                    cout << total << " :: "<< rez <<endl;
                    if(print_wrong_input)cout << "-------------------------------"<<endl;
                }
                else
                {
                    assert(0);
                }
            }
            else
            {
                illegal++;
            }
        }
        else
        {
            work_a_b_tree();
        }
    }
    if(AUTO_TEST)
    {
        cout << "Operations per test case: N = " << max_n <<endl;
        cout << total+illegal <<" generated test cases cases" <<endl;
        cout << "Illegal test cases: " << illegal <<" out of " << total+illegal <<endl;
        cout << "Solved test cases: "<< correct <<"/"<<total << " :: "<< 100.0*((double)correct/total)<<  "%" << endl;
    }
    return 0;
}
