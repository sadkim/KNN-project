#ifndef FDE20_BONUSPROJECT_3_KNN_HPP
#define FDE20_BONUSPROJECT_3_KNN_HPP

#include "Matrix.hpp"
#include <vector>
#include <algorithm>
#include <map>
#include <stdexcept>
using namespace std;

//structure of elements in the priority queue
struct PQEntry {
    Matrix::Entry entry;
    int* prt2Value;
    PQEntry(Matrix::Entry entry, int* prt2Value) : entry(entry), prt2Value(prt2Value){};
};

struct PriorityQueue {
private:
    // vector to store heap elements
    vector<PQEntry> heap;

    //map<unsigned int, unsigned int> processed;

    unsigned int PARENT(int i) {
        return (i - 1) / 2;
    }

    unsigned int LEFT(int i) {
        return (2*i + 1);
    }

    unsigned int RIGHT(int i) {
        return (2*i + 2);
    }

    void heapify_down(int i) {

        while(LEFT(i) < size()){
            unsigned int smallest = LEFT(i);
            if (RIGHT(i) < size() && heap[smallest+1].entry.weight < heap[smallest].entry.weight) {
                smallest += 1;
            }
            if (heap[smallest].entry.weight < heap[i].entry.weight) {
                *heap[i].prt2Value = smallest;
                *heap[smallest].prt2Value = i;
                swap(heap[i], heap[smallest]);
                i = smallest;
            } else {
                break;
            }
        }
    }

    void heapify_up(int i){

        while(i && heap[PARENT(i)].entry.weight > heap[i].entry.weight) {
            *heap[i].prt2Value = PARENT(i);
            *heap[PARENT(i)].prt2Value = i;
            swap(heap[i], heap[PARENT(i)]);
            i = PARENT(i);
        }
    }

public:
    map<unsigned int, int> processed;

    unsigned int size() {
        return heap.size();
    }

    bool empty() {
        return size() == 0;
    }

    // insert key into the heap
    void push(Matrix::Entry entry)
    {
        auto index = processed.find(entry.column);
        if(index == processed.end()) {
            processed[entry.column] = size();
            heap.push_back(PQEntry(entry, reinterpret_cast<int *>( &processed.at(entry.column))));
            heapify_up(size() - 1);
        } else {
            if (index->second >= 0 && entry.weight < heap[index->second].entry.weight) {
                heap[index->second].entry.weight = entry.weight;
                heapify_up(index->second);
            }
        }
    }

    Matrix::Entry pop() {
        *(heap.back().prt2Value) = 0;
        *(heap[0].prt2Value) = -1;

        Matrix::Entry root = heap[0].entry;
        heap[0] = heap.back();
        heap.pop_back();

        heapify_down(0);
        return root;
    }
};





//---------------------------------------------------------------------------
/// Find the top k neighbors for the node start. The directed graph is stored in
/// matrix m and you can use getNeighbors(node) to get all neighbors of a node.
/// A more detailed description is provided in Matrix.hpp.
/// The function should return the k nearest neighbors in sorted order as vector
/// of Matrix::Entry, where Entry->column is the neighbor node and Entry->weight
/// is the cost to reach the node from the start node.
vector<Matrix::Entry> getKNN(const Matrix &m, unsigned start, unsigned k) {

    using Entry = Matrix::Entry;
    vector<Entry> result;
    result.reserve(k);

    PriorityQueue pq;
    int step = 0;
    unsigned currentNode = start;
    double addWeight = 0.0;

    while (step < k){
        for (auto &e : m.getNeighbors(currentNode)) {
            pq.push(Entry(e.column, e.weight+addWeight));
        }
        if(!pq.empty()){
            Entry root = pq.pop();
            currentNode = root.column;
            addWeight = root.weight;
            result.push_back(root);
            step++;
        }
        else{
            break;
        }
    }

  return result;
}

//---------------------------------------------------------------------------

#endif // FDE20_BONUSPROJECT_3_KNN_HPP
