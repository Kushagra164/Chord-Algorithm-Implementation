#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i < b; i++)
#define ll long long
#define pb push_back
#define all(a) a.begin(),a.end()
#define MOD 1000000007

using namespace std;

// Search Key
// Add Node
// Add Key and corresponding value
// Stabilization
// Show table


//Code By Kush

class Node{
    public:
    vector<vector<int>> FingerTable;
    map<int,int> keys;
    int id;
    int m;
    set<int> availNodes;

    void buildNode(int sze,int total_nodes,int n,set<int> &nodes){
        id = n;
        m = sze;
        availNodes = nodes;
        for (int i=0;i<m;i++){
            int temp_node = (n+(1<<i))%total_nodes;
            FingerTable.push_back({i,temp_node,find_dst_node(temp_node)});
        }
    }
    
    int find_dst_node(int temp_node){
        auto it = lower_bound(availNodes.begin(),availNodes.end(),temp_node);
        int node = *it;
        if (it == availNodes.end()) node = *availNodes.begin();
        return node;
    }

    void insertKey(int key,int value){
        keys[key]=value;
    }

    void updateTable(set<int> &nodes){
        availNodes = nodes;
        for (int i=0;i<m;i++){
            int temp_node = FingerTable[i][1];
            FingerTable[i][2] = find_dst_node(temp_node);
        }
        return ;
    }

    int findValueofKey(int key){
        auto it = keys.find(key);
        if (it == keys.end()) return INT_MIN;
        return it->second;
    }
};



class CHORD{
    public:
        int m,total_nodes;
        map<int,Node> idVsNode;
        set<int> availNodes;

        CHORD(int num){
            m = num;
            total_nodes = (1<<m);
        }

        void stabilize(){
            // update all tables
            // cout << "Working\n";
            for (auto &mp:idVsNode){
                auto node = &mp.second ;
                node->updateTable(availNodes);
            }
        }

        // Search Algo implemented here
        pair<int,int> search(int k){
            int key = k%(total_nodes);
            auto it = lower_bound(availNodes.begin(),availNodes.end(),key);
            if (it == availNodes.end()) it = availNodes.begin();
            pair<int,int> ans;
            int node = *it;
            ans.first = node;
            auto loc_node = &idVsNode[node];
            int value = loc_node->findValueofKey(key);
            if (value == INT_MIN) return {-1,-1};
            ans.second = value;
            return ans;
        }

        int findPredecessor(int node){
            int prev = *availNodes.rbegin();
            // if (availNodes.find(node) == availNodes.begin()) return *availNodes.rbegin();
            for (auto it:availNodes){
                if (it == node) break;
                prev = it;
            }
            return prev;
        }

        void addNode(int node){
            availNodes.insert(node);
            Node nd;
            nd.buildNode(m,total_nodes,node,availNodes);
            idVsNode[node]=nd;
            // Take keys from successor
            int suc = nd.FingerTable[0][2];
            if (suc == node) return ;
            Node* sucnode = &idVsNode[suc];
            // yaha pe error hai
            // predecessor nikal lo
            int pre = findPredecessor(node);
            if (node>pre){
                for (auto key:sucnode->keys){
                    // Case 1: pre<key.first<node
                    if ((key.first <= node && key.first>pre)) {
                        nd.insertKey(key.first,key.second);
                    }
                    // Case 2: node<pre
                    else break;
                }
            }
            else {
                for (auto &key:sucnode->keys){
                    if ((key.first>pre)||(key.first<=node)) {
                        nd.insertKey(key.first,key.second);
                    }
                }
            }
            for (auto &key:nd.keys){
                auto it = sucnode->keys.find(key.first);
                sucnode->keys.erase(it);
            }

            return ;
        }

        void addKey(int k,int value){
            int key = k%(total_nodes);
            auto it = lower_bound(availNodes.begin(),availNodes.end(),key);
            if (it == availNodes.end()) it = availNodes.begin();
            int node = *it ;
            auto temp=&idVsNode[node];
            temp->insertKey(k,value);
        }

        void showTable(int node){
            if (idVsNode.find(node) == idVsNode.end()) {
                cout << "No node found\n";
                cout << "\n";
                return ;
            }
            Node nd = idVsNode[node];
            cout << "Finger Table for node: " << node << "\n";
            cout << "(i)" << "(id+2^i)" << "(Dst_Node)\n";
            for(auto vec:nd.FingerTable){
                cout << " " << vec[0] << "    " << vec[1] << "       " << vec[2] << "\n";  
            }
            cout << "\n";
        }
        
};

void solve() {
    // Get the m value
    int m;
    // cout << "Enter m value: ";
    cin >> m;
    // Make Nodes for chord protocol
    CHORD chord(m);
    // cout << "\nEnter the number of queries: ";
    int queries;
    cin >> queries;
    while(queries--){
        string s;
        cin >> s;
        // Search karna hai...shi chal rha hai
        if (s == "Search"){
            int key;
            cin >> key;
            auto ans = chord.search(key);
            if (ans.first == -1) cout << "Key is not present\n";
            else cout << "Node at which key is present : " << ans.first << "\nKey Value = " << ans.second<<"\n";
            cout << "\n";
        }
        // AddNode hogaya
        else if (s == "AddNode"){
            int node;
            cin >> node;
            chord.addNode(node);
            chord.stabilize() ;
        }
        // AddKey hogaya....shi chal rha
        else if (s == "AddKey"){
            int key,value;
            cin >> key >> value ;
            chord.addKey(key,value);
        }
        // Show hogaya....shi chal rha
        else if (s == "Show"){
            int node;
            cin >> node;
            chord.showTable(node);
        }
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}