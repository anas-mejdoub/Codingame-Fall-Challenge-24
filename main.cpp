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
    float price;
    pod podInc;
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

set<pair<int, int>> createdTubes;

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
            }
        }
    }
    builds[index].matched = true;
    return id;
}

bool checkTubCreated(int src, int dest)
{
    if (createdTubes.find({src, dest}) != createdTubes.end())
    {
        // Tube already created, return false to avoid duplication
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
int main()
{
    vector<string> arr;
    vector<buildings> builds;
    landing_pad lpad;
    vector <landing_pad> landPods;
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
    vector <tub> tubArr;
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
                        // cerr <<  "new builds src " << tubArr[j].source << " dest " << tubArr[j].destination << " created " <<tubArr[j].created << std::endl;
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
        if (test == 1)
        {
        for (int t1 = 0; t1 < landPods.size(); t1++)
        {
            if (landPods[t1].tubCreated == false)
            {
                timesCreated = 0;
            for(int e2 = 0; e2 < builds.size(); e2++)
            {
                    int destination = tubMatching(landPods[t1], builds);
                    if (timesCreated > 4)
                        break;
                    if (checkTubCreated(landPods[t1].id, destination))
                    {
                        tub t;
                        t.source = landPods[t1].id;
                        t.destination = destination;
                        // cerr << "src " << t.source << " " << destination << std::endl;
                        t.created = false;
                        t.price = distance(landPods[t1].x, landPods[t1].y, builds[e2].x, builds[e2].y) * 10;
                        t.podInc.created = false;
                        landPods[t1].tubs.push_back(t);
                        createdTubes.insert({t.source, t.destination}); // Track the created tube
                        timesCreated++;
                    }

            }
            }
            landPods[t1].tubCreated = true;
        }

        }
        // tubArr.clear();

        for (int o = 0; o < landPods.size(); o++)
        {

            landPods[o].tubs = getUniqueTubs(landPods[o].tubs);
            for (int k = 0; k < landPods[o].tubs.size(); k++)
            {
                cerr << landPods[o].tubs[k].source << " " << landPods[o].tubs[k].destination << std::endl;
            }
        }

    
        bool a = false; 
        for (int j = 0; j < landPods.size(); j++)
        {
            int numsAstr = landPods[j].nums_atsr;
            for (int x = 0; x < landPods[j].tubs.size(); x++)
            {
            cerr << "res " << resources << " price "  << landPods[j].tubs[x].price<< endl;
                    if (resources >= landPods[j].tubs[x].price && !landPods[j].tubs[x].created)
                    {
                        action = true;
                        cout << "TUBE " << landPods[j].tubs[x].source << " " << landPods[j].tubs[x].destination << ";";
                        resources -= landPods[j].tubs[x].price;
                        landPods[j].tubs[x].created = true;
                    }
                    if (resources - 1000 >= 0 && landPods[j].tubs[x].podInc.created == false && landPods[j].tubs[x].created == true)
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
                    a = true;
            }
        }
                if (!action)
                    cout << " WAIT ";
                cout << std::endl;
        landPodSize = landPods.size();
    }
}