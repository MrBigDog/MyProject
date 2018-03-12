///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_triangle.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : triangle class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

template<class real>
triangle<real>::triangle()
{
	m_v[0] = vector3<real>::s_zero;
	m_v[1] = vector3<real>::s_zero;
	m_v[2] = vector3<real>::s_zero;
}

template<class real>
triangle<real>::triangle( const vector3<real>& rkv0, const vector3<real>& rkv1, const vector3<real>& rkv2 )
{
	m_v[0] = rkv0;
	m_v[1] = rkv1;
	m_v[2] = rkv2;
}

template<class real>
triangle<real>::triangle( const vector3<real> akv[3] )
{
	m_v[0] = akv[0];
	m_v[1] = akv[1];
	m_v[2] = akv[2];
}

template<class real>
triangle<real>::~triangle(void)
{
}

template <class real>
real triangle<real>::distance (const vector3<real>& rkq) const
{
	vector3<real> kDiff = m_v[0] - rkq;
	vector3<real> kE0 = m_v[1] - m_v[0], kE1 = m_v[2] - m_v[0];
	real fA00 = kE0.squared_length();
	real fA01 = kE0.dot(kE1);
	real fA11 = kE1.squared_length();
	real fB0 = kDiff.dot(kE0);
	real fB1 = kDiff.dot(kE1);
	real fC = kDiff.squared_length();
	real fDet = math<real>::fabs_(fA00*fA11-fA01*fA01);
	real fS = fA01*fB1-fA11*fB0;
	real fT = fA01*fB0-fA00*fB1;
	real fSqrDist;

	if ( fS + fT <= fDet )
	{
		if ( fS < (real)0.0 )
		{
			if ( fT < (real)0.0 )  // region 4
			{
				if ( fB0 < (real)0.0 )
				{
					if ( -fB0 >= fA00 )
						fSqrDist = fA00+((real)2.0)*fB0+fC;
					else
						fSqrDist = fC-fB0*fB0/fA00;
				}
				else
				{
					if ( fB1 >= (real)0.0 )
						fSqrDist = fC;
					else if ( -fB1 >= fA11 )
						fSqrDist = fA11+((real)2.0)*fB1+fC;
					else
						fSqrDist = fC-fB1*fB1/fA11;
				}
			}
			else  // region 3
			{
				if ( fB1 >= (real)0.0 )
					fSqrDist = fC;
				else if ( -fB1 >= fA11 )
					fSqrDist = fA11+((real)2.0)*fB1+fC;
				else
					fSqrDist = fC-fB1*fB1/fA11;
			}
		}
		else if ( fT < (real)0.0 )  // region 5
		{
			if ( fB0 >= (real)0.0 )
				fSqrDist = fC;
			else if ( -fB0 >= fA00 )
				fSqrDist = fA00+((real)2.0)*fB0+fC;
			else
				fSqrDist = fC-fB0*fB0/fA00;
		}
		else  // region 0
		{
			// minimum at interior point
			real fInvDet = ((real)1.0)/fDet;
			fS *= fInvDet;
			fT *= fInvDet;
			fSqrDist = fS*(fA00*fS+fA01*fT+((real)2.0)*fB0) +
				fT*(fA01*fS+fA11*fT+((real)2.0)*fB1)+fC;
		}
	}
	else
	{
		real fTmp0, fTmp1, fNumer, fDenom;

		if ( fS < (real)0.0 )  // region 2
		{
			fTmp0 = fA01 + fB0;
			fTmp1 = fA11 + fB1;
			if ( fTmp1 > fTmp0 )
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-2.0f*fA01+fA11;
				if ( fNumer >= fDenom )
				{
					fSqrDist = fA00+((real)2.0)*fB0+fC;
				}
				else
				{
					fS = fNumer/fDenom;
					fT = (real)1.0 - fS;
					fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
						fT*(fA01*fS+fA11*fT+((real)2.0)*fB1)+fC;
				}
			}
			else
			{
				if ( fTmp1 <= (real)0.0 )
					fSqrDist = fA11+((real)2.0)*fB1+fC;
				else if ( fB1 >= (real)0.0 )
					fSqrDist = fC;
				else
					fSqrDist = fC-fB1*fB1/fA11;
			}
		}
		else if ( fT < (real)0.0 )  // region 6
		{
			fTmp0 = fA01 + fB1;
			fTmp1 = fA00 + fB0;
			if ( fTmp1 > fTmp0 )
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-((real)2.0)*fA01+fA11;
				if ( fNumer >= fDenom )
				{
					fT = (real)1.0;
					fS = (real)0.0;
					fSqrDist = fA11+((real)2.0)*fB1+fC;
				}
				else
				{
					fT = fNumer/fDenom;
					fS = (real)1.0 - fT;
					fSqrDist = fS*(fA00*fS+fA01*fT+((real)2.0)*fB0) +
						fT*(fA01*fS+fA11*fT+((real)2.0)*fB1)+fC;
				}
			}
			else
			{
				if ( fTmp1 <= (real)0.0 )
					fSqrDist = fA00+((real)2.0)*fB0+fC;
				else if ( fB0 >= (real)0.0 )
					fSqrDist = fC;
				else
					fSqrDist = fC-fB0*fB0/fA00;
			}
		}
		else  // region 1
		{
			fNumer = fA11 + fB1 - fA01 - fB0;
			if ( fNumer <= (real)0.0 )
			{
				fSqrDist = fA11+((real)2.0)*fB1+fC;
			}
			else
			{
				fDenom = fA00-2.0f*fA01+fA11;
				if ( fNumer >= fDenom )
				{
					fSqrDist = fA00+((real)2.0)*fB0+fC;
				}
				else
				{
					fS = fNumer/fDenom;
					fT = (real)1.0 - fS;
					fSqrDist = fS*(fA00*fS+fA01*fT+((real)2.0)*fB0) +
						fT*(fA01*fS+fA11*fT+((real)2.0)*fB1)+fC;
				}
			}
		}
	}

	return math<real>::sqrt_( math<real>::fabs_( fSqrDist) );
}
template <class real>
real triangle<real>::area() const
{
	real v_dis1 = (m_v[0] - m_v[1]).length();
	real v_dis2 = (m_v[0] - m_v[2]).length();
	real v_dis3 = (m_v[1] - m_v[2]).length();
	real v_p = ( v_dis3 + v_dis2 + v_dis1 ) * 0.5;
	v_dis1 = v_p - v_dis1;
	v_dis2 = v_p - v_dis2;
	v_dis3 = v_p - v_dis3;
	v_dis1 = v_dis1<real(0)?real(0):v_dis1;
	v_dis2 = v_dis1<real(0)?real(0):v_dis2;
	v_dis3 = v_dis1<real(0)?real(0):v_dis3;
	real v_area = math_d::sqrt_( v_p * v_dis1 * v_dis2 * v_dis3 );

	return v_area;
}

