/*
Project Name: Page Replacement Algorithms Simulator
Author:       Mariam Ashraf
Major:        Computer Engineering
Date:         May 2025
Description:  A C++ command-line tool that simulates Virtual Memory Page Replacement Algorithms 
              (FIFO, Optimal, LRU, LFU, MRU, and MFU). It benchmarks page faults, tracks page hits, 
              and visualizes frame allocations using advanced STL containers.
 */
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <queue>

using namespace std;

struct StepInfo {
    int page;
    vector<int> frames;
    bool fault;
    int replaced;
};

vector<StepInfo> steps;

void printAllSteps(const vector<StepInfo>& steps, int frame_count) {
    cout << "\nReference String:\n";
    cout << "Page:  ";
    for (const auto& step : steps) {
        cout << setw(3) << step.page << " ";
    }
    cout << "\n";

    for (int i = 0; i < frame_count; ++i) {
        cout << "Frame " << i+1 << ":";
        for (const auto& step : steps) {
            if (i < step.frames.size() && step.frames[i] != -1) {
                if (step.fault && step.frames[i] == step.page) {
                    cout << " [" << setw(2) << step.frames[i] << "]"; 
                } else if (step.replaced != -1 && step.frames[i] == step.replaced) {
                    cout << " (" << setw(2) << step.frames[i] << ")"; 
                } else {
                    cout << "  " << setw(2) << step.frames[i] << " ";
                }
            } else {
                cout << "  " << setw(2) << "-" << " ";
            }
        }
        cout << "\n";
    }

    cout << "\n";
}

int FIFO(const vector<int>& ref_string, int frame_count) {
    unordered_set<int> s;
    queue<int> indexes;
    int page_faults = 0;
    steps.clear();
    
    vector<int> current_frames(frame_count, -1); 
    
    for (int i = 0; i < ref_string.size(); i++) {
        StepInfo step;
        step.page = ref_string[i];
        step.fault = false;
        step.replaced = -1;
        
        if (s.find(ref_string[i]) != s.end()) {
            // Page hit - no change
            step.frames = current_frames;
            steps.push_back(step);
            continue;
        }
        
        // Page fault
        page_faults++;
        step.fault = true;
        
        if (s.size() < frame_count) {
            s.insert(ref_string[i]);
            indexes.push(ref_string[i]);
            current_frames[s.size()-1] = ref_string[i]; 
        } else {
            int val = indexes.front();
            indexes.pop();
            s.erase(val);
            s.insert(ref_string[i]);
            indexes.push(ref_string[i]);
            
            
            auto it = find(current_frames.begin(), current_frames.end(), val);
            if (it != current_frames.end()) {
                *it = ref_string[i];
                step.replaced = val;
            }
        }
        
        step.frames = current_frames;
        steps.push_back(step);
    }
    
    cout << "\nFIFO Algorithm:";
    printAllSteps(steps, frame_count);
    return page_faults;
}

int Optimal(const vector<int>& ref_string, int frame_count) {
    vector<int> current_frames(frame_count, -1);
    int page_faults = 0;
    steps.clear();
    
    for (int i = 0; i < ref_string.size(); i++) {
        StepInfo step;
        step.page = ref_string[i];
        step.fault = false;
        step.replaced = -1;
        
        auto it = find(current_frames.begin(), current_frames.end(), ref_string[i]);
        if (it != current_frames.end()) {
            step.frames = current_frames;
            steps.push_back(step);
            continue;
        }
        
        page_faults++;
        step.fault = true;
        
        auto empty_it = find(current_frames.begin(), current_frames.end(), -1);
        if (empty_it != current_frames.end()) {
            *empty_it = ref_string[i];
        } else {

            int farthest = i, replace_idx = 0;
            for (int j = 0; j < frame_count; j++) {
                int k;
                for (k = i + 1; k < ref_string.size(); k++) {
                    if (current_frames[j] == ref_string[k]) {
                        if (k > farthest) {
                            farthest = k;
                            replace_idx = j;
                        }
                        break;
                    }
                }
                if (k == ref_string.size()) {
                    replace_idx = j;
                    break;
                }
            }
            step.replaced = current_frames[replace_idx];
            current_frames[replace_idx] = ref_string[i];
        }
        
        step.frames = current_frames;
        steps.push_back(step);
    }
    
    cout << "\nOptimal Algorithm:";
    printAllSteps(steps, frame_count);
    return page_faults;
}

