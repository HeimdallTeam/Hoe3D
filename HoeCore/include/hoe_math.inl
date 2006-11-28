
#ifndef _HE_MATH_INL__
#define _HE_MATH_INL__


HOE_INLINE float Abs(float f)
{
	if (f >= 0)
		return f;
	else 
		return -f;
}

// Matrix & vector

//////// vector ////////////////////////////////////////

//// 2
HOE_INLINE double Vector2::Magnitude(void) const
{
	return sqrt( (x * x) + (y * y));
}

HOE_INLINE float Vector2::MagnitudeF(void) const
{
	return sqrtf( (x * x) + (y * y));
}


//// 3

HOE_INLINE void Vector3::RotateY(float angle)
{
	const register float s = sinf(angle);
	const register float c = cosf(angle);
	const register float tx = c*x + s*z;
	z = -s*x + c*z;
	x = tx;
}

HOE_INLINE double Vector3::Magnitude(void) const
{
	return sqrt( (x * x) + (y * y) + (z * z) );
}

HOE_INLINE float Vector3::MagnitudeF(void) const
{
	return sqrtf( (x * x) + (y * y) + (z * z) );
}

HOE_INLINE const Vector3 & Vector3::Normalize(void)
{
	float magnitude = (float)Magnitude();			

	x /= magnitude;							
	y /= magnitude;							
	z /= magnitude;						
	
	return *this;
}

HOE_INLINE void Vector3::Multiply(const Vector3 &v,const Matrix &m)
{
	x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
}

HOE_INLINE const Vector3 &  Vector3::Multiply(const Matrix &m)
{
	Vector3 v = *this;
	Multiply(v,m);
	return *this;
}

HOE_INLINE void Vector3::Max(const Vector3 & v)
{
	if (v.x > x) x = v.x;
	if (v.y > y) y = v.y;
	if (v.z > z) z = v.z;
}

HOE_INLINE void Vector3::Min(const Vector3 & v)
{
	if (v.x < x) x = v.x;
	if (v.y < y) y = v.y;
	if (v.z < z) z = v.z;
}

//////// quaternions ///////////////////////////////////

HOE_INLINE double Quat::Magnitude(void) const
{
	return sqrt( w*w + x*x + y*y + z*z);
}

HOE_INLINE void Quat::Normalize(void)
{
	float mag = (float)Magnitude();
	w = w / mag;
	x = x / mag;
	y = y / mag;
	z = z / mag;
}

HOE_INLINE void Quat::GetMatrix(Matrix * m) const 
{
	m->_11  = 1 - 2 * ( y*y + z*z );
    m->_12  =     2 * ( x*y - z*w );
    m->_13  =     2 * ( x*z + y*w );    
	m->_21  =     2 * ( x*y + z*w );    
	m->_22  = 1 - 2 * ( x*x + z*z );    
	m->_23  =     2 * ( y*z - x*w );    
	m->_31  =     2 * ( x*z - y*w );    
	m->_32  =     2 * ( y*z + x*w );    
	m->_33 = 1 - 2 * ( x*x + y*y );    
	m->_14  = m->_24 = m->_34 = m->_41 = m->_42 = m->_43 = 0.f;    
	m->_44 = 1.f;
}

//////// Matrix ////////////////////////////////////////
HOE_INLINE void Matrix::Identity(void)
{
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f;
}

HOE_INLINE void Matrix::RotationX(const float angle)
{
	_12 = _13 = _14 = 0.0f;   
	_21 = _24 = 0.0f;	
	_31 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _44 = 1.0f;
	_22 = _33 = (float)cosf( angle );
	_23 = -(_32 = (float)sinf( angle ));
}


HOE_INLINE void Matrix::RotationY(const float angle)
{
    
    _12 = _14 = 0.0f;	
	_21 = _23 = _24 = 0.0f;
	_32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_22 = _44 = 1.0f;
	_11 = _33 = (float)cosf( angle );
	_13 = -(_31 = (float)sinf( angle ));

}

