#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#define MAX_VERTEXS 17
#define INF 1e+038
class Graph
{
public:
    int cur_pos,desti;
    enum Opt{Fastest=1,Shortest,Check_key_paths} option;
    bool rush;
    float cost[4][MAX_VERTEXS+1][MAX_VERTEXS+1];
    bool SetCost();
    void UserOption();
    void findFastestPath();
    void findShortestPath();
    bool findKeyPath();
    bool ShortestPath(int*);
private:
    enum Means{walk=1,bicycle,car}means;
    enum Spot{dormitory_1=1,dormitory_2,dormitory_3,dormitory_4,dormitory_5,library,dining_hall_1,
        dining_hall_2,teaching_building,main_building,supermarket_1,supermarket_2,
        supermarket_3,student_activity_center,lecture_hall_1,lecture_hall_2,playground}spot;
    bool IsRush(int, int);
    char* enumstr(int);
};
#endif // MAP_H_INCLUDED

/*************************************
 *Name:Changmao Cheng                *
 *Student ID: 2012030040006          *
 *Class ID: 2012001020               *
 *School: Yingcai Experimental School*
 *Date: 2014/6/21                    *
 *************************************/
