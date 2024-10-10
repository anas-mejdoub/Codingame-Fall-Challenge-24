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
    bool upgraded;
    double sorting;
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
set<pair<int, int>>createdPods;
set<pair<int, int>> createdTeleports;
set<pair<int, int>> teleportTocreate;
set<pair<int, int>> upgradedTubs;
set<pair<int, int>> addedPods;
std::multiset<int> PodMatched;
// std::multiset<int> PodMatched;

set<pair<int, int>> landPodsTypes;

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
int countSubstringOccurrences(const std::string& str, const std::string& sub) {
    if (sub.empty()) return 0;  // Prevent infinite loop if substring is empty

    int count = 0;
    size_t pos = str.find(sub);

    while (pos != std::string::npos) {
        count++;
        // Move the position forward by the length of the substring
        pos = str.find(sub, pos + sub.length());
    }

    return count;
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
bool pointOnSegment(Point& A, Point& B, Point& C) {
    double epsilon = 0.0000001;
    double distAB = distance(B, A);
    double distAC = distance(A, C);
    double distBC = distance(B, C);

    return (std::fabs((distAC + distBC) - distAB) < epsilon);
}
bool checkBuildingInter(Point src, Point dest, vector <buildings> &builds, vector <landing_pad> landPods, int id, int destId)
{
    for (int i = 0; i < builds.size(); i++)
    {
        if (builds[i].id != destId)
        {

        Point buildsXY = Point{builds[i].x, builds[i].y};
        if (pointOnSegment(src, dest, buildsXY))
        {
            return true;
        }
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
                // cerr << lXY.x << lXY.y << endl;
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
int getClosestMatch(landing_pad landPod, vector<buildings> &builds, int type)
{
    double min = 2147483647;
    int id = -1;
    for (int i = 0; i < builds.size(); i++)
    {
        if (builds[i].type == type)
        {
            if (distance(Point{landPod.x, landPod.y}, Point{builds[i].x, builds[i].y}) < min)
            {
                min = distance(Point{landPod.x, landPod.y}, Point{builds[i].x, builds[i].y});
                id = builds[i].id;
            }
        }
        // if (MatchedTubes.find({landPod.id, builds[i].id}) == MatchedTubes.end())
        // {
        //     for (int j = 0; j < landPod.astronuts.size(); j++)
        //     {
        //         if ( landPod.astronuts[j] == builds[i].type)
        //         {
        //             if (min > distance(Point{landPod.x, landPod.y}, Point{builds[i].x, builds[i].y}))
        //             {
        //                 // cerr << "min " << min << endl;
        //                 id = builds[i].id;
        //                 min = distance(Point{landPod.x, landPod.y}, Point{builds[i].x, builds[i].y});
        //                 break;
        //             }
        //         }
        //     }
        // }
    }
    return id;
}
bool checkIntersection(Point a, Point b, vector<tub> tubArr, vector <landing_pad> landPods, vector<buildings> builds)
{
    for (int j = 0; j < landPods.size(); j++)
    {

        for (int i = 0; i <landPods[j].tubs.size(); i++)
        {
            if (segmentsIntersect(a, b, getXYByIdLand(landPods[j].tubs[i].source, landPods), getXYById(landPods[j].tubs[i].destination, builds)))
            {
                return true;
            }
        }
    }
    return false;
}
int tubMatching(landing_pad landPod, vector<buildings> &builds, vector <tub> tubArr, vector <landing_pad> landPods)
{
    double minDistance = std::numeric_limits<double>::max(); // Start with the maximum possible distance
    int closestBuildingId = -1; // Id of the closest building found so far

    // Iterate over all buildings
    for (int j = 0; j < builds.size(); j++)
    {
        // Check if the building has already been matched
        if (MatchedTubes.find({landPod.id, builds[j].id}) == MatchedTubes.end())
        {
            // Iterate over all astronauts in order of preference
            for (int i = 0; i < landPod.astronuts.size(); i++)
            {
                // Check if the building type matches the astronaut type
                if (landPod.astronuts[i] == builds[j].type)
                {
                    // Calculate the distance to the building
                    Point px = Point{builds[j].x, builds[j].y};
                    double currentDistance = distance(Point{landPod.x, landPod.y}, px);

                    // If the distance is smaller than the smallest distance found so far, update the smallest distance and the closest building id
                    if (currentDistance < minDistance) {
                        minDistance = currentDistance;
                        closestBuildingId = builds[j].id;
                    }

                    // Break the loop over the astronauts since we found a match
                    break;
                }
            }
        }
    }

    // If a matching building was found, add it to the matched tubes
    if (closestBuildingId != -1) {
        MatchedTubes.insert({landPod.id, closestBuildingId});
    }

    // Return the id of the closest matching building, or -1 if no matching building was found
    return closestBuildingId;
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
bool compareTubsByPrice(const tub& a, const tub& b) {
    return a.price > b.price;
}

void sortTubsByPrice(std::vector<tub>& tubs) {
    std::sort(tubs.begin(), tubs.end(), compareTubsByPrice);
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
// bool checkIntersection(Point a, Point b, vector<tub> tubArr, vector <landing_pad> landPods, vector<buildings> builds)
// {
//     for (int j = 0; j < landPods.size(); j++)
//     {

//         for (int i = 0; i <landPods[j].tubs.size(); i++)
//         {
//             if (segmentsIntersect(a, b, getXYByIdLand(landPods[j].tubs[i].source, landPods), getXYById(landPods[j].tubs[i].destination, builds)))
//             {
//                 // cerr << "inte trrue" << endl;
//                 return true;
//             }
//         }
//     }
//     return false;
// }
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

void matchingLandpods(landing_pad& l, vector<landing_pad>& landPods, vector<tub>& tubArr, vector<buildings>& builds)
{
    double minDistance = std::numeric_limits<double>::max();
    int closestPadIndex = -1;

    for (int i = 0; i < landPods.size(); ++i)
    {
        for (const auto& astronaut : l.astronuts)
        {
            if (landPodsTypes.find({landPods[i].id, astronaut}) != landPodsTypes.end())
            {
                Point px = Point{landPods[i].x, landPods[i].y};
                double currentDistance = distance(Point{l.x, l.y}, px);
                if (currentDistance < minDistance)
                {
                    minDistance = currentDistance;
                    closestPadIndex = i;
                }
            }
        }
    }

    if (closestPadIndex != -1)
    {
        tub t;
        t.source = l.id;
        t.destination = landPods[closestPadIndex].id;
        t.created = false;
        t.price = minDistance * 10;
        t.podInc.created = false;
        t.teleportCreated = false;
        tubArr.emplace_back(t);
        l.tubs.emplace_back(t);
        createdTubes.insert({t.source, t.destination});
    }
}
int typeBuildById(vector<buildings> builds, int id)
{
    for (int i = 0; i < builds.size(); i++)
    {
        if (builds[i].id == id)
            return builds[i].type;
    }
    return -1;
}

bool checkDuplicatedType(landing_pad &l, int type, double distanceDest, vector<buildings> builds, int destId)
{
    for (int i= 0; i < l.tubs.size(); i++)
    {
        if (l.tubs[i].price <= (distanceDest * 10) && type == typeBuildById(builds, l.tubs[i].destination))
        {
            return false;
        }
    }
    return true;
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
                    int destination = tubMatching(landPods[t1], builds, tubArr, landPods);
                        if (timesCreated > 4)
                            break;
                        int type = typeBuildById(builds, destination);
                        Point pxy = getXYById(destination, builds);
                        if (checkDuplicatedType(landPods[t1], type, distance(Point{landPods[t1].x, landPods[t1].y}, pxy), builds, destination) && !checkBuildingInter(Point{landPods[t1].x, landPods[t1].y}, pxy, builds, landPods, landPods[t1].id, destination) && checkTubCreated(landPods[t1].id, destination) && !checkIntersection(Point{landPods[t1].x,landPods[t1].y} , getXYById(destination, builds) ,tubArr, landPods, builds) && destination != -1  && createdTubes.find({landPods[t1].id, destination}) ==createdTubes.end())
                        {
                            tub t;
                            t.source = landPods[t1].id;
                            t.destination = destination;
                            landPodsTypes.insert({landPods[t1].id, type});
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
            sortTubsByPrice(landPods[o].tubs);
        }
        isTeleports = true;
        for (int y = 0; y < landPods.size(); y++)
        {
             if (landPods[y].tubs.size() < 1)
            {
                    matchingLandpods(landPods[y], landPods, tubArr, builds);
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
            for (int x = 0; x < landPods[j].tubs.size(); x++)
            {
                    Point pxy = getXYById(landPods[j].tubs[x].destination, builds);
                    if (resources >= landPods[j].tubs[x].price && !landPods[j].tubs[x].created )
                    {
                        action = true;
                        cout << "TUBE " << landPods[j].tubs[x].source << " " << landPods[j].tubs[x].destination << ";";
                        resources -= landPods[j].tubs[x].price;
                        landPods[j].tubs[x].created = true;
                        createdTubes.insert({landPods[j].tubs[x].source, landPods[j].tubs[x].destination});
                    }
                    if (resources - 1000 >= 0 && landPods[j].tubs[x].created == true && createdTubes.find({landPods[j].tubs[x].source, landPods[j].tubs[x].destination}) != createdTubes.end() && createdPods.find({landPods[j].tubs[x].source, landPods[j].tubs[x].destination}) == createdPods.end())
                    {
                        action = true;
                        cout << " POD ";
                        cout << indexPodId  << " " << landPods[j].tubs[x].source << " " << landPods[j].tubs[x].destination << " " << landPods[j].tubs[x].source << ";";
                        resources -= 1000;
                        pods[x].created = true;
                        landPods[j].tubs[x].podInc.created = true;
                        string s;
                        createdPods.insert({landPods[j].tubs[x].source, landPods[j].tubs[x].destination});
                        s = to_string(indexPodId);
                        s+= to_string(landPods[j].tubs[x].source);
                        s+= to_string(landPods[j].tubs[x].destination);
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