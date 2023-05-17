#include "bits/stdc++.h"
using namespace std;

vector< vector<int> > vec;
vector< vector<int> > tree;
vector< vector<int> > nontree;

int edgcnt;

void generateGraph(int n)
{
    vec.resize(n+10);
    tree.resize(n+10);
    nontree.resize(n+10);
    cin>>edgcnt;
    for(int i=1;i<=edgcnt;i++)
    {
        int x,y;
        cin>>x>>y;
        vec[x].push_back(y);
        vec[y].push_back(x);
    }
    return ;
}

int Time;

void dfsBR(int u, int p,vector<int> &low,vector<int> &disc) 
{
    low[u]  = ++Time;
    disc[u] = Time;
    for (int v : vec[u]) 
    {
        if (v == p) 
        {
            continue; 
        }
        else if (!disc[v]) 
        {  
            dfsBR(v, u, low, disc); 
            low[u] = min(low[u], low[v]); 
            tree[u].push_back(v);
        } 
        else
        {
            low[u] = min(low[u], disc[v]);
            if(disc[v]<disc[u])
                nontree[u].push_back(v);
        }
    }
}

int totid;

void printComponent(int node,int par,int id,vector<int> &vis,vector<int> &disc,vector<int> &low)
{
    if(par)
    {
        cout<<" edge "<<node<<"-"<<par<<" component id "<<id<<endl;
    }
    for(auto A:tree[node])
    {
        if((low[A]>=disc[node] && node!=1 ) || (node==1 && tree[1].size()>1))
        {
            totid++;
            printComponent(A,node,totid,vis,disc,low);
        }
        else 
        {
            printComponent(A,node,id,vis,disc,low);
        }
    }
    for(auto A:nontree[node])
    {
        cout<<" edge "<<node<<"-"<<A<<" component id "<<id<<endl;
    }
    return ;
}


void findBiconnectedComponents(int n)
{
    vector<int> low(n+10,0);
    vector<int> disc(n+10,0);
    vector<int> vis(n+10,0);
    vector<int> comp(n+10,0);
    dfsBR(1,0,low,disc);
    printComponent(1,0,++totid,vis,disc,low);
    return ;
}

int main(int argc,char* argv[])
{
    int n;
    cin>>n;
    generateGraph(n);
    auto begin = std::chrono::high_resolution_clock::now();
    findBiconnectedComponents(n);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Time measured: %.3f seconds.\n", elapsed_time.count() * 1e-9);
    return 0;
}
