#include "bits/stdc++.h"
using namespace std;

mutex check;
mutex dsuArrayLock;

class UnionFind 
{
    public :
    vector<int> par;
    vector<int> rnk;
    void init(int n)
    {

        par.resize(n+1);
        rnk.resize(n+1);
        for(int i=0;i<=n;i++)
        {
            par[i]=i;
            rnk[i]=0;
        }
    }
    int get(int x)
    {
        if(x==par[x])
        {
            return x;
        }
        else 
        {
            return par[x]=get(par[x]);
        }
    }
    int merge(int x,int y)
    {
        int px = get(x);
        int py = get(y);
        if(px==py)
            return 0;
        if(rnk[px]<rnk[py])
        {
            par[px]=py;
        }
        else if(rnk[px]>rnk[py])

        {
            par[py]=px;
        }
        else 
        {
            par[px]=py;
            rnk[py]++;
        }
        return 1;
    }
};

int LG;
vector< int > depth;    //depth of nodes
vector< int > dfsend;   //endtime of nodes
vector< int > lowTimeMin;
vector< int > lowTimeMax;
vector< int>  lcaStore;
vector< vector<int> > par;  //2-d vector having parent of nodes in the spanning tree - binary lifting
vector< vector< pair<int,int> > > tree; //spanning tree
vector< pair< pair<int,int> ,int> > edges; //edges in the original graph
vector< vector< pair<int,int> > > adj;  //adjacency list of the original graph
vector<int> parentEdgeSpanningTree; //parent edge index of the node u in spanning tree
vector< bool > nontree; //will mark edge index i as tree or non tree

void generateGraph(int n)
{
    adj.resize(n+1);
    tree.resize(n+1);
    int m;
    cin>>m;
    int x,y;
    for(int i=1;i<=m;i++)
    {
        cin>>x>>y;
        adj[x].push_back({y,edges.size()});
        adj[y].push_back({x,edges.size()});
        edges.push_back({{x,y},edges.size()});
        nontree.push_back(true);
    }
    lcaStore.resize(edges.size()+1);
    parentEdgeSpanningTree.resize(edges.size()+1);
    return ;
}

int ptr;
vector<int> dfsorder;

void generateSpanningTree(int n)
{
    queue<int> qu;
    vector<bool> vis(n+1,false);
    qu.push(1);
    depth.resize(n+1);
    dfsorder.resize(n+1);
    dfsend.resize(n+1);
    vis[1]=true;
    while(!qu.empty())
    {
        int cur = qu.front();
        qu.pop();
        for(auto A:adj[cur])
        {
            if(!vis[A.first])
            {
                vis[A.first]=true;
                tree[cur].push_back(A);
                nontree[A.second]=false;
                qu.push(A.first);
            }
        }
    }
    return ;
}

void dfsSpanningTree(pair<int,int> node,int hei)
{
    depth[node.first]=hei;
    dfsorder[node.first]=ptr++;
    for(auto A:tree[node.first])
    {
        dfsSpanningTree(A,hei+1);
        parentEdgeSpanningTree[A.first]=A.second;
    }
    dfsend[node.first]=ptr-1;
    return ;
}

void initLCA(int n)
{       
    int lg=log2(n)+1;
    par.resize(n+3);
    LG=lg+1;
    for(int i=0;i<=n;i++)
    {
        par[i].resize(lg+3);
    }
    par[1][0]=0;
    for(int i=1;i<=n;i++)
    {
        for(auto A:tree[i])
        {
            par[A.first][0]=i;
        }
    }
    for(int i=1;i<LG;i++)
    {
        for(int j=1;j<=n;j++)
        {
            par[j][i]=par[par[j][i-1]][i-1];
        }
    }
    return ;
}

int getLCA(int u, int v) 
{
    if (depth[u] < depth[v]) 
        swap(u, v);
    for (int i = LG-1; i >= 0; i--) 
    {
        if (depth[par[u][i]] >= depth[v]) 
        {
            u = par[u][i];
        }
    }
    if (u == v) 
    {
        return u;
    }
    for (int i = LG-1; i >= 0; i--) 
    {
        if (par[u][i] != par[v][i]) 
        {
            u = par[u][i];
            v = par[v][i];
        }
    }
    return par[u][0];
}


