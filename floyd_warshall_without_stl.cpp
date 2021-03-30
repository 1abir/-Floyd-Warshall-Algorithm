//
// Created by abirm on 11/18/2018.
//

#include <cstdlib>
#include <cmath>
#include <iostream>

#define INFINITe 99999

using namespace std;

template<typename T>
class Vector {
    unsigned int _size;
    unsigned int _capacity;
    unsigned int Log;
    T *buffer;
public:
    Vector() : _size(0), _capacity(0), Log(0) {}

    unsigned int size() const {
        return _size;
    }

    ~Vector() {
        //try{delete[] buffer;} catch (const std::exception& e) {cout<<e.what()<<endl;}
    }

    void reserve(unsigned int capacity) {
        T *newBuffer = (T *) malloc(capacity * 8 * sizeof(T));

        for (unsigned int i = 0; i < _size; i++)
            newBuffer[i] = buffer[i];

        _capacity = capacity;
        //try{delete[] buffer;} catch (const std::exception& e) {cout<<e.what()<<endl;}

        buffer = newBuffer;
    }

    void resize(unsigned int size) {
        Log = ceil(log((double) size) / log(2.0));
        reserve(1 << Log);
        _size = size;
    }

    T &operator[](unsigned int index) {
        return buffer[index];
    }

    void push_back(const T &v) {
        if (_size >= _capacity) {
            reserve(1 << Log);
            Log++;
        }
        buffer[_size++] = v;
    }

    Vector<T> &operator=(const Vector<T> &v) {
        //try{delete[] buffer;} catch (const std::exception& e) {cout<<e.what()<<endl;}
        _size = v._size;
        Log = v.Log;
        _capacity = v._capacity;
        buffer = (T *) calloc(_capacity, sizeof(T));
        for (unsigned int i = 0; i < _size; i++)
            buffer[i] = v.buffer[i];
        return *this;
    }

};

class FloydWarshal {
    unsigned int nNodes;
    int **distance;
    int **next;
    int **predecessor;
    bool negative_cycle;

public:

    FloydWarshal(unsigned int nNodes) : nNodes(nNodes) {
        initializedistance(nNodes);
        negative_cycle=false;
    }

    void initializedistance(unsigned int nodes) {
        distance = new int *[nodes];
        next = new int *[nodes];
        predecessor=new int *[nodes];
        for (int i = 0; i < nodes; ++i) {
            distance[i] = new int[nodes];
            next[i] = new int[nodes];
            predecessor[i]=new int [nodes];
            for (int j = 0; j < nodes; ++j) {
                next[i][j] = j;
                predecessor[i][j]=i;
                if (i == j)
                    distance[i][j] = 0;
                else
                    distance[i][j] = INFINITe;
            }
        }
    }

    void add_edge(unsigned int u, unsigned int v, int weight) {
        if (u >= nNodes || v >= nNodes)
            return;
        distance[u][v] = weight;
    }

    bool floydWarshal() {
        for (int k = 0; k < nNodes; ++k)
            for (int i = 0; i < nNodes; ++i)
                for (int j = 0; j < nNodes; ++j)
                    if (distance[i][k] != INFINITe && distance[k][j] != INFINITe)
                        if (distance[i][j] > distance[i][k] + distance[k][j]) {
                            distance[i][j] = distance[i][k] + distance[k][j];
                            next[i][j] = next[i][k];
                            predecessor[i][j]=predecessor[k][j];
                        }
        for (int i = 0; i < nNodes; ++i) {
            if (distance[i][i] < 0)
                negative_cycle = true;
            return negative_cycle;
        }
        return false;
    }

    Vector<int> findPath(int i, int j) {
        Vector<int> p;
        if (distance[i][j] == INFINITe)
            return p;
        p.push_back(i);
        while (i != j) {
            i = next[i][j];
            p.push_back(i);
        }
        return p;
    }

    void print_distance() {
        cout << "All pair shortest paths:"<<endl;

        for (int i = 0; i < nNodes; ++i) {
            for (int j = 0; j < nNodes; ++j) {
                if (distance[i][j] == INFINITe)
                    cout << "       inf";
                else
                    printf("%9d", distance[i][j]);
            }
            cout << endl;
        }
        cout << endl;
    }
    void print_predecessor() {
        cout <<"Predecessor Matrix:"<< endl;

        for (int i = 0; i < nNodes; ++i) {
            for (int j = 0; j < nNodes; ++j) {
                if (distance[i][j] == INFINITe)
                    cout << "       inf";
                else
                    printf("%9d", predecessor[i][j]);
            }
            cout << endl;
        }
        cout << endl;
    }
    void print(Vector<int> path)
    {
        for (int k = 0; k < path.size(); ++k) {
            cout << path[k] + 1 << "   ->   ";
        }
        cout<<"\b\b\b\b\b\n";
    }
};

int main() {
    freopen("inputFloyd.txt", "r", stdin);
    int nodes,edges,weight;
    cin >> nodes>>edges;

    FloydWarshal floydWarshal(nodes);
    unsigned int u,v;
    for (int i = 0; i < edges; ++i) {
        cin>>u>>v>>weight;
        floydWarshal.add_edge(u-1,v-1,weight);
    }
    bool negative_cycle = floydWarshal.floydWarshal();
    floydWarshal.print_distance();
    cout<<endl;
    floydWarshal.print_predecessor();
    cout << "Negative Cycle: " <<( negative_cycle?"YES":"NO" ) << endl;

//    Vector<int> path;
//    for (int i = 0; i < nodes; ++i) {
//        for (int j = 0; j < nodes; ++j) {
//            cout<<"Path from "<<i+1<<" to "<<j+1<<" : ";
//            floydWarshal.print(floydWarshal.findPath(i,j));
//        }
//    }

    return 0;
}