template <class real>
bool triangle<real>::intersect( ray<real>& in_ray, triangle<real>& in_triangle )
{
	// compute the offset origin, edges, and normal
	vector3<real> k_diff = in_ray.ray_origin - in_triangle.m_v[0];
	vector3<real> k_edge1 = in_triangle.m_v[1] - in_triangle.m_v[0];
	vector3<real> k_edge2 = in_triangle.m_v[2] - in_triangle.m_v[0];
	vector3<real> k_normal = k_edge1.cross(k_edge2);

	// Solve Q + t*D = b1*E1 + b2*E2 (Q = k_diff, D = ray direction,
	// E1 = k_edge1, E2 = k_edge2, N = Cross(E1,E2)) by
	//   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
	//   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
	//   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
	double d_dis_to_normal = in_ray.ray_direction.dot(k_normal);
	double d_sign;
	if ( d_dis_to_normal > math_d::s_zero_tolerance )
	{
		d_sign = 1.0;
	}
	else if ( d_dis_to_normal < -math_d::s_zero_tolerance )
	{
		d_sign = -1.0;
		d_dis_to_normal = -d_dis_to_normal;
	}
	else
	{
		// Ray and triangle are parallel, call it a "no intersection"
		// even if the ray does intersect.
		return false;
	}

	double d_dis_to_qxe2 = d_sign * in_ray.ray_direction.dot( k_diff.cross( k_edge2 ) );
	if ( d_dis_to_qxe2 >= 0.0 )
	{
		double fDdE1xQ = d_sign * in_ray.ray_direction.dot( k_edge1.cross( k_diff ) );
		if ( fDdE1xQ >= 0.0 )
		{
			if ( d_dis_to_qxe2 + fDdE1xQ <= d_dis_to_normal )
			{
				// line intersects triangle, check if ray does
				double fQdN = -d_sign * k_diff.dot( k_normal );

				if ( fQdN >= 0.0 )
				{
					// ray intersects triangle
					return true;
				}
				// else: t < 0, no intersection
			}
			// else: b1+b2 > 1, no intersection
		}
		// else: b2 < 0, no intersection
	}
	// else: b1 < 0, no intersection

	return false;
}

