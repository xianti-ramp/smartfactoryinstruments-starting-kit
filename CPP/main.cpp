#include <iostream>
#include <algorithm>
#include <queue>
#include "data.h"

using namespace std;

inline auto Min(const auto& a, const auto& b)
{
    return (a < b) ? a : b;
}
inline auto Max(const auto& a, const auto& b)
{
    return (a > b) ? a : b;
}
inline void UpdateLower(auto& a, const auto& b)
{
    a = Min(a, b);
}
inline void UpdateUpper(auto& a, const auto& b)
{
    a = Max(a, b);
}

void PrintVec(const vector<uint32_t>& vec)
{
    for (const auto& v : vec) {
        cout << v << " ";
    }
    cout << "\n";
}

struct Workshop {
    uint32_t minTi {MAX_U32};
    uint32_t maxTi {0};
    array<uint32_t, ENGINE_TYPE_NUM> anyRidOfEngines{MAX_U32, MAX_U32, MAX_U32};
    bool operator<(const Workshop& rhs) const
    {
        return minTi < rhs.minTi;
    }
};

int main()
{
    InputData inputData;
    inputData.Read();
    vector<Workshop> workshops(inputData.W);
    for (uint32_t wid = 0; wid < inputData.W; wid++) {
        const Window& window = inputData.windows[wid];
        Workshop& workshop = workshops[window.workshopIndex];
        UpdateLower(workshop.minTi, wid);
        UpdateUpper(workshop.maxTi, inputData.L * inputData.M + wid);
    }

    vector<uint32_t> widOfTi;
    for (uint32_t i = 0; i < (inputData.L + 1); i++) {
        for (uint32_t wid = 0; wid < inputData.M; wid++) {
            widOfTi.emplace_back(wid);
        }
    }
    for (uint32_t wid = inputData.M; wid < inputData.W; wid++) {
        const Window& window = inputData.windows[wid];
        Workshop& workshop = workshops[window.workshopIndex];
        widOfTi.emplace_back(wid);
        UpdateLower(workshop.minTi, widOfTi.size() - 1);
        if (window.canSelfLoop) {
            for (uint32_t i = 0;  i < inputData.L; i++) {
                widOfTi.emplace_back(wid);
            }
        }
        UpdateUpper(workshop.maxTi, widOfTi.size() - 1);
    }
    sort(workshops.begin(), workshops.end());

    vector<bool> isDevicesInPipeline(inputData.D, false);
    for (uint32_t eid : inputData.pipeline.edgeIndexs) {
        const Edge& edge = inputData.edges[eid];
        isDevicesInPipeline[edge.sendDid] = true;
        isDevicesInPipeline[edge.recvDid] = true;
    }

    // Collect statistics on the workshop region that support a certain
    // type of equipment in the workshop.
    for (uint32_t rid = 0; rid < inputData.R; rid++) {
        const Region& region = inputData.regions[rid];
        auto& anyRidOfEngines = workshops[region.workshopIndex].anyRidOfEngines;
        switch (region.energyType) {
            case 0:
                anyRidOfEngines[0] = rid;
                anyRidOfEngines[1] = rid;
                break;
            case 1:
                anyRidOfEngines[0] = rid;
                break;
            case 2:
                anyRidOfEngines[1] = rid;
                break;
            case 3:
                anyRidOfEngines[2] = rid;
                break;
            case 4:
                anyRidOfEngines[2] = rid;
                break;
        }
    }

    vector<vector<uint32_t>> nextEdgeMgr(inputData.D);
    vector<vector<uint32_t>> prevEdgeMgr(inputData.D);
    for (uint32_t eid = 0; eid < inputData.E; eid++) {
        const Edge& edge = inputData.edges[eid];
        nextEdgeMgr[edge.sendDid].emplace_back(eid);
        prevEdgeMgr[edge.recvDid].emplace_back(eid);
    }
    queue<uint32_t> didQueue;
    vector<uint32_t> inCnt(inputData.D, 0);
    for (uint32_t did = 0; did < inputData.D; did++) {
        inCnt[did] = prevEdgeMgr[did].size();
        if (inCnt[did] == 0) {
            didQueue.push(did);
        }
    }
    
    // Record the workshop region where each device is installed.
    vector<uint32_t> ridOfdid(inputData.D, MAX_U32);
    vector<uint32_t> minTiOfdid(inputData.D, 0);
    vector<uint32_t> widOfPid(inputData.pipeline.edgeNum + 1, MAX_U32);

    uint32_t pid {0};
    int32_t preTi {0};
    while (!didQueue.empty()) {
        uint32_t curDid = didQueue.front();
        didQueue.pop();
        if (isDevicesInPipeline[curDid]) {
            uint32_t startTi = minTiOfdid[curDid];
            uint32_t engineType = inputData.devices[curDid].engineType;
            if (pid != 0) {
                const Edge& edge = inputData.edges[inputData.pipeline.edgeIndexs[pid - 1]];
                UpdateUpper(startTi, preTi + (edge.type == 0));
            }

            for (uint32_t ti = startTi; ti < widOfTi.size(); ti++) {
                uint32_t wid = widOfTi[ti];
                const Window& window = inputData.windows[wid];
                if (!window.supportOfEngines[engineType]) {
                    continue;
                }
                const Workshop& workshop = workshops[window.workshopIndex];
                uint32_t rid = workshop.anyRidOfEngines[engineType];
                if (rid == MAX_U32) {
                    continue;
                }
                ridOfdid[curDid] = rid;
                widOfPid[pid] = wid;
                pid = pid + 1;
                preTi = ti;
                break;
            }
        } else {
            uint32_t engineType = inputData.devices[curDid].engineType;
            for (uint32_t i = 0; i < inputData.N; i++) {
                const Workshop& workshop = workshops[i];
                uint32_t rid = workshop.anyRidOfEngines[engineType];
                if (rid == MAX_U32) {
                    continue;
                }
                if (workshop.maxTi > minTiOfdid[curDid]) {
                    ridOfdid[curDid] = rid;
                    break;
                }
            }
        }
        if (ridOfdid[curDid] == MAX_U32) {
            cout << "wrong in " << curDid << "\n";
            return 0;
        }
        const Workshop& workshop = workshops[inputData.regions[ridOfdid[curDid]].workshopIndex];
        for (uint32_t eid : nextEdgeMgr[curDid]) {
            const Edge& edge = inputData.edges[eid];
            uint32_t curDid = edge.recvDid;
            UpdateUpper(minTiOfdid[curDid], workshop.minTi + (edge.type == 0));
            inCnt[curDid]--;
            if (inCnt[curDid] == 0) {
                didQueue.push(curDid);
            }
        }
    }

    cout << inputData.D << "\n";
    PrintVec(ridOfdid);

    cout << (inputData.pipeline.edgeNum + 1) << "\n";
    PrintVec(widOfPid);

    return 0;
}