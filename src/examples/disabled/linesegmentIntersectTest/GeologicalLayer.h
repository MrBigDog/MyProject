#ifndef GEOLOGICALLAYER_H
#define GEOLOGICALLAYER_H 1

#include "TileRegister.h"
#include <osg/Node>
#include <osg/observer_ptr>
#include <string>

//----------自定义地质体模型Source
class GeologicalModelSource : public osg::Referenced
{
public:
	GeologicalModelSource() {}
	~GeologicalModelSource() {}

	osg::Node* createNode()
	{
		osg::ref_ptr<osg::Node> node = createNodeImplementation();
		return node.release();
	}

private:
	virtual osg::Node* createNodeImplementation() = 0;
};

//
class GeologicalLayer0 :public osg::Referenced
{
public:
	struct Callback :public osg::Referenced
	{
		virtual void onVisibleChanged(bool isVisible) {}
		virtual void onLightEnableChanged(bool isLightEnable) {}
	};
	typedef std::vector<osg::ref_ptr<Callback> > CallbackVector;

public:
	GeologicalLayer0(const std::string& name, osg::Node* node);
	GeologicalLayer0(const std::string& name, const std::string& url);
	GeologicalLayer0(const std::string& name, GeologicalModelSource* modelSource);

	~GeologicalLayer0();

	const std::string& getName() const;

	TileRegister* getTileRegister() const { return _tileRegister; }

	void setVisible(bool isVisible);
	bool getVisible() const;

	void setLightEnable(bool isLightEnable);
	bool getLightEnable() const;

	void addCallback(Callback* cb);
	void removeCallback(Callback* cb);

	osg::Node* getSceneGraph() const { return _graph.get(); }
	osg::Node* getOrCreateSceneGraph();

private:
	bool _isVisible;
	bool _isLightEnable;

	std::string _name;
	CallbackVector _callbacks;
	osg::observer_ptr<osg::Node> _graph;
	osg::ref_ptr<GeologicalModelSource> _modelSource;
	osg::ref_ptr<TileRegister> _tileRegister;
};

typedef std::vector<osg::ref_ptr<GeologicalLayer0> > GeologicalLayer0Vector;

#endif // GeologicalLayer_h__
