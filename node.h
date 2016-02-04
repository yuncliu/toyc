#include <stdio.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define  LOG(fmt, args...)  if (1) printf(fmt, ##args)

class Node {
    public:
        enum nodeType {
            CONSTANT = 10,
            VARIABLES,
            OPERATION,
        };
        enum opType {
            opADD =1,
            opMINUS,
            opMUL,
            opDIV,
            opIF,
            opWHILE,
            opASSIGN,
            opSEMICOLON,
            NoneOP,
        };
        Node(double value);
        Node(string name, double value);
        Node(opType op);
        virtual ~Node();
        double ex();
        double exop();
        int addchild(Node* n);
        Node operator+ (const Node& n);
        nodeType type;
        opType   operation;
        string   name;
        double   value;
        vector<Node*> nodes;
};
