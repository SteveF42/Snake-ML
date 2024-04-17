#include "Genome.hpp"
#include "Util.hpp"
#include "random"
#include <stack>
#include <queue>
#include <iterator>
#include <unordered_map>

using std::make_shared;

Genome::Genome(int inputs, int outputs, bool initInputs)
{
    this->inputs = inputs;
    this->outputs = outputs;
    this->fitness = 0;

    if (initInputs)
    {
        initialize();
    }
    // initialize inputs and outputs
}

Genome::Genome(const Genome &other)
{
    this->inputs = other.inputs;
    this->outputs = other.outputs;
    this->fitness = other.fitness;
    this->allNodes = other.allNodes;
    this->allLinks = other.allLinks;
    this->layers = other.layers;
}

Genome::~Genome()
{
    allNodes.clear();
    allLinks.clear();
    layers.clear();
}

void Genome::initialize()
{
    networkLayer inputLayer(INPUT);
    networkLayer outputLayer(OUTPUT);
    for (int i = 0; i < inputs; i++)
    {
        allNodes.insert({i, make_shared<NodeGene>((int)i, INPUT)});
        inputLayer.addNode(allNodes[i].get());
    }
    for (int i = 0; i < outputs; i++)
    {
        allNodes.insert({i + inputs, make_shared<NodeGene>((int)i + inputs, OUTPUT)});
        outputLayer.addNode(allNodes[i + inputs].get());
    }
    layers.insert({OUTPUT_LAYER, outputLayer});
    layers.insert({INPUT_LAYER, inputLayer});

    // initialize links
    int idx = 0;
    for (int i = 0; i < inputs; i++)
    {
        for (int j = 0; j < outputs; j++)
        {
            if(randDouble(0, 1) > Config::initialConnectionProbability)
                continue;
            LinkPtr newLink = make_shared<LinkGene>(allNodes[j + inputs].get(), allNodes[i].get(), randDouble(-1, 1), idx++);
            allNodes[i]->addToLink(*newLink);
            allLinks.insert({newLink->getID(), newLink});
        }
    }
}

double Genome::getFitness()
{
    // double penalty = (allNodes.size() + allLinks.size()) * 0.001;
    return fitness;
}

void Genome::setFitness(double fitness)
{
    this->fitness = fitness;
}

double Genome::distance(const Genome &other)
{
    // calculate the distance between two genomes
    int s1 = this->allLinks.size();
    int s2 = other.allLinks.size();

    auto p1 = allLinks.begin();
    auto p2 = other.allLinks.begin();

    int excess = 0;
    int similar = 0;
    int disjoint = 0;
    double weightDiff = 0;
    int idx1 = 0;
    int idx2 = 0;

    if (s1 > 0 && s2 > 0)
    {
        auto e1 = allLinks.end();
        auto e2 = other.allLinks.end();
        while (p1 != this->allLinks.end() && p2 != other.allLinks.end())
        {
            LinkGene *conn1 = p1->second.get();
            LinkGene *conn2 = p2->second.get();

            if (conn1->getID() == conn2->getID())
            {
                similar++;
                weightDiff += std::abs(conn1->getWeight() - conn2->getWeight());
                p1++;
                p2++;
                idx1++;
                idx2++;
            }
            else if (conn1->getID() < conn2->getID())
            {
                disjoint++;
                p1++;
                idx1++;
            }
            else
            {
                disjoint++;
                p2++;
                idx2++;
            }
        }

        excess = std::max(s1 - idx1, s2 - idx2);
        weightDiff /= similar;
    }
    else if (s1 > 0)
    {
        excess = s1;
    }
    else
    {
        excess = s2;
    }

    double N = std::max(s1, s2);
    double distance = ((Config::excessCoefficient * excess + N + Config::disjointCoefficient * disjoint) / N) + Config::weightDiffCoefficient * weightDiff;
    return distance;
}

