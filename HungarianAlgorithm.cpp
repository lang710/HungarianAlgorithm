#include <iostream>
#include <queue>
using namespace std;

void hungarianAlgorithm(int g_size,int **graph,int g1_size,int* g1,int g2_size,int* g2){
    int i,j,k;
    //初始化匹配树
    int **match=new int*[g_size];
    for(i=0;i<g_size;i++)
        match[i]=new int[g_size];
    for(i=0;i<g_size;i++)
        for(j=0;j<g_size;j++)
            match[i][j]=0;

    //标记节点的访问，用于生成一棵交替路径树
    int *visit=new int[g_size];
    queue<int> father,son;
    //h_tree存储交替路径树，并用于发现增广路径
    int **h_tree=new int*[g_size];
    for(i=0;i<g_size;i++)
        h_tree[i]=new int[g_size];

    bool jump,free,hasfreenode;
    int fnode,snode,i_node,t_node;

    free=true;
    while(free) {
        free=false;
        i_node = -1;
        for (i = 0; i < g1_size; i++) {
            jump = false;
            for (j = 0; j < g2_size; j++)
                if (match[g1[i]][g2[j]] == 1) {
                    jump = true;
                    break;
                }
            if (!jump) {
                i_node = g1[i];
                break;
            }
        }

        if (i_node > -1) {
            for(i=0;i<g_size;i++)
                visit[i]=0;
            visit[i_node] = 1;
            father.push(i_node);
            //初始化交替路径树
            for (j = 0; j < g_size; j++)
                for (k = 0; k < g_size; k++)
                    h_tree[j][k] = 0;

            while (true) {
                while (!father.empty()) {
                    fnode = father.front();
                    father.pop();
                    for (j = 0; j < g2_size; j++) {
                        if (visit[g2[j]] == 0 && graph[fnode][g2[j]] == 1 &&
                            match[fnode][g2[j]] == 0) {             //g1到g2的未匹配边（i，j）
                            h_tree[fnode][g2[j]] = 1;
                            visit[g2[j]] = 1;
                            son.push(g2[j]);
                        }
                    }
                }
                while (!son.empty()) {
                    snode = son.front();
                    son.pop();
                    hasfreenode = true;
                    for (j = 0; j < g1_size; j++) {
                        if (visit[g1[j]] == 0 && match[snode][g1[j]] == 1) {     //g2到g1的匹配边
                            h_tree[snode][g1[j]] = 1;
                            visit[g1[j]] = 1;
                            father.push(g1[j]);
                            hasfreenode = false;
                            break;
                        }
                    }
                    if (hasfreenode) {
                        t_node = snode;
                        free = true;
                    }
                }
                if (father.empty())
                    break;
            }
        }
        if (free) {
            int tmp;
            bool augment = true;
            while (augment) {
                augment = false;
                for (j = 0; j < g_size; j++) {
                    if (h_tree[j][t_node] == 1) {
                        tmp = 1 - match[j][t_node];
                        match[j][t_node] = tmp;
                        match[t_node][j] = tmp;
                        t_node = j;
                        augment = true;
                        break;
                    }
                }
            }
        }
    }


    for(i=0;i<g_size-1;i++) {
        for (j = i+1; j < g_size; j++) {
            if (match[i][j] > 0)
                cout << i << ' ' << j << ' ' << endl;
        }
    }
}

int main(){
    int g_size;
    cin>>g_size;
    int **graph=new int*[g_size];
    int i,j;
    for(i=0;i<g_size;i++)
        graph[i]=new int[g_size];
    for(i=0;i<g_size;i++)
        for(j=0;j<g_size;j++)
            graph[i][j]=0;
    int g1_size,g2_size;
    cin>>g1_size>>g2_size;
    int *g1=new int[g1_size];
    int *g2=new int[g2_size];
    for(i=0;i<g1_size;i++)
        cin>>g1[i];
    for(i=0;i<g2_size;i++)
        cin>>g2[i];
    int edges,left,right;
    cin>>edges;
    for(i=0;i<edges;i++){
        cin>>left>>right;
        graph[left][right]=1;
        graph[right][left]=1;
    }

    hungarianAlgorithm(g_size,graph,g1_size,g1,g2_size,g2);

    return 0;
}

/*
 *
input:
10
5 5
0 2 4 6 8
1 3 5 7 9
10
0 1
0 3
1 2
2 3
2 5
3 4
5 6
5 8
6 7
6 9

output:
0 1
2 5
3 4
6 9

input:
12
6 6
0 2 5 7 8 10
1 3 4 6 9 11
17
0 1
0 4
1 2
1 5
2 3
2 6
3 7
4 5
4 8
5 6
5 9
6 7
6 10
7 11
8 9
9 10
10 11

output:
0 4
1 5
2 3
6 10
7 11
8 9

 */