int LRU(const vector<int>& ref_string, int frame_count) {
    vector<int> current_frames(frame_count, -1);
    unordered_map<int, int> last_used;
    int page_faults = 0;
    steps.clear();
    
    for (int i = 0; i < ref_string.size(); i++) {
        StepInfo step;
        step.page = ref_string[i];
        step.fault = false;
        step.replaced = -1;
        
        auto it = find(current_frames.begin(), current_frames.end(), ref_string[i]);
        if (it != current_frames.end()) {
            last_used[ref_string[i]] = i;
            step.frames = current_frames;
            steps.push_back(step);
            continue;
        }
    
        page_faults++;
        step.fault = true;
        
        auto empty_it = find(current_frames.begin(), current_frames.end(), -1);
        if (empty_it != current_frames.end()) {
            *empty_it = ref_string[i];
        } else {
            int lru = INT_MAX, replace_idx = 0;
            for (int j = 0; j < frame_count; j++) {
                if (last_used[current_frames[j]] < lru) {
                    lru = last_used[current_frames[j]];
                    replace_idx = j;
                }
            }
            step.replaced = current_frames[replace_idx];
            current_frames[replace_idx] = ref_string[i];
        }
        
        last_used[ref_string[i]] = i;
        step.frames = current_frames;
        steps.push_back(step);
    }
    
    cout << "\nLRU Algorithm:";
    printAllSteps(steps, frame_count);
    return page_faults;
}

int LFU(const vector<int>& ref_string, int frame_count) {
    vector<int> current_frames(frame_count, -1);
    unordered_map<int, int> freq;
    unordered_map<int, int> last_used;
    int page_faults = 0;
    steps.clear();
    
    for (int i = 0; i < ref_string.size(); i++) {
        StepInfo step;
        step.page = ref_string[i];
        step.fault = false;
        step.replaced = -1;
        
        auto it = find(current_frames.begin(), current_frames.end(), ref_string[i]);
        if (it != current_frames.end()) {
            freq[ref_string[i]]++;
            last_used[ref_string[i]] = i;
            step.frames = current_frames;
            steps.push_back(step);
            continue;
        }
        
        page_faults++;
        step.fault = true;
        
        auto empty_it = find(current_frames.begin(), current_frames.end(), -1);
        if (empty_it != current_frames.end()) {
            *empty_it = ref_string[i];
            freq[ref_string[i]] = 1;
            last_used[ref_string[i]] = i;
        } else {
            int lfu = current_frames[0];
            for (int j = 1; j < frame_count; j++) {
                if (freq[current_frames[j]] < freq[lfu] || 
                   (freq[current_frames[j]] == freq[lfu] && last_used[current_frames[j]] < last_used[lfu])) {
                    lfu = current_frames[j];
                }
            }
            step.replaced = lfu;
            freq.erase(lfu);
            *find(current_frames.begin(), current_frames.end(), lfu) = ref_string[i];
            freq[ref_string[i]] = 1;
        }
        
        last_used[ref_string[i]] = i;
        step.frames = current_frames;
        steps.push_back(step);
    }
    
    cout << "\nLFU Algorithm:";
    printAllSteps(steps, frame_count);
    return page_faults;
}

int MRU(const vector<int>& ref_string, int frame_count) {
    vector<int> current_frames(frame_count, -1);
    unordered_map<int, int> last_used;
    int page_faults = 0;
    steps.clear();
    
    for (int i = 0; i < ref_string.size(); i++) {
        StepInfo step;
        step.page = ref_string[i];
        step.fault = false;
        step.replaced = -1;
        
        auto it = find(current_frames.begin(), current_frames.end(), ref_string[i]);
        if (it != current_frames.end()) {
            last_used[ref_string[i]] = i;
            step.frames = current_frames;
            steps.push_back(step);
            continue;
        }
        
        page_faults++;
        step.fault = true;
        
        auto empty_it = find(current_frames.begin(), current_frames.end(), -1);
        if (empty_it != current_frames.end()) {
            *empty_it = ref_string[i];
        } else {
            int mru = -1, replace_idx = 0;
            for (int j = 0; j < frame_count; j++) {
                if (last_used[current_frames[j]] > mru) {
                    mru = last_used[current_frames[j]];
                    replace_idx = j;
                }
            }
            step.replaced = current_frames[replace_idx];
            current_frames[replace_idx] = ref_string[i];
        }
        
        last_used[ref_string[i]] = i;
        step.frames = current_frames;
        steps.push_back(step);
    }
    
    cout << "\nMRU Algorithm:";
    printAllSteps(steps, frame_count);
    return page_faults;
}

