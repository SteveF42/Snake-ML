#ifndef Node_HPP
#define Node_HPP

#include "ActivationRelu.hpp"
#include "LinkGene.hpp"
#include <memory>
#include <vector>

using std::vector;
class LinkGene;
class NodeGene;
typedef std::shared_ptr<NodeGene> NodePtr;

enum NodeType
{
    INPUT,
    HIDDEN,
    OUTPUT
};

class NodeGene
{
public:
    NodeGene(int id, NodeType type);
    NodeGene(NodeType type, double bias);
    NodeGene(int id, NodeType type, double bias);
    NodeGene(NodeGene &other);
    void addToLink(const LinkGene &link);
    void removeLink(const LinkGene &link);

    int getID() const;
    double getBias() const;
    void setBias(double bias);
    NodeType getType() const;
    vector<const LinkGene *> getToLinks() const;

    void addAccumalator(double value);
    void resetAccumalator();
    double activate();
    double output;

    static void setNextID(int id);
    static NodePtr getNewNode();
    inline static int nextID = 0;

    int layer = 0;
private:
    double bias;
    double accumalator;
    int id;
    NodeType type;
    ActivationRelu activationRelu;
    vector<const LinkGene *> toLinks;
};

#endif