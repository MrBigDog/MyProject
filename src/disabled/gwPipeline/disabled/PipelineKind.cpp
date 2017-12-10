#include <gwEarth/pipeline/PipelineKind.h>

namespace gwEarth {

pipeKind::pipeKind()
    : containerLayer_(NULL)
    , jointLayer_(NULL)
    , equipmentLayer_(NULL)
    , wellLayer_ ( NULL)
{
}

pipeKind::~pipeKind()
{
    removeAllLayer();
}

void pipeKind::setNmae(const std::string& pipeKindNmae)
{
    propsNode_->setStringValue("name", pipeKindNmae);
}

std::string pipeKind::getName()
{
    return propsNode_->getStringValue("name");
}

void pipeKind::setVisible(bool isVisible)
{
    if(isVisible_ == isVisible)
        return;

    if(containerLayer_&&containerLayer_!=NULL)
        containerLayer_->setVisible(isVisible);
    if(jointLayer_&&jointLayer_!=NULL)
        jointLayer_->setVisible(isVisible);
    if(equipmentLayer_&&equipmentLayer_!=NULL)
        equipmentLayer_->setVisible(isVisible);
    if(wellLayer_&&wellLayer_!=NULL)
        wellLayer_->setVisible(isVisible);

    propsNode_->setBoolValue("visible", isVisible);
}

bool pipeKind::getVisible()
{
    return propsNode_->getBoolValue("visible");
}


std::string pipeKind::getWorkspaceName()
{
    return workspaceName_;
}

void pipeKind::addContainerLayer()
{
    map_->addModelLayer(containerLayer_);
    propsNode_->addChild("container_layer");
}

//container
void pipeKind::removeContainerLayer()
{
    map_->removeModelLayer(containerLayer_);
    propsNode_->removeChild("container_layer");
}

void pipeKind::setContainerLayerVisible(bool isVisible)
{
    if(!containerLayer_||containerLayer_==NULL)
        return;
    containerLayer_->setVisible(isVisible);

//    GWPropertyNode* propnode = propsNode_->getChild("container_layer");
//    if(propnode)
//    {
//        propnode->setBoolValue("visible", isVisible);
//    }
}

bool pipeKind::getContainerLayerVisible()
{
    if(!containerLayer_||containerLayer_==NULL)
        return false;

    return containerLayer_->getVisible();
}

//void pipeKind::setContainerLayerName(const std::string &name)
//{
//    if(!containerLayer_||containerLayer_==NULL)
//        return;
//    containerLayer_->setName(name);
//}

std::string pipeKind::getContainerLayerName()
{
    if(!containerLayer_||containerLayer_==NULL)
        return "";

    return containerLayer_->getName();
}

PipelineLayer* pipeKind::getContainerLayer()
{
    return containerLayer_.get();
}

//joint
void pipeKind::addjointLayer()
{
    map_->addModelLayer(jointLayer_);
    propsNode_->addChild("joint_layer");
}

void pipeKind::removeJointLayer()
{
    map_->removeModelLayer(jointLayer_);
    propsNode_->removeChild("joint_layer");
}

void pipeKind::setJointLayerVisible( bool isVisible)
{
    if(!jointLayer_ || jointLayer_ ==NULL)
        return;
    jointLayer_->setVisible(isVisible);
}

bool pipeKind::getJointLayerVisible()
{
    if(!jointLayer_||jointLayer_==NULL)
        return false;
    return jointLayer_->getVisible();
}

//void pipeKind::setJointLayerName(const std::string &name)
//{
//    if(!jointLayer_||jointLayer_==NULL)
//        return;
//    jointLayer_->setName(name);
//}

std::string pipeKind::getJointLayerName()
{
    if(!jointLayer_||jointLayer_==NULL)
        return "";
    return jointLayer_->getName();
}

PipelineLayer* pipeKind::getJointLayer()
{
    return jointLayer_.get();
}

//equipment
void pipeKind::addEquipmentLayer()
{
    map_->addModelLayer( equipmentLayer_ );
    propsNode_->addChild("equipment_layer");
}

void pipeKind::removeEquipmentLayer()
{
    map_->removeModelLayer(equipmentLayer_);
    propsNode_->removeChild("equipment_layer");
}

void pipeKind::setEquipmentLayerVisible(bool isVisible)
{
    if(!equipmentLayer_||equipmentLayer_==NULL)
        return;

    equipmentLayer_->setVisible(isVisible);
}

bool pipeKind::getEquipmentLayerVisible()
{
    if(!equipmentLayer_||equipmentLayer_==NULL)
        return false;

    return equipmentLayer_->getVisible();
}

std::string pipeKind::getEquipmentLayerName()
{
    if(!equipmentLayer_||equipmentLayer_==NULL)
        return "";
    return equipmentLayer_->getName();
}

PipelineLayer* pipeKind::getEquipmentLayer()
{
    return equipmentLayer_.get();
}

//well
void pipeKind::addWellLayer()
{
    map_->addModelLayer(wellLayer_);
    propsNode_->addChild("well_layer");
}

void pipeKind::removeWellLayer()
{
    map_->removeModelLayer(wellLayer_);
    propsNode_->removeChild("well_layer");
}

void pipeKind::setWellLayerVisible(bool isVisible)
{
    if(!wellLayer_ || wellLayer_==NULL)
        return;

    wellLayer_->setVisible(isVisible);

    propsNode_->setBoolValue("visible", isVisible);
}

bool pipeKind::getWellLayerVisible()
{
    if(!wellLayer_ || wellLayer_==NULL)
        return false;
    return wellLayer_->getVisible();
}


std::string pipeKind::getWellLayerName()
{
    if(!wellLayer_||wellLayer_==NULL)
        return "";
    return wellLayer_->getName();
}

PipelineLayer* pipeKind::getWellLayer()
{
    return wellLayer_.get();
}


void pipeKind::removeAllLayer()
{
    map_->removeModelLayer(containerLayer_);
    map_->removeModelLayer(jointLayer_);
    map_->removeModelLayer(equipmentLayer_);
    map_->removeModelLayer(wellLayer_);

    propsNode_->removeAllChildren();
}

}