int MFU(const vector<int>& ref_string, int frame_count) {
    vector<int> current_frames(frame_count, -1);
    unordered_map<int, int> freq;
    int page_faults = 0;
    steps.clear();
    
    for (int i = 0; i < ref_string.size(); i++) {
        StepInfo step;
        step.page = ref_string[i];
        step.fault = false;
        step.replaced = -1;
        
        auto it = find(current_frames.begin(), current_frames.end(), ref_string[i]);
        if (it != current_frames.end()) {
            freq[ref_string[i]]++;
            step.frames = current_frames;
            steps.push_back(step);
            continue;
        }
        
        page_faults++;
        step.fault = true;
        
        auto empty_it = find(current_frames.begin(), current_frames.end(), -1);
        if (empty_it != current_frames.end()) {
            *empty_it = ref_string[i];
            freq[ref_string[i]] = 1;
        } else {
            int mfu = current_frames[0];
            for (int j = 1; j < frame_count; j++) {
                if (freq[current_frames[j]] > freq[mfu]) {
                    mfu = current_frames[j];
                }
            }
            step.replaced = mfu;
            freq.erase(mfu);
            *find(current_frames.begin(), current_frames.end(), mfu) = ref_string[i];
            freq[ref_string[i]] = 1;
        }
        
        step.frames = current_frames;
        steps.push_back(step);
    }
    
    cout << "\nMFU Algorithm:";
    printAllSteps(steps, frame_count);
    return page_faults;
}

void displayMenu() {
    cout << "\n\nPage Replacement Algorithm Simulator\n";
    cout << "================================\n";
    cout << "1. FIFO (First-In First-Out)\n";
    cout << "2. Optimal\n";
    cout << "3. LRU (Least Recently Used)\n";
    cout << "4. LFU (Least Frequently Used)\n";
    cout << "5. MRU (Most Recently Used)\n";
    cout << "6. MFU (Most Frequently Used)\n";
    cout << "7. Run All Algorithms\n";
    cout << "8. Exit\n";
    cout << "Enter your choice (1-8): ";
}

int main() {
    vector<int> ref_string;
    int frame_count, n, value;
    
    cout << "Enter the number of pages in reference string: ";
    cin >> n;
    cout << "Enter the reference string (space separated): ";
    for (int i = 0; i < n; i++) {
        cin >> value;
        ref_string.push_back(value);
    }
    
    cout << "Enter number of frames: ";
    cin >> frame_count;
    
    unordered_map<string, int> results;
    bool running = true;
    
    while (running) {
        displayMenu();
        int choice;
        cin >> choice;
        
        int faults = 0;
        switch(choice) {
            case 1:
                faults = FIFO(ref_string, frame_count);
                results["FIFO"] = faults;
                cout << "\nTotal Page Faults: " << faults << endl;
                break;
            case 2:
                faults = Optimal(ref_string, frame_count);
                results["Optimal"] = faults;
                cout << "\nTotal Page Faults: " << faults << endl;
                break;
            case 3:
                faults = LRU(ref_string, frame_count);
                results["LRU"] = faults;
                cout << "\nTotal Page Faults: " << faults << endl;
                break;
            case 4:
                faults = LFU(ref_string, frame_count);
                results["LFU"] = faults;
                cout << "\nTotal Page Faults: " << faults << endl;
                break;
            case 5:
                faults = MRU(ref_string, frame_count);
                results["MRU"] = faults;
                cout << "\nTotal Page Faults: " << faults << endl;
                break;
            case 6:
                faults = MFU(ref_string, frame_count);
                results["MFU"] = faults;
                cout << "\nTotal Page Faults: " << faults << endl;
                break;
            case 7:
                results["FIFO"] = FIFO(ref_string, frame_count);
                results["Optimal"] = Optimal(ref_string, frame_count);
                results["LRU"] = LRU(ref_string, frame_count);
                results["LFU"] = LFU(ref_string, frame_count);
                results["MRU"] = MRU(ref_string, frame_count);
                results["MFU"] = MFU(ref_string, frame_count);
                break;
            case 8:
                running = false;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        
        if (choice >= 1 && choice <= 7) {
            char continue_choice;
            cout << "\nDo you want to continue? (y/n): ";
            cin >> continue_choice;
            if (tolower(continue_choice) != 'y') {
                running = false;
            }
        }
    }
    
    if (!results.empty()) {
        cout << "\n\nFinal Summary of Page Faults:\n";
        cout << "============================\n";
        for (const auto& result : results) {
            cout << setw(8) << left << result.first << ": " << result.second << " page faults\n";
        }
    }
    
    cout << "\nThank you\n";
    return 0;
}