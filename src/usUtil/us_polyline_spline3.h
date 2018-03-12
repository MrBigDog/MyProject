#ifndef US_POLYLINE_SPLINE3_H_
#define US_POLYLINE_SPLINE3_H_

//#include <usUtil\Export.h>

namespace uniscope_globe
{
	template <class real>
	class polyline_spline3 : public tcb_spline3<real>
	{
	public:
		polyline_spline3(int iSegments, real* afTime, vector3<real>* akPoint,
			real* afTension, real* afContinuity, real* afBias);
		virtual ~polyline_spline3(void);

	public:
		virtual vector3<real> get_position(real fTime) const;
	};

	template <class real>
	polyline_spline3<real>::polyline_spline3(int iSegments, real* afTime, vector3<real>* akPoint, real* afTension, real* afContinuity, real* afBias)
		: tcb_spline3<real>(iSegments, afTime, akPoint, afTension, afContinuity, afBias)
	{

	}

	template <class real>
	polyline_spline3<real>::~polyline_spline3(void)
	{

	}

	template <class real>
	vector3<real> polyline_spline3<real>::get_position(real fTime)  const
	{
		int iKey;
		real fDt;
		get_key_info(fTime, iKey, fDt);

		fDt /= (m_afTime[iKey + 1] - m_afTime[iKey]);

		vector3<real> dir = m_akPoint[iKey + 1] - m_akPoint[iKey];
		real len = dir.length();
		dir.normalize();

		vector3<real> kResult = m_akA[iKey] + dir * fDt * len;

		return kResult;
	}

}

#endif // US_POLYLINE_SPLINE3_H_