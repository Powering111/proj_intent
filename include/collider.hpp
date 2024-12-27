#pragma once
#include "raylib.h"
#include "raymath.h"

class PointCollider;
class LineCollider;
class CircleCollider;


class Collider{
public:
    virtual bool collide(Collider const& other) const = 0;
protected:
    virtual bool collide(PointCollider const& other) const = 0;
    virtual bool collide(LineCollider const& other) const = 0;
    virtual bool collide(CircleCollider const& other) const = 0;

    friend class PointCollider;
    friend class LineCollider;
    friend class CircleCollider;
};

class PointCollider: public Collider{
public:
    Vector2 position;

    PointCollider(Vector2 c_position): position(c_position) {};
    
    bool collide(Collider const& other) const {
        return other.collide(*this);
    }
protected:
    friend class LineCollider;
    friend class CircleCollider;
    bool collide(PointCollider const& other) const;
    bool collide(LineCollider const& other) const;
    bool collide(CircleCollider const& other) const;
};


class LineCollider: public Collider{
public:
    Vector2 start, end;
    LineCollider(Vector2 c_start, Vector2 c_end): start(c_start), end(c_end) {};
    bool collide(Collider const& other) const {
        return other.collide(*this);
    }
protected:
    friend class PointCollider;
    friend class CircleCollider;

    bool collide(PointCollider const& other) const;
    bool collide(LineCollider const& other) const;
    bool collide(CircleCollider const& other) const;
};


class CircleCollider: public Collider{
public:
    Vector2 center;
    float radius;
    CircleCollider(Vector2 c_center, float c_radius): center(c_center), radius(c_radius) {};
    bool collide(Collider const& other) const {
        return other.collide(*this);
    }
protected:
    friend class PointCollider;
    friend class LineCollider;

    bool collide(PointCollider const& other) const;
    bool collide(LineCollider const& other) const;
    bool collide(CircleCollider const& other) const;
};