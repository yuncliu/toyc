#include <stdio.h>
#include <vector>
#include <map>
#include <string>
using namespace std;


class Node {
    public:
        Node();
        virtual ~Node();
        enum nodeType {
            CONSTANT,
            IDENTIFIER,
            OPERATION,
        };
        enum opType {
            opADD,
            opIF,
        };
        double ex();
        double ex(opType, vector<Node>& nodes);
        double exop(opType, vector<Node>& nodes);
        nodeType type;
        double value;
        static map<string, double> symbol_table;
};
