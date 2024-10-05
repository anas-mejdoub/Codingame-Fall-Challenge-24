#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <unistd.h>
using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
typedef struct tub
{
    int id;
    int source;
    int destination;
    int capacity;
    bool created;
}tub;
typedef struct landing
{
    int id;
    int x;
    int y;
    int nums_atsr;
    bool tubCreated;
    vector <tub>tubs;
    vector <int>astronuts;
} landing_pad;

typedef struct buildings
{
    int type;
    int x;
    int y;
    int id;
    bool matched;
} buildings;


typedef struct pod
{
    int id;
    int type;
    bool created;
    bool path_defined;

}pod;

std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
int distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow((x2-x1), 2) + pow(y2 - y1, 2));
}
int checkTypeAst(landing_pad &landPods, int type)
{
    int i = 0;
    int len = 0;
    for (i = 0; i < landPods.astronuts.size(); i++)
    {
        if (len == 10)
            break;
        if ( landPods.astronuts[i] == type)
        {
            landPods.astronuts[i] = -1;
            len++;
        }

    }
    if (len > 10)
        len = 10;
    return len;
}

int tubMatching(landing_pad landPod, vector<buildings> &builds)
{
    int max = 0;
    int len = 0;
    int id = 0;
    int index = 0;
    for (int j = 0; j < builds.size(); j++)
    {
        if (builds[j].matched == false)
        {

        for (int i = 0; i < landPod.astronuts.size(); i++)
        {
            if ( landPod.astronuts[i] == builds[j].type)
            {
                len++;
            }
        }
        if (len > max)
        {
            max = len;
            id = builds[j].id;
            index = j;
            // builds[j].type = -1;
        }
        }
    }
    builds[index].matched = true;
    // cerr << id << std::endl;
    return id;
}

