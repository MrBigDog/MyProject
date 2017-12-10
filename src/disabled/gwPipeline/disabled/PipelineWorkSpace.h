#ifndef gwPipelineWorkSpace_h__
#define gwPipelineWorkSpace_h__


#include <gwEarth/pipeline/PipelineKind.h>
#include <gwBase/props/props.h>

#include <string>
#include <vector>
#include <map>

namespace gwEarth
{

class pipeWorkspaceCallback
{
public:
    pipeWorkspaceCallback() {}

    virtual void onPipeKindAdd()=0;
    virtual void onPipeKindRemove()=0;
};
//

class pipeWorkspace
{
public:
//    pipeWorkspace();
//    pipeWorkspace(const std::string& workspaceName);
    pipeWorkspace( GWPropertyNode* propsNode);
    ~pipeWorkspace();

    void setVisible(bool isVisible);
    bool getVisible();

    void setName(const std::string& name);
    std::string getName();

    void addPipeKind(pipeKind* pipekind);

    void removePipeKind(const std::string& kindName);
    void removePipeKind(pipeKind* pipekind);

    void setPipeKindVisible(const std::string& kindName, bool isVisible);
    void setPipeKindVisible(pipeKind* pipekind, bool isVisible);

    bool getPipeKindVisible(const std::string& kindName);
    bool getPipeKindVisible(pipeKind* pipekind);

    void addCallback(pipeWorkspaceCallback* cb);
    void removeCallback(pipeWorkspaceCallback* cb);
    void fireCallback();




private:
    std::map<std::string, pipeKind*> pipeKindMap_;
    std::vector<pipeWorkspaceCallback*> callbackList_;

    GWPropertyNode_ptr propsNode_;
};

}

#endif // gwPipelineWorkSpace_h__













