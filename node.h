#include <stdio.h>
#include <vector>
#include <map>
#include <string>
using namespace std;


class Node {
    public:
        Node(double value);
        Node(string name, double value);
        virtual ~Node();
        enum nodeType {
            CONSTANT,
            VARIABLES,
            OPERATION,
        };
        enum opType {
            opADD,
            opIF,
        };
        double ex();
        double ex(opType, vector<Node>& nodes);
        double exop(opType, vector<Node>& nodes);
        Node operator+ (const Node& n);
        nodeType type;
        string   name;
        double   value;
};