Genome *Genome::crossGenomes(const Genome &dominant, const Genome &recessive)
{
    Genome *child = new Genome(dominant.inputs, dominant.outputs, false);
    std::unordered_map<int, NodeGene *> nodeMap;

    // cross nodes
    for (const auto &[nodeID, dominantNeuron] : dominant.allNodes)
    {
        NodeGene *recessiveNeuron = recessive.findNode(nodeID);
        NodePtr newNode;
        if (recessiveNeuron == nullptr)
        {
            newNode = make_shared<NodeGene>(*dominantNeuron);
        }
        else
        {
            NodeGene* t = crossNeurons(*dominantNeuron, *recessiveNeuron);
            newNode = NodePtr(t);
        }

        child->allNodes.insert({nodeID, newNode});
        nodeMap.insert({newNode->getID(), newNode.get()});
    }

    // cross links
    for (const auto &[linkID, dominantLink] : dominant.allLinks)
    {
        LinkGene *recessiveLink = recessive.findLink(linkID);
        if (recessiveLink == nullptr)
        {
            int id1 = dominantLink->getFromNode()->getID();
            int id2 = dominantLink->getToNode()->getID();
            NodeGene *fromNode = nodeMap[id1];
            NodeGene *toNode = nodeMap[id2];

            LinkPtr newLink = make_shared<LinkGene>(toNode, fromNode, dominantLink->getWeight(), linkID);
            fromNode->addToLink(*newLink);
            child->allLinks.insert({linkID, newLink});
        }
        else
        {
            NodeGene *fromNode = nodeMap[dominantLink->getFromNode()->getID()];
            NodeGene *toNode = nodeMap[dominantLink->getToNode()->getID()];

            double weight = randNumber(2) == 0 ? dominantLink->getWeight() : recessiveLink->getWeight();
            bool enabled = randNumber(2) == 0 ? dominantLink->isEnabled() : recessiveLink->isEnabled();
            LinkPtr newLink = make_shared<LinkGene>(toNode, fromNode, weight, linkID);
            newLink->setEnabled(enabled);
            fromNode->addToLink(*newLink);
            child->allLinks.insert({linkID, newLink});
        }
    }
    child->buildLayers();
    return child;
}

vector<double> Genome::activate(vector<double> inputs)
{
    vector<double> genomeOutputs;
    int outIdx = 0;
    if (inputs.size() != this->inputs)
    {
        throw std::runtime_error("Invalid input size " + std::to_string(inputs.size()) + " expected " + std::to_string(this->inputs));
    }

    // idk I saw other people do this
    for (const auto &[nodeID, node] : allNodes)
    {
        node->resetAccumalator();
    }

    // first two will be the input and output layers
    for (const auto &[layerId, layer] : layers)
    {
        for (const auto &node : layer.nodes)
        {
            // we have to process this layer last
            if (layer.type == OUTPUT)
                break;
            if (layer.type == INPUT)
            {
                node->output = inputs[outIdx++];
            }
            else if (layer.type == HIDDEN)
            {
                node->activate();
            }
            for (const auto &link : node->getToLinks())
            {
                if (link->isEnabled() == false)
                    continue;

                NodeGene *toNode = link->getToNode();
                toNode->addAccumalator(link->getWeight() * node->output);
            }
        }
    }

    // process output layer
    for (const auto &node : layers[OUTPUT_LAYER].nodes)
    {
        genomeOutputs.push_back(node->activate());
    }

    // normalizes the output from 0.0-1.0
    activationSoftmax(genomeOutputs);
    return genomeOutputs;
}

map<int, LinkPtr> &Genome::getLinks()
{
    return allLinks;
}

map<int, NodePtr> &Genome::getNodes()
{
    return allNodes;
}

map<int, networkLayer> Genome::getLayers()
{
    return layers;
}

void Genome::mutate()
{
    bool createLayers = false;
    if (randDouble(0, 1) < Config::mutateAddNodeProbability)
    {
        addNode();
        buildLayers();
    }
    if (randDouble(0, 1) < Config::mutateRemoveNodeProbability)
    {
        removeNode();
        buildLayers();
    }
    if (randDouble(0, 1) < Config::mutateAddLinkProbability)
    {
        addLink();
        buildLayers();
    }
    if (randDouble(0, 1) < Config::mutateRemoveLinkProbability)
    {
        removeLink();
        buildLayers();
    }
    if (randDouble(0, 1) < Config::mutateEnableLinkProbability)
    {
        toggleWeight();
    }
    if (randDouble(0, 1) < Config::mutateWeightShiftProbability)
    {
        weightShift();
    }
    if (randDouble(0, 1) < Config::mutateWeightRandomProbability)
    {
        weightRandom();
    }
    if (randDouble(0, 1) < Config::mutateBiasShiftProbability)
    {
        shiftBias();
    }
}

void Genome::addNode()
{
    if(allLinks.size() == 0)
        return;
    // get a random link
    LinkGene *link = getRandomLink();
    // disable the link
    link->setEnabled(false);
    // create a new node
    NodePtr newNode = NodeGene::getNewNode();
    // create two new links
    LinkPtr toNewNode = make_shared<LinkGene>(newNode.get(), link->getFromNode(), link->getWeight());
    LinkPtr fromNewNode = make_shared<LinkGene>(link->getToNode(), newNode.get(), 1);
    // insert new links
    allLinks.insert({toNewNode->getID(), toNewNode});
    allLinks.insert({fromNewNode->getID(), fromNewNode});
    // add the new node
    allNodes.insert({newNode->getID(), newNode});
    // add links to nodes
    newNode->addToLink(*fromNewNode);

    NodeGene *originalFrom = link->getFromNode();
    originalFrom->addToLink(*toNewNode);
}

