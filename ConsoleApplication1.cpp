#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

#define M_PI 3.14159265358979323846

class Point3D {
public:
    double x, y, z;
};

class Vector3D {
public:
    double x, y, z;
};

class Curve3D {
public:
    virtual ~Curve3D() {}
    virtual Point3D getPoint(double t) const = 0;
    virtual Vector3D getDerivative(double t) const = 0;
};

class Circle : public Curve3D {
public:
    Circle(double radius) : radius(radius) {}

    Point3D getPoint(double t) const override {
        double x = radius * std::cos(t);
        double y = radius * std::sin(t);
        return { x, y, 0.0 };
    }

    Vector3D getDerivative(double t) const override {
        double dx_dt = -radius * std::sin(t);
        double dy_dt = radius * std::cos(t);
        return { dx_dt, dy_dt, 0.0 };
    }

    double getRadius() const {
        return radius;
    }

private:
    double radius;
};

class Ellipse : public Curve3D {
public:
    Ellipse(double radiusX, double radiusY) : radiusX(radiusX), radiusY(radiusY) {}

    Point3D getPoint(double t) const override {
        double x = radiusX * std::cos(t);
        double y = radiusY * std::sin(t);
        return { x, y, 0.0 };
    }

    Vector3D getDerivative(double t) const override {
        double dx_dt = -radiusX * std::sin(t);
        double dy_dt = radiusY * std::cos(t);
        return { dx_dt, dy_dt, 0.0 };
    }

private:
    double radiusX, radiusY;
};

class Helix : public Curve3D {
public:
    Helix(double radius, double step) : radius(radius), step(step) {}

    Point3D getPoint(double t) const override {
        double x = radius * std::cos(t);
        double y = radius * std::sin(t);
        double z = step * t / (2 * M_PI);
        return { x, y, z };
    }

    Vector3D getDerivative(double t) const override {
        double dx_dt = -radius * std::sin(t);
        double dy_dt = radius * std::cos(t);
        double dz_dt = step / (2 * M_PI);
        return { dx_dt, dy_dt, dz_dt };
    }

private:
    double radius, step;
};

int main() {
    std::vector<std::unique_ptr<Curve3D>> curves;

    for (int i = 0; i < 10; ++i) {
        double radius = static_cast<double>(rand()) / RAND_MAX * 10.0 + 1.0;
        double step = static_cast<double>(rand()) / RAND_MAX * 0.5 + 0.1;

        int choice = rand() % 3;
        switch (choice) {
        case 0:
            curves.push_back(std::make_unique<Circle>(radius));
            break;
        case 1:
            curves.push_back(std::make_unique<Ellipse>(radius, radius * 0.5));
            break;
        case 2:
            curves.push_back(std::make_unique<Helix>(radius, step));
            break;
        }
    }

    double t = M_PI / 4.0;
    for (const auto& curve : curves) {
        Point3D point = curve->getPoint(t);
        Vector3D derivative = curve->getDerivative(t);

        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")";
        std::cout << " Derivative: (" << derivative.x << ", " << derivative.y << ", " << derivative.z << ")\n";
    }

    std::vector<Circle*> circles;
    for (const auto& curve : curves) {
        if (auto circle = dynamic_cast<Circle*>(curve.get())) {
            circles.push_back(circle);
        }
    }

    std::sort(circles.begin(), circles.end(), [](const Circle* a, const Circle* b) {
        return a->getRadius() < b->getRadius();
        });

    double totalRadiusSum = 0.0;
    for (const auto& circle : circles) {
        totalRadiusSum += circle->getRadius();
    }

    std::cout << "Total sum of radii: " << totalRadiusSum << std::endl;

    return 0;
}
