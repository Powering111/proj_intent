#include "collider.hpp"


// TODO: handle divide by zero cases

bool PointCollider::collide(PointCollider const& other) const {
    return Vector2Equals(other.position, position);
}

bool PointCollider::collide(LineCollider const& other) const {
    // return Vector2LineAngle(Vector2Normalize(Vector2Subtract(other.start, position)), Vector2Normalize(Vector2Subtract(position, other.end))) == 0.0f;
    return false; // Let's say that they cannot collide.
}
bool PointCollider::collide(CircleCollider const& other) const {
    return Vector2Distance(position, other.center) <= other.radius;
}


bool LineCollider::collide(PointCollider const& other) const {
    return other.collide(*this);
}
bool LineCollider::collide(LineCollider const& other) const {
    return false; // TODO
}
bool LineCollider::collide(CircleCollider const& other) const {
    return (Vector2Distance(start, other.center) <= other.radius) ||
        (Vector2Distance(end, other.center) <= other.radius) ||
        (((abs((end.y-start.y)*other.center.x - (end.x-start.x)*other.center.y + end.x*start.y - end.y*start.x) / Vector2Distance(start, end)) <= other.radius) && // Distance between the line and point
            (abs(Vector2Angle(Vector2Subtract(start, other.center),Vector2Subtract(end, other.center))) >= 1.57079632679)
        ); // Angle larger than 90 degrees
}

bool CircleCollider::collide(PointCollider const& other) const {
    return other.collide(*this);
}
bool CircleCollider::collide(LineCollider const& other) const {
    return other.collide(*this);
}
bool CircleCollider::collide(CircleCollider const& other) const {
    return Vector2Distance(center, other.center) <= (radius + other.radius);
}