void Genome::removeNode()
{
    if (layers.size() <= 2)
        return;
    // node index can only be hidden
    NodeGene *nodeToDelete = getRandomNode(false);

    // remove all links to and from the node
    for (auto it = allLinks.begin(); it != allLinks.end();)
    {
        LinkGene *link = it->second.get();
        if (link->getFromNode() == nodeToDelete)
        {
            it = allLinks.erase(it);
        }
        else if (link->getToNode() == nodeToDelete)
        {
            link->getFromNode()->removeLink(*link);
            it = allLinks.erase(it);
        }
        else
        {
            it++;
        }
    }
    // remove the node
    removeNodeFromLayer(nodeToDelete);
    allNodes.erase(nodeToDelete->getID());
}

void Genome::addLink()
{
    // from node can be inputs or hidden. to nodes can be hidden or outputs
    NodeGene *fromNode = getRandomNode(false, true);
    NodeGene *toNode = getRandomNode(true, false);
    // check if the link already exists
    for (const auto &[linkID, link] : allLinks)
    {
        if (link->getFromNode() == fromNode && link->getToNode() == toNode)
        {
            link->setEnabled(true);
            return;
        }
    }

    // only allow feed forward network
    if (containsCycle(toNode, fromNode->getID()) || onSameLayer(fromNode, toNode))
    {
        return;
    }

    // create the link
    LinkPtr newLink = make_shared<LinkGene>(toNode, fromNode, randDouble(Config::min, Config::max));
    fromNode->addToLink(*newLink);
    allLinks.insert({newLink->getID(), newLink});
}

void Genome::removeLink()
{
    // get a random link
    if (allLinks.size() == 0)
    {
        return;
    }
    int linkIndex = randNumber(allLinks.size());
    auto it = allLinks.begin();
    std::advance(it, linkIndex);
    LinkGene *link = it->second.get();

    // prevents input and output links from being removed
    // if (link->getFromNode()->getType() == INPUT && link->getToNode()->getType() == OUTPUT)
    // {
    //     link->setEnabled(false);
    //     return;
    // }
    link->getFromNode()->removeLink(*link);
    allLinks.erase(link->getID());
}

void Genome::weightShift()
{
    if (allLinks.size() == 0)
        return;
    LinkGene *link = getRandomLink();
    double adjustWeight = randDouble(-1, 1) + link->getWeight();
    adjustWeight = std::min(adjustWeight, Config::max);
    adjustWeight = std::max(adjustWeight, Config::min);
    link->setWeight(adjustWeight);
}

void Genome::toggleWeight()
{
    if (allLinks.size() == 0)
        return;
    LinkGene *link = getRandomLink();
    link->setEnabled(!link->isEnabled());
}

void Genome::weightRandom()
{
    if (allLinks.size() == 0)
        return;
    LinkGene *link = getRandomLink();
    link->setWeight(randDouble(-1, 1));
}

void Genome::shiftBias()
{
    NodeGene *node = getRandomNode(true);
    double adjustBias = (randDouble(-1, 1) * Config::weightShiftStrength) + node->getBias();
    adjustBias = std::min(adjustBias, Config::max);
    adjustBias = std::max(adjustBias, Config::min);
    node->setBias(adjustBias);
}

NodeGene* Genome::crossNeurons(const NodeGene &lhs, const NodeGene &rhs)
{
    int id = lhs.getID();
    double bias = randNumber(1) == 0 ? lhs.getBias() : rhs.getBias();

    NodeGene* newNode = new NodeGene(id, lhs.getType(), bias);
    return newNode;
}

// CHANGE ALL NODES VECTOR TO AN UNORDERED MAP FOR FASTER ACCESS TIMES AND EASIER NODE DELETION
LinkGene *Genome::crossLinks(const LinkGene &lhs, const LinkGene &rhs)
{
    double weight = randNumber(1) == 0 ? lhs.getWeight() : rhs.getWeight();
    bool enabled = randNumber(1) == 0 ? lhs.isEnabled() : rhs.isEnabled();
    auto child = new LinkGene(lhs.getFromNode(), lhs.getToNode(), weight, lhs.getID());
    child->setEnabled(enabled);
    return child;
}

bool Genome::containsCycle(NodeGene *toNode, int fromNode)
{
    std::stack<NodeGene *> dfs;
    std::unordered_set<NodeGene *> visited;
    dfs.push(toNode);

    while (dfs.size() > 0)
    {
        NodeGene *current = dfs.top();
        dfs.pop();
        if (visited.find(current) != visited.end())
            continue;

        visited.insert(current);
        for (const LinkGene *neighbor : current->getToLinks())
        {
            if (neighbor->getToNode()->getID() == fromNode)
            {
                return true;
            }
            dfs.push(neighbor->getToNode());
        }
    }
    return false;
}

