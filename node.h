#include <stdio.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define  LOG(fmt, args...)  if (1) printf(fmt, ##args)
class NodeList;

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
            opPRINT,
            opLIST,
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
        NodeList* nodelist;
};

class NodeList {
    public:
        vector<Node*> nodelist;
        void push_back(Node* p);
        double ex();
};