void initLowtime(int n)
{
    lowTimeMin.resize(n+1);
    lowTimeMax.resize(n+1);
    for(int i=1;i<=n;i++)
    {
        lowTimeMin[i]=dfsorder[i];
        lowTimeMax[i]=dfsorder[i];
    }
    //use multiple threads in here
    auto lambda_func_parallel = [&](int l, int r)
    {
        for(int i=l;i<=r;i++)
        {
            auto A=edges[i];
            if(nontree[A.second])
            {
                int u = A.first.first;
                int v = A.first.second;
                int l = getLCA(u,v);
                lcaStore[A.second]=l;
                if(l==u or l==v)
                {
                    if(depth[u]>depth[v])
                    {
                        swap(u,v);
                    }
                    lowTimeMin[v]=min(lowTimeMin[v],dfsorder[u]);
                    lowTimeMax[v]=max(lowTimeMax[v],dfsorder[u]);
                }
                else 
                {
                    lowTimeMin[u]=min(lowTimeMin[u],dfsorder[v]);
                    lowTimeMax[u]=max(lowTimeMax[u],dfsorder[v]);
                    lowTimeMin[v]=min(lowTimeMin[v],dfsorder[u]);
                    lowTimeMax[v]=max(lowTimeMax[v],dfsorder[u]);

                }
            }
        }
    };
    thread t[1000];
    int nth=50;
    for(int i=0;i<nth;i++)
    {
        t[i]=std::thread(lambda_func_parallel,i*edges.size()/nth,(i+1)*edges.size()/nth-1);
    }
    for(int i=0;i<nth;i++)
    {
        t[i].join();
    }
    function<void(int)> dfsLowTime = [&](int u) 
    {
        for (auto v : tree[u]) 
        {
            dfsLowTime(v.first);
            lowTimeMin[u]=min(lowTimeMin[u],lowTimeMin[v.first]);
            lowTimeMax[u]=max(lowTimeMax[u],lowTimeMax[v.first]);
        }
 
    };
    dfsLowTime(1);
    return ;
}

void findBiconnectedComponents(int n)
{
    UnionFind dsu;
    dsu.init(edges.size());
    for(auto A:edges)
    {
        if(nontree[A.second])   
        {
            int u = A.first.first;
            int v = A.first.second;
            // int l = getLCA(u,v);
            int l = lcaStore[A.second];
            if(l==u or l==v)
            {
                if(l==v)
                    swap(u,v);
                dsu.par[A.second]=dsu.get(parentEdgeSpanningTree[v]);
            }
            else 
            {
                dsu.merge(parentEdgeSpanningTree[u],parentEdgeSpanningTree[v]);
                if(dfsorder[u]>dfsorder[v])
                {
                    dsu.merge(parentEdgeSpanningTree[u],A.second);
                }
                else 
                {
                    dsu.merge(parentEdgeSpanningTree[v],A.second);
                }
            }
        }
        else 
        {
            // merge this with parent of v
            int u = A.first.first;
            int v = A.first.second;
            if(depth[u]>depth[v])
                swap(u,v);
            if(depth[u]==0)
                continue;;
            int w = par[u][0];
            if(!(dfsorder[u]<=lowTimeMin[v] && lowTimeMax[v]<=dfsend[u]))
            {
                dsu.merge(parentEdgeSpanningTree[v],parentEdgeSpanningTree[u]);
            }
        }
    }
    for(auto A:edges)
    {
        cout<<" edge "<<A.first.first<<"-"<<A.first.second<<" component id "<<dsu.get(A.second)<<endl;
    }
    return ;
}

int main(int argc,char* argv[])
{
    int n;
    cin>>n;
    generateGraph(n);
    auto begin = std::chrono::high_resolution_clock::now();
    generateSpanningTree(n);
    dfsSpanningTree({1,0},0);
    initLCA(n);
    initLowtime(n);
    findBiconnectedComponents(n);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed_time.count() * 1e-9);
    return 0;
}

/*
11 14
1 2
2 3
3 4
4 2
1 3
4 5
5 6
6 4
5 11
5 10
6 7
7 8
7 9
8 9
*/
