/* Code written by Kliment Serafimov */

#include <fstream>
#include <iomanip>
#include <iostream>

#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <stack>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#define P push
#define f first
#define s second
#define pb push_back
#define mp make_pair
#define DEC 0.00000001
#define MAX 2139062143
#define inf MAX/2
#define MAX_63  1061109567
#define MAXll 9187201950435737471
#define bp(a) __builtin_popcount(a)
#define rand(a, b) ((rand()%(b-a+1))+a)
#define MEM(a, b) memset(a, b, sizeof(a))
#define sort_v(a) sort(a.begin(), a.end())
#define rev_v(a)  reverse(a.begin(), a.end())

//#define fin cin
//#define fout cout
using namespace std;
//ifstream fin(".in");
//ofstream fout(".out");

#define maxB 8
///NODE of A-B Tree


#define NUM_TESTS 1000000
#define AUTO_TEST 0
#define print_queries (1-AUTO_TEST)
#define max_n 20
#define max_constant 10
#define max_time max_n*max_n
#define print_wrong_input 1
#define print_input 0
#define deep_print 0

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
    int removes;
    int adds;
    int val;
    int cut;
    int max_adds;
    int max_removes;

    void init()
    {
        removes = 0;
        adds = 0;
        max_adds = 0;
        val = -inf;
        cut = 0;
        max_removes = 0;
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
        max_adds = to_copy->max_adds;
        cut = to_copy->cut;
        max_removes = to_copy->max_removes;
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
            max_removes = 1;
        }
        else if(is_push(type))
        {
            adds = 1;
            max_adds = 1;
            val = _val;
        }
        else
        {
            assert(0);
        }
    }
    void merge(one_side *right_operand)
    {
        max_removes = max(max_removes, removes-adds+right_operand->max_removes);
        max_adds = max(max_adds, max(0, adds-right_operand->removes)+right_operand->max_adds);
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
    void overfill(one_side *other_side)
    {
        //cout << "here: " <<endl;
        //cout << print(this) << " " <<print(other_side) <<endl;
        if(removes>0 && other_side->adds>0)
        {
            //cout << "wrap 1" <<endl;
            int tmp_adds = other_side->adds;
            other_side->cut += min(other_side->adds, removes);
            other_side->max_adds -= min(other_side->adds, removes);
            other_side->adds = max(0, other_side->adds-removes);
            if (max_removes<min(removes, tmp_adds)) assert(0);
            //assert(max_removes>=delta_removes);
            max_removes = max(0, max_removes - min(removes, tmp_adds));
            removes = max(0, removes-tmp_adds);
        }
        if(other_side->removes>0 && adds>0)
        {
            other_side->overfill(this);
            /*
            cout << "wrap 2" <<endl;
            int tmp_adds = adds;
            cut += min(adds, other_side->removes);
            max_adds -= min(adds, other_side->removes);
            adds = max(0, adds-other_side->removes);
            assert(other_side->max_removes>=max(0, other_side->removes - tmp_adds));
            other_side->max_removes -= max(0, other_side->removes - tmp_adds);
            other_side->removes = max(0, other_side->removes - tmp_adds);*/
        }
        if(removes>0 && other_side->max_adds>0)
        {
            //cout << "wrap 3" <<endl;
            int tmp_max_adds = other_side->max_adds;
            other_side->cut += min(other_side->max_adds, removes);
            other_side->max_adds = max(0, other_side->max_adds-removes);
            other_side->removes+=min(tmp_max_adds, removes);
            other_side->max_removes+=min(tmp_max_adds, removes);
            //assert(max_removes>=delta_remove);
            if (max_removes< min(removes, tmp_max_adds))assert(0);
            max_removes = max(0, max_removes - min(removes, tmp_max_adds));
            removes = max(0, removes-tmp_max_adds);
        }
        if(other_side->removes>0 && max_adds>0)
        {
            other_side->overfill(this);
            /*cout << "wrap 4" <<endl;
            int tmp_max_adds = max_adds;
            cut += min(adds, other_side->removes);
            max_adds = max(0, max_adds-other_side->removes);
            removes += max(0, removes - tmp_max_adds);
            other_side->removes = max(0, removes - tmp_max_adds);*/
        }/*
        if(adds>0 && other_side->max_removes>0)
        {
            cout << "wrap 5" <<endl;
            int tmp_max_removes = other_side->max_removes;
            cut += min(other_side->max_removes, adds);
            other_side->max_removes = max(0, other_side->max_removes-adds);
            adds = max(0, adds-tmp_max_removes);
        }
        if(other_side->adds>0 && max_removes>0)
        {
            cout << "wrap 6" <<endl;
            int tmp_max_removes = max_removes;
            other_side->cut += min(max_removes, other_side->adds);
            max_removes = max(0, max_removes-other_side->adds);
            other_side->adds = max(0, other_side->adds-tmp_max_removes);
        }*/

    }
    string print(one_side *p)
    {
        return "v=" + int_to_string(p->val) + ", [-" + int_to_string(p->removes) + ", +" +int_to_string(p->adds) + ", max=+"+int_to_string(p->max_adds) + ", min=-"+int_to_string(p->max_removes)+"]";
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
    void wrap()
    {
        one[0]->overfill(one[1]);
    }
    int get(int side)
    {
        //assert(one[side]->val != -inf);
        return one[side]->val;
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
    element *delta_query(int moment)
    {
        if(moment<left_bound)
        {
            return new element();
        }
        if(num_children == 0)
        {
            return operation;
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
        return ret;
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
            ret->merge(operation);
            ret->wrap();
            if(deep_print)
            {
                cout << "leaf:" <<endl;
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
                    cout << "Walk to child at i = " << i << "(prewalk state:)"<<endl;
                    print_e(ret);
                }
                ret = new element(child[i]->wrap_query(ret, moment));
                break;
            }
            else
            {
                ret->merge(child[i]->operation);
                if(deep_print)
                {
                    cout <<"after merge with i = " <<i <<endl;
                    print_e(ret);
                }
                ret->wrap();
                if(deep_print)
                {
                    cout << "after wrap with i = "<<i <<endl;
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


    int query_value_at_order(int moment, element *cumulative, int order, int cut, int side)
    {
        //cout << "order: "<< order << " + "<< cut << " at: "<< endl;print_e(cumulative);
        if(num_children == 0)
        {
            cumulative->merge(operation);
            cumulative->wrap();
            if(cumulative->one[side]->adds+cumulative->one[side]->cut == order + cut)
            {
                return operation->get(side);
            }
            else
            {
                return -inf;
            }
        }
        element *up_to_next_query = new element(cumulative);
        int query_at = 0;
        for(int i = 0; i<num_children && child[i]->left_bound<=moment; i++)
        {
            element *child_operation = child[i]->operation;
            if(child[i]->right_bound>moment)
            {
                child_operation = child[i]->delta_query(moment);
                //cout << "child " << i << endl;
                //print_e(child_operation);
            }
            int low_order = cumulative->one[side]->adds-child_operation->one[side]->removes+cumulative->one[side]->cut;
            int high_order = cumulative->one[side]->adds+child_operation->one[side]->adds+cumulative->one[side]->cut;
            if(low_order<order+cut && order+cut<=high_order)
            {
                up_to_next_query = new element(cumulative);
                query_at = i;
            }
            cumulative->merge(child_operation);
            if(deep_print)
            {

                cout << "cumulative at i = " << i <<endl;
                cout << "after merge: "<<endl;
                print_e(cumulative);
            }
            cumulative->wrap();
            if(deep_print)
            {
                cout << "after wrap" <<endl;
                print_e(cumulative);
            }
        }
        //cout << "expand at: "  << query_at <<endl;
        return child[query_at]->query_value_at_order(moment, up_to_next_query, order, cut, side);
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
    int query_value_at_order(int moment, element* cumulative, int order, int cut, int side)
    {
        return root->query_value_at_order(moment, cumulative, order, cut, side);
    }
    void insert(int moment, int type, int val)
    {
        if(root == NULL)
        {
            //cout << "new root!" <<endl;
            root = new node(a, b, moment, inf, new element(type, val));
            root->update_bounds();
        }
        else
        {
            node* augment_node = root->insert(moment, type, val);
            if(augment_node != NULL)
            {
                //cout << "new root!" <<endl;
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
        if(find_order->one[side]->adds>=1)
        {
            return retro_deque->query_value_at_order(moment, new element(), find_order->one[side]->adds, find_order->one[side]->cut, side);
        }
        else
        {
            if(print_queries) cout << "SHIFT" <<endl;
            if(find_order->one[1-side]->adds<find_order->one[side]->removes)
            {
                //assert(0);
                return -inf;
            }
            return retro_deque->query_value_at_order(moment, new element(), 1, find_order->one[1-side]->cut+find_order->one[1-side]->removes+find_order->one[side]->removes, 1-side);
        }
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
    int c = 0;
    retroactive_deque* retro_deque = new retroactive_deque();
    int q;
    if(AUTO_TEST) q = in.size();
    else cin >> q;
    for(int i = 0; i<q; i++)
    {
        //cout <<i << " " << q <<endl
        int t, type, val = -inf;
        if(AUTO_TEST)
        {
            t = in[i].f.f, type = in[i].f.s, val = in[i].s;
        }
        else cin >> t >> type;
        ///type, description
        ///0, pop R
        ///1, push R
        ///2, pop L
        ///3, push L
        ///4, query R
        ///5, query L
        ///6, delete
        if(type == 4 || type == 5)
        {
            if(print_queries)cout <<"query: "<< t <<" "<< type <<endl;
            if(AUTO_TEST)out.pb(retro_deque->query(t, type%2));
            if(print_queries) cout <<"result at: " << t <<" :: "<< retro_deque->query(t, type%2) <<endl;
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
            if(print_queries)cout <<"query: "<< t <<" "<< type <<" "<< val <<endl;
            retro_deque->insert(t, type, val);
        }
        if(print_queries)retro_deque->print();
    }
    return 0;
}

/// SHTO AKO IMPLEMENTIRASH DAG NA & and or instead od +, * (or even + and *, to have sort of clasification maping from vals to things), shto presmeetuvaat operacii na kompjuter i so ovaa DS da pravime queries na operaciite, i inite, preku genetic programing, za kreiranje na mrezi koi mozat da ucat funkcii
///Design a fully retroactive deque data structure. Your data structure must support Insert$(t,\textit{update})$ and Delete$(t,\textit{update})$ where $\textit{update}$ is among the following operations:
///push-left$(x)$: Insert $x$ as the new left element*
///push-right$(x)$: Insert $x$ as the new right element*
///pop-left$()$: Remove the leftmost element*
///pop-right$()$: Remove the rightmost element*

///Your data structure must also support Query$(t,\textit{query})$ where $\textit{query}$ is among the following operations:

///left$()$: Return the leftmost element*
///right$()$: Return the rightmost element*
///[optional] get$(i)$: Return the $i$th element* in the list (where $i=0$ means leftmost)


vector<int> oracle;
bool time_line[max_time+1];
class test
{
public:
    bool illegal;
    test(int n)
    {
        in.clear();
        out.clear();
        oracle.clear();
        MEM(time_line, 0);
        int q_size = 0;
        if(print_input)cout << n <<endl;
        for(int i = 0; i<n; i++)
        {
            int t, type, val;
            if(i < n/50)
            {
                t = i+1;
                time_line[t] = 1;
                type = 1;
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
                val = rand(1, max_constant);
            }
            q_size+=(type == 1 || type == 3);
            q_size-=(type == 0 || type == 2);
            if(q_size == 0 && (type == 4 || type == 5))
            {
                type = 2*rand(0, 1)+1;
                q_size++;
            }
            if(q_size<0)
            {
                type = 2*rand(0, 1)+1;
                q_size+=2;
            }
            in.pb(mp(mp(t, type), val));
            if(print_input)
            {
                cout << t <<" "<< type;if(type == 1 || type == 3)cout<< " "<< val;cout <<endl;
            }
        }
        vector<pair<pair<int, int>, int> > partial;
        illegal = false;
        for(int i = 0; i<n; i++)
        {
            if(in[i].f.s == 4 || in[i].f.s == 5)
            {
                element *rez = new element();
                deque<int> q;
                for(int j = 0; j<partial.size() && partial[j].f.f<=in[i].f.f; j++)
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
                        assert(0);
                    }
                    else
                    {
                        illegal = true;
                        assert(q.size()==0);
                    }
                }
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
                    oracle.pb(-inf);
                }
            }
            else if(in[i].f.s == 6)
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
            else
            {
                partial.pb(in[i]);
                sort_v(partial);
            }
        }
        //cout << "ORACLE COMPLETE" <<endl;
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
    int T = NUM_TESTS;
    int n = max_n;
    if(!AUTO_TEST) T = 1;
    double correct = 0;
    double total = 0;
    while(T--)
    {
        test *test_case;
        if(AUTO_TEST) test_case = new test(n);
        work_a_b_tree();
        if(AUTO_TEST)
        {
            string rez = test_case->check();
            correct+=(rez == "OK");
            total+=(rez != "ILLEGAL");
            cout << total << " :: "<< rez <<endl;
            if(print_wrong_input)cout << "-------------------------------"<<endl;
        }
    }
    cout <<"correct: "<< correct <<"/"<<total << " :: "<< 100.0*((double)correct/total)<<endl;
    return 0;
}
