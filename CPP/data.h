#pragma

#include <array>
#include <vector>

using namespace std;

constexpr uint32_t ENERGY_TYPE_NUM = 5;
constexpr uint32_t ENGINE_TYPE_NUM = 3;
constexpr uint32_t MAX_U32 = 0xFFFFFFFF;

struct Energy {
    uint32_t processTime;
    void Read()
    {
        cin >> processTime;
    }
};

struct Region {
    uint32_t workshopIndex;
    uint32_t energyType;
    void Read()
    {
        cin >> workshopIndex;
        cin >> energyType;
    }
};

struct Window {
    bool canSelfLoop;
    uint32_t workshopIndex;
    uint32_t costFactor;
    array<bool, ENGINE_TYPE_NUM> supportOfEngines;
    void Read()
    {
        cin >> canSelfLoop;
        cin >> workshopIndex;
        cin >> costFactor;
        for (uint32_t i = 0; i < ENGINE_TYPE_NUM; i++) {
            cin >> supportOfEngines[i];
        }
    }
};

struct Device {
    uint32_t engineType;
    array<uint32_t, ENERGY_TYPE_NUM> costOfEnergys;
    void Read()
    {
        cin >> engineType;
        for (uint32_t i = 0; i < ENERGY_TYPE_NUM; i++) {
            cin >> costOfEnergys[i];
        }
    }
};

struct Edge {
    uint32_t type;
    uint32_t sendDid;
    uint32_t recvDid;
    void Read()
    {
        cin >> type;
        cin >> sendDid;
        cin >> recvDid;
    }
};

struct Pipeline{
    uint32_t edgeNum;
    vector<uint32_t> edgeIndexs;
    void Read()
    {
        cin >> edgeNum;
        edgeIndexs.resize(edgeNum);
        for (uint32_t i = 0; i < edgeNum; i++) {
            cin >> edgeIndexs[i];
        }
    }
};

struct InputData {
    uint32_t K;
    array<Energy, ENERGY_TYPE_NUM> energys;
    uint32_t N;
    uint32_t R;
    vector<Region> regions;
    uint32_t L;
    uint32_t M;
    uint32_t W;
    vector<Window> windows;
    uint32_t D;
    vector<Device> devices;
    uint32_t E;
    vector<Edge> edges;
    Pipeline pipeline;

    void Read()
    {
        cin >> K;
        for (Energy& energy : energys) {
            energy.Read();
        }
        cin >> N;
        cin >> R;
        regions.resize(R);
        for (Region& region : regions) {
            region.Read();
        }
        cin >> L;
        cin >> M;
        cin >> W;
        windows.resize(W);
        for (Window& window : windows) {
            window.Read();
        }
        cin >> D;
        devices.resize(D);
        for (Device& device : devices) {
            device.Read();
        }
        cin >> E;
        edges.resize(E);
        for (Edge& edge : edges) {
            edge.Read();
        }
        pipeline.Read();
    }
};