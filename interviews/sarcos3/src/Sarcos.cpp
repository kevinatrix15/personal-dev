// Sarcos.cpp : Defines the entry point for the application.

#include <iostream>

struct Vec3
{
	double x, y, z;
};

double vecDot(const Vec3& v1, const Vec3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

void print(const Vec3& v)
{
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << "\n";
}

// The assumption made here is that the 3x3 matrix is
// formatted as such:
// [[v0.x, v1.x, v2.x]
//  [v0.y, v1.y, v2.y]
//  [v0.z, v1.z, v2.z]
// ]
struct Mat33
{
	Vec3 col[3];

	Mat33(const Vec3& v0, const Vec3& v1, const Vec3& v2) : col{v0, v1, v2}
	{
	}
};

void swap(double& aRef, double& bRef)
{
	double tmp = aRef;
	aRef = bRef;
	bRef = tmp;
}

// [[v0.x, v1.x, v2.x]        [[v0.x, v0.y, v0.z]
//  [v0.y, v1.y, v2.y]   =>    [v1.x, v1.y, v1.z]
//  [v0.z, v1.z, v2.z]         [v2.x, v2.y, v2.z]
// ]						  ]
void transMat(Mat33& matRef)
{
	swap(matRef.col[0].y, matRef.col[1].x);
	swap(matRef.col[0].z, matRef.col[2].x);
	swap(matRef.col[1].z, matRef.col[2].y);
}

void print(const Mat33& m)
{
	std::cout << m.col[0].x << ", " << m.col[1].x << ", " << m.col[2].x << "\n";
	std::cout << m.col[0].y << ", " << m.col[1].y << ", " << m.col[2].y << "\n";
	std::cout << m.col[0].z << ", " << m.col[1].z << ", " << m.col[2].z << "\n";
}

struct Node
{
	Mat33 data;
	Node* children;
	unsigned int numChildren;
};

void print(const Node& n)
{
	// TODO: consider passing in as Node*.
	// TODO: include check for nullptr
	print(n.data);
	// TODO: replace below with BFS / DFS call
	for (size_t i = 0; i < n.numChildren; ++i) {
		print(n.children[i]);
	}
}

int main()
{
	const Vec3 v1{ 1.0, 2.0, 3.0 };
	const Vec3 v2{ 2.0, 4.0, 6.0 };
	const auto r = vecDot(v1, v2);

	std::cout << "v1 dot v2: " << "\n";
	std::cout << "v1: ";
	print(v1);
	std::cout << "v2: ";
	print(v2);
	std::cout << "res: " << r << "\n";

	const Vec3 m1{ 1.0, 2.0, 3.0 };
	const Vec3 m2{ 4.0, 5.0, 6.0 };
	const Vec3 m3{ 7.0, 8.0, 9.0 };
	Mat33 m(m1, m2, m3);
	std::cout << "m: " << "\n";
	print(m);

	std::cout << "m': " << "\n";
	transMat(m);
	print(m);

	return 0;
}
