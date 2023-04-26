#include "bits/stdc++.h"
using namespace std;

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
            par[i]=0;
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
            dsuArrayLock.lock();
            par[px]=py;
            rnk[py]++;
            dsuArrayLock.unlock();
        }
        return 1;
    }
};

int LG;
vector< int > depth;
vector< int > dfsend;
vector< int > upmerge;
vector< vector<int> > par;
vector< vector< pair<int,int> > > tree;
vector< pair< pair<int,int> ,int> > edges;
vector< vector< pair<int,int> > > adj;
vector<int> parentEdgeSpanningTree;
vector< bool > nontree;

void generateGraph(int n)
{
    adj.resize(n+1);
    tree.resize(n+1);
    for(int i=1;i<=n;i++)
    {
        for(int j=i+1;j<=n;j++)
        {
            int cur = rand()%2;
            if(cur == 1)
            {
                adj[i].push_back({j,edges.size()});
                adj[j].push_back({i,edges.size()});
                edges.push_back({{i,j},edges.size()});
                nontree.push_back(true);
            }
        }
    }
    parentEdgeSpanningTree.resize(edges.size()+1);
    return ;
}

int ptr;
vector<int> dfsorder;

void generateSpanningTree(int n)
{
    queue<int> qu;
    vector<int> vis(n+1,0);
    qu.push(1);
    depth.resize(n+1);
    dfsorder.resize(n+1);
    dfsend.resize(n+1);
    while(!qu.empty())
    {
        int cur = qu.front();
        qu.pop();
        for(auto A:adj[cur])
        {
            if(!vis[A.first])
            {
                tree[cur].push_back(A);
                nontree[A.second]=false;
                qu.push(A.first);
            }
            else 
            {
                nontree[A.second]=true;
            }
        }
    }
}

void dfsSpanningTree(pair<int,int> node)
{
    dfsorder[node.first]=ptr++;
    for(auto A:tree[node.first])
    {
        dfsSpanningTree(A);
        parentEdgeSpanningTree[A.second]=node.second;
    }
    return ;
}

void initLCA(int n)
{       
    int lg=log2(n)+1;
    par.resize(n+1);
    for(int i=1;i<=n;i++)
    {
        par[i].resize(lg+1);
        for(auto A:tree[i])
        {
            par[A.first][0]=i;
        }
    }
    for(int i=1;i<lg;i++)
    {
        for(int j=1;j<=n;j++)
        {
            par[j][i]=par[par[j][i-1]][i-1];
        }
    }
    LG=lg;
    return ;
}


int getLCA(int u, int v) 
{
    if (dfsorder[u] < depth[v]) 
        swap(u, v);
    for (int i = LG-1; i >= 0; i--) {
        if (depth[par[u][i]] >= depth[v]) {
            u = par[u][i];
        }
    }
    if (u == v) {
        return u;
    }
    for (int i = LG-1; i >= 0; i--) {
        if (par[u][i] != par[v][i]) {
            u = par[u][i];
            v = par[v][i];
        }
    }
    return par[u][0];
}


void findBiconnectedComponents(int n)
{
    UnionFind dsu;
    dsu.init(edges.size());
    for(auto A:edges)
    {
        if(nontree[A.second])
        {
            // check which type of edge is this
            // if(dfsorder[])
            // lca(u,v)
            // 
            int u = A.first.first;
            int v = A.first.second;
            int l = getLCA(u,v);
            if(l==u or l==v)
            {
                if(l==v)
                    swap(u,v);
                dsu.merge(A.second,parentEdgeSpanningTree[v]);
            }
            else 
            {
                dsu.merge(parentEdgeSpanningTree[u],parentEdgeSpanningTree[v]);
            }
        }
        else 
        {
            // merge this with parent of v
            int u = A.first.first;
            int v = A.first.second;
            if(depth[u]>depth[v])
                swap(u,v);
            if(depth[u]==1)
                continue;;
            int w = par[u][0];
            if(!(dfsorder[v]<dfsorder[u] && dfsorder[u]<dfsend[v]))
            {
                dsu.merge(A.second,parentEdgeSpanningTree[u]);
            }
        }
    }
    cout<<"Biconnected Components are :\n";
    for(int i=0;i<edges.size();i++)
    {
        cout<<edges[i].first.first<<" "<<edges[i].first.second<<" "<<dsu.par[edges[i].second]<<" ";
    }
    cout<<endl;
}

int main()
{

    
    return 0;
}