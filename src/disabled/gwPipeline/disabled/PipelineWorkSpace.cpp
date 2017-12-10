#include <gwEarth/pipeline/PipelineWorkSpace.h>

namespace gwEarth
{

	///////////////////////////////////////////////////
	pipeWorkspace::pipeWorkspace(GWPropertyNode *propsNode)
		: propsNode_(propsNode)
	{

	}

	pipeWorkspace::~pipeWorkspace()
	{
		std::map<std::string, pipeKind*>::iterator it = pipeKindMap_.begin();
		for(; it!=pipeKindMap_.end(); ++it)
		{
			pipeKind* pk = it->second;
			if(pk && pk!=NULL)
			{
				delete pk;
				pk = NULL;
			}
			pipeKindMap_.erase(it);
		}

		propsNode_->removeAllChildren();
	}

	void pipeWorkspace::setVisible(bool isVisible)
	{
		if(propsNode_->getBoolValue("visible") == isVisible) return;

		std::map<std::string, pipeKind*>::iterator it = pipeKindMap_.begin();
		for(; it!= pipeKindMap_.end(); ++it)
		{
			pipeKind* pk = it->second;
			if(!pk||pk==NULL) continue;

			pk->setVisible(isVisible);
		}

		propsNode_->setBoolValue("visible", isVisible);
	}

	bool pipeWorkspace::getVisible()
	{
		return propsNode_->getBoolValue("visible");
	}

	void pipeWorkspace::setName(const std::string& name)
	{
		propsNode_->setStringValue("name", name);
	}

	std::string pipeWorkspace::getName()
	{
		return propsNode_->getStringValue("name");
	}

	void pipeWorkspace::addPipeKind(pipeKind* pipekind)
	{
		if(!pipekind || pipekind==NULL) return;
		std::string kindname = pipekind->getName();
		pipeKindMap_[kindname] = pipekind;

		//propsNode_->addChild(pipekind->getName());
	}

	void pipeWorkspace::removePipeKind(const std::string& kindName)
	{
		pipeKind* pk = pipeKindMap_[kindName];
		if(pk && pk!=NULL)
		{
			delete pk;
			pk = NULL;
		}
		pipeKindMap_.erase(kindName);

		propsNode_->removeChild(kindName);
	}

	void pipeWorkspace::removePipeKind(pipeKind *pipekind)
	{
		if(!pipekind || pipekind==NULL) return;

		delete pipekind;
		pipekind = NULL;

		pipeKindMap_.erase(pipekind->getName());

		propsNode_->removeChild(pipekind->getName());
	}

	void pipeWorkspace::setPipeKindVisible(const std::string& kindName, bool isVisible)
	{
		pipeKind* pk = pipeKindMap_[kindName];

		if(!pk || pk==NULL) return;

		pk->setVisible(isVisible);
	}

	bool pipeWorkspace::getPipeKindVisible(const std::string& kindName)
	{
		pipeKind* pk = pipeKindMap_[kindName];
		if(!pk || pk==NULL) return false;

		return pk->getVisible();
	}

}
