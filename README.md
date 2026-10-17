<div align="center">

# Iron Nexus

Command-line railway management system in C++ where every data structure is written from scratch.

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Compiler](https://img.shields.io/badge/Compiler-g%2B%2B-A42E2B?style=flat-square&logo=gnu&logoColor=white)
![STL containers](https://img.shields.io/badge/STL%20containers-none-2E7D32?style=flat-square)
![Interface](https://img.shields.io/badge/Interface-CLI-4D4D4D?style=flat-square&logo=windowsterminal&logoColor=white)

</div>

## Overview

Iron Nexus manages the day-to-day data of a small railway: registered trains, the coaches coupled to each train, the station network between cities, and seat bookings. Instead of using `std::vector`, `std::map` or other library containers, it implements an AVL tree, doubly linked list, adjacency-list graph, hash table, BST and stacks by hand with raw pointers and manual `new`/`delete`. Everything is driven from a colored, menu-based console interface, and all data can be saved to and restored from plain-text files. It was built together with teammates Haris Said and Haris Shafiq.

## Key Features

- **Train registry**: an AVL tree keyed by train ID with insert, remove, search, in-order, pre-order and post-order display, and a per-train detail view.
- **Coach management**: a doubly linked list per train. Add a coach at the front, at the back or at a position (the walk starts from whichever end is nearer), remove by ID, traverse forward or backward, and reverse the train orientation.
- **Station network**: a weighted undirected graph stored as an adjacency list (up to 15 stations). Dijkstra finds the shortest path by distance (km) or by travel time (min), a breadth-first search checks connectivity, and the network can be shown as a list or an adjacency matrix.
- **Seat booking**: a hash table with linear probing and a prime-sized table gives fast booking, cancellation and lookup, while a BST kept in sync gives sorted, booked-only and available-only displays.
- **Operation log**: a linked-list stack of timestamped entries that can be viewed in full, limited to the most recent N, filtered by module, peeked or popped.
- **Undo and redo**: a two-stack design covering 13 action types across trains, coaches, seat bookings, stations and routes. A new action clears the redo stack.
- **File persistence**: save and load the train registry (including coaches and seat charts), the network and the log, individually or all at once. Trains are written in pre-order so the AVL tree rebuilds cleanly on load, and the seat BST can also be exported as a hierarchical structure file.
- **Terminal UI**: ANSI colors, a box-drawn banner and menu frames, with Windows virtual-terminal mode switched on at startup.

## Tech Stack

| Area | Technology |
|------|------------|
| Language | C++17 |
| Data structures | AVL tree, BST, doubly linked list, singly linked stacks, adjacency list, hash table (all hand-written) |
| Algorithms | Dijkstra shortest path, breadth-first search, tree traversals, linear probing |
| Build | GNU Make or a direct `g++` command |
| Storage | Plain-text `.dat` files under `data/` |
| Interface | Console menus with ANSI escape codes |

## Project Structure

```text
.
|-- DS Project/
|   |-- main.cpp       # Entry point and all menus
|   |-- Train.*        # AVL tree train registry
|   |-- Coach.*        # Doubly linked list of coaches
|   |-- Network.*      # Station graph, Dijkstra, BFS
|   |-- Seat.*         # Hash table + BST seat chart
|   |-- Logger.*       # Linked-list stack operation log
|   |-- UndoRedo.*     # Two-stack undo and redo
|   |-- FileIO.*       # Save and load to text files
|   |-- Utils.*        # Colors, banner, string and input helpers
|   `-- Makefile       # Build rules
|-- .gitignore
`-- README.md
```

## Getting Started

### Prerequisites

- A C++17 compiler: `g++` (MinGW-w64 on Windows, GCC or Clang elsewhere)
- GNU Make (optional, only for the Makefile route)

### Installation

```bash
git clone https://github.com/shahramshafiq/railway-management-system.git
cd railway-management-system
```

### Run

Build with Make from the repository root:

```bash
make
./IronNexus.exe
```

Or compile directly with `g++`:

```bash
g++ -std=c++17 -Wall -Wextra -O2 -o IronNexus.exe "DS Project/main.cpp" "DS Project/Utils.cpp" "DS Project/Train.cpp" "DS Project/Coach.cpp" "DS Project/Seat.cpp" "DS Project/Network.cpp" "DS Project/Logger.cpp" "DS Project/UndoRedo.cpp" "DS Project/FileIO.cpp"
./IronNexus.exe
```

Run it from the repository root: the program creates a `data/` folder in the current directory the first time you save. The program reads no environment variables.

## Usage

The main menu opens seven areas: Train Registry, Coach Management, Railway Track Network, Seating Chart, Operation Log, Undo / Redo and Save / Load Data. A typical session:

1. **Train Registry**: register a train with a positive numeric ID and a name.
2. **Coach Management**: add coaches to it (type Economy, Business or VIP, plus a seat count).
3. **Railway Track Network**: add stations, connect them with routes (distance and travel time), then run the shortest path by distance or by time.
4. **Seating Chart**: initialize the chart for the train, then book and cancel seats by seat number and passenger name.
5. **Undo / Redo**: step back or forward through those changes.
6. **Save / Load Data**: write everything to `data/trains.dat`, `data/network.dat` and `data/log.dat`, or save the seat BST structure to `data/bst_structure.dat`.

## Limitations and Roadmap

- The network holds at most 15 stations, set by the `MAX_STATIONS` constant in `Network.h`.
- Undo and redo history lives in memory only and is not written to the data files.
- Screen clearing and creation of the `data/` folder shell out through `system()`, with Windows handled first and a Unix fallback.
- There are no automated tests yet; adding unit tests for each data structure is the natural next step.

## Author

**Shahram Shafiq** - [GitHub](https://github.com/shahramshafiq) | [LinkedIn](https://www.linkedin.com/in/shahramshafiq/)