bool checkTubCreated(vector <tub> tubArr, int src, int dest)
{
    for (int i = 0; i < tubArr.size(); i++)
    {
        if (tubArr[i].source == src && tubArr[i].destination == dest)
            return false;
    }
    return true;
}
int main()
{
    vector<string> arr;
    vector<buildings> builds;
    landing_pad lpad;
    vector <landing_pad> landPods;
    vector <tub> tubArr;
    int landPodSize = 0;
    vector <pod> pods;
    // game loop
            int building_id_1;
            int building_id_2;
            int capacity;
            string pod_properties;
            string building_properties;
    int test = 1;
    while (1) {
        int resources;
        cin >> resources; cin.ignore();
        int num_travel_routes;
        cin >> num_travel_routes; cin.ignore();
        for (int i = 0; i < num_travel_routes; i++) {
            cin >> building_id_1 >> building_id_2 >> capacity; cin.ignore();
            tub t;
            t.source = building_id_1;
            t.destination = building_id_2;
            t.capacity = capacity;
            tubArr.push_back(t);
        }
        int num_pods;
        cin >> num_pods; cin.ignore();
        for (int i = 0; i < num_pods; i++) {
            getline(cin, pod_properties);
        }
        int num_new_buildings;
        cin >> num_new_buildings; cin.ignore();
        for (int i = 0; i < num_new_buildings; i++) {
            getline(cin, building_properties);
            arr.push_back(building_properties);

        }
        // land
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i][0] == '0')
            {
                // cerr << "YES " << arr[i] << std::endl;
                
                vector<string>pad = split(arr[i], ' ');
                lpad.id = std::stoi(pad[1]);
                lpad.x = std::stoi(pad[2]);
                lpad.y = std::stoi(pad[3]);
                lpad.nums_atsr = std::stoi(pad[4]);
                for (int j = 5; j < std::stoi(pad[4]); j++)
                {
                    lpad.astronuts.push_back(std::stoi(pad[j]));
                }
                lpad.tubCreated = false;
                landPods.push_back(lpad);
                // cerr << "size " << landPods.size() << std::endl;
            }
            else
            {
                vector<string>pad = split(arr[i], ' ');
                buildings b;
                if (pad.size() == 4)
                {
                    b.type = std::stoi(pad[0]);
                    b.id = std::stoi(pad[1]);
                    b.x = std::stoi(pad[2]);
                    b.y = std::stoi(pad[3]);
                    b.matched = false;
                    builds.push_back(b);
                }
            }
        }
        if (tubArr.size() != 0)
        {
            for (int i = 0; i < landPods.size(); i++)
            {
                for (int j = 0; j < tubArr.size(); j++)
                {
                    if (tubArr[j].source == landPods[i].id)
                    {
                        landPods[i].tubs.push_back(tubArr[i]);
                    }
                }
            }
        }
        for (int tt = 0; tt < builds.size(); tt++)
        {
            pod d;
            d.id = tt;
            d.created = false;
            d.path_defined = false;
            pods.push_back(d);
        }
        int tubsN = 0;
        bool action = false;
        if (test == 1)
        {
        for (int t1 = 0; t1 < landPods.size(); t1++)
        {
            // if (landPodSize == landPods.size())
            //     break;
            if (landPods[t1].tubCreated == false)
            {

            for(int e2 = 0; e2 < builds.size(); e2++)
            {

                if (resources > 0)
                {
                    action = true;
                    int destination = tubMatching(landPods[t1], builds); 
                    if (checkTubCreated(tubArr, landPods[t1].id, destination))
                    {
                        
                    cout << "TUBE " << landPods[t1].id << " " << destination << ";";
                    tub t;
                    t.source = landPods[t1].id;
                    t.destination = destination;
                    tubArr.push_back(t);
                    landPods[t1].tubs.push_back(t);
                    // cerr << "distance " << distance(landPods[t1].x, landPods[t1].y, builds[e2].x, builds[e2].y) * 10 << std::endl;
                    resources -= distance(landPods[t1].x, landPods[t1].y, builds[e2].x, builds[e2].y) * 10;
                    }
                }
            }
            }
            landPods[t1].tubCreated = true;
        }

        }
        // for (int i = 0; i < tubArr.size(); i++)
        // {
        //     cerr << tubArr.
        // }

        bool a = false; 
        for (int j = 0; j < landPods.size(); j++)
        {

            int numsAstr = landPods[j].nums_atsr;
            for (int x = 0; x < landPods[j].tubs.size(); x++)
            {
                if (a &&  resources >= 1000)
                {
                    a = false;
                    cout << ";";
                }
                if (resources >= 1000 && !pods[x].created)
                {
                    action = true;
                    cout << " POD ";
                    cout << pods[x].id  << " " << landPods[j].tubs[x].source << " " << landPods[j].tubs[x].destination << " " << landPods[j].tubs[x].source;
                resources -= 1000;
                numsAstr -= 10;
                pods[x].created = true;
                }
                
                // while (true && !pods[x].path_defined && pods[x].created)
                // {
                //     int len = checkTypeAst(landPods[j], builds[x].type);
                //     if (len == 0)
                //         break;
                //     if (len && numsAstr)
                //     {

                //         action = true;
                //         cout << " " << landPods[j].id << " " << builds[x].id;
                //     }
                //     numsAstr -= len;
                // }
                //     pods[x].path_defined = true;
                a = true;
            }
            // for (int x = 0; x < builds.size(); x++)
            // {
            //     if (a &&  resources >= 1000)
            //     {
            //         a = false;
            //         cout << ";";
            //     }
            //     if (resources >= 1000 && !pods[x].created)
            //     {
            //         action = true;
            //         cout << " POD ";
            //         cout << pods[x].id  << " " << landPods[j].id << " " << builds[x].id << " " << landPods[j].id;
            //     resources -= 1000;
            //     numsAstr -= 10;
            //     pods[x].created = true;
            //     }
                
            //     while (true && !pods[x].path_defined && pods[x].created)
            //     {
            //         int len = checkTypeAst(landPods[j], builds[x].type);
            //         if (len == 0)
            //             break;
            //         if (len && numsAstr)
            //         {

            //             action = true;
            //             cout << " " << landPods[j].id << " " << builds[x].id;
            //         }
            //         numsAstr -= len;
            //     }
            //         pods[x].path_defined = true;
            //     a = true;
            // }

        }
                if (!action)
                    cout << " WAIT ";
                cout << std::endl;
        test++;
        landPodSize = landPods.size();
    }
}