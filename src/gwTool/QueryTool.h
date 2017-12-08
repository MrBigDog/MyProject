#ifndef GWTOOL_QUERYTOOL_H
#define GWTOOL_QUERYTOOL_H 1

#include <compiler.h>
#include <gwTool/export.h>
#include <osgEarthUtil/RTTPicker>

namespace gwTool
{
	class GWTOOL_EXPORT QueryTool : public osgEarth::Util::RTTPicker
	{
	public:
		class QueryToolCallback : public osgEarth::Util::RTTPicker::Callback
		{
		public:
			QueryToolCallback();

			void highlightPickResult(const osgEarth::ObjectID& id);
			void cancelHighlight();
		private:
			virtual void onHit(osgEarth::ObjectID id);
			virtual void onMiss();
			virtual bool accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa);
		};

	public:
		QueryTool();
		void removeAllChildren();
		void installHighlighter(osg::StateSet* stateSet, int attrLocation);
		void unstallHighlighter(osg::StateSet* stateSet, int attrLocation);

	private:
	};
}


#endif // QueryTool_h__