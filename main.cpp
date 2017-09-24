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
///b degree of A-B Tree

#define NUM_TESTS 100000
#define AUTO_TEST 1
#define random_seed 1
#define print_queries (1-AUTO_TEST)
#define max_n 20
#define max_constant max_n*2
#define max_time max_n*10
#define print_wrong_input 1
#define print_input 0
#define deep_print_query print_queries
#define deep_print print_queries

string int_to_string(int n)
{
    if(n <= -inf/2 || n >=inf/2)
    {
        return "/";
    }
    if(n == 0)
    {
        return "0";
    }
    int tmp_n = n;
    n = abs(n);
    string ret;
    while(n>0)
    {
        ret+=(n%10)+'0';
        n/=10;
    }
    rev_v(ret);
    if(tmp_n<0)
    {
        return "-"+ret;
    }
    else
    {
        return ret;
    }
}

class one_side
{
public:
    int val;

    int sum;

    int low;
    int high;

    int removes;
    int adds;

    int cut;

    int prefix;
    int lazy_push;

    int prefix_delta;
    int max_prefix;
    int min_prefix;
    int hidden;
    int hidden_depth;
    int first_drop;
    int last_inc;
    void init()
    {
        sum = 0;
        low = 0;
        high = 0;
        val = -inf;
        removes = 0;
        adds = 0;
        cut = 0;
        prefix = 0;
        lazy_push = 0;
        prefix_delta = 0;
        max_prefix = 0;
        min_prefix = 0;
        hidden = 0;
        hidden_depth = 0;
        first_drop = inf;
        last_inc = -inf;
    }
    one_side()
    {
        init();
    }
    one_side(one_side* to_copy)
    {
        init();
        sum = to_copy->sum;
        low = to_copy->low;
        high = to_copy->high;
        val = to_copy->val;
        removes = to_copy->removes;
        adds = to_copy->adds;
        cut = to_copy->cut;
        prefix = to_copy->prefix;
        lazy_push = to_copy->lazy_push;
        prefix_delta = to_copy->prefix_delta;
        hidden = to_copy->hidden;
        first_drop = to_copy->first_drop;
        last_inc = to_copy->last_inc;
        hidden_depth = to_copy->hidden_depth;
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
        init();
        assert(0<=type && type <=4);
        if(is_pop(type))
        {
            sum = -1;
            low = -1;
            removes = 1;
            prefix = -1;
            max_prefix = -1;
            min_prefix = -1;
            first_drop = -1;
        }
        else if(is_push(type))
        {
            sum = 1;
            high = 1;
            val = _val;
            adds = 1;
            prefix = 1;
            max_prefix = 1;
            min_prefix = 1;
            last_inc = 1;
        }
        else
        {
            assert(0);
        }
    }
    void merge(one_side *right_operand)
    {
        assert(0);
        assert(right_operand->cut == 0);
        high = max(high, sum+right_operand->high);
        low = min(low, sum+right_operand->low);
        hidden += right_operand->hidden + min(right_operand->removes, adds);
        if(min(right_operand->removes, adds)>0)
        {
            hidden_depth = adds - min(right_operand->removes, adds);
        }
        else
        {
            hidden_depth = right_operand->hidden_depth+adds;
        }
        first_drop = min(first_drop, (prefix-right_operand->removes)+inf*(right_operand->removes == 0));
        last_inc = max(last_inc, (prefix+right_operand->adds)-inf*(right_operand->adds == 0));
        removes = removes + max(0, right_operand->removes-adds);
        adds = max(0, adds-right_operand->removes) + right_operand->adds;
        //prefix_check(right_operand);
        prefix=right_operand->prefix;
        sum+=right_operand->sum;
        if(right_operand->val != -inf)
        {
            val = right_operand->val;
        }
    }
    void prefix_check(one_side *right_operand)
    {
        if(deep_print && !(right_operand->prefix+prefix_delta == prefix+right_operand->sum))
        {
            cout << "prefix: " << right_operand->prefix <<" with "<< prefix_delta << " should be "<<  prefix <<" " << right_operand->sum <<endl;
            assert(right_operand->prefix+prefix_delta == prefix+right_operand->sum);
        }
    }
    int prefix_wrap(one_side *right_operand)
    {
        assert(right_operand->removes>=0 && adds>=0);
        first_drop = min(right_operand->first_drop+prefix, min(first_drop, (prefix-right_operand->removes)+inf*(right_operand->removes == 0)));
        last_inc = max(last_inc, (prefix+right_operand->adds)-inf*(right_operand->adds == 0));
        hidden = right_operand->hidden+ min(right_operand->removes, adds);
        if(min(right_operand->removes, adds)>0)
        {
            hidden_depth = adds - min(right_operand->removes, adds);
        }
        else
        {
            hidden_depth = right_operand->hidden_depth+adds;
        }
        removes = removes + max(0, right_operand->removes-adds);
        adds = max(0, adds-right_operand->removes) + right_operand->adds;
        prefix_check(right_operand);
        max_prefix = max(max_prefix, prefix+right_operand->adds);
        min_prefix = min(min_prefix, prefix-right_operand->removes);
        prefix=right_operand->prefix+prefix_delta;
        sum+=right_operand->sum;
        if(right_operand->val != -inf)
        {
            //val = right_operand->val;
        }
        return prefix_overfill();
    }
    int prefix_overfill()
    {
        int spill = 0;
        if(deep_print)cout << "before applying overfill to one side" <<endl << print(this) << endl;
        if(min_prefix<0)
        {
            ///  )
            ///)( )
            ///| ( |   |)))|)
            ///|) (|(((|   | )
            spill=-min_prefix;
            prefix-=min_prefix;
            prefix_delta-=min_prefix;
            max_prefix-=min_prefix;
            first_drop-=min_prefix;
            if(first_drop == 0 && hidden == 0)
            {
                if(deep_print)cout <<"reset first drop" <<endl;
                first_drop = inf;
            }
            else if(first_drop == 0 && hidden >= 1)
            {
                if(deep_print)cout << "set first drop to HD:" << hidden_depth <<endl;
                first_drop = hidden_depth;
            }
            sum-=min_prefix;
            removes+=min_prefix;
            min_prefix = 0;
        }
        return spill;
    }
    void push_lazy(one_side *parent)
    {
        prefix += parent->lazy_push;
        lazy_push+=parent->lazy_push;
    }
    void set_lazy(one_side *prewrap_leaf, bool do_add)
    {
        ///if only propagating leafs upwards
        ///assert(abs(prewrap_leaf->sum)<=1);
        if(do_add)
        {
            prefix+=prewrap_leaf->sum;
            lazy_push+=prewrap_leaf->sum;
        }
        else
        {
            prefix-=prewrap_leaf->sum;
            lazy_push-=prewrap_leaf->sum;
        }
    }
    void set_prefix(one_side *last)
    {
        prefix = last->prefix;
    }
    one_side *insert_new_node_in_place_of_current_node_and_return_modified_current_node(one_side *new_side)
    {
        one_side *ret = new one_side(this);
        ret->prefix+=new_side->sum;
        val = new_side->val;
        prefix -= sum;
        prefix += new_side->sum;
        sum = new_side->sum;
        low = new_side->low;
        high = new_side->high;
        adds = new_side->adds;
        lazy_push = new_side->lazy_push;
        cut = new_side->cut;
        prefix_delta = new_side->prefix_delta;
        hidden = new_side->hidden;
        first_drop = new_side->first_drop;
        last_inc = new_side->last_inc;
        hidden_depth = new_side->hidden_depth;
        return ret;
    }
    void add_prefix(one_side *prev)
    {
        prefix+=prev->prefix;
    }
    string print(one_side *p)
    {
        return "v="+int_to_string(p->val)+",("+
        "(s="+int_to_string(p->sum)+","+
        "p="+int_to_string(p->prefix)+
        //",w="+int_to_string(p->lazy_push)+
        "),("
        //+"h="+int_to_string(p->high) +", l="+int_to_string(-p->low) + "),(+"
        + int_to_string(p->adds) + ", -"+int_to_string(p->removes)+"),(drop="+int_to_string(first_drop)+",up="+int_to_string(last_inc)+"),(d="+int_to_string(p->prefix_delta)+",hid="+int_to_string(p->hidden)+")";
    }
};

