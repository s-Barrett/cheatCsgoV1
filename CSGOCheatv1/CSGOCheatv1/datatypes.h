// Basic 2D vector structure for screen coordinates
struct Vec2 {
    float x; // Lowercase for consistency
    float y;

    Vec2() : x(0.0f), y(0.0f) {} // Default constructor
    Vec2(float _x, float _y) : x(_x), y(_y) {} // Parameterized constructor
};

// 3D vector structure for world coordinates
struct Vec3 {
    float x;
    float y; 
    float z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// 4x4 matrix for view transformations
struct ViewMatrix {
    float matrix[16]{}; // Lowercase for consistency

    ViewMatrix() {} // Default constructor
    
    // Helper to access elements
    float& operator[](int i) {
        return matrix[i];
    }
    
    const float& operator[](int i) const {
        return matrix[i];
    }
};