HOE_INLINE void Matrix::RotationZ(const float angle)
{
    
    _13 = _14 = 0.0f;
	_23 = _24 = 0.0f;	
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_33 = _44 = 1.0f;
	_11 = _22 = (float)cosf( angle );
	_12 = -(_21 = (float)sinf( angle ));

}

HOE_INLINE void Matrix::Scale(const float s)
{
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f; 
	_11 = _22 = _33 = s;
	_44 = 1.0f;

}

HOE_INLINE void Matrix::Scale(const float sx,const float sy,const float sz)
{
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f; 
	_11 = sx;
	_22 = sy;
	_33 = sz;
	_44 = 1.0f;

}

HOE_INLINE void Matrix::Scale(const Vector3 &s)
{
    _12 = _13 = _14 = _41 = 0.0f;
    _21 = _23 = _24 = _42 = 0.0f;
    _31 = _32 = _34 = _43 = 0.0f; 
	_11 = s.x;
	_22 = s.y;
	_33 = s.z;
	_44 = 1.0f;

}

HOE_INLINE void Matrix::Translate(const float x,const float y,const float z)
{
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = 0.0f;
    _21 = _23 = _24 = 0.0f;
    _31 = _32 = _34 = 0.0f;
	_41 = x;
	_42 = y;
	_43 = z;

}

