#include <stdio.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define  LOG(fmt, args...)  if (0) printf(fmt, ##args)

class Node {
    public:
        enum nodeType {
            CONSTANT = 10,
            VARIABLES,
            OPERATION,
            FUNCTION,
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
        Node(string name);
        Node(opType op);
        virtual ~Node();
        double ex();
        double exop();
        void push_parameter(Node* n);
        nodeType        type;
        opType          operation;
        string          name;
        double          value;
        vector<Node*>   nodes;
};
