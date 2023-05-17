## Command to Run Tarjan.cpp
       If we want to check the Tarjan for N nodes , run the command below :
    1. g++ Tarjan.cpp -std=c++20 -o Tarjan.out
    2. ./Tarjan.out 
       Then enter n,m where n and m are the number of nodes and the number of edges respectively
       Then etner m lines each containing two space separated integers u and v denoting an edge u-v
       Ensure that the graph is connected to get accurate results


## Command to Run Tarjan-Vishkin.cpp
       If we want to check the Tarjan-Vishkin for N nodes , run the command below :
    1. g++ Tarjan-Vishkin.cpp -std=c++20 -o Tarjan-Vishkin.out
    2. ./Tarjan-Vishkin.out 
       Then enter n,m where n and m are the number of nodes and the number of edges respectively
       Then etner m lines each containing two space separated integers u and v denoting an edge u-v
       Ensure that the graph is connected to get accurate results

## Expected Output

    The expected output of both the algorithms will be  :
    1. edge x-y component id z
       Here x-y denotes edge between x and y and z denotes component id , edges
       having same component id will belong to the same biconnected component
       in the graph
       
## Testing
  
    The code(modified for oj) was tested on 
   [library checker](https://judge.yosupo.jp/submission/139721). 