HOE_INLINE void Matrix::Translate(const Vector3 &v)
{
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = 0.0f;
    _21 = _23 = _24 = 0.0f;
    _31 = _32 = _34 = 0.0f;
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

HOE_INLINE void Matrix::Adjung(const Matrix &m)
{
	_11 = m._22 * m._33 * m._44 + m._23 * m._34 * m._42 + m._32 * m._43 * m._24 - m._24 * m._33 * m._42 - m._23 * m._32 * m._44 - m._22 * m._34 * m._43;
	_12 = - m._12 * m._33 * m._44 - m._13 * m._34 * m._42 - m._32 * m._43 * m._14 + m._14 * m._33 * m._42 + m._13 * m._32 * m._44 + m._12 * m._34 * m._43;
	_13 = m._12 * m._23 * m._44 + m._13 * m._24 * m._42 + m._22 * m._43 * m._14 - m._14 * m._23 * m._42 - m._13 * m._22 * m._44 - m._12 * m._24 * m._43;
	_14 = - m._12 * m._23 * m._34 - m._13 * m._24 * m._32 - m._22 * m._33 * m._14 + m._14 * m._23 * m._32 + m._13 * m._22 * m._34 + m._12 * m._24 * m._33;
	_21 = - m._21 * m._33 * m._44 - m._23 * m._34 * m._41 - m._31 * m._43 * m._24 + m._24 * m._33 * m._41 + m._23 * m._31 * m._44 + m._21 * m._34 * m._43;
	_22 = m._11 * m._33 * m._44 + m._13 * m._34 * m._41 + m._31 * m._43 * m._14 - m._14 * m._33 * m._41 - m._13 * m._31 * m._44 - m._11 * m._34 * m._43;
	_23 = - m._11 * m._23 * m._44 - m._13 * m._24 * m._41 - m._21 * m._43 * m._14 + m._14 * m._23 * m._41 + m._13 * m._21 * m._44 + m._11 * m._24 * m._43;
	_24 = m._11 * m._23 * m._34 + m._13 * m._24 * m._31 + m._21 * m._33 * m._14 - m._14 * m._23 * m._31 - m._13 * m._21 * m._34 - m._11 * m._24 * m._33;
	_31 = m._21 * m._32 * m._44 + m._22 * m._34 * m._41 + m._31 * m._42 * m._24 - m._24 * m._32 * m._41 - m._22 * m._31 * m._44 - m._21 * m._34 * m._42;
	_32 = - m._11 * m._32 * m._44 - m._12 * m._34 * m._41 - m._31 * m._42 * m._14 + m._14 * m._32 * m._41 + m._12 * m._31 * m._44 + m._11 * m._34 * m._42;
	_33 = m._11 * m._22 * m._44 + m._12 * m._24 * m._41 + m._21 * m._42 * m._14 - m._14 * m._22 * m._41 - m._12 * m._21 * m._44 - m._11 * m._24 * m._42;
	_34 = - m._11 * m._22 * m._34 - m._12 * m._24 * m._31 - m._21 * m._32 * m._14 + m._14 * m._22 * m._31 + m._12 * m._21 * m._34 + m._11 * m._24 * m._32;
	_41 = - m._21 * m._32 * m._43 - m._22 * m._33 * m._41 - m._31 * m._42 * m._23 + m._23 * m._32 * m._41 + m._22 * m._31 * m._43 + m._21 * m._33 * m._42;
	_42 = m._11 * m._32 * m._43 + m._12 * m._33 * m._41 + m._31 * m._42 * m._13 - m._13 * m._32 * m._41 - m._12 * m._31 * m._43 - m._11 * m._33 * m._42;
	_43 = - m._11 * m._22 * m._43 - m._12 * m._23 * m._41 - m._21 * m._42 * m._13 + m._13 * m._22 * m._41 + m._12 * m._21 * m._43 + m._11 * m._23 * m._42;
	_44 = m._11 * m._22 * m._33 + m._12 * m._23 * m._31 + m._21 * m._32 * m._13 - m._13 * m._22 * m._31 - m._12 * m._21 * m._33 - m._11 * m._23 * m._32;

}

HOE_INLINE void Matrix::Transpoze(const Matrix &m)
{
	_11 = m._11;_12 = m._21; _13 = m._31; _14 = m._41;
	_21 = m._12;_22 = m._22; _23 = m._32; _24 = m._42;
	_31 = m._13;_32 = m._23; _33 = m._33; _34 = m._43;
	_41 = m._14;_42 = m._24; _43 = m._34; _44 = m._44;
}

HOE_INLINE void Matrix::Transpoze()
{
	Matrix m = *this;
	Transpoze(m);
}

HOE_INLINE float Matrix::Inverse( const Matrix &m)
{
	Adjung(m);

	const float det = m._11 * _11 + m._12 * _21 + m._13 * _31 + m._14 * _41;

	if (det == 0)
		return 0;

	_11 = _11 / det;
	_12 = _12 / det;
	_13 = _13 / det;
	_14 = _14 / det;
	_21 = _21 / det;
	_22 = _22 / det;
	_23 = _23 / det;
	_24 = _24 / det;
	_31 = _31 / det;
	_32 = _32 / det;
	_33 = _33 / det;
	_34 = _34 / det;
	_41 = _41 / det;
	_42 = _42 / det;
	_43 = _43 / det;
	_44 = _44 / det;

	return det;
}

HOE_INLINE void Matrix::Camera(const Vector3 &pos,const Vector3 &look)
{

	Vector3 pos2(-pos.x,-pos.y,-pos.z);

	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;	

	_13 = look.x;
	_23 = look.y;
	_33 = look.z;
	_43 = HoeDot(look,pos2);

	const float rvl = (float)sqrt((look.z*look.z)+(look.x*look.x));

	_11 = look.z / rvl;
	_21 = 0;	
	_31 = -look.x / rvl;
	_41 = pos2.x * _11 + pos2.z * _31;

	_12 = look.y * _31;
	_22 = look.z * _11 - look.x * _31;
	_32 = - look.y * _11;
	_42 = pos2.x * _12 + pos2.y * _22 + pos2.z * _32;

}

HOE_INLINE void Matrix::Perspective(const float w,const float h,const float zn,const float zf)
{
	_11 = 2*zn/w;
	_12 = _13 = _14 = 0;
	_22 = 2*zn/h;
	_21 = _23 = _24 = 0;
	_33 = zf/(zf-zn);
	_31 = _32 = 0;
	_34 = 1.0f;
	_43 = zn*zf/(zn-zf);
	_41 = _42 = _44 = 0;

}

HOE_INLINE void Matrix::PerspectiveFov(const float fovX,const float a,const float zn,const float zf)
{
	const float w = 1/tanf(fovX/2);
	const float h = w/a;
	_11 = w;
	_12 = _13 = _14 = 0;
	_22 = h;
	_21 = _23 = _24 = 0;
	_33 = zf/(zf-zn);
	_31 = _32 = 0;
	_34 = 1.0f;
	_43 = zn*zf/(zn-zf);
	_41 = _42 = _44 = 0;

}

HOE_INLINE void Matrix::Ortho(const float w,const float h,const float zn,const float zf)
{
	_11 = 2/w;
	_12 = _13 = _14 = 0;
	_22 = 2/h;
	_21 = _23 = _24 = 0;
	_33 = 1.0f/(zf-zn);
	_31 = _32 = 0;
	_44 = 1.0f;
	_43 = zn/(zn-zf);
	_41 = _42 = _34 = 0;

}

HOE_INLINE void Matrix::Ortho(const float l,const float r,const float b,const float t,const float zn,const float zf)
{
	_11 = 2/(r-l);
	_12 = _13 = _14 = 0;
	_22 = 2/(t-b);
	_21 = _23 = _24 = 0;
	_33 = 1.0f/(zf-zn);
	_31 = _32 = 0;
	_34 = 0.0f;
	_43 = zn/(zn-zf);
	_41 = (l+r)/(l-r);
	_42 = (t+b)/(b-t);
	_44 = 1.0f;

}

HOE_INLINE void Matrix::Multiply(const Matrix &a,const Matrix &b)
{

	_11 = b._11 * a._11 + b._21 * a._12 + b._31 * a._13 + b._41 * a._14;
	_12 = b._12 * a._11 + b._22 * a._12 + b._32 * a._13 + b._42 * a._14;
	_13 = b._13 * a._11 + b._23 * a._12 + b._33 * a._13 + b._43 * a._14;
	_14 = b._14 * a._11 + b._24 * a._12 + b._34 * a._13 + b._44 * a._14;

	_21 = b._11 * a._21 + b._21 * a._22 + b._31 * a._23 + b._41 * a._24;
	_22 = b._12 * a._21 + b._22 * a._22 + b._32 * a._23 + b._42 * a._24;
	_23 = b._13 * a._21 + b._23 * a._22 + b._33 * a._23 + b._43 * a._24;
	_24 = b._14 * a._21 + b._24 * a._22 + b._34 * a._23 + b._44 * a._24;

	_31 = b._11 * a._31 + b._21 * a._32 + b._31 * a._33 + b._41 * a._34;
	_32 = b._12 * a._31 + b._22 * a._32 + b._32 * a._33 + b._42 * a._34;
	_33 = b._13 * a._31 + b._23 * a._32 + b._33 * a._33 + b._43 * a._34;
	_34 = b._14 * a._31 + b._24 * a._32 + b._34 * a._33 + b._44 * a._34;

	_41 = b._11 * a._41 + b._21 * a._42 + b._31 * a._43 + b._41 * a._44;
	_42 = b._12 * a._41 + b._22 * a._42 + b._32 * a._43 + b._42 * a._44;
	_43 = b._13 * a._41 + b._23 * a._42 + b._33 * a._43 + b._43 * a._44;
	_44 = b._14 * a._41 + b._24 * a._42 + b._34 * a._43 + b._44 * a._44;

}

HOE_INLINE void Matrix::Multiply(const Matrix &m)
{
	Matrix a = *this;
	Multiply(a,m);
}

HOE_INLINE void Matrix::MultiplyLeft(const Matrix &m)
{
	Matrix a = *this;
	Multiply(m,a);
}

HOE_INLINE void BoundingBox3::Set(const Vector3 & v)
{
	min = max = v;
	ball = (float)v.Magnitude();
}

HOE_INLINE void BoundingBox3::Add(const Vector3 & v)
{
	min.Min(v);
	max.Max(v);
	register float m = v.MagnitudeF();
	if (m > ball)
		ball = m;
}	
HOE_INLINE void BoundingBox3::Set(const BoundingBox3 & b)
{
	*this = b;
}

HOE_INLINE void BoundingBox3::Add(const BoundingBox3 & b)
{
	min.Min(b.min);
	max.Max(b.max);
	if (b.ball > ball)
		ball = b.ball;
}

HOE_INLINE void BoundingBox3::Compute(const Vector3 *first, dword numvert, uint stride)
{
	assert(first && numvert > 0 && "bad parameters BoundingBox::Compute");
	Set(*first);
	for (dword i=1; i < numvert;i++)
	{
		register const Vector3 * v = (const Vector3 *)(((const byte*)first)+stride*i);
		Add(*v);
	}
}

//////// func //////////////////////////////////////////

HOE_INLINE Matrix* HoeMatrixLookAtLH_(Matrix &out,const Vector3 &eye,const Vector3 &at,const Vector3 &up)
{
/*	Vector3 zaxis = at - eye;
	HoeNormal(zaxis);

	Vector3 xaxis;
	HoeCross(up, zaxis, zaxis);
	HoeNormal(xaxis);

	Vector3 yaxis;
	HoeCross(zaxis, xaxis, yaxis);

 xaxis.x           yaxis.x           zaxis.x          0
 xaxis.y           yaxis.y           zaxis.y          0
 xaxis.z           yaxis.z           zaxis.z          0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1*/
}

HOE_INLINE float HoeDot(const Vector3 &vec1,const Vector3 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

HOE_INLINE float HoeDot(const Vector2 &vec1,const Vector2 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

HOE_INLINE void HoeCross(const Vector3 &vec1, const Vector3 &vec2,Vector3 &cross)
{

	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	cross.x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
														
	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	cross.y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
														
	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	cross.z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));
}

HOE_INLINE const Vector3 HoeCross(const Vector3 &vec1, const Vector3 &vec2)
{
	Vector3 cross;
	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	cross.x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
														
	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	cross.y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
														
	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	cross.z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));
	return cross;
}

