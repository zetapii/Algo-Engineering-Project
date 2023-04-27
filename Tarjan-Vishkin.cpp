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
            // dsuArrayLock.lock();
            par[px]=py;
            // dsuArrayLock.unlock();
        }
        else if(rnk[px]>rnk[py])
        {
            // dsuArrayLock.lock();
            par[py]=px;
            // dsuArrayLock.unlock();
        }
        else 
        {
            // dsuArrayLock.lock();
            par[px]=py;
            rnk[py]++;
            // dsuArrayLock.unlock();
        }
        return 1;
    }
};

int LG;
vector< int > depth;    //depth of nodes
vector< int > dfsend;   //endtime of nodes
vector< int > lowTimeMin;
vector< int > lowTimeMax;
vector< vector<int> > par;  //2-d vector having parent of nodes in the spanning tree - binary lifting
vector< vector< pair<int,int> > > tree; //spanning tree
vector< pair< pair<int,int> ,int> > edges; //edges in the original graph
vector< vector< pair<int,int> > > adj;  //adjacency list of the original graph
vector<int> parentEdgeSpanningTree; //parent edge index of the node u in spanning tree
vector< bool > nontree; //will mark edge index i as tree or non tree

int maps[20][20];

void initmap()
{
    //tree edges
    maps[1][11]=1;
    maps[11][1]=1;
    maps[11][6]=1;
    maps[6][11]=1;
    maps[6][10]=1;
    maps[10][6]=1;
    maps[6][7]=1;
    maps[7][6]=1;
    maps[7][5]=1;
    maps[5][7]=1;
    maps[5][9]=1;
    maps[9][5]=1;
    maps[5][3]=1;
    maps[3][5]=1;
    maps[3][4]=1;
    maps[4][3]=1;
    maps[4][12]=1;
    maps[12][4]=1;
    maps[12][8]=1;
    maps[8][12]=1;
    maps[8][2]=1;
    maps[2][8]=1;

    //back edges
    maps[1][10]=1;
    maps[10][1]=1;
    maps[6][5]=1;
    maps[5][6]=1;
    maps[1][9]=1;
    maps[9][1]=1;
    maps[6][4]=1;
    maps[4][6]=1;
    maps[4][2]=1;
    maps[2][4]=1;
    return ;
}

void generateGraph(int n)
{
    adj.resize(n+1);
    tree.resize(n+1);
    for(int i=1;i<=n;i++)
    {
        for(int j=i+1;j<=n;j++)
        {
            int cur = rand()%4;
            if(maps[i][j] == 1)
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
    for(auto A:edges)
    {
        if(nontree[A.second])
        {
            int u = A.first.first;
            int v = A.first.second;
            int l = getLCA(u,v);
            if(l==u or l==v)
            {
                if(depth[u]>depth[v])
                {
                    swap(u,v);
                }
                // lowTimeMin[par[v][0]]=min(lowTimeMin[par[v][0]],dfsorder[u]);
                // lowTimeMax[par[v][0]]=max(lowTimeMax[par[v][0]],dfsorder[u]);
                lowTimeMin[v]=min(lowTimeMin[v],dfsorder[u]);
                lowTimeMax[v]=max(lowTimeMax[v],dfsorder[u]);
            }
            else 
            {
                // lowTimeMin[par[u][0]]=min(lowTimeMin[par[u][0]],dfsorder[v]);
                // lowTimeMax[par[u][0]]=max(lowTimeMax[par[u][0]],dfsorder[v]);
                // lowTimeMin[par[v][0]]=min(lowTimeMin[par[v][0]],dfsorder[u]);
                // lowTimeMax[par[v][0]]=max(lowTimeMax[par[v][0]],dfsorder[u]);
                lowTimeMin[u]=min(lowTimeMin[u],dfsorder[v]);
                lowTimeMax[u]=max(lowTimeMax[u],dfsorder[v]);
                lowTimeMin[v]=min(lowTimeMin[v],dfsorder[u]);
                lowTimeMax[v]=max(lowTimeMax[v],dfsorder[u]);

            }
        }
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
            // check which type of edge is this
            // if(dfsorder[])
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
                if(dfsorder[u]>dfsorder[v])
                    dsu.merge(parentEdgeSpanningTree[u],A.second);
                else 
                    dsu.merge(parentEdgeSpanningTree[v],A.second);
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
    cout<<"Biconnected Components are :\n";
    for(int i=0;i<edges.size();i++)
    {
        cout<<edges[i].first.first<<" "<<edges[i].first.second<<" "<<dsu.par[edges[i].second]<<"\n";
    }
    cout<<endl;
    return ;
}

int main()
{
    initmap();
    int n = 12;
    generateGraph(n);
    // for(auto A:edges)
    // {
    //     cout<<A.first.first<<" "<<A.first.second<<endl;
    // }
    generateSpanningTree(n);
    dfsSpanningTree({1,0},0);
    initLCA(n);
    initLowtime(n);
    findBiconnectedComponents(n);
    for(int i=1;i<=n;i++)
    {
        for(auto B:tree[i])
        {
            cout<< " tree edge "<<" "<<i<<" "<<B.first<<endl;
        }
    }
    for(auto A:edges)
    {
        if(nontree[A.second])
        {
            cout<<A.first.first<<" non tree "<<A.first.second<<endl;
        }
    }
    // for(int i=1;i<=n;i++)
    // {
    //     cout<<i<<" low time min "<<" "<<dfsorder[i]<<" "<<lowTimeMin[i]<<endl;
    // }
    
    // for(int i=0;i<edges.size();i++)
    // {
    //     cout<<" nontree value " <<edges[i].first.first<<" "<<edges[i].first.second<<" "<<nontree[edges[i].second]<<endl;
    // }
    // for(int i=1;i<=n;i++)
    // {
    //     cout<<i<<" "<<parentEdgeSpanningTree[i]<<" par \n";
    // }   
    return 0;
}