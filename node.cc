#include "node.h"
extern map<string, Node*> symbol_table;

Node::Node(double d){
    this->type = CONSTANT;
    this->value = d;
}

Node::Node(string name, double value) {
    this->type  = VARIABLES;
    this->value = value;
    this->name  = name;
    symbol_table.insert(pair<string, Node*>(name, this));
}

Node::~Node(){
}

double Node::ex() {
    switch(this->type) {
        case CONSTANT:
            return this->value;
            break;
        case VARIABLES:
            break;
        case OPERATION:
            break;
    }
    return 0;
}
double Node::ex(Node::opType op, vector<Node>& nodes) {
    switch(this->type) {
        case CONSTANT:
            return this->value;
            break;
        case VARIABLES:
            break;
        case OPERATION:
            this->exop(op, nodes);
            break;
    }
    return 0;
}

double Node::exop(Node::opType op, vector<Node>& nodes) {
    switch(op) {
        case opADD:
            this->value = nodes[0].value + nodes[1].value;
            break;
        case opIF:
            if (nodes[0].value) {
                nodes[1].ex();
            }
            break;
        default:
            break;
    }
    return 0;
}
Node Node::operator+ (const Node& n) {
    Node t(0.0);
    t.value = this->value + n.value;
    return t;
}