HOE_INLINE float HoeAngleBetweenVectorsF(const Vector2 &v1, const Vector2 &v2)
{							
	// Get the dot product of the vectors
	float dotProduct = HoeDot(v1, v2);				

	// Get the product of both of the vectors magnitudes
	float vectorsMagnitude = v1.MagnitudeF() * v2.MagnitudeF();

	// Return the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
	return( acos( dotProduct / vectorsMagnitude ) );
}

// colision

HOE_INLINE bool HoePointInBox(Vector3 &p,float minx,float miny, float minz, float maxx, float maxy, float maxz)
{
	return !(p.x < minx || p.x > maxx || p.y < miny || p.y > maxy || p.z < minz || p.z > maxz);
}

HOE_INLINE bool HoeSphereInBox(Vector3 &p,float radius,float minx,float miny, float minz, float maxx, float maxy, float maxz)
{
	return !((p.x+radius) < minx || (p.x-radius) > maxx || (p.y+radius) < miny || (p.y-radius) > maxy || (p.z+radius) < minz || (p.z-radius) > maxz);
}

HOE_INLINE void HoePlaneNormal(const Vector3 &a,const Vector3 &b, const Vector3 &c,Vector3 &normal)
{
	HoeCross(b-a,c-a,normal);

	normal.Normalize();
}

