#include <iostream>
#include <vector>
#include <map>
#include <queue>

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
       for(auto i = OpList.begin(); i!=OpList.end(); i++){
           int new_val = i->apply(current);
           node& current_node = adj_list[current];
           current_node.children.push_back(std::make_pair(new_val,*i));
           auto res = adj_list.find(new_val);
           if( res == adj_list.end()){
               // newly discovered node
               if (MIN_LVL != -1 && current_node.lvl >= MIN_LVL)
                   continue;
               if( current_node.value == to ){
                   MIN_LVL = MIN_LVL == -1 ? current_node.lvl : MIN_LVL;
                   continue;
               }
               adj_list[new_val] = node(new_val,current, current_node.lvl+1);
               q.push(new_val);
           }
           else{
               //old node
               if (new_val == to && (MIN_LVL == -1|| MIN_LVL > current_node.lvl+1 ) )
                   MIN_LVL = current_node.lvl+1;
               update_nodes(res->second,current_node.value,current_node.lvl+1);
               }
           }


       }
    }



std::vector<int> find_path(int from, int to){
    std::vector r;
    r.reserve(MIN_LVL+1);
    int cur = from;
    auto res = adj_list.find(cur);
    while(res!= adj_list.end()){

    }
}

void build_for_Plus3Mult2(int from, int to){
    adj_list.clear();
    MIN_LVL = -1;
    adj_list[from] = node(from,0);
    OpList = {Op(1),Op(2)};

    build_graph(from,to);

}


int main()
{
    build_for_Plus3Mult2(2,100);
}
