#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<iomanip>
#include"Graph.h"
using namespace std;
char* Graph::enumstr(int s)
{//字符输出enum枚举量
    switch(s)
    {
       case dormitory_1: return "dormitory_1";break;
       case dormitory_2: return "dormitory_2";break;
       case dormitory_3: return "dormitory_3";break;
       case dormitory_4: return "dormitory_4";break;
       case dormitory_5: return "dormitory_5";break;
       case library:     return "library";break;
       case dining_hall_1: return "dining_hall_1";break;
       case dining_hall_2: return "dining_hall_2";break;
       case teaching_building: return "teaching_building";break;
       case main_building: return "main_building";break;
       case supermarket_1: return "supermarket_1";break;
       case supermarket_2: return "supermarket_2";break;
       case supermarket_3: return "supermarket_3";break;
       case student_activity_center: return "student_activity_center";break;
       case lecture_hall_1: return "lecture_hall_1";break;
       case lecture_hall_2: return "lecture_hall_2";break;
       case playground: return "playground";break;
       default: return "NO_SUCH_PLACE";
    }
}
bool Graph::IsRush(int p1, int p2)
{
    if(rush==false) return false;
    if(p1>p2)//p1,p2要有序
    {
        int temp = p1;
        p1 = p2;
        p2 = temp;
    }
    switch(means)
    {
    case walk:
    case bicycle:
        if((p1==dormitory_2&&p2==dining_hall_1)||(p1==dining_hall_1&&p2==student_activity_center))
            return true;
    case car:
        if((p1==dining_hall_1&&p2==teaching_building)||(p1==dining_hall_2&&p2==teaching_building))
            return true;
        else return false;
    default: return false;
    }
}
bool Graph::SetCost()
{
    FILE* fp;
    if((fp = fopen("avenue.txt","r"))==NULL)
    {
        printf("cannot open \"avenue.txt\"!\n");
        return false;
    }
    for(int i=1; i<=MAX_VERTEXS; i++)
        for(int j=1; j<=MAX_VERTEXS; j++)
        {
            fscanf(fp,"%f",&cost[0][i][j]);
            if(cost[0][i][j]==-1)cost[0][i][j]=INF;//无通路
            cost[car][i][j] = cost[0][i][j];
            if(cost[car][i][j]<INF/2) cost[car][i][j]/=10;//计算正常情况下汽车通过该条道所用时间
        }
    fclose(fp);
    if(means==car)return true;
    if((fp = fopen("walkway.txt","r"))==NULL)
    {
        printf("cannot open \"walkway.txt\"!\n");
        return false;
    }
    for(int i=1; i<=MAX_VERTEXS; i++)
        for(int j=1; j<=MAX_VERTEXS; j++)
           {
            fscanf(fp,"%f",&cost[walk][i][j]);
            if(cost[walk][i][j]==-1)cost[walk][i][j]=INF;//无通路
            if(cost[0][i][j]>cost[walk][i][j])
                cost[bicycle][i][j]=cost[0][i][j]=cost[walk][i][j];
            else cost[bicycle][i][j]=cost[walk][i][j]=cost[0][i][j];
            //由于步行和自行车既能走小路也能走大道，故取短者
            if(cost[bicycle][i][j]<INF/2)cost[bicycle][i][j]/=5;//计算正常情况下自行车通过该条道所用时间
           }
    fclose(fp);
    return true;
}
bool Graph::ShortestPath(int next[MAX_VERTEXS+1])
{
    bool found[MAX_VERTEXS+1];
    int u;
    float dist[MAX_VERTEXS+1],Min;
    for(int i=1;i<=MAX_VERTEXS;i++)
    {
        found[i] = false;
        dist[i] = cost[means][desti][i];//逆向求解最短路径，方便路径的记录和输出
        if(dist[i]< INF/2)
            next[i] = desti;
        //先将与desti相通的点（这些点可能是最短路径上的点）路径的下一点初始化为desti
    }
    dist[desti] = 0;
    found[desti] = true;
    for(int k=1;k<=MAX_VERTEXS;k++)
    {
        Min = INF/2;
        u = -1;
        for(int i=1; i<=MAX_VERTEXS; i++)
		if(dist[i]<Min&&!found[i])
		{
			Min = dist[i];
			u = i;
		}
		if(-1==u)continue;
		found[u] = true;
		if(u==cur_pos)break;
		for(int w=1;w<=MAX_VERTEXS;w++)
            if(!found[w])
                if(dist[u]+cost[means][u][w]<dist[w])
                    {
                        dist[w] = dist[u] + cost[means][u][w];
                        next[w] = u;
                //此处的next为正向路径的下一点(不断更新)，这里筛选的路径为逆向(上一点)
                    }
    }
    if(u!=cur_pos) return false;
    return true;
}
void Graph::findFastestPath()
{
    if(rush==false)//不在高峰期，最快的路径即为最短的路径
    {
        findShortestPath();
        return;
    }
    for(int i=1;i<=MAX_VERTEXS;i++)
      for(int j=i+1;j<=MAX_VERTEXS;j++)
        {
            if(IsRush(i,j)==true)
                 cost[means][j][i]=cost[means][i][j]=2*cost[means][j][i];
                  /*      正常速度 高峰路段(部分路径，在图上标有三角形)
                    步行：1m/s     0.5m/s
                  自行车：5m/s     2.5m/s
                    汽车：10m/s    5m/s  */
        }
    findShortestPath();
    return;
}
void Graph::findShortestPath()
{
    if(cur_pos==desti)
    {
        printf("You are already there!\n");
        return;
    }
    int next[MAX_VERTEXS+1];
    if(ShortestPath(next)==false)
        printf("Sorry, there are no available paths from %s to %s.\n",enumstr(cur_pos),enumstr(desti));
    else
    {//路径存在
        printf("\nPath:\n");
        int n=0,num,b[MAX_VERTEXS+1],flag=0,u=cur_pos;
        float sum=0,duration=0;
        while(u!=desti)
        {
            n++;
            printf("%d.%s--->%.1f m--->%s\n",n,enumstr(u),cost[0][u][next[u]],enumstr(next[u]));
            b[n] = u;
            sum += cost[0][u][next[u]]; //计算总路长
            if(IsRush(u,next[u])==true&&option==Shortest)
                duration += 2*cost[means][u][next[u]];
            else  duration += cost[means][u][next[u]];//求总耗时
            u = next[u];
        }
        printf("The length of the path is about %.1f m.\n",sum);
        printf("It costs you about %.2f minutes.\n",duration/60);
    start:
        printf("Are there some roads blocked?(Y/N):");
        char ch;
        if(scanf("%s",&ch),ch=='N')return;
        if(ch!='Y')goto start;
        printf("Choose the corresponding serial numbers in the path and end with -1:\n");
        while(scanf("%d",&num),num!=-1)
        if(num<=n)
            {
                cost[means][b[num]][next[b[num]]]=cost[means][next[b[num]]][b[num]]=INF;
                flag = 1;
            }
        else printf("Illegal input!Illegal parts ignored.\n");
        if(1==flag)findShortestPath();
    }
    return;
}
bool Graph::findKeyPath()
{
    int next[MAX_VERTEXS+1],a[MAX_VERTEXS+1];
    if(cur_pos==desti)
        return false;
    if(ShortestPath(next)==false)
        return false;
    int t=cur_pos,flag=0;
    float temp;
    printf("The key paths from %s to %s:\n",enumstr(cur_pos),enumstr(desti));
    while(t!=desti)
    {
        temp = cost[means][t][next[t]];
        cost[means][next[t]][t]=cost[means][t][next[t]]=INF;
        if(ShortestPath(a) == false)
            {
                printf("%s--->%.1f m--->%s\n",enumstr(t),cost[0][t][next[t]],enumstr(next[t]));
                flag=1;
            }
        cost[means][next[t]][t] = cost[means][t][next[t]] = temp;
        t = next[t];
    }
    if(flag==1) return true;
    else return false;
}
void Graph::UserOption()
{
    char ch;
    int opt;
    printf("----------------------Welcome to digital campus----------------------");
    printf("\nConfirm your current position and destination\n(input the serial number):\n");
start_1:
    printf("1.dormitory_1\n2.dormitory_2\n3.dormitory_3\n4.dormitory_4\n5.dormitory_5\n6.library\n");
    printf("7.dining_hall_1\n8.dining_hall_2\n9.teaching_building\n10.main_building\n11.supermarket_1\n");
    printf("12.supermarket_2\n13.supermarket_3\n14.student_activity_center\n15.lecture_hall_1\n16.lecture_hall_2\n17.playground\n");
    scanf("%d%d",&cur_pos,&desti);
    if(cur_pos<dormitory_1||cur_pos>playground||desti<dormitory_1||desti>playground)
    {
        printf("Illegal input!\nPlease reconfirm your current position and  destination\n(input the serial number):\n");
        goto start_1;
    }
    printf("\nIs it during the rush hour?(Y/N):");
start_2:
    if(scanf("%s",&ch),ch=='Y')rush = true;
    else if(ch=='N')
        {
            rush = false;
        }
    else
    {
        printf("Illegal input!\nPlease input Y(yes) or N(no):");
        goto start_2;
    }
    printf("\nChoose your means of transportation(input the serial number):\n");
start_3:
    printf("1.walk\n2.bicycle\n3.car\n");
    scanf("%d",&means);
    if(means<walk||means>car)
    {
        printf("Illegal input!\nPlease re-choose your means of transportation(input the serial number):\n");
        goto start_3;
    }
    printf("\nwhich one of the routes do you prefer(input the serial number):\n");
start_4:
    printf("1.Fastest\n2.Shortest\n3.Check_key_paths\n");
    scanf("%d",&option);
    if(SetCost()==false)return;
    if(Fastest==option)
        findFastestPath();
    else if(Shortest==option)
        findShortestPath();
    else if(Check_key_paths==option)
        {
            if(findKeyPath()==false)
                printf("No key paths.\n");
        }
    else
    {
        printf("Illegal input!\nPlease re-enter your preference(input the serial number):\n");
        goto start_4;
    }
    printf("\nSelect your next option:\n");
start_5:
    printf("0.Exit\n1.reconfirm your current position and destination\n2.rush hour reconfirmation(Y/N)\n");
    printf("3.re-choose your means of transportation\n4.reset your preference\n");
    scanf("%d",&opt);
    switch(opt)
    {
    case 0:break;
    case 1:
        goto start_1;
        break;
    case 2:
        goto start_2;
        break;
    case 3:
        goto start_3;
        break;
    case 4:
        goto start_4;
        break;
    default:
        printf("Illegal input!\nSelect your next option again:\n");
        goto start_5;
        break;
    }
    return;
}

/*************************************
 *Name:Changmao Cheng                *
 *Student ID: 2012030040006          *
 *Class ID: 2012001020               *
 *School: Yingcai Experimental School*
 *Date: 2014/6/21                    *
 *************************************/