template <class real>
bool triangle<real>::pick( ray<real>& in_ray, triangle<real>& in_triangle, vector3<real>& out_point, real min_t )
{
	vector3<real> rayOrg = in_ray.ray_origin;
	vector3<real> rayDelta = in_ray.ray_direction;

	vector3<real> p0 = in_triangle.m_v[0];
	vector3<real> p1 = in_triangle.m_v[1];
	vector3<real> p2 = in_triangle.m_v[2];

	// closest intersection found so far. ( Start with 1.0 )
	float minT = min_t;

	// compute clockwise edge vectors.
	vector3<real> e1 = ( p1 - p0 );
	vector3<real> e2 = ( p2 - p1 );

	// Compute surface normal. (Unnormalized)
	vector3<real> n = e1.cross( e2 );

	// Compute gradient, which tells us how steep of an angle
	// we are approaching the *front* side of the triangle
	float dot = float( vector3<real>::dot( n , rayDelta ) );

	// Check for a ray that is parallel to the triangle or not
	// pointing toward the front face of the triangle.
	//
	// Note that this also will reject degenerate triangles and
	// rays as well. We code this in a very particular
	// way so that NANs will bail here. (This does not
	// behave the same as "dot >= 0.0f" when NANs are involved.)
	if ( !( dot < 0.0f ) )
	{
		return false;
	}

	// Compute d value for the plane equation. We will
	// use the plane equation with d on the right side://
	// Ax + By + Cz = d
	float d = float( vector3<real>::dot( n , p0 ) );

	// Compute parametric point of intersection with the plane
	// containing the triangle, checking at the earliest
	// possible stages for trivial rejection
	float t = float( d - vector3<real>::dot( n , rayOrg ) );

	// Is ray origin on the backside of the polygon? Again,
	// we phrase the check so that NANs will bail
	if ( !( t <= 0.0f ) )
	{
		return false;
	}

	// Closer intersection already found? (Or does
	// ray not reach the plane?)
	//
	// since dot < 0:
	//
	// t/dot > minT
	//
	// is the same as
	//
	// t < dot*minT
	//
	// (And then we invert it for NAN checking...)
	if ( !( t >= dot * minT ) )
	{
		return false;
	}

	// OK, ray intersects the plane. Compute actual parametric
	// point of intersection

	t /= dot;
	assert( t >= 0.0f );
	assert( t <= minT );

	// Compute 3D point of intersection
	vector3<real> p = rayOrg + rayDelta * t;

	// Find dominant axis to select which plane
	// to project onto, and compute u's and v's
	float u0, u1, u2;
	float v0, v1, v2;
	if ( fabs(n.x) > fabs(n.y) ) 
	{
		if ( fabs(n.x) > fabs(n.z) ) 
		{
			u0 = float( p.y - p0.y );
			u1 = float( p1.y - p0.y );
			u2 = float( p2.y - p0.y );
			v0 = float( p.z - p0.z );
			v1 = float( p1.z - p0.z );
			v2 = float( p2.z - p0.z );
		}
		else
		{
			u0 = float( p.x - p0.x );
			u1 = float( p1.x - p0.x );
			u2 = float( p2.x - p0.x );
			v0 = float( p.y - p0.y );
			v1 = float( p1.y - p0.y );
			v2 = float( p2.y - p0.y );
		}
	} 
	else
	{
		if ( fabs(n.y) > fabs(n.z) ) 
		{
			u0 = float( p.x - p0.x );
			u1 = float( p1.x - p0.x );
			u2 = float( p2.x - p0.x );
			v0 = float( p.z - p0.z );
			v1 = float( p1.z - p0.z );
			v2 = float( p2.z - p0.z );
		}
		else
		{
			u0 = float( p.x - p0.x );
			u1 = float( p1.x - p0.x );
			u2 = float( p2.x - p0.x );
			v0 = float( p.y - p0.y );
			v1 = float( p1.y - p0.y );
			v2 = float( p2.y - p0.y );
		}
	}

	// Compute denominator, check for invalid
	float temp = u1 * v2 - v1 * u2;
	if ( !( temp != 0.0f ) ) 
	{
		return false;
	}
	temp = 1.0f / temp;

	// Compute barycentric coordinates, checking for out-of-range
	// at each step
	float alpha = (u0 * v2 - v0 * u2) * temp;
	if (!(alpha >= 0.0f)) 
	{
		return false;
	}

	float beta = (u1 * v0 - v1 * u0) * temp;
	if (!(beta >= 0.0f)) 
	{
		return false;
	}

	float gamma = 1.0f - alpha - beta;
	if (!(gamma >= 0.0f)) 
	{
		return false;
	}

	// pick point
	out_point = p;

	return true;
}