HOE_INLINE float HoePlaneDistance(Vector3 &normal, Vector3 &point)
{	
	// Use the Plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.
	// So, we come up with D = -(Ax + By + Cz)
														
	return  - ((normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z));
}

HOE_INLINE float HoeDistance(Vector2 &a, Vector3 &b)
{	
	const register float x = a.x-b.x;
	const register float y = a.y-b.y;
	return  sqrtf(x*x+y*y);
}
HOE_INLINE float HoeDistance(Vector3 &a, Vector3 &b)
{	
	const register float x = a.x-b.x;
	const register float y = a.y-b.y;
	const register float z = a.z-b.z;
	return  sqrtf(x*x+y*y+z*z);
}

HOE_INLINE int HoeClassifySphere(Vector3 &pos, 
				   Vector3 &normal, Vector3 &a, float radius, float &distance)
{
	// Here we use the famous distance formula to find the distance the center point
	// of the sphere is from the triangle's Plane.  
	distance = (normal.x * pos.x + normal.y * pos.y
		+ normal.z * pos.z + HoePlaneDistance(normal, a));

	if(distance >= 0 && distance < radius)
		return P_INTERSECTS;
	else if(distance >= radius)
		return P_FRONT;
	
	return P_BEHIND;
}

bool HoeInsidePolygon(Line2 * lines,int num_lines,const float x,const float y,int i1,int i2)
{
	for (int i=0;i < num_lines;i++)
	{
		if (i == i1 || i == i2)
			continue;
		if (lines[i].Compute(x,y) < 0.00000)
			return false;
	}
	return true;
}


