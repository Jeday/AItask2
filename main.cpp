#include <iostream>
#include <vector>
#include <map>
#include <queue>

int MIN_LVL = -1;


class Op
{
public:
    virtual int apply(int base) {}
    virtual bool is_result_of(int res){}
    Op() {}
};

class Plus3: Op{
public:
    virtual int apply(int base) { return base+3;}
    virtual bool is_result_of(int res){
        return true;
    }
    Plus3() {}
};


class Mult2: Op{
public:
    virtual int apply(int base) { return base*2;}
    virtual bool is_result_of(int res){
        return res % 2;
    }
    Mult2() {}
};

class Minus2: Op{
public:
    virtual int apply(int base) { return base-2;}
    virtual bool is_result_of(int res){
        return true;
    }
    Minus2() {}
};

std::vector<Op> OpList;

class node
{
public:
    int lvl;
    int value;
    std::vector<std::pair<int,Op>> children;
    node(): value(-1), lvl(-1) {}
    node(int val, int lv): value(val), lvl(lv) {
        children.reserve(3);
    }
};


std::map<int,node> adj_list;



void build_graph(int from, int to){
    std::queue<int> q;
    q.push(from);
    while(!q.empty()){
       int current = q.front();
       q.pop;
       for(auto i = OpList.begin(); i!=OpList.end(); i++){
           new_val = i->apply(current);
           node& current_node = adj_list[current];
           adj_list[current].children.push_back(std::make_pair(new_val,*i));
           auto res = adj_list.find(new_val);
           if( res == adj_list.end()){
               // newly discovered node
               if (MIN_LVL != -1 && current_node.lvl > MIN_LVL)
                   continue;
               adj_list[new_val] = node(new_val,lvl+1);
               q.push(new_val);
           }
           else{
               //old node
               if (new_val == to && (MIN_LVL == -1|| MIN_LVL > current_node.lvl+1 ) ){
                   MIN_LVL = current_node.lvl+1;
               }
               if(res->lvl > current_node.lvl+1)
                   res->lvl = current_node.lvl+1;
           }


       }
    }
}

void build_for_Plus3Mult2(int from, int to){
    adj_list.clear();
    MIN_LVL = -1;
    adj_list[to] = node(to,-1);
    adj_list[from] = node(from,0);
    OpList = {Plus3(),Mult2()};

    build_graph(from,to);

}


int main()
{

}
