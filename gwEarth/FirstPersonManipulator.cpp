#include <gwEarth/FirstPersonManipulator.h>
#include <iostream>

#define STATURE_BUFFER 0.05
#define VeryCloseDistance 0.5

namespace gwEarth
{
	static double getHeightOfDriver()
	{
		double height = 1.5;
		if (getenv("OSG_DRIVE_MANIPULATOR_HEIGHT"))
		{
			height = osg::asciiToDouble(getenv("OSG_DRIVE_MANIPULATOR_HEIGHT"));
		}
		std::cout << "FirstPersonManipulator::_height set to ==" << height << std::endl;
		return height;
	}

	FirstPersonManipulator::FirstPersonManipulator(const FirstPersonManipulator &param, const osg::CopyOp &copyop)
	{
		initParameters();
	}

	FirstPersonManipulator::FirstPersonManipulator()
	{
		initParameters();
	}

	FirstPersonManipulator::~FirstPersonManipulator()
	{
	}

	void FirstPersonManipulator::initParameters()
	{
		_modelScale = 0.6;
		_velocity = 0.0;
		_velocityAD = 0.0;
		_stature = getHeightOfDriver();
		_buffer = _stature;
		_pitch = 0.0;
		_speedFactor = 1.0;
		_tempSpeedFactor = _speedFactor;
		_isDrag = false;
		_inertia = false;
		_pitchWKeyPressed = false;
		_pitchAKeyPressed = false;
		_pitchSKeyPressed = false;
		_pitchDKeyPressed = false;
		_shiftKeyPressed = false;

		_inertiaSpeed = osg::Vec3d(0.0, 0.0, 0.0);
		_speedX = 0.0;
		_speedY = 0.0;

		_jumpFactor = 1.0;
		_rotateSpeed = 1.0;
		_gravity = -4.9;
		home(0);
		//_homeEye = osg::Vec3d(_homeEye._v[0], _homeEye._v[1]+1000, _homeEye._v[2]+2000);
	}

	void FirstPersonManipulator::setNode(osg::Node* node)
	{
		_node = node;
		if (_node.get())
		{
			const osg::BoundingSphere& boundingSphere = _node->getBound();

			_modelScale = boundingSphere._radius;
			//_height = sqrtf(_modelScale)*0.03;
			//_buffer = sqrtf(_modelScale)*0.05;

			_stature = getHeightOfDriver();
			_buffer = _stature;
		}
		if (getAutoComputeHomePosition()) computeHomePosition();
	}

	const osg::Node* FirstPersonManipulator::getNode() const
	{
		return _node.get();
	}

	osg::Node* FirstPersonManipulator::getNode()
	{
		return _node.get();
	}

