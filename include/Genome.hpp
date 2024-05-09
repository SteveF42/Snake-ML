#ifndef GENOME_HPP
#define GENOME_HPP
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <memory>
#include "NodeGene.hpp"
#include "LinkGene.hpp"
using std::map;
using std::unordered_set;
using std::vector;

typedef std::shared_ptr<NodeGene> NodePtr;
typedef std::shared_ptr<LinkGene> LinkPtr;

struct networkLayer
{
    networkLayer(NodeType type = HIDDEN) : type(type) {}
    void addNode(NodeGene *node)
    {
        nodes.insert(node);
    }
    unordered_set<NodeGene *> nodes;
    NodeType type;
};

class Genome
{
public:
    Genome(int inputs, int outputs, bool initInputs = true, int hidden = 0);
    Genome(const Genome &other);
    Genome() {}
    ~Genome();
    void initialize();
    double getFitness();
    void setAdjustedFitness(double adjustedFitness);
    double getAdjustedFitness();
    double getComplexity();
    void setFitness(double fitness);
    void mutate();
    double distance(const Genome &other);
    static Genome *crossGenomes(const Genome &dominant, const Genome &recessive);

    vector<double> activate(vector<double> inputs);
    map<int, LinkPtr> &getLinks();
    map<int, NodePtr> &getNodes();
    map<int, networkLayer> getLayers();

    // mutations
    void addNode();
    void removeNode();
    void addLink();
    void removeLink();
    void weightRandom();
    void toggleWeight();
    void weightShift();
    void shiftBias();
    void buildLayers();

    Genome operator=(const Genome &other);
    bool child = false;

    inline static const int INPUT_LAYER = -2;
    inline static const int OUTPUT_LAYER = -1;

private:
    double fitness;
    double adjustedFitness;
    int inputs;
    int outputs;
    int hidden;
    // first n + m nodes are inputs and outputs
    map<int, NodePtr> allNodes;
    map<int, LinkPtr> allLinks;
    map<int, networkLayer> layers;

    static NodeGene *crossNeurons(const NodeGene &lhs, const NodeGene &rhs);
    static LinkGene *crossLinks(const LinkGene &lhs, const LinkGene &rhs);

    // util
    bool containsCycle(NodeGene *toNode, int fromNode);
    void removeNodeFromLayer(NodeGene *node);
    bool onSameLayer(NodeGene *lhs, NodeGene *rhs);
    void addNodeToLayer(NodeGene *node);
    void searchLayers(NodeGene *inputNode);
    NodeGene *getRandomNode(bool includeOuputs = false, bool includeInputs = false);
    LinkGene *getRandomLink();
    NodeGene *findNode(int nodeID) const;
    LinkGene *findLink(int linkID) const;
    void activationSoftmax(vector<double> &v);
};

#endif