class element
{
public:
    one_side *one[2];
    bool defined;
    void init()
    {
        defined = true;
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
    void prefix_wrap(element *right_operand)
    {
        int spill[2] = {0, 0};
        spill[0] = one[0]->prefix_wrap(right_operand->one[0]);
        spill[1] = one[1]->prefix_wrap(right_operand->one[1]);
        int c = 1;
        while(c==1)
        {
            c = 0;
            for(int k = 0; k<2; k++)
            {
                if(spill[k]>0)
                {
                    c++;
                    if(deep_print)cout << "Spill at side: " << k <<" by: "<< spill[k] <<endl;
                    if(
                       //one[1-k]->sum<spill[k] ||
                       //one[1-k]->high<spill[k]||
                       one[1-k]->adds<spill[k] || one[1-k]->prefix<spill[k])
                    {
                        if(deep_print)
                        {
                            cout << "BACK SPILL: "<< spill[k] <<endl;
                            //cout << one[1-k]->sum << " " << one[1-k]->high <<" "
                            cout << one[1-k]->adds <<" "<< one[1-k]->prefix << endl;
                        }
                    }

                    if(deep_print)cout << "before spill: " << print() <<endl;
                    one[1-k]->adds=max(0, one[1-k]->adds-spill[k]);
                    one[1-k]->prefix-=spill[k];
                    one[1-k]->sum-=spill[k];
                    one[1-k]->prefix_delta-=spill[k];
                    one[1-k]->first_drop -= spill[k];
                    one[1-k]->last_inc -=spill[k];
                    one[1-k]->hidden_depth--;///IS IT?

                    if(one[1-k]->first_drop<0)
                    {
                        one[1-k]->min_prefix -= min(one[1-k]->hidden, spill[k]);
                        one[1-k]->removes = -one[1-k]->min_prefix;
                        one[1-k]->hidden = max(0, one[1-k]->hidden-spill[k]);

                        if(deep_print)cout << "min prefix of " << 1-k <<" is "<< one[1-k]->min_prefix <<endl;
                    }

                    if(deep_print)cout << "after spill: " << print() <<endl;
                }
            }
            if(c>=2)
            {
                if(deep_print)
                {
                    cout << "NON-ROBUST double empty deque" <<endl;
                    assert(0);
                }
                else
                {
                    //cout << "HAS TO" <<endl;
                    //break;
                }
            }

            spill[0] = one[0]->prefix_overfill();
            spill[1] = one[1]->prefix_overfill();
        }
    }
    void push_lazy(element *parent_lazy)
    {
        one[0]->push_lazy(parent_lazy->one[0]);
        one[1]->push_lazy(parent_lazy->one[1]);
    }
    void clear_push()
    {
        one[0]->lazy_push = 0;
        one[1]->lazy_push = 0;
    }
    void set_lazy(element *removed_child, bool do_add)
    {
        one[0]->set_lazy(removed_child->one[0], do_add);
        one[1]->set_lazy(removed_child->one[1], do_add);
    }
    bool do_push()
    {
        return (one[0]->lazy_push!=0 || one[1]->lazy_push!=0);
    }
    int get(int side)
    {
        //assert(one[side]->val != -inf);
        return one[side]->val;
    }
    void set_prefix(element *last)
    {
        one[0]->set_prefix(last->one[0]);
        one[1]->set_prefix(last->one[1]);
    }
    void add_prefix(element *prev)
    {
        one[0]->add_prefix(prev->one[0]);
        one[1]->add_prefix(prev->one[1]);
    }
    element* insert_new_node_in_place_of_current_node_and_return_modified_current_node(element *new_data)
    {
        one[0] = one[0]->insert_new_node_in_place_of_current_node_and_return_modified_current_node(new_data->one[0]);
        one[1] = one[1]->insert_new_node_in_place_of_current_node_and_return_modified_current_node(new_data->one[1]);
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
        operation = new element(operand);
    }
    node(int _a, int _b)
    {
        a = _a;
        b = _b;
    }

    void push_lazy()
    {
        if(num_children>0)
        {
            if(operation->do_push())
            {
                for(int i = 0;i<num_children;i++)
                {
                    child[i]->operation->push_lazy(operation);
                    if(child[i]->num_children == 0)
                    {
                        child[i]->operation->clear_push();
                    }
                }
                operation->clear_push();
            }
        }
        else
        {
            operation->clear_push();
        }
    }
    pair<node*, element*> create_leaf(int moment, int type, int val)
    {
        if(moment>=left_bound)
        {
            assert(moment<right_bound);
            int old_rb = right_bound;
            right_bound = min(right_bound, moment);
            node* ret_node = new node(a, b, moment, old_rb, new element(type, val));
            ret_node->operation->set_prefix(operation);
            ret_node->operation->set_lazy(ret_node->operation, true);
            ret_node->operation->clear_push();
            return mp(ret_node, ret_node->operation);
        }
        else
        {
            element *new_operation = operation->insert_new_node_in_place_of_current_node_and_return_modified_current_node(new element(type, val));
            assert(operation->one[0]->cut == 0);
            node *new_node = new node(a, b, left_bound, right_bound, operation);
            right_bound = left_bound;
            left_bound = moment;
            operation = new_operation;
            return mp(new_node, operation);
        }
    }
    void update_bounds()
    {
        if(num_children>0)
        {
            left_bound = child[0]->left_bound;
            right_bound = child[num_children-1]->right_bound;
            operation->set_prefix(child[num_children-1]->operation);
        }
    }
    element* update_operand(element *prefix)
    {
        if(num_children>0)
        {
            operation = new element(prefix);
            for(int i = 0; i<num_children; i++)
            {
                if(i!=num_children-1)
                {
                    child[i]->right_bound = child[i+1]->left_bound;
                }
                if(deep_print_query)
                {
                    cout << "Update merge at i = " << i << " " << child[0]->left_bound <<" "<< child[num_children-1]->right_bound<< ": "<< endl<< operation->print() << " AND "<< endl << child[i]->operation->print() <<endl;
                }
                operation->prefix_wrap(child[i]->operation);
            }
            update_bounds();
        }
        return operation;
    }
    element* insert_node(int moment, element* prefix, int type, int val)
    {
        //assert(left_bound<=moment && moment<right_bound);
        node* new_node = NULL;
        element* prefix_to_right_leaf = NULL;
        for(int i = 0; i<num_children; i++)
        {
            bool is_smallest = (i == 0 && moment < left_bound);
            if(is_smallest || (child[i]->left_bound <=moment && moment < child[i]->right_bound))
            {
                pair<node*, element*> delta_tree = child[i]->insert(moment, prefix, type, val);
                new_node =  delta_tree.f;
                prefix_to_right_leaf = delta_tree.s;
                for(int j = i+1;j<num_children;j++)
                {
                    element *tmp_leaf_node = new element(prefix_to_right_leaf);
                    prefix_to_right_leaf->prefix_wrap(child[j]->operation);
                    child[j]->operation->set_lazy(tmp_leaf_node, true);
                }
                if(new_node != NULL)
                {
                    for(int j = num_children; j>i; j--)
                    {
                        child[j+1] = child[j];
                    }
                    child[i+1] = new_node;
                    num_children++;
                }
                break;
            }
        }
        return prefix_to_right_leaf;
    }
    pair<node*, element*> split_node(element *prefix)
    {
        node* ret_node = NULL;
        element* full_prefix = NULL;
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
            if(deep_print)cout << "new left operand:" <<endl;
            element *next_prefix = update_operand(prefix);
            if(deep_print)cout << "new right operand:" <<endl;
            full_prefix = ret_node->update_operand(next_prefix);
        }
        else
        {
            full_prefix = update_operand(prefix);
        }
        return mp(ret_node, full_prefix);
    }
    pair<node*, element*> insert(int moment, element* prefix, int type, int val)
    {
        push_lazy();
        if(num_children == 0)
        {
            return create_leaf(moment, type, val);
        }
        element* prefix_to_right_of_leaf = insert_node(moment, prefix, type, val);
        return split_node(prefix);
    }
    pair<bool, element*> delete_at(int moment, element* prefix)
    {
        assert(0);
        assert(left_bound<=moment && moment<right_bound);
        if(num_children == 0)
        {
            return mp(true, operation);
        }
        push_lazy();
        pair<bool, element*> delta_tree = mp(false, new element());
        element* removed_leaf = NULL;
        for(int i = 0; i<num_children; i++)
        {
            if(child[i]->left_bound<=moment && moment<child[i]->right_bound)
            {
                delta_tree = child[i]->delete_at(moment);
                bool tree_change = delta_tree.f;
                removed_leaf = delta_tree.s;
                for(int j = i+1;j<num_children;j++)
                {
                    child[j]->operation->set_lazy(removed_leaf, false);
                    //child[j]->push_lazy();
                }
                if(tree_change)
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
        update_operand(prefix);
        if(num_children<a)
        {
            return mp(true, removed_leaf);
        }
        else
        {
            return mp(false, removed_leaf);
        }
    }
    element *delta_query(int moment)
    {
        if(moment<left_bound)
        {
            return new element();
        }
        push_lazy();
        if(num_children == 0)
        {
            return operation;
        }
        element* ret = new element();
        for(int i = 0; i<num_children; i++)
        {
            if(child[i]->left_bound<=moment && moment<child[i]->right_bound)
            {
                element* partial_node = child[i]->delta_query(moment);
                if(deep_print_query)
                {
                    cout << "delta query merge end: " <<endl << ret->print() << " AND "<<endl<< partial_node->print() <<endl;
                }
                ret->merge(partial_node);
                break;
            }
            else
            {

                if(deep_print_query)
                {
                    cout << "delta querry merge mid: " << ret->print() << " AND "<< endl<< child[i]->operation->print() <<endl;
                }
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
            if(deep_print)
            {
                cout << "wrap at leaf: " << endl << ret->print() << endl << operation->print() <<endl <<endl;
            }
            ret->prefix_wrap(operation);
            ///ret->wrap(operation);
            if(deep_print)
            {
                cout << "leaf:" <<endl;
                print_e(ret);
            }
            return ret;
        }
        push_lazy();
        for(int i = 0; i<num_children; i++)
        {
            if(child[i]->left_bound<=moment && moment<child[i]->right_bound)
            {
                if(deep_print)
                {
                    cout << "Walk query to child at i = " << i << "(prewalk state:)"<<endl;
                    print_e(ret);
                }

                ret = child[i]->wrap_query(ret, moment);
                //ret = new element(child[i]->wrap_query(ret, moment));
                break;
            }
            else
            {
                if(deep_print)
                {
                    cout << "query wrap at i = " << i << endl << ret->print() << endl << child[i]->operation->print() <<endl <<endl;
                }

                ret->prefix_wrap(child[i]->operation);
                //ret->wrap(child[i]->operation);
                if(deep_print)
                {
                    cout << "after query wrap with i = "<<i <<endl;
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
        push_lazy();
        right_node->push_lazy();
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
        if(deep_print)
        {
            cout << "order: "<< order << " + "<< cut << " at: "<< endl;
            print_e(cumulative);
        }
        if(num_children == 0)
        {
            if(deep_print)
            {
                cout << "order wrap at leaf" << endl << cumulative->print() << endl << operation->print() <<endl <<endl;
            }
            cumulative->prefix_wrap(operation);
            int at_order = cumulative->one[side]->prefix-cumulative->one[side]->prefix_delta;
            if(deep_print)
            {
                cout << "after order wrap at leaf: " << cumulative->print() <<endl;
                cout << "leaf at order: " << at_order <<endl;
            }
            //cumulative->wrap(operation);
            //int at_order = cumulative->one[side]->adds + cumulative->one[side]->cut;
            if(at_order == order + cut)
            {
                return operation->get(side);
            }
            else
            {
                return -inf;
            }
        }
        push_lazy();
        element *up_to_next_query = new element(cumulative);
        int query_at = 0;
        for(int i = 0; i<num_children && child[i]->left_bound<=moment; i++)
        {
            //cout << i <<endl;
            element *child_operation = child[i]->operation;
            if(child[i]->right_bound>moment)
            {
                if(deep_print)cout << "query partial child at i = " << i <<endl;
                child_operation = child[i]->delta_query(moment);
                if(deep_print)cout << "back from partial query at i = " << i << " result: " <<endl << child_operation->print() <<endl;;
                //child_operation->add_prefix(cumulative);
                child_operation->clear_push();
                if(deep_print)cout << child_operation->print() <<endl << "test low high is next" <<endl;
                //cout << "child " << i << endl;
                //print_e(child_operation);
            }
            //int low_order = cumulative->one[side]->sum+child_operation->one[side]->low  +cumulative->one[side]->cut -  min(0, cumulative->one[side]->sum - child_operation->one[side]->removes);
            //int high_order = cumulative->one[side]->sum+child_operation->one[side]->high  +cumulative->one[side]->cut - min(0, cumulative->one[side]->sum - child_operation->one[side]->removes);
            int low_order = cumulative->one[side]->prefix-child_operation->one[side]->removes - cumulative->one[side]->prefix_delta;
            int high_order = cumulative->one[side]->prefix+child_operation->one[side]->adds - cumulative->one[side]->prefix_delta;
            if(deep_print)
            {
                cout << "i = " << i << ", low high: "<< low_order <<" "<< high_order <<endl;
            }
            if(low_order<order+cut && order+cut<=high_order)
            {
                up_to_next_query = new element(cumulative);
                query_at = i;
                if(deep_print)cout << "take" <<endl;
            }

            if(deep_print)
            {
                cout << "order wrap at i = " << i << endl << cumulative->print() << endl << child_operation->print() <<endl;
            }
            cumulative->prefix_wrap(child_operation);
            //cumulative->wrap(child_operation);
            if(deep_print)
            {
                cout << "after wrap" <<endl;
                print_e(cumulative);
                cout << endl;
            }
        }
        if(deep_print)cout << "expand at: "  << query_at <<endl;
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
            //cout << "num_children = " << num_children << " :: ";
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
            //root->update_bounds();
        }
        else
        {
            node* augment_node = root->insert(moment, type, val).f;
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
            cout << "ORDER COUNT" <<endl;
            retro_deque->print_e(find_order);
        }
        int order = find_order->one[side]->prefix;
        if(order>=1)
        {
            return retro_deque->query_value_at_order(moment, new element(), order, -find_order->one[side]->prefix_delta, side);
        }
        else
        {
            if(print_queries) cout << "SHIFT" <<endl;
            if(find_order->one[1-side]->prefix<1)
            {
                return -inf;
            }
            return retro_deque->query_value_at_order(moment, new element(), 1, -find_order->one[1-side]->prefix_delta, 1-side);
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
bool constant[max_n+1];
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
        MEM(constant, 0);
        int q_size = 0;
        if(print_input)cout << n <<endl;
        for(int i = 0; i<n; i++)
        {
            int t, type, val;
            if(i < -n/20)
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
                type = rand(0, 5);
                val = rand(0, max_constant);
                while(constant[val] == 1)
                {
                    val+=rand(0, max_constant);
                    val%=(max_constant+1);
                }
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
                cout << t <<" "<< type;
                if(type == 1 || type == 3)cout<< " "<< val;
                cout <<endl;
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
                    illegal = true;
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
        if(AUTO_TEST) test_case = new test(n);
        work_a_b_tree();
        if(AUTO_TEST)
        {
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
                illegal++;
            }
        }
    }
    cout << "skiped: " << illegal <<" out of " << total+illegal <<endl;
    cout <<"correct: "<< correct <<"/"<<total << " :: "<< 100.0*((double)correct/total)<<endl;
    return 0;
}
