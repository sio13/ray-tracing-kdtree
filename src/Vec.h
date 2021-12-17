#pragma once

class Vec {
 public:  
  Vec(float x, float y, float z) : x_(x), y_(y), z_(z) {}
  Vec() : x_(0), y_(0), z_(0) {}

  float x() const { return x_; }
  float y() const { return y_; }
  float z() const { return z_; }

  Vec operator+=(const Vec& other);
  Vec operator-=(const Vec& other);
  Vec operator+(const Vec& other) const;
  Vec operator-(const Vec& other) const;
  Vec operator-() const;
  Vec operator*(float k) const;
  Vec operator/(float k) const;

  float length() const;
  Vec unit() const;
  float dot(const Vec& other) const;
  Vec cross(const Vec& other) const;

 private:
  float x_, y_, z_;
};