	bool FirstPersonManipulator::intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection, osg::Vec3d& normal) const
	{
		osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi = new osgUtil::LineSegmentIntersector(start, end);

		osgUtil::IntersectionVisitor iv(lsi.get());
		iv.setTraversalMask(_intersectTraversalMask);

		_node->accept(iv);

		if (lsi->containsIntersections())
		{
			intersection = lsi->getIntersections().begin()->getWorldIntersectPoint();
			normal = lsi->getIntersections().begin()->getWorldIntersectNormal();
			return true;
		}
		return false;
	}

	void FirstPersonManipulator::computeHomePosition()
	{
		if (_node.get())
		{
			const osg::BoundingSphere& boundingSphere = _node->getBound();

			osg::Vec3d ep = boundingSphere._center;
			osg::Vec3d bp = ep;

			osg::CoordinateFrame cf = getCoordinateFrame(ep);

			ep -= getUpVector(cf)* _modelScale*0.0001;
			bp -= getUpVector(cf)* _modelScale;

			// check to see if any obstruction in front.
			bool positionSet = false;

			osg::Vec3d ip, np;
			if (intersect(ep, bp, ip, np))
			{
				osg::Vec3d uv;
				if (np * getUpVector(cf) > 0.0) uv = np;
				else uv = -np;

				ep = ip;
				ep += getUpVector(cf)*_stature;
				osg::Vec3d lv = uv^osg::Vec3d(1.0, 0.0, 0.0);

				setHomePosition(ep, ep + lv, uv);

				positionSet = true;
			}

			if (!positionSet)
			{
				bp = ep;
				bp += getUpVector(cf)*_modelScale;

				if (intersect(ep, bp, ip, np))
				{
					osg::Vec3d uv;
					if (np*getUpVector(cf) > 0.0) uv = np;
					else uv = -np;

					ep = ip;
					ep += getUpVector(cf)*_stature;
					osg::Vec3d lv = uv^osg::Vec3d(1.0, 0.0, 0.0);
					setHomePosition(ep, ep + lv, uv);

					positionSet = true;
				}
			}

			if (!positionSet)
			{
				setHomePosition(
					boundingSphere._center + osg::Vec3d(0.0, -2.0 * boundingSphere._radius, 0.0),
					boundingSphere._center + osg::Vec3d(0.0, -2.0 * boundingSphere._radius, 0.0) + osg::Vec3d(0.0, 1.0, 0.0),
					osg::Vec3d(0.0, 0.0, 1.0));
			}
		}
	}

	void FirstPersonManipulator::home(const GUIEventAdapter& ea, GUIActionAdapter& us)
	{
		if (getAutoComputeHomePosition()) computeHomePosition();

		computePosition(_homeEye, _homeCenter, _homeUp);

		_velocity = 0.0;

		_pitch = 0.0;

		us.requestRedraw();
		us.requestContinuousUpdate(false);

		//us.requestWarpPointer((ea.getXmin()+ea.getXmax())/2.0f,(ea.getYmin()+ea.getYmax())/2.0f);
		flushMouseEventStack();
	}

	void FirstPersonManipulator::home(double)
	{
		_homeEye = osg::Vec3d(0, 0, 1);
		_homeCenter = osg::Vec3d(0, 1, 0);
		_homeUp = osg::Vec3d(0, 0, 1);
		computePosition(_homeEye, _homeCenter, _homeUp);
		_eye += osg::Vec3d(0, 110, 0);
		_velocity = 0.0;

		_pitch = 0.0;

		flushMouseEventStack();
	}

	void FirstPersonManipulator::init(const GUIEventAdapter& ea, GUIActionAdapter& us)
	{
		flushMouseEventStack();
		flushKBEventStack();

		us.requestContinuousUpdate(false);

		_velocity = 0.0;

		osg::Vec3d ep = _eye;

		osg::CoordinateFrame cf = getCoordinateFrame(ep);

		osg::Matrixd rotation_matrix;
		rotation_matrix.makeRotate(_rotation);
		osg::Vec3d sv = osg::Vec3d(1.0, 0.0, 0.0) * rotation_matrix;
		osg::Vec3d bp = ep;
		bp -= getUpVector(cf)*_modelScale;

		bool positionSet = false;
		osg::Vec3d ip, np;
		if (intersect(ep, bp, ip, np))
		{
			osg::Vec3d uv;
			if (np*getUpVector(cf) > 0.0) uv = np;
			else uv = -np;

			ep = ip + uv*_stature;
			osg::Vec3d lv = uv^sv;

			computePosition(ep, ep + lv, uv);

			positionSet = true;
		}
		if (!positionSet)
		{
			bp = ep;
			bp += getUpVector(cf)*_modelScale;

			if (intersect(ep, bp, ip, np))
			{
				osg::Vec3d uv;
				if (np*getUpVector(cf) > 0.0f) uv = np;
				else uv = -np;

				ep = ip + uv*_stature;
				osg::Vec3d lv = uv^sv;

				computePosition(ep, ep + lv, uv);

				positionSet = true;
			}
		}
		if (ea.getEventType() != GUIEventAdapter::RESIZE)
		{
			us.requestWarpPointer((ea.getXmin() + ea.getXmax()) / 2.0f, (ea.getYmin() + ea.getYmax()) / 2.0f);
		}
	}

	bool FirstPersonManipulator::handle(const GUIEventAdapter& ea, GUIActionAdapter& us)
	{
		if (ea.getEventType() == GUIEventAdapter::FRAME)
		{
			addKBEvent(ea);
			_isFrame = true;
			if (calcMovement()) us.requestRedraw();

			return false;
		}

		if (ea.getHandled()) return false;

		switch (ea.getEventType())
		{
		case(GUIEventAdapter::MOVE):
		{
			return true;
		}
		case(GUIEventAdapter::KEYDOWN):
		{
			stopInertia();
			if (ea.getKey() == '0')
			{
				//show the cursor
				osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&us);
				osgViewer::Viewer::Windows w;
				viewer->getWindows(w);
				for (osgViewer::Viewer::Windows::iterator itr = w.begin(); itr != w.end(); ++itr)
				{
					(*itr)->useCursor(osgViewer::GraphicsWindow::LeftArrowCursor);
				}
				return true;
			}

			//	W A S D
			else if (ea.getKey() == 'R' || ea.getKey() == 'r')
			{
				_pitchWKeyPressed = true;
			}
			else if (ea.getKey() == 'D' || ea.getKey() == 'd')
			{
				_pitchAKeyPressed = true;
			}
			else if (ea.getKey() == 'F' || ea.getKey() == 'f')
			{
				_pitchSKeyPressed = true;
			}
			else if (ea.getKey() == 'g' || ea.getKey() == 'G')
			{
				_pitchDKeyPressed = true;
			}
			else if (ea.getKey() == GUIEventAdapter::KEY_Space)
			{
				_velocityJump = _jumpFactor*_stature;
				return true;
			}
			else if (ea.getKey() == GUIEventAdapter::KEY_Shift_L || ea.getKey() == GUIEventAdapter::KEY_Shift_R)
			{
				_shiftKeyPressed = true;
			}
			else if (ea.getKey() == 'U' || ea.getKey() == 'u')
			{
				//��߲�����500
				if (_stature <= 500)
				{
					osg::Vec3d np, ip;
					//������߲���Խ���컨��;
					if (!intersect(_eye, osg::Vec3(_eye._v[0], _eye._v[1], _eye._v[2] + 0.1), ip, np))
					{
						_stature += 0.1;
					}
				}
			}
			else if (ea.getKey() == 'L' || ea.getKey() == 'l')
			{
				if (_stature > 0.1)
				{
					_stature -= 0.1;
				}
			}

			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)
			{
				_speedY = _rotateSpeed;
				_speedX = 0;
				_isKBControl = true;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)
			{
				_speedX = 0;
				_speedY = -_rotateSpeed;
				_isKBControl = true;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)
			{
				_speedY = 0;
				_speedX = _rotateSpeed;
				_isKBControl = true;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)
			{
				_speedY = 0;
				_speedX = -_rotateSpeed;
				_isKBControl = true;
			}

			break;
		}
		case(GUIEventAdapter::KEYUP):
		{
			//	flushKBEventStack();
			//	W A S D
			if (ea.getKey() == 'R' || ea.getKey() == 'r')
			{
				_pitchWKeyPressed = false;
			}
			else if (ea.getKey() == 'D' || ea.getKey() == 'd')
			{
				_pitchAKeyPressed = false;
			}
			else if (ea.getKey() == 'F' || ea.getKey() == 'f')
			{
				_pitchSKeyPressed = false;
			}
			else if (ea.getKey() == 'G' || ea.getKey() == 'g')
			{
				_pitchDKeyPressed = false;
			}
			else if (ea.getKey() == GUIEventAdapter::KEY_Shift_L || ea.getKey() == GUIEventAdapter::KEY_Shift_R)
			{
				_shiftKeyPressed = false;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up || ea.getKey() == osgGA::GUIEventAdapter::KEY_Down ||
				ea.getKey() == osgGA::GUIEventAdapter::KEY_Left || ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)
			{
				_isKBControl = false;
			}
			break;
		}

		case(osgGA::GUIEventAdapter::PUSH):
		{
			addMouseEvent(ea);
			return 0;
		}
		case(GUIEventAdapter::DRAG):
		{
			addMouseEvent(ea);
			_isDrag = true;
			return 0;
		}

		case(GUIEventAdapter::RELEASE):
		{
			flushMouseEventStack();
			return 0;
		}
		}

		return true;
	}

	void FirstPersonManipulator::getUsage(osg::ApplicationUsage& usage) const
	{
		usage.addKeyboardMouseBinding("Drive: Space", "Reset the viewing position to home");
		usage.addKeyboardMouseBinding("Drive: q", "Use mouse y for controlling speed");
		usage.addKeyboardMouseBinding("Drive: a", "Use mouse middle,right mouse buttons for speed");
		usage.addKeyboardMouseBinding("Drive: Down", "Cursor down key to look downwards");
		usage.addKeyboardMouseBinding("Drive: Up", "Cursor up key to look upwards");
	}

	void FirstPersonManipulator::flushKBEventStack()
	{
		_ga_kb_t0 = NULL;
		_ga_kb_t1 = NULL;
	}

	void FirstPersonManipulator::flushMouseEventStack()
	{
		_ga_t1 = NULL;
		_ga_t0 = NULL;
		_isKBControl = false;
		stopInertia();
	}

	void FirstPersonManipulator::addKBEvent(const GUIEventAdapter& ea)
	{
		_ga_kb_t1 = _ga_kb_t0;
		_ga_kb_t0 = &ea;
	}
	void FirstPersonManipulator::addMouseEvent(const GUIEventAdapter& ea)
	{
		_ga_t1 = _ga_t0;
		_ga_t0 = &ea;
		_isKBControl = false;
		stopInertia();
	}

	void FirstPersonManipulator::setByMatrix(const osg::Matrixd& matrix)
	{
		_eye = matrix.getTrans();

		_rotation = matrix.getRotate();

		osg::Matrixd rotation_matrix;
		rotation_matrix.makeRotate(_rotation);

		osg::Vec3d up = osg::Vec3d(0.0, 0.0, 1.0);
		osg::Vec3d aheadV = osg::Vec3d(0.0, 0.0, -1.0) * rotation_matrix;
		osg::Vec3d sv = aheadV ^ up;

		osg::Matrixd rotation_matrix2(sv[0], up[0], -aheadV[0], 0.0,
			sv[1], up[1], -aheadV[1], 0.0,
			sv[2], up[2], -aheadV[2], 0.0,
			0.0, 0.0, 0.0, 1.0);
		_rotation = rotation_matrix2.getRotate().inverse();
		_pitch = 0;
	}

	osg::Matrixd FirstPersonManipulator::getMatrix() const
	{
		//return osg::Matrixd::rotate(_rotation);
		return osg::Matrixd::rotate(_pitch, 1.0, 0.0, 0.0)*osg::Matrixd::rotate(_rotation)*osg::Matrixd::translate(_eye);
	}

	osg::Matrixd FirstPersonManipulator::getInverseMatrix() const
	{
		//	return osg::Matrixd::translate(-_eye)*osg::Matrixd::rotate(-_pitch,1.0,0.0,0.0);
		return osg::Matrixd::translate(-_eye)*osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::rotate(-_pitch, 1.0, 0.0, 0.0);
	}

	void FirstPersonManipulator::computePosition(const osg::Vec3 &vecEye, const osg::Vec3 &vecCenter, const osg::Vec3 &vecUp)
	{
		osg::Vec3d lv = vecCenter - vecEye;

		osg::Vec3d f(lv);
		f.normalize();
		osg::Vec3d s(f^vecUp);
		s.normalize();
		osg::Vec3d u(s^f);
		u.normalize();

		osg::Matrixd rotation_matrix(
			s[0], u[0], -f[0], 0.0,
			s[1], u[1], -f[1], 0.0,
			s[2], u[2], -f[2], 0.0,
			0.0, 0.0, 0.0, 1.0);

		_eye = vecEye;
		_rotation = rotation_matrix.getRotate().inverse();
	}

	bool FirstPersonManipulator::calcMovement()
	{
		bool mouseEventValid = false;
		bool kbEventValid = false;

		//�ж��Ƿ���������������߼����¼�;
		if (!(_ga_t0.get() == NULL || _ga_t1.get() == NULL)) mouseEventValid = true;
		if (!(_ga_kb_t0.get() == NULL || _ga_kb_t1.get() == NULL)) kbEventValid = true;

		double dt = 0.0;

		//��ȡ���������¼�֮���ʱ��;
		if (kbEventValid)
		{
			dt = _ga_kb_t0->getTime() - _ga_kb_t1->getTime();
		}

		if (dt <= 0.001f)
		{
			dt = 0.0;
			return false;
		}

#pragma region �ƶ��ٶȺϳ�

		//�����ٶ�;
		{
			//WS��ǰ��
			if (_pitchWKeyPressed && !_pitchSKeyPressed)
				_velocity = 6 * _speedFactor;
			else if (_pitchSKeyPressed && !_pitchWKeyPressed)
				_velocity = -6 * _speedFactor;
			else
				_velocity = 0.0;

			//AD�� ���� �����ƶ��ٶ���ǰ���ƶ��ٶȵ�1/2�����ӽ���ʵ;
			if (_pitchAKeyPressed && !_pitchDKeyPressed)
			{
				_velocityAD = -3 * _speedFactor;
			}
			else if (_pitchDKeyPressed && !_pitchAKeyPressed)
			{
				_velocityAD = 3 * _speedFactor;
			}
			else
			{
				_velocityAD = 0.0;
			}

			//���ٶ�;
			if (_shiftKeyPressed == true)
			{
				_speedFactor = 3.0;
			}
			else
			{
				_speedFactor = _tempSpeedFactor;
			}

			//jump
			if (_isFrame)
			{
				//��Ծ���ԣ���ͷ�Ƶ���Ծ�߶Ⱥ���һ��������ٶ�����;
				_velocityJump += _gravity * dt;
				_isFrame = false;
			}
		}

#pragma endregion 

		osg::CoordinateFrame cf = getCoordinateFrame(_eye);

		osg::Matrixd rotation_matrix;
		rotation_matrix.makeRotate(_rotation);

		osg::Vec3d up = osg::Vec3d(0.0, 0.0, 1.0);
		osg::Vec3d aheadV = osg::Vec3d(0.0, 0.0, -1.0) * rotation_matrix;
		osg::Vec3d sv = aheadV ^ up;

		aheadV.normalize();

#pragma region  ���������ӽ���ת 

		// ��������ƶ��¼���ֻ������������¼��������϶�����ʱ���߼��̿���ʱ�ſ��Դ���
		//��ģ����Ҫ��������ӽ���������ת�Ĳ�����speedX���Ƶ��Ƿ�λ�ǣ�speedY���Ƶ��Ǹ�����
		if (mouseEventValid && _isDrag || _isKBControl)
		{
			if (!_isKBControl)
			{
				_speedX = _ga_t1->getX() - _ga_t0->getX();
				_speedY = _ga_t1->getY() - _ga_t0->getY();
			}
			if (_speedX != 0 || _speedY != 0)
			{
				//moveSpeedFactor������ƶ������ת�ǶȵĿ��Ʊ�������ֵԽ�����
				//�ƶ������ٶ�Խ��;
				float moveSpeedFactor = 0.001;
				_pitch += _speedY * moveSpeedFactor*_rotateSpeed;

				if (_pitch > osg::PI)
					_pitch -= osg::PI;

				osg::Quat yaw_rotation;
				yaw_rotation.makeRotate(-_speedX * moveSpeedFactor*_rotateSpeed, up);

				_rotation *= yaw_rotation;			//	new rotation quaternion  based on mouseX

				rotation_matrix.makeRotate(_rotation);
				aheadV = osg::Vec3d(0.0, 0.0, -1.0)  * rotation_matrix;

				sv = aheadV ^ up;
				sv.normalize();
			}
			_isDrag = false;
		}
#pragma endregion 

#pragma region ���������ƶ�

		//��������¼�;
		if (kbEventValid)
		{
			if (fabs(_velocity*dt) > 1e-8 || fabs(_velocityAD*dt) > 1e-8 || fabs(_velocityJump*dt) > 1e-8)
			{
				osg::Vec3d speed = (aheadV*_velocity + sv*_velocityAD);
				if (speed.length2() > 0.0)
				{
					_inertiaSpeed = speed;
				}
				_eye = getFinalPosition(_eye, speed*dt);
			}
		}
#pragma endregion 

#pragma region ����

		//���ԣ�����ǰ���������˼����������ҺͿ�������ģʽ��������ſ��Խ��й���
	// 	if (_pitchAKeyPressed==false&&
	// 		_pitchDKeyPressed==false&&
	// 		_pitchSKeyPressed==false&&
	// 		_pitchWKeyPressed==false&&
	// 		_inertia&&
	// 		_inertiaSpeed.length2()>0.0)
	// 	{
	// 		_eye = getFinalPosition(_eye, _inertiaSpeed*dt);
	// 	}
#pragma endregion

		return true;
	}

	//��ȡһ�����յ�eye�㣬eye��ˮƽ�ƶ��ʹ�ֱ���������ƶ��ϳɡ�;
	osg::Vec3 FirstPersonManipulator::getFinalPosition(osg::Vec3 oldPosition, osg::Vec3 speed)
	{
		osg::Matrixd rotation_matrix;
		rotation_matrix.makeRotate(_rotation);

		osg::Vec3d up = osg::Vec3d(0.0, 0.0, 1.0);
		osg::Vec3d aheadV = osg::Vec3d(0.0, 0.0, -1.0)  * rotation_matrix;
		osg::Vec3d sv = aheadV ^ up;

		aheadV.normalize();

		//�����ٶȼ����ǰ���ľ���;
		osg::Vec3 finalPos;
		float moveDistance = speed.length() + VeryCloseDistance;
		speed.normalize();

		//���������ȡֱ��ǰ�еõ��ĵ�;
		finalPos = getPosition(0, oldPosition, speed*moveDistance);

		//ǰ�еõ��ĵ�����������һ���ϳɵĵ�;
		osg::Vec3 gravity = up*_velocityJump;
		osg::Vec3d np, ip;

		//����if�������������ǣ��������������ĸ����ཻ���Լ��ཻ��ķ�Ӧ��
		//����Ŀ����Ϊ�˵õ�һ�����������ĵ㣨eyeλ�ã�������������ڵ�����ߡ�

		//�жϼ��������ĺϳɵĵ�ϳɺ���ģ����û���ཻ;
		if (intersect(finalPos, finalPos + gravity, ip, np))
		{
			//������ཻ�����ߴ��ཻ�����ƣ��ƶ���eyeλ�õ��������Ӧ�������
			if (_velocityJump <= 0.0)
			{
				/*statureSpace= ��� - ��������������λ�õ�����ľ���					     ;
				��Ҫ����¥�ݵ�ʱ�����staureSpace�ܴ󣬴ֱ���̧�����λ���Ե�			     ;
				�ܲ���������Ե���������������λ�������ܽ���ʱ�����Ծһ��				 ;
				��Ծ�߶ȵ���statureSpace�Ĵ�С�����������������Եú�������					 ;
				STATURE_BUFFER��һ����ֵ����statureSpaceС����ߵĶ��ٱ�ʱ����Ծ��		     ;
				STATURE_BUFFER���ù�С����¥�ݵ�ʱ�������Ƥ��һ�������������ϻ����ٵ�����   ;
				*/
				float statureSpace = _stature - (finalPos + gravity - ip).length();
				if (statureSpace > _stature*STATURE_BUFFER && statureSpace > 1.0)
				{
					_velocityJump = statureSpace;
				}
				else
				{
					finalPos = ip + up*_stature;
					osg::Vec3d ip1, np1;
					//�ж�̧�����ߵ�ʱ���Ƿ����컨����ײ;
					if (intersect(ip + up*_stature*0.1, finalPos, ip1, np1))
					{
						finalPos = oldPosition;
					}
					_velocityJump = 0.0;
				}
				aheadV = up^sv;
			}
			else
				_velocityJump = 0.0;

			computePosition(_eye, _eye + aheadV, up);
		}
		else
		{
			//û���ཻ���жϺϳɺ�ĵ㵽��������Ƿ�������;
			osg::Vec3d fp = finalPos + gravity;
			osg::Vec3d lfp = fp - up*(_stature*1.1);

			//������ཻ���ԣ�����ɹ��ʹ��ཻ�㽫��������;
			if (intersect(fp, lfp, ip, np))
			{
				if ((ip - lfp).length() < _stature*0.8)
				{
					float statureSpace = _stature - (finalPos + gravity - ip).length();

					//statureSpace������ߵ�ĳ������ֵ���ߴ���1�׾���Ծ����������ֱ��̧������ӵ㡣��Ϊ��statureSpace
					//�ǳ���ʱ���ֱ���̧�����ߺܲ������;
					if (statureSpace > _stature*STATURE_BUFFER && statureSpace > 1.0)
					{
						_velocityJump = statureSpace;
					}
					else
					{
						finalPos = ip + up*_stature;
						osg::Vec3d ip1, np1;
						//�ж�̧�����ߵ�ʱ���Ƿ����컨����ײ;
						if (intersect(ip + up*_stature*0.1, finalPos, ip1, np1))
						{
							finalPos = oldPosition;
						}
						_velocityJump = 0.0;
					}
					aheadV = up^sv;
					computePosition(_eye, _eye + aheadV, up);
				}
			}
			else
				//����治�ཻ��ֱ�ӽ�ˮƽ�ƶ�Ӧ�õ��ĵ���������ӡ�;
				finalPos = finalPos + gravity;
		}

		return finalPos;
	}

	//������ˮƽ�ƶ����ݹ������Ӧ����ײ���Լ���ײ��Ӧ�����һ�������ˮƽ�ƶ��㡣
	osg::Vec3 FirstPersonManipulator::getPosition(int recursionDepth, osg::Vec3 oldPosition, osg::Vec3 speed)
	{
		//�ٶ�С��0.002���ߵݹ���ȴ���5�ͷ���
		if (speed.length() < (VeryCloseDistance + 0.002) || recursionDepth > 5)
		{
			return oldPosition;
		}

		osg::Vec3d ip, np;
		osg::Vec3 slideSpeed;

		//�ж�ǰ�к���ģ���Ƿ��н���;
		if (intersect(oldPosition, oldPosition + speed, ip, np))
		{
			//sinAlpha���ٶȷ�������ײ��ķ�����֮��ļн�
			double cosAlpha = (speed*np) / (speed.length()*np.length());
			double Alpha = acosf(cosAlpha);
			double sinAlpha = -sin(Alpha);

			//ͨ���нǽ����ٶȷֽ⣬�����ཻƽ�滬�������㻬���ĵ㡣;
			osg::Vec3 speedBuffer = speed;
			speedBuffer.normalize();
			slideSpeed = (speedBuffer^np) ^ np;
			slideSpeed.normalize();

			float moveDistance = speed.length() - VeryCloseDistance;

			moveDistance = moveDistance*sinAlpha;
			moveDistance -= VeryCloseDistance;
			slideSpeed = slideSpeed*moveDistance;
		}
		else
		{
			//�����һ����û����ģ���н���ͷ��ظõ�;
			float moveDistance = speed.length() - VeryCloseDistance;
			speed.normalize();
			return oldPosition + speed*moveDistance;
		}
		//�����ݹ���㣨��Ҫ����ƽ�滬������һ����;
		return getPosition(++recursionDepth, oldPosition, slideSpeed);
	}

	void FirstPersonManipulator::resetCamera(void)
	{
		// 	_eye = _originPose.m_ptCameraPos;
		// 	_pitch = _originPose.m_fltPitching;
		// 	osg::Quat yaw_rotation;
		// 	yaw_rotation.makeRotate(_originPose.m_fltAzimuth,m_pCoordinateParam->getReversePlumbLine());
		// 
		// 	_rotation *= yaw_rotation;			//	new rotation quaternion  based on mouseX
	}
}
