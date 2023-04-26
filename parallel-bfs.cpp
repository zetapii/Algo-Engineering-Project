#include <iostream>
#include <thread>
#include <mutex>
#include <sys/time.h>
#include "bits/stdc++.h"
using namespace std;

// function to be executed by a thread
vector<int> vec[200000];

int vis[10000];
int par_[2000000];
vector<int> tree[10000];

mutex vis_array;
mutex dsu_array;
 
int par[2000000];
int rank_[10000];

vector< pair<int,int> > edges;

int q;
int get(int x)
{
q++;
    if(x==par[x])
        return x;
    return par[x]=get(par[x]);
}

int merge(int x,int y)
{
    int px = get(x);
    int py = get(y);
    if(px==py)
        return 0;
    if(rank_[px]<rank_[py])
    {
        par[px]=py;
    }
    else if(rank_[px]>rank_[py])
    {
        par[py]=px;
    }
    else 
    {
        dsu_array.lock();
        par[px]=py;
        rank_[py]++;
        dsu_array.unlock();
    }
    return 1;
}

void bfs(int node) 
{
    queue<int> qu;
    qu.push(node);
    while(!qu.empty())
    {
        int cur=qu.front();
        // cout<<cur<<endl;
        qu.pop();
        for(auto A:vec[cur])
        {
            if(vis[A]==0)
            {  
                vis_array.lock();
                // acquire the lock
                if(vis[A]==0)
                {
                    par_[A]=cur;
                    vis[A]=1;
                    tree[cur].push_back(A);
                    qu.push(A);
                }
                else 
                {
                    cout<<"fuck\n";
                }
                vis_array.unlock(); // release the lock
            }
        }
    }
}


int Time;

int dfsBR(int u, int p,vector<int> &low,vector<int> &disc) {
  low[u]  = ++Time;
  disc[u] = Time;
  for (int& v : vec[u]) {
    if (v == p) continue; // we don't want to go back through the same path.
                          // if we go back is because we found another way back
    if (!disc[v]) { // if V has not been discovered before
      dfsBR(v, u, low, disc);  // recursive DFS call
      low[u] = min(low[u], low[v]); // low[v] might be an ancestor of u
    } else // if v was already discovered means that we found an ancestor
      low[u] = min(low[u], disc[v]); // finds the ancestor with the least discovery time
  }
}

void BR(int n)
{
    vector<int> low(10000,0);
    vector<int> disc(10000,0);
    for (int u = 0; u < n; u++)
        if (!disc[u])
            dfsBR(u, u,low,disc);
        for(int u=0;u<n;u++)
        {
            low[u]=0;
            disc[u]=0;
        }
        Time=0;
    for(int u=0; u<n ;u++)
    {
            if (!disc[u])
                dfsBR(u, u,low,disc);
    }
}


int intime;
int tot[100000];

void tfs(int node)
{
    tot[node]=++intime;
    for(auto A:tree[node])
        tfs(A);
}

int p=1000000;

void assign_comp(int l,int r)
{
    for(int i=l;i<=r;i++)
    {
        if(p>0)
        {
            // cout<<"hey \n";
            if(merge(edges[l].first,edges[l].second)>0)
            {
                p--;
            }
        }            
    }
}

int main() 
{
    int n = 1000;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<i;j++)
        {
            int cur = 1;
            if(cur)
                vec[i].push_back(j),
                vec[j].push_back(i),
                edges.push_back({i,j});
        }
    }
    for(int i=0;i<edges.size();i++)
    {
        par[i]=i;   
    }
    auto begin = std::chrono::high_resolution_clock::now();
    BR(n);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<(elapsed_time.count() * 1e-9);
    vis[0]=1;
    vis[1]=1;
    vis[2]=1;
    vis[3]=1;
    vis[4]=1;
    tree[0].push_back(1);
    tree[0].push_back(2);
    tree[0].push_back(3);
    tree[0].push_back(4);
    begin = std::chrono::high_resolution_clock::now();
    thread t1(bfs,1);
    thread t2(bfs,2);
    thread t3(bfs,3);
    thread t4(bfs,4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
// now we have the tree
    tfs(0);
    end = std::chrono::high_resolution_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<(elapsed_time.count() * 1e-9);
    // //spanning tree generation done
    begin = std::chrono::high_resolution_clock::now();
    end = std::chrono::high_resolution_clock::now();

    thread t1_(assign_comp,0,edges.size()/4-1);
    thread t2_(assign_comp,edges.size()/4,edges.size()/2-1);
    thread t3_(assign_comp,edges.size()/2,edges.size()*3/4-1);
    thread t4_(assign_comp,edges.size()*3/4,edges.size()-1);
    t1_.join();
    t2_.join();
    t3_.join();
    t4_.join();
    // elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // cout<<(elapsed_time.count() * 1e-9);
    cout<<"ff" <<q;
    return 0;
}