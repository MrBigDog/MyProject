#ifndef gwPipelineKind_h__
#define gwPipelineKind_h__


#include <gwEarth/pipeline/PipelineLayer.h>
#include <gwBase/props/props.h>
#include <osgEarth/Map>

#include <string>
#include <map>

namespace gwEarth {

class pipeKind
{
public:
    pipeKind();
    ~pipeKind();

    void setNmae(const std::string& pipeKindNmae);
    std::string getName();

    void setVisible(bool isVisible);
    bool getVisible();

    void setWorkspaceNmae(const std::string& workspaceNmae);
    std::string getWorkspaceName();

    void addContainerLayer();
    void removeContainerLayer();
    void setContainerLayerVisible(bool isVisible);
    bool getContainerLayerVisible();
    std::string getContainerLayerName();
    PipelineLayer* getContainerLayer();

    void addjointLayer();
    void removeJointLayer();
    void setJointLayerVisible(bool isVisible);
    bool getJointLayerVisible();
    std::string getJointLayerName();
    PipelineLayer* getJointLayer();

    void addEquipmentLayer();
    void removeEquipmentLayer();
    void setEquipmentLayerVisible(bool isVisible);
    bool getEquipmentLayerVisible();
    std::string getEquipmentLayerName();
    PipelineLayer* getEquipmentLayer();

    void addWellLayer();
    void removeWellLayer();
    void setWellLayerVisible(bool isVisible);
    bool getWellLayerVisible();
    std::string getWellLayerName();
    PipelineLayer* getWellLayer();

    void removeAllLayer();

private:

    osg::ref_ptr<osgEarth::Map> map_;
//    std::map<std::string, PipelineLayer> layerMap_;
    osg::ref_ptr<PipelineLayer> containerLayer_;
    osg::ref_ptr<PipelineLayer> jointLayer_;
    osg::ref_ptr<PipelineLayer> equipmentLayer_;
    osg::ref_ptr<PipelineLayer> wellLayer_;

    std::string workspaceName_;
    std::string name_;

    bool isVisible_;

    GWPropertyNode_ptr propsNode_;
};

}
#endif // gwPipelineKind_h__
