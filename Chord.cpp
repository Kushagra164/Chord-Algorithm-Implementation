#include <bits/stdc++.h>
#define rep(i,a,b) for(int i = a; i < b; i++)
#define ll long long
#define pb push_back
#define all(a) a.begin(),a.end()
#define MOD 1000000007

using namespace std;

// Functionalities Offerred:

// Add Key
// Search Key
// Add Node
// Stabilization
// Show table
// Eclipse attack



//Code By Kush

class Node{
    public:
        int id;
        vector<vector<int>> FingerTable; 
        set<int> keys;
};


class CHORD{
    public:
        int m,total_nodes,f_node,lst_node;
        map<int,Node> idVsNode;
        set<int> MaliciousNodes,Keys,availNodes;

        CHORD(int x){
            this->m  = x;
            this->total_nodes = (1<<m);
            this->f_node = total_nodes;
            this->lst_node = -1;
            // cout << f_node ;
        }

        // Implemented Searching Algo
        int findKey(int key){
            if (Keys.find(key) == Keys.end()){
                return -1;
            }
            set<int> vis;
            return findKey1(f_node,key,vis);
        }

        int findKey1(int node,int key,set<int> &vis){
            if (idVsNode[node].keys.find(key) != idVsNode[node].keys.end()) return node;
            vis.insert(node);
            auto vec = idVsNode[node].FingerTable;
            int prev = node;
            for (int i=0;i<m;i++){
                int dst_node = vec[i][2];
                if (dst_node == key) return dst_node;
                else if (dst_node<key && prev<dst_node){
                    prev = dst_node;
                }
                // else if (dst_node<key && prev>dst_node){
                //     suc = prev;
                //     break;
                // }
                // else if (dst_node>key && prev>dst_node){
                //     suc = prev;
                //     break;
                // }
                // else if (dst_node>key && prev<dst_node){
                //     suc = prev;
                //     break;
                // }
                else break;
            }
            if (vis.find(prev) == vis.end()) return findKey1(prev,key,vis);
            return -1;
            
        }

        int brutefindKey(int key){
            if (Keys.find(key) == Keys.end()){
                return -1;
            }
            if (key>lst_node || key<=f_node) return f_node;
            int temp = *availNodes.lower_bound(key);
            return temp;
        }

        // Just lower bound and search for the optimal node
        // Done
        void AddKey(int key){
            if (availNodes.size() == 0) {
                cout << "No Nodes are present in the system currently\n";
                return ;
            }
            Keys.insert(key);
            if (key>lst_node){
                idVsNode[f_node].keys.insert(key);
                return ;
            }
            int node = *availNodes.lower_bound(key);
            idVsNode[node].keys.insert(key);
            return ;
        }

        // Add New Node
        void AddNode(int node){
            // If it is a malicious node
            if (MaliciousNodes.find(node) != MaliciousNodes.end()){
                cout << "It is a Malicious Node.\n";
                return ;
            }
            // If it is the first node to join the system
            if (availNodes.empty()){
                f_node = node;
                lst_node = node;
                availNodes.insert(node);
                Node nd ;
                nd.id = node;
                for (int i=0;i<m;i++){
                    nd.FingerTable.push_back({i,(node+(1<<i))%total_nodes,node});
                } 
                idVsNode[node] = nd;
                return ;
            }

            // find successor and predecessor
            int successor = findSuccessor(node);
            int predecessor = findPredecessor(node);
            Node nd;
            nd.id = node;
            
            // First take keys from successor
            // If the current node is the second node to join
            if (predecessor == successor){
                set<int> temp;
                if (node<predecessor){
                    for (auto x:idVsNode[predecessor].keys){
                        if (x<=node || x>predecessor){
                            temp.insert(x);
                            nd.keys.insert(x);
                        }
                    }
                }
                else{
                    for (auto x:idVsNode[predecessor].keys){
                        if (x<=node && x>predecessor){
                            temp.insert(x);
                            nd.keys.insert(x);
                        }
                    }
                }
                for (auto x:temp){
                    idVsNode[predecessor].keys.erase(x);
                }
            }

            // If predecessor < node < successor
            else if (predecessor < successor){
                set<int> temp;
                for (auto x:idVsNode[successor].keys){
                    if (x<=node){
                        nd.keys.insert(x);
                        temp.insert(x);
                    }
                    else break;
                }
                for (auto x:temp){
                    idVsNode[successor].keys.erase(x);
                }
            }

            // If predecessor > successor
            else{
                set<int> temp;
                if (node>predecessor){
                    for (auto x:idVsNode[successor].keys){
                        if (x>predecessor && x<=node){
                            nd.keys.insert(x);
                            temp.insert(x);
                        }
                    }
                }
                else if (node<successor){
                    for (auto x:idVsNode[successor].keys){
                        if (x>predecessor || x<=node){
                            nd.keys.insert(x);
                            temp.insert(x);
                        }
                    }
                }
                for (auto x:temp){
                    idVsNode[successor].keys.erase(x);
                }
            }

            availNodes.insert(node);
            f_node = min(f_node,node);
            lst_node = max(lst_node,node);

            // Build Finger Table for this node
            for (int i=0;i<m;i++){
                int temp_node = (node+(1<<i))%total_nodes;
                int dst_node = findSuccessor(temp_node);

                nd.FingerTable.push_back({i,temp_node,dst_node});
            }

            idVsNode[node] = nd;
            
            Stabilize();
            return ;
        }

