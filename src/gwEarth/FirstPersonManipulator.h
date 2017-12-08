/**************************************************************************************
 *     Author  : T先生 （BigDog）
 *     Date    : 2017/03/03
 *     Describe: 本模块初衷就是实现一个类似于CS的第一人称视角射击漫游器
 *
 *   References:《Improved Collision detection and Response》      Kasper Fauerby
 *			    《基于OSG的分布式汽车驾驶模拟器运行仿真及碰撞检测研究》 汪璇
 *				《微观交通仿真中的碰撞检测算法研究》                  何伟
 **************************************************************************************/
#ifndef GWEARTH_FIRSTPERSONMANIPULATOR_H
#define GWEARTH_FIRSTPERSONMANIPULATOR_H 1

#include <Compiler.h>
#include <gwEarth/export.h>
#include <osgViewer/Viewer>
#include <osgGA/CameraManipulator>
#include <osgUtil/LineSegmentIntersector>

using namespace std;
using namespace osgGA;

namespace gwEarth
{
	class GWEARTH_EXPORT FirstPersonManipulator : public osgGA::CameraManipulator
	{
	public:
		explicit FirstPersonManipulator(void);
		explicit FirstPersonManipulator(const FirstPersonManipulator &param, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

		void initParameters();

		virtual ~FirstPersonManipulator();

		virtual const char* className() const { return "FirstPersonManipulator"; }

		/** Get the position of the matrix manipulator using a 4x4 Matrix.*/
		virtual void setByMatrix(const osg::Matrixd& matrix);

		/** Set the position of the matrix manipulator using a 4x4 Matrix.*/
		virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }

		/** Get the position of the manipulator as 4x4 Matrix.*/
		virtual osg::Matrixd getMatrix() const;

		/** Get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
		virtual osg::Matrixd getInverseMatrix() const;

		virtual void setNode(osg::Node*);

		virtual const osg::Node* getNode() const;

		virtual osg::Node* getNode();

		virtual void computeHomePosition();

		virtual void home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
		virtual void home(double /*unused*/);
		virtual void init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

		/** Get the keyboard and mouse usage of this manipulator.*/
		virtual void getUsage(osg::ApplicationUsage& usage) const;

		void setModelScale(double in_ms) { _modelScale = in_ms; }
		double getModelScale() const { return _modelScale; }

		void setVelocity(double in_vel) { _velocity = in_vel; }
		double getVelocity() const { return _velocity; }

		void setHeight(double in_h) { _stature = in_h; }
		double getHeight() const { return _stature; }

		void setSpeedFactor(float factor) { _speedFactor = factor; _tempSpeedFactor = factor; }
		float getSpeedFactor() { return _speedFactor; }
		void setRotateSpeed(float fac) { _rotateSpeed = fac; }
		virtual void computePosition(const osg::Vec3 &vecEye, const osg::Vec3 &vecCenter, const osg::Vec3 &vecUp);

		virtual void resetCamera(void);

		bool intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection, osg::Vec3d& normal) const;

		//重设两个键盘、鼠标事件;
		void flushMouseEventStack();
		void flushKBEventStack();

		//添加鼠标、键盘事件;
		void addMouseEvent(const osgGA::GUIEventAdapter& ea);
		void addKBEvent(const osgGA::GUIEventAdapter& ea);

		//计算相机移动;
		bool calcMovement();

		osg::Vec3 getFinalPosition(osg::Vec3 oldPosition, osg::Vec3 speed);
		osg::Vec3 getPosition(int recursionDepth, osg::Vec3 oldPosition, osg::Vec3 speed);

		void stopInertia() { _inertiaSpeed = osg::Vec3d(0.0, 0.0, 0.0); }
	private:
		//两次鼠标事件;
		osg::ref_ptr<const GUIEventAdapter> _ga_t1;
		osg::ref_ptr<const GUIEventAdapter> _ga_t0;

		//两次键盘事件;
		osg::ref_ptr<const GUIEventAdapter> _ga_kb_t1;
		osg::ref_ptr<const GUIEventAdapter> _ga_kb_t0;

		osg::observer_ptr<osg::Node>    _node;

		double _modelScale;
		double _velocity;
		double _velocityAD;
		double _stature;              //身高，即相机位置距地面的距离;
		double _buffer;
		float  _speedFactor;          //速度因子，因子越大速度越快;
		float  _tempSpeedFactor;
		osg::Vec3d   _eye;
		osg::Quat    _rotation;
		double       _pitch;

		bool		_pitchWKeyPressed;
		bool		_pitchSKeyPressed;
		bool		_pitchDKeyPressed;
		bool		_pitchAKeyPressed;

		bool        _shiftKeyPressed;

		double		_speedX, _speedY;
		float		_velocityJump;
		float       _jumpFactor;
		float       _rotateSpeed;

		bool		_isFrame;
		bool        _isDrag;
		bool        _isKBControl;    //是不是键盘操纵相机;
		bool        _inertia;
		osg::Vec3d  _inertiaSpeed;
		float       _gravity;

	protected:
		virtual bool DoLMouseDragNavigation(const osgGA::GUIEventAdapter &ea) { return false; };
		virtual bool DoRMouseDragNavigation(const osgGA::GUIEventAdapter &ea) { return false; };
		virtual bool DoMMouseDragNavigation(const osgGA::GUIEventAdapter &ea) { return false; };
		virtual bool DoKeyNavigation(void) { return false; }

	};
}

#endif // GWEARTH_FIRSTPERSONMANIPULATOR_H