HOE_INLINE bool HoeInsidePolygon(const Vector3 &intersection, const Vector3 &a,const Vector3 &b,const Vector3 &c)
{
	register double angle = 0.0;					
	
	angle += HoeAngleBetweenVectors(a-intersection, b-intersection);
	angle += HoeAngleBetweenVectors(b-intersection, c-intersection);
	angle += HoeAngleBetweenVectors(c-intersection, a-intersection);
											
	return (angle >= (MATCH_FACTOR * (2.0 * HOE_PI)));
		
}

HOE_INLINE void HoeGetCollisionOffset(Vector3 &normal, float radius, float distance,Vector3 &offset)
{
	offset = Vector3(0, 0, 0);

	if(distance > 0)
	{
		float distanceOver = radius - distance;
		offset = normal * distanceOver;
	}
	else 
	{
		float distanceOver = radius + distance;
		offset = normal * -distanceOver;
	}
}

HOE_INLINE bool HoeEdgePlanesCollision(Vector3 &center, 
						 Vector3 &a,Vector3 &b,Vector3 &c, Vector3 &normal, float radius)
{
	// Create some variables to hold each edge planes classification with the sphere
	int edge1 = 0, edge2 = 0, edge3 = 0;
	float distance1 = 0, distance2 = 0, distance3 = 0;

	//////////////// Calculate Edge 1 ////////////////

	// Get a vector from the edge of the triangle
	Vector3 vector = b - a;
	Vector3 edgenormal;
	// Get the vector perpendicular from the normal of the triangle and this edge vector
	HoeCross(vector, normal,edgenormal);

	// Before we can call this new vector a normal, we need to normalize it
	edgenormal.Normalize();

	// Find the classification of the sphere and the first Plane
	edge1 = HoeClassifySphere(center, edgenormal, a, radius, distance1);
	if (edge1 == P_BEHIND)
		return false;
	//////////////// Calculate Edge 2 ////////////////

	// Get a vector from the second edge of the triangle
	vector = c - b;

	// Get the vector perpendicular from the normal of the triangle and this edge vector
	HoeCross(vector, normal,edgenormal);

	// Before we can call this new vector a normal, we need to normalize it
	edgenormal.Normalize();

	// Find the classification of the sphere and the first Plane
	edge2 = HoeClassifySphere(center, edgenormal, a, radius, distance2);
	if (edge2 == P_BEHIND)
		return false;
	//////////////// Calculate Edge 3 ////////////////

	// Get a vector from the edge of the triangle
	vector = a - c;

	// Get the vector perpendicular from the normal of the triangle and this edge vector
	HoeCross(vector, normal,edgenormal);

	// Before we can call this new vector a normal, we need to normalize it
	edgenormal.Normalize();

	// Find the classification of the sphere and the first Plane
	edge3 = HoeClassifySphere(center, edgenormal, a, radius, distance3);
	if (edge2 == P_BEHIND)
		return false;
	
	if(edge1 == P_INTERSECTS && Abs(distance1) < radius/2.0f) 
		return true;
	else if(edge2 == P_INTERSECTS && Abs(distance2) < radius/2.0f) 
		return true;
	else if(edge3 == P_INTERSECTS && Abs(distance3) < radius/2.0f) 
		return true;

	return false;
}

#endif // _HE_MATH_INL__
