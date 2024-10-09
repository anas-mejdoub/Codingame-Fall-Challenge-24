#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <unistd.h>
#include <set> // Include this at the top
using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
typedef struct pod
{
    int id;
    int type;
    bool created;
    bool path_defined;

}pod;
int indexPodId = 0;
typedef struct tub
{
    int id;
    int source;
    int destination;
    int capacity;
    bool created;
    double price;
    pod podInc;
    bool teleportCreated;
}tub;
typedef struct Point {
    double x, y;
} Point;
typedef struct landing
{
    int id;
    double x;
    double y;
    int nums_atsr;
    bool tubCreated;
    vector <tub>tubs;
    vector <int>astronuts;
} landing_pad;

typedef struct buildings
{
    int type;
    double x;
    double y;
    int id;
    bool matched;
} buildings;
bool isTeleports = false;
set<pair<int, int>> createdTubes;
set<pair<int, int>> MatchedTubes;
std::set<int> createdSources;
set<pair<int, int>> createdTeleports;
set<pair<int, int>> teleportTocreate;

std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
Point getXYById(int id, vector<buildings> &builds)
{
    Point p;
    for (int i = 0; i < builds.size(); i++)
    {
        if (builds[i].id == id)
        {
            p.x = builds[i].x;
            p.y = builds[i].y;
            return p;
        }
    }
    return p;
}
Point getXYByIdLand(int id, vector<landing_pad> &builds)
{
    Point p;
    for (int i = 0; i < builds.size(); i++)
    {
        if (builds[i].id == id)
        {
            p.x = builds[i].x;
            p.y = builds[i].y;
            return p;
        }
    }
    return p;
}
int sign(int val) {
    if (val > 0) return 1;
    else if (val < 0) return -1;
    return 0;
}
std::vector<landing_pad> removeDuplicates(const std::vector<landing_pad>& landPods) {
    std::set<int> unique_ids;
    std::vector<landing_pad> unique_landPads;

    for (const auto& pad : landPods) {
        if (unique_ids.find(pad.id) == unique_ids.end()) {
            unique_landPads.push_back(pad);  // Keep the pad with a unique id
            unique_ids.insert(pad.id);       // Add id to the set of unique ids
        }
    }
    return unique_landPads;
}
double distance(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}
// Function to determine the orientation of three points
int orientation(Point p1, Point p2, Point p3) {
    double prod = (p3.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (p3.x - p1.x);
    return sign(prod);
}

// Function to check if two segments intersect
bool segmentsIntersect(Point A, Point B, Point C, Point D) {
    return orientation(A, B, C) * orientation(A, B, D) < 0 &&
           orientation(C, D, A) * orientation(C, D, B) < 0;
}

// double distance(const Point& p1, const Point& p2) {
//     return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
// }

// Function to check if a point is on a segment
bool pointOnSegment(const Point& A, const Point& B, const Point& C) { 
    const double epsilon = 0.0000001;

    // Check if C is collinear with A and B
    if (std::fabs(distance(B, A) + distance(A, C) - distance(B, C)) >= epsilon) {
        return false; // Not collinear
    }

    // Check if C is within the bounds of A and B
    return (std::min(A.x, B.x) <= C.x && C.x <= std::max(A.x, B.x) &&
            std::min(A.y, B.y) <= C.y && C.y <= std::max(A.y, B.y));
}
bool checkBuildingInter(Point src, Point dest, vector <buildings> &builds, vector <landing_pad> landPods, int id)
{
    for (int i = 0; i < builds.size(); i++)
    {
        Point buildsXY = Point{builds[i].x, builds[i].y};
        if (pointOnSegment(src, dest, buildsXY))
        {
            return true;
        }
    }
    for (int i = 0; i < landPods.size(); i++)
    {
        if (landPods[i].id != id )
        {
            Point lXY = Point{landPods[i].x, landPods[i].y};
            // Point lXY = getXYByIdLand(landPods[i].id, landPods);
            if (id == 8 && landPods[i].id == 2)
            {
                cerr << lXY.x << lXY.y << endl;
            }
            if (pointOnSegment(src, dest, lXY))
            {
                return true;
            }
        }
    }
    return false;
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
int getClosestMatch(landing_pad landPod, vector<buildings> &builds)
{
    double min = 2147483647;
    int id = -1;
    for (int i = 0; i < builds.size(); i++)
    {
        if (MatchedTubes.find({landPod.id, builds[i].id}) == MatchedTubes.end())
        {
            for (int j = 0; j < landPod.astronuts.size(); j++)
            {
                if ( landPod.astronuts[j] == builds[i].type)
                {
                    if (min > distance(Point{landPod.x, landPod.y}, Point{builds[i].x, builds[i].y}))
                    {
                        // cerr << "min " << min << endl;
                        id = builds[i].id;
                        min = distance(Point{landPod.x, landPod.y}, Point{builds[i].x, builds[i].y});
                        break;
                    }
                }
            }
        }
    }
    return id;
}
int tubMatching(landing_pad landPod, vector<buildings> &builds)
{
    int max = 0;
    int len = 0;
    int id = -1;
    int index = -1;
    for (int j = 0; j < builds.size(); j++)
    {
        if (MatchedTubes.find({landPod.id, builds[j].id}) == MatchedTubes.end())
        {
            for (int i = 0; i < landPod.astronuts.size(); i++)
            {
                if ( landPod.astronuts[i] == builds[j].type)
                {
                if (landPod.id == 3)
                {
                    // cerr << "m here " << builds[j].id << endl;
                }
                    id = builds[j].id;
                    // MatchedTubes.insert({landPod.id, id});
                    // return id;
                    // break;
                    len++;
                }
            }
            if (len > max)
            {
                max = len;
                id = builds[j].id;
                index = j;
            }
        }
    }
    MatchedTubes.insert({landPod.id, id});
    return id;
}

bool checkTubCreated(int src, int dest)
{
    if (createdTubes.find({src, dest}) != createdTubes.end())
    {
        return false;
    }
    return true;
}
std::vector<tub> getUniqueTubs(const std::vector<tub>& tubs) {
    std::set<std::pair<int, int>> seenTubs; // To track unique (source, destination) pairs
    std::vector<tub> uniqueTubs; // To store only unique tubs

    for (const auto& t : tubs) {
        // Skip if source and destination are the same
        if (t.source == t.destination) {
            continue;
        }

        // Create a pair of source and destination
        std::pair<int, int> tubPair = {t.source, t.destination};

        // Check if the tubPair is already seen
        if (seenTubs.find(tubPair) == seenTubs.end()) {
            // If not seen, add it to the uniqueTubs vector and mark it as seen
            uniqueTubs.push_back(t);
            seenTubs.insert(tubPair);
        }
    }

    // Return the vector of unique tubs
    return uniqueTubs;
}

// Point getXYById(int id, vector<buildings> &builds)
// {
//     Point p;
//     for (int i = 0; i < builds.size(); i++)
//     {
//         if (builds[i].id == id)
//         {
//             p.x = builds[i].x;
//             p.y = builds[i].y;
//             return p;
//         }
//     }
//     return p;
// }
// Point getXYByIdLand(int id, vector<landing_pad> &builds)
// {
//     Point p;
//     for (int i = 0; i < builds.size(); i++)
//     {
//         if (builds[i].id == id)
//         {
//             p.x = builds[i].x;
//             p.y = builds[i].y;
//             return p;
//         }
//     }
//     return p;
// }
bool checkIntersection(Point a, Point b, vector<tub> tubArr, vector <landing_pad> landPods, vector<buildings> builds)
{
    for (int j = 0; j < landPods.size(); j++)
    {

        for (int i = 0; i <landPods[j].tubs.size(); i++)
        {
            if (segmentsIntersect(a, b, getXYByIdLand(landPods[j].tubs[i].source, landPods), getXYById(landPods[j].tubs[i].destination, builds)))
            {
                // cerr << "inte trrue" << endl;
                return true;
            }
        }
    }
    return false;
}
int teleporterMatching(landing_pad landPod, vector<buildings> &builds)
{
    int len = 0;
    int max = 0;
    int id = -1;
    for (int i = 0; i < builds.size(); i++)
    {
        len  = 0;
        for (int j = 0; j < landPod.astronuts.size(); j++)
        {
            if (createdSources.find(builds[i].id) != createdSources.end())
            {
                // cerr << "break " << endl;
                break;
            }
            if (landPod.astronuts[j] == builds[i].type)
            {
                len++;
            }
        }
        if (len > max)
        {
            id = builds[i].id;
            max = len;
        }
    }
    return id;
}

int matchingLandpods(landing_pad l, vector <landing_pad> landPods, vector <tub> tubArr, vector<buildings> builds)
{
    int id = 0;
    int len = 0;
    int max = 0;
    for (int i = 0; i < landPods.size(); i++)
    {
        if (landPods[i].tubs.size() > 0 && !checkIntersection(Point{l.x,l.y} ,Point{landPods[i].x,landPods[i].y},tubArr, landPods, builds) && !checkBuildingInter(Point{l.x,l.y}, Point{landPods[i].x,landPods[i].y}, builds, landPods, landPods[i].id))
        {
            // int min = l.astronuts.size() < landPods[i].astronuts.size() ? l.astronuts.size() : landPods[i].astronuts.size();
            // for (int j = 0; j < min; j++)
            // {
            //     if ()
            // }
            return landPods[i].id;
        }
    }
}
int main()
{
    // vector<string> arr;
    vector<buildings> builds;
    // landing_pad lpad;
    vector <landing_pad> landPods;
    int landPodSize = 0;
    vector <pod> pods;
            int building_id_1;
            int building_id_2;
            int capacity;
            string pod_properties;
            string building_properties;
    int test = 1;
    while (1) {
    vector <tub> tubArr;
        vector<string> arr;
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
            t.created = true;
            t.podInc.created = true;
            t.teleportCreated = true;
            tubArr.push_back(t);
        }
        int num_pods;
        cin >> num_pods; cin.ignore();
        for (int i = 0; i < num_pods; i++) {
            getline(cin, pod_properties);
        }
        int num_new_buildings;
        // cerr << "num " << num_new_buildings << endl;
        cin >> num_new_buildings; cin.ignore();
        for (int i = 0; i < num_new_buildings; i++) {
            getline(cin, building_properties);
            arr.push_back(building_properties);
        }
        
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i][0] == '0')
            {     
                landing_pad lpad;           
                vector<string>pad = split(arr[i], ' ');
                lpad.id = std::stoi(pad[1]);
                lpad.x = std::stoi(pad[2]);
                lpad.y = std::stoi(pad[3]);
                lpad.nums_atsr = std::stoi(pad[4]);
                
                for (int j = 5; j < pad.size(); j++)
                {
                    lpad.astronuts.push_back(std::stoi(pad[j]));
                }
                lpad.tubCreated = false;
                landPods.push_back(lpad);
            }
            else
            {
                vector<string>pad = split(arr[i], ' ');
                buildings b;
                if (pad.size() >= 4)
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
        arr.clear();
        if (tubArr.size() != 0)
        {
            for (int i = 0; i < landPods.size(); i++)
            {
                for (int j = 0; j < tubArr.size(); j++)
                {
                    if (tubArr[j].source == landPods[i].id)
                    {
                        landPods[i].tubs.push_back(tubArr[j]);
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
        int timesCreated = 0;
        for (int t1 = 0; t1 < landPods.size(); t1++)
        {
            if (landPods[t1].tubCreated == false)
            {
                timesCreated = 0;
                for(int e2 = 0; e2 < builds.size(); e2++)
                {

                    // if (e2 > 11)
                    //     break;
                    int destination = tubMatching(landPods[t1], builds);
                        if (timesCreated > 4)
                            break;
                        Point pxy = getXYById(destination, builds);
                         
                        if (checkTubCreated(landPods[t1].id, destination) && !checkIntersection(Point{landPods[t1].x,landPods[t1].y} , getXYById(destination, builds) ,tubArr, landPods, builds) && destination != -1 && !checkBuildingInter(Point{landPods[t1].x,landPods[t1].y}, pxy, builds, landPods, landPods[t1].id) && createdTubes.find({landPods[t1].id, destination}) ==createdTubes.end())
                        {
                            tub t;
                            t.source = landPods[t1].id;
                            t.destination = destination;
                            t.created = false;
                            t.price = distance(Point{landPods[t1].x, landPods[t1].y}, pxy) * 10;
                            t.podInc.created = false;
                            t.teleportCreated = false;
                            tubArr.push_back(t);
                            landPods[t1].tubs.push_back(t);
                            createdTubes.insert({t.source, t.destination});
                            timesCreated++;
                landPods[t1].tubCreated = true;
                        }
                }

            }

        }
        for (int o = 0; o < landPods.size(); o++)
        {
            landPods[o].tubs = getUniqueTubs(landPods[o].tubs);
        }
        isTeleports = true;
        for (int y = 0; y < landPods.size(); y++)
        {
             if (landPods[y].tubs.size() == 0)
            {
                    int teleDest = teleporterMatching(landPods[y], builds);
                    if (teleDest != -1 && teleportTocreate.find({landPods[y].id, teleDest}) == teleportTocreate.end() && createdSources.find(landPods[y].id) == createdSources.end())
                    {
                        isTeleports = false;
                        teleportTocreate.insert({landPods[y].id, teleDest});
                    }
            }
        }
        for (int j = 0; j < landPods.size(); j++)
        {
            int numsAstr = landPods[j].nums_atsr;
            for (int x = 0; x < landPods[j].tubs.size(); x++)
            {
                    Point pxy = getXYById(landPods[j].tubs[x].destination, builds);
                    if (resources > landPods[j].tubs[x].price && !landPods[j].tubs[x].created && !checkIntersection(Point{landPods[j].x,landPods[j].y} , getXYById(landPods[j].tubs[x].destination, builds) ,tubArr, landPods, builds))
                    {
                        action = true;
                        cout << "TUBE " << landPods[j].tubs[x].source << " " << landPods[j].tubs[x].destination << ";";
                        resources -= landPods[j].tubs[x].price;
                        landPods[j].tubs[x].created = true;
                        createdTubes.insert({landPods[j].tubs[x].source, landPods[j].tubs[x].destination});
                    }
                    if (resources - 1000 >= 0 && landPods[j].tubs[x].podInc.created == false && landPods[j].tubs[x].created == true && createdTubes.find({landPods[j].tubs[x].source, landPods[j].tubs[x].destination}) != createdTubes.end())
                    {
                        action = true;
                        cout << " POD ";
                        cout << indexPodId  << " " << landPods[j].tubs[x].source << " " << landPods[j].tubs[x].destination << " " << landPods[j].tubs[x].source << ";";
                        resources -= 1000;
                        numsAstr -= 10;
                        pods[x].created = true;
                        landPods[j].tubs[x].podInc.created = true;
                        indexPodId++;
                    }
            }
            
        }
            if (resources - 5000 >= 0)
            {
                for (const auto& p : teleportTocreate)
                {
                    if (resources - 5000 < 0)
                        break;
                    if (resources - 5000 >= 0 && createdSources.find(p.first) == createdSources.end()&& createdSources.find(p.second) == createdSources.end())
                    {
                        cout << "TELEPORT " << p.first << " " << p.second << ";";
                        resources -= 5000;
                        isTeleports = false;
                        createdSources.insert(p.first);
                        createdSources.insert(p.second);
                    }
                        // a++;
                }
            }
                if (!action)
                    cout << " WAIT ";
                cout << std::endl;
        landPodSize = landPods.size();
    }
}