NodeGene *Genome::getRandomNode(bool includeOutput, bool includeInputs)
{
    int rand = randNumber(layers.size() - 2) + 2;
    if (includeInputs)
    {
        rand -= 2;
        rand = rand == 1 ? rand + 1 : rand;
    }
    else if (includeOutput)
        rand -= 1;

    auto it = layers.begin();
    std::advance(it, rand);
    const networkLayer &layer = it->second;
    int nodeIndex = randNumber(layer.nodes.size());
    auto nodeIt = layer.nodes.begin();
    std::advance(nodeIt, nodeIndex);
    return *nodeIt;
}
LinkGene *Genome::getRandomLink()
{
    int rand = randNumber(allLinks.size());
    auto it = allLinks.begin();
    std::advance(it, rand);
    return it->second.get();
}

NodeGene *Genome::findNode(int nodeID) const
{
    if (allNodes.find(nodeID) != allNodes.end())
    {
        return allNodes.at(nodeID).get();
    }
    return nullptr;
}

LinkGene *Genome::findLink(int linkID) const
{
    if(allLinks.find(linkID) != allLinks.end())
    {
        return allLinks.at(linkID).get();
    }
    return nullptr;
}

Genome Genome::operator=(const Genome &other)
{
    this->fitness = other.fitness;
    this->inputs = other.inputs;
    this->outputs = other.outputs;
    this->allNodes = other.allNodes;
    this->allLinks = other.allLinks;
    this->layers = other.layers;
    return *this;
}

void Genome::activationSoftmax(vector<double> &v)
{
    double sum = 0;
    double max = INT16_MIN;
    for (auto &val : v)
    {
        max = std::max(max, val);
    }
    for (auto &val : v)
    {
        val = std::exp(val - max);
        sum += val;
    }
    for (auto &val : v)
    {
        val /= sum;
    }
}

void Genome::removeNodeFromLayer(NodeGene *node)
{
    for (auto &[key, layer] : layers)
    {
        if (layer.type == INPUT || layer.type == OUTPUT)
            continue;

        if (layer.nodes.find(node) != layer.nodes.end())
        {
            layer.nodes.erase(node);
            if (layer.nodes.size() == 0)
            {
                layers.erase(key);
            }
            return;
        }
    }
}

bool Genome::onSameLayer(NodeGene *lhs, NodeGene *rhs)
{
    for (const auto &[key, layer] : layers)
    {
        if (layer.nodes.find(lhs) != layer.nodes.end() && layer.nodes.find(rhs) != layer.nodes.end())
        {
            return true;
        }
    }
    return false;
}

void Genome::addNodeToLayer(NodeGene *node)
{
    int layer = node->layer;
    if (layers.find(layer) == layers.end())
    {
        networkLayer newLayer(HIDDEN);
        newLayer.addNode(node);
        layers.insert({layer, newLayer});
    }
    else
    {
        layers[layer].addNode(node);
    }
}

void Genome::searchLayers(NodeGene *inputNode)
{
    unordered_set<NodeGene *> visited;
    std::queue<NodeGene *> q;
    int depth = 1;
    q.push(inputNode);
    while (!q.empty())
    {
        int size = q.size();
        for (int i = 0; i < size; i++)
        {
            NodeGene *current = q.front();
            q.pop();
            // if (visited.find(current) != visited.end())
            //     continue;

            current->layer = std::max(current->layer, depth);
            // visited.insert(current);
            for (const LinkGene *link : current->getToLinks())
            {
                q.push(link->getToNode());
            }
        }
        depth++;
    }
}

void Genome::buildLayers()
{
    // n = # of nodes
    // m = # of links
    this->layers.clear();
    networkLayer inputLayer(INPUT);
    networkLayer outputLayer(OUTPUT);
    std::vector<NodeGene *> hiddenNodes;

    for (const auto &[key, node] : allNodes)
    {
        node->layer = 0;
        if (node->getType() == INPUT)
            inputLayer.addNode(node.get());
        else if (node->getType() == OUTPUT)
            outputLayer.addNode(node.get());
        else
            hiddenNodes.push_back(node.get());
    }
    layers.insert({INPUT_LAYER, inputLayer});
    layers.insert({OUTPUT_LAYER, outputLayer});
    for (const auto &node : inputLayer.nodes)
    {
        searchLayers(node);
    }
    for (const auto &node : hiddenNodes)
    {
        addNodeToLayer(node);
    }
}