# Page Replacement Algorithms Simulator (C++)

A robust systems-programming simulation tool written in C++11 that models how Operating Systems manage **Virtual Memory** and handle **Page Faults**. 

When physical RAM is full, the OS must decide which page to evict to make room for an incoming page. This command-line utility simulates, visualizes, and benchmarks six industry-standard page replacement strategies.

---

## Implemented Algorithms

The simulator provides high-performance implementations for the following algorithms:

1. **FIFO (First-In, First-Out):** Evicts the oldest page in the frame queue. (Implemented using `std::queue` and `std::unordered_set`).
2. **Optimal Algorithm:** Evicts the page that will not be used for the longest period of time in the future. (Provides the theoretical minimum page fault rate).
3. **LRU (Least Recently Used):** Evicts the page that has not been accessed for the longest time. (Tracked efficiently using `std::unordered_map`).
4. **LFU (Least Frequently Used):** Evicts the page with the lowest access frequency/count.
5. **MRU (Most Recently Used):** Evicts the page that was most recently accessed.
6. **MFU (Most Frequently Used):** Evicts the page with the highest access frequency/count.

---

## Installation

git clone [https://github.com/MariamAshraf25/page-replacement-simulator.git](https://github.com/MariamAshraf25/page-replacement-simulator.git)

---

## Author
**Mariam Ashraf** | Computer Engineering Student - Faculty of Engineering - Capital University (Formerly Helwan)  
[LinkedIn Profile](https://www.linkedin.com/in/mariam-ashraf-84415b2b8)
