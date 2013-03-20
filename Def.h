#ifndef __DEF_H
#define __DEF_H

struct Point 
{//Point coordinate
	float x, y, z;
};

struct Vector 
{ //向量
	float x, y, z;

    Vector& operator += (const Vector &v2){//這三小
	    this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
	    return *this;
    }
};

inline Point operator + (const Point&p, const Point &v)
{
	Point p2 = {p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

inline Point operator * (const int a, const Point &v)
{
	Point p2 = {a*v.x, a* v.y, a*v.z };
	return p2;
}

inline Point operator + (const Point&p, const Vector &v)
{
	Point p2 = {p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

inline Point operator - (const Point&p, const Vector &v)
{
	Point p2 = {p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

inline Vector operator + (const Vector&v1, const Vector &v2)
{
	Vector v = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

inline Vector operator - (const Point&p1, const Point &p2)
{
	Vector v = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

inline Vector operator * (float c, const Vector &v)
{
	Vector v2 = {v.x *c, v.y * c, v.z * c };
	return v2;
}

inline Vector operator - (const Vector&v1, const Vector &v2)
{
	Vector v = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

inline float operator * (const Vector&v1, const Vector &v2 ) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float length(Vector v) //向量長度
{
   return sqrt(v*v);
}

inline void normalize(Vector v) //正交化
{
   float length = sqrt(v*v);
   v.x = v.x/length;
   v.y = v.y/length;
   v.z = v.z/length;
}

struct Color 
{
    float red, green, blue;

    inline Color & operator += (const Color &c2 ) {
	    this->red +=  c2.red;
        this->green += c2.green;
        this->blue += c2.blue;
	    return *this;
    }
};

inline Color operator * (const Color&c1, const Color &c2 ) 
{
	Color c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
	return c;
}

inline Color operator + (const Color&c1, const Color &c2 ) 
{
	Color c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
	return c;
}

inline Color operator * (float coef, const Color &c ) 
{
	Color c2 = {c.red * coef, c.green * coef, c.blue * coef};
	return c2;
}

struct material 
{
    float reflection;
	float red, green, blue;
};

struct Sphere 
{
	Point pos;
	float radius;
	Color color;
};

struct Light 
{
	Point pos;
	Color color;
};

struct Ray 
{
	Point start;
	Vector dir;
};

#endif //__DEF_H
