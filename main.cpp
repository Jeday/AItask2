#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>

int MIN_LVL = -1;


class Op
{
public:
    int type; // 0 - none, 1 - plus3, 2 - multp2 , 3 - minus2,
    int apply(int base) {
        switch (type) {
        case 1:
            return base+3;
            break;
        case 2:
            return base*2;
            break;
        case 3:
            return base-2;
            break;
        default:
            return -1;
            break;
        }
    }

    int rev_apply(int base) {
        switch (type) {
        case 1:
            return base-3;
            break;
        case 2:
            return base/2;
            break;
        case 3:
            return base+2;
            break;
        default:
            return -1;
            break;
        }
    }

    bool is_result_of(int res){
        switch (type) {
        case 1:
            return true;
            break;

        case 2:
            return res%2 == 0;
            break;
        case 3:
            return true;
            break;
        default:
            return false;
            break;
        }
    }
    Op() {
        type = 0;
    }
    Op(int _type){
        type = _type;
    }

};

std::vector<Op> OpList;

class node
{
public:
    int lvl;
    int value;
    int father;
    std::vector<std::pair<int,Op>> children;
    node(): value(-1), lvl(-1) {}
    node(int val, int lv, int fat): value(val), lvl(lv), father(fat) {
        children.reserve(3);
    }
};


std::map<int,node> adj_list;


void update_nodes(node& nd,int f,int lv){
    // only update if new lvl is better
    if(lv < nd.lvl){

        nd.father = f;
        nd.lvl = lv;
        for(auto i = nd.children.begin(); i != nd.children.end();i++){
            auto f = adj_list.find(i->first);
            if (f != adj_list.end())
                 update_nodes(f->second,nd.value,lv+1);
        }
    }
}



void build_graph(int from, int to){
    std::queue<int> q;
    q.push(from);
    while(!q.empty()){
       int current = q.front();
       q.pop();
        // get refrence to current node for convinience
        node& current_node = adj_list[current];

       // stop if  found target node
       if( current_node.value == to ){
           MIN_LVL = MIN_LVL == -1 ? current_node.lvl : MIN_LVL;
           continue;
       }

       // stop if lvl boundary exceded
       if (MIN_LVL != -1 && current_node.lvl >= MIN_LVL)
           continue;

        // for all types of operations
       for(auto i = OpList.begin(); i!=OpList.end(); i++){
           // calculate new value
           int new_val = i->apply(current);
           // add new val to children
           current_node.children.push_back(std::make_pair(new_val,*i));

           // working with new val now
           auto res = adj_list.find(new_val);
           if( res == adj_list.end()){
               // newly discovered node; add to list and push to queue
               adj_list[new_val] = node(new_val, current_node.lvl+1,current_node.value);
               q.push(new_val);
           }
           else{
               //already found node
               // update lvl boundary
               if (new_val == to && (MIN_LVL == -1|| MIN_LVL > current_node.lvl+1 ) )
                   MIN_LVL = current_node.lvl+1;
               // update lvl and fathers
               update_nodes(res->second,current_node.value,current_node.lvl+1);
               }
           }
       }
    }

void build_graph_rev(int from, int to){
    std::queue<int> q;
    q.push(from);
    while(!q.empty()){
       int current = q.front();
       q.pop();
        // get refrence to current node for convinience
        node& current_node = adj_list[current];

       // stop if  found target node
       if( current_node.value == to ){
           MIN_LVL = MIN_LVL == -1 ? current_node.lvl : MIN_LVL;
           continue;
       }

       // stop if lvl boundary exceded
       if (MIN_LVL != -1 && current_node.lvl >= MIN_LVL)
           continue;

        // for all types of operations
       for(auto i = OpList.begin(); i!=OpList.end(); i++){
           // calculate new value
           int new_val;
           if (i->is_result_of(current))
                  new_val = i->rev_apply(current);
           else
               continue;
           // add new val to children
           current_node.children.push_back(std::make_pair(new_val,*i));
           // working with new val now
           auto res = adj_list.find(new_val);
           if( res == adj_list.end()){
               // newly discovered node; add to list and push to queue
               adj_list[new_val] = node(new_val, current_node.lvl+1,current_node.value);
               q.push(new_val);
           }
           else{
               //already found node
               // update lvl boundary
               if (new_val == to && (MIN_LVL == -1|| MIN_LVL > current_node.lvl+1 ) )
                   MIN_LVL = current_node.lvl+1;
               // update lvl and fathers
               update_nodes(res->second,current_node.value,current_node.lvl+1);
               }
           }
       }
    }



std::vector<int> find_path(int from, int to){
    std::vector<int> r;
    r.reserve(MIN_LVL+1);
    int cur = to;
    auto res = adj_list.find(cur);
    while( cur != from){
        r.push_back(cur);
        res = adj_list.find(res->second.father);
        if(res!= adj_list.end())
            cur = res->first;
        else
            break;

    }
    r.push_back(from);
    return r;
}

void print_results(std::vector<int>& vec, bool is_reversed,double time, int from, int to, std::string name){
    std::cout<<name<<" from "<<from<<" to "<<to<<" at "<<time<<" seconds: ";
    if (!is_reversed)
        for(size_t i = vec.size(); i!=0; i--)
            i-1 == 0 ? std::cout<<vec[i-1] : std::cout<<vec[i-1]<<"->";
     else
        for(size_t i = 0; i<vec.size(); i++)
            i == vec.size()-1 ? std::cout<<vec[i] : std::cout<<vec[i]<<"->";
     std::cout<<std::endl;
}


void build_for_Plus3Mult2(int from, int to){
    adj_list.clear();
    MIN_LVL = -1;
    adj_list[from] = node(from,from,0);
    OpList = {Op(1),Op(2)};
    std::clock_t begin = std::clock();
    build_graph(from,to);
    std::clock_t end = std::clock();
    auto path = find_path(from,to);
     print_results(path,false, double(end - begin) / CLOCKS_PER_SEC, from,to," PLUS3MULT2 tree");
}

void build_for_Plus3Mult2Minus2(int from, int to){
    adj_list.clear();
    MIN_LVL = -1;
    adj_list[from] = node(from,from,0);
    OpList = {Op(1),Op(2),Op(3)};
    std::clock_t begin = std::clock();
    build_graph(from,to);
    std::clock_t end = std::clock();
    auto path = find_path(from,to);
    print_results(path,false, double(end - begin) / CLOCKS_PER_SEC, from,to," PLUS3MULT2MINIS2 tree");
}

void build_for_Plus3Mult2Rev(int from,int to){
    adj_list.clear();
    MIN_LVL = -1;
    adj_list[to] = node(to,to,0);
    OpList = {Op(1),Op(2)};
    std::clock_t begin = std::clock();
    build_graph_rev(to,from);
    std::clock_t end = std::clock();
    auto path = find_path(to,from);
    print_results(path,true, double(end - begin) / CLOCKS_PER_SEC, from,to," PLUS3MULT2 reversed tree");
}



int main()
{
    int from = 2;
    int to = 1000000;
    build_for_Plus3Mult2Rev(from,to);
    build_for_Plus3Mult2(from,to);
    build_for_Plus3Mult2Minus2(from,to);

}