        // Find Successor of a node
        int findSuccessor(int node){
            if (node > lst_node ) return f_node;
            int temp = *availNodes.lower_bound(node);
            return temp;
        }

        // Find Predeccessor of a node
        int findPredecessor(int node){
            if (node < f_node || node>lst_node) return lst_node;
            int temp = *upper_bound(availNodes.rbegin(),availNodes.rend(),node,greater<int>());
            return temp;
        }

        // Update all the finger tables
        void Stabilize(){
            for (auto node:availNodes){
                idVsNode[node].FingerTable.clear();
                for (int i=0;i<m;i++){
                    
                    int temp_node = (node+(1<<i))%total_nodes;
                    int dst_node = findSuccessor(temp_node);

                    idVsNode[node].FingerTable.push_back({i,temp_node,dst_node});
                }
            }

            return ;
        }

        // Print the Finger Table
        // Done
        void ShowTable(int node){
            if (idVsNode.find(node) == idVsNode.end()) {
                cout << "No node found\n";
                return ;
            }
            Node nd = idVsNode[node];
            cout << "Finger Table for node: " << node << "\n";
            cout << "(i)" << "(id+2^i)" << "(Dst_Node)\n";
            for(auto vec:nd.FingerTable){
                cout << " " << vec[0] << "    " << vec[1] << "       " << vec[2] << "\n";  
            }
            cout << "\n";
            cout << "Keys Present at this node: " << "\n";
            for (auto x:nd.keys) cout << x << " ";
            cout << "\n";
            return ;
        }

        // Make malicious nodes and change the finger table
        void doEclipseAttackOn(int node){
            auto fingerTable = idVsNode[node].FingerTable;
            for (int i=0;i<m;i++){
                if (fingerTable[i][1] != fingerTable[i][2]){
                    AddNode(fingerTable[i][1]);
                    MaliciousNodes.insert(fingerTable[i][1]);
                }
            }
        }

        // Print Malcious Nodes
        // Done
        void printMaliciousNodes(){
            cout << "Malicious Nodes are: \n";
            for (auto x:MaliciousNodes) cout << x << " ";
            cout << "\n";
            return ;
        }
};

void solve(){
    // Enter the m value
    int m;
    cin >> m;
    
    CHORD chord(m);

    // Enter the number of queries
    int q;
    cin >> q;

    while(q--){
        string s;
        cin >> s;

        // Search for key
        // Done
        if (s == "Search"){
            int key;
            cin >> key ;
            int ans = chord.findKey(key);
            if (ans == -1){
                int ans_again = chord.brutefindKey(key);
                if (ans_again == -1) cout << "Key is not present in the System\n";
                else cout << "Key is present at Node: " << ans_again << "\n";
            }
            else cout << "Key is present at Node: " << ans << "\n";
        }

        // Add Key
        // Done
        if (s == "AddKey"){
            int key;
            cin >> key;
            chord.AddKey(key);
            cout << "Successfully Added Key: " << key << "\n";
        }

        // Add Node
        // Done
        if (s == "AddNode"){
            int node;
            cin >> node;
            chord.AddNode(node);
            // chord.Stabilize();
            cout << "Successfully Added Node: " << node << "\n";
        }

        // Show Finger Table
        // Done
        if (s == "Show"){
            int node;
            cin >> node;
            chord.ShowTable(node);
        }

        // Eclipse Attack
        if (s == "EclipseAttackOn"){
            int node;
            cin >> node;
            chord.doEclipseAttackOn(node);
            cout << "After attack: " << "\n";
            cout << "\n";
            chord.ShowTable(node);
            cout << "\n";
            chord.printMaliciousNodes();
        }

        cout << "\n";
    } 

}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}