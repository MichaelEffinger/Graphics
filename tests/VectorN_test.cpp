#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../VectorN.hpp"

using namespace ES;

TEST_CASE("VectorN constructors", "[VectorN]") {
    Vector3<float> v1{1.0f, 2.0f, 3.0f};
    REQUIRE(v1[0] == 1.0f);
    REQUIRE(v1[1] == 2.0f);
    REQUIRE(v1[2] == 3.0f);

    VectorN<float,4>(5.0f,4.0f,5,4);
    VectorN<float,3>(5,4,3);

    Vector3<float>(3,2,1);

    Vector2<float> v2{4.0f, 5.0f};
    Vector3<float> v3{4.0f,5.0f,6.0f};
    Vector3<float> v3parameter(4.0f,5.0f,3.0f);
    Vector3<float> v3Int(4,5,6);

    REQUIRE(v3 == v3Int);

    Vector3<float> v35(v2, 6.0f);
    VectorN<float,10> v10(v3,4,5,6,7,8,9,10);

    REQUIRE(v35[0] == 4.0f);
    REQUIRE(v35[1] == 5.0f);
    REQUIRE(v35[2] == 6.0f);

    auto v4 = Vector3<float>{1.0f, 2.0f, 3.0f};
    auto v5 = Vector4<float>(v4, 4.0f); 
    REQUIRE(v5[3] == 4.0f);
}

TEST_CASE("VectorN accessors - x, y, z, w", "[VectorN]") {
    Vector4<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    
    REQUIRE(v.x() == 1.0f);
    REQUIRE(v.y() == 2.0f);
    REQUIRE(v.z() == 3.0f);
    REQUIRE(v.w() == 4.0f);
    
    v.x() = 10.0f;
    v.y() = 20.0f;
    v.z() = 30.0f;
    v.w() = 40.0f;
    
    REQUIRE(v.x() == 10.0f);
    REQUIRE(v.y() == 20.0f);
    REQUIRE(v.z() == 30.0f);
    REQUIRE(v.w() == 40.0f);
}

TEST_CASE("VectorN arithmetic operators", "[VectorN]") {
    Vector3<float> a{1.0f, 2.0f, 3.0f};
    Vector3<float> b{3.0f, 2.0f, 1.0f};
    Vector3<float> zero(0,0,0);
    
    auto c = a + b;
    REQUIRE(c[0] == 4.0f);
    REQUIRE(c[1] == 4.0f);
    REQUIRE(c[2] == 4.0f);
    REQUIRE(c.at(0) == 4.0f);
    REQUIRE(c.at(1) == 4.0f);
    REQUIRE(c.at(2) == 4.0f);
    REQUIRE(c.x() ==4.0f);
    REQUIRE(c.y() ==4.0f);
    REQUIRE(c.z() == 4.0f);

    auto d = c - b;
    REQUIRE(a == d);
   
    a += b;
    REQUIRE(a == c);
    
    auto e = -b;
    REQUIRE(e[0] == -3.0f);
    REQUIRE(e[1] == -2.0f);
    REQUIRE(e[2] == -1.0f);
    
    auto f = b * 2.0f;
    REQUIRE(f[0] == 6.0f);
    REQUIRE(f[1] == 4.0f);
    REQUIRE(f[2] == 2.0f);
    
    f /= 2.0f;
    REQUIRE(f == b);

    #ifdef NDEBUG
    auto g = e/0;
    REQUIRE(g==zero);
    #endif
}

TEST_CASE("VectorN scalar multiplication with scalar on left", "[VectorN]") {
    Vector3<float> v{1.0f, 2.0f, 3.0f};
    
    auto result = 2.0f * v;
    
    REQUIRE(result[0] == 2.0f);
    REQUIRE(result[1] == 4.0f);
    REQUIRE(result[2] == 6.0f);
}

TEST_CASE("VectorN -= operator", "[VectorN]") {
    Vector3<float> a{5.0f, 6.0f, 7.0f};
    Vector3<float> b{1.0f, 2.0f, 3.0f};
    
    a -= b;
    
    REQUIRE(a == Vector3<float>{4.0f, 4.0f, 4.0f});
}

TEST_CASE("VectorN *= operator", "[VectorN]") {
    Vector3<float> v{2.0f, 3.0f, 4.0f};
    
    v *= 3.0f;
    
    REQUIRE(v == Vector3<float>{6.0f, 9.0f, 12.0f});
}
    
TEST_CASE("VectorN dot and cross", "[VectorN]") {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};        
    REQUIRE(a.dot(b) == 0.0f);
    auto c = a.cross(b);
    REQUIRE(c == Vector3<float>{0.0f, 0.0f, 1.0f});        
    Vector3<float> d{0.0f, 1.0f, 0.0f};
    d.cross_in_place(a);
    REQUIRE(d == Vector3<float>{0.0f, 0.0f, -1.0f});
}
        
TEST_CASE("VectorN magnitude and normalize, almost equal, along with math::approx", "[VectorN], [math::approx]") {
    Vector3<float> a{3.0f, 4.0f, 0.0f};
    Vector3<float>b = a *.1f;
    b /= .1f;

    REQUIRE (a.almost_equal(b)==true);

    float five = 5.0f + 1e-6f;
    REQUIRE(ES::math::approx_equal(five, 5));

    REQUIRE(ES::math::approx_equal(a.magnitude(),5.0f));

    REQUIRE(ES::math::approx_equal(a.magnitude_squared(),25.0f));
       
    auto norm = a.normalize();
    REQUIRE(ES::math::approx_equal(norm.magnitude(),1.0f));
    
    a.normalize_in_place();
    REQUIRE(ES::math::approx_equal(a.magnitude(),1.0f));
}
    
TEST_CASE("VectorN hadamard and scalar operations", "[VectorN]") {
    Vector3<float> a{1.0f, 2.0f, 3.0f};
    Vector3<float> b{2.0f, 3.0f, 4.0f};
    Vector3<float>adouble = a*2;
    Vector3<float>asave = a;
    Vector3<float> zero(0.0f,0.0f,0.0f);
    
    auto c = a.hadamard_product(b);
    REQUIRE(c == Vector3<float>{2.0f, 6.0f, 12.0f});
    
    a.hadamard_product_in_place(b);
    REQUIRE(a == c);
    
    auto d = adouble.hadamard_divide(asave);
    REQUIRE(d == Vector3<float>{2.0f, 2.0f, 2.0f});
    
    REQUIRE(d == adouble.hadamard_divide_in_place(asave));
    
    #ifdef NDEBUG
    a.hadamard_divide_in_place(zero);
    REQUIRE(a==zero);
    #endif
}

TEST_CASE("VectorN angle between vectors", "[VectorN]") {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};
    
    float angle = a.angle(b);
   // REQUIRE(ES::math::approx_equal(angle, ES::math::pi<float> / 2.0f));
    
    Vector3<float> parallel{2.0f, 0.0f, 0.0f};
    angle = a.angle(parallel);
    REQUIRE(ES::math::approx_equal(angle, 0.0f));
    
    Vector3<float> opposite{-1.0f, 0.0f, 0.0f};
    angle = a.angle(opposite);
   // REQUIRE(ES::math::approx_equal(angle, ES::math::pi<float>));
}
    
TEST_CASE("VectorN << operator", "[VectorN]") {
    Vector2<float> v2{1.0f, 2.0f};
    auto v3 = v2 << 3.0f;
    REQUIRE(v3 == Vector3<float>{1.0f, 2.0f, 3.0f});
}
       
TEST_CASE("VectorN type conversion", "[VectorN]") {
    Vector3<float> v3{1.0f, 2.0f, 3.0f};

    Vector2<float> v2 = static_cast<VectorN<float,2>>(v3);

    REQUIRE(v2 == Vector2<float>{1.0f, 2.0f});
    
    auto v5 = static_cast<VectorN<float, 5>>(v3);
    REQUIRE(v5[0] == 1.0f);
    REQUIRE(v5[1] == 2.0f);
    REQUIRE(v5[2] == 3.0f);
    REQUIRE(v5[3] == 0.0f);
    REQUIRE(v5[4] == 0.0f);
}
    
TEST_CASE("VectorN clamp and almost_equal", "[VectorN]") {
    Vector3<float> v{1.0f, -1.0f, 5.0f};
    auto clamped = v.clamp(-0.5f, 2.0f);
    REQUIRE(clamped == Vector3<float>{1.0f, -0.5f, 2.0f});
    
    v.clamp_in_place(-1.0f, 1.0f);
    REQUIRE(v == Vector3<float>{1.0f, -1.0f, 1.0f});
    
    Vector3<float> a{0.0f, 0.1f, 0.2f};
    Vector3<float> b{0.0f, 0.1f, 0.2f};
    REQUIRE(a.almost_equal(b, 0.01f));
}
    
TEST_CASE("VectorN projection, reflection, refraction", "[VectorN]") {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};
    
    auto proj = a.project_onto(b);
    REQUIRE(proj == Vector3<float>{0.0f, 0.0f, 0.0f});
    
    Vector3<float> n{0.0f, 1.0f, 0.0f};
    auto refl = a.reflect_safe(n);
    REQUIRE(refl == Vector3<float>{1.0f, 0.0f, 0.0f});  
    
    Vector3<float> incoming{0.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 1.0f, 0.0f};
    auto refr = incoming.refract_safe(normal, 1.0f, 1.5);
    REQUIRE(refr.magnitude() <= 1.0f);
}

TEST_CASE("VectorN lerp - linear interpolation", "[VectorN]") {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{10.0f, 20.0f, 30.0f};
    
    auto mid = a.lerp(b, 0.5f);
    REQUIRE(mid == Vector3<float>{5.0f, 10.0f, 15.0f});
    
    auto start = a.lerp(b, 0.0f);
    REQUIRE(start == a);
    
    auto end = a.lerp(b, 1.0f);
    REQUIRE(end == b);
    
    auto quarter = a.lerp(b, 0.25f);
    REQUIRE(quarter == Vector3<float>{2.5f, 5.0f, 7.5f});
}

TEST_CASE("VectorN lerp_in_place - in-place linear interpolation", "[VectorN]") {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{10.0f, 20.0f, 30.0f};
    
    a.lerp_in_place(b, 0.5f);
    REQUIRE(a == Vector3<float>{5.0f, 10.0f, 15.0f});
}

TEST_CASE("VectorN reflect - reflection across unit normal", "[VectorN]") {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    incoming.normalize_in_place();
    Vector3<float> normal{0.0f, 1.0f, 0.0f};
    
    auto reflected = incoming.reflect(normal);
    
    REQUIRE(ES::math::approx_equal(reflected.y(), incoming.y() * -1.0f));
    REQUIRE(ES::math::approx_equal(reflected.magnitude(), incoming.magnitude()));
}

TEST_CASE("VectorN reflect_in_place - in-place reflection", "[VectorN]") {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    incoming.normalize_in_place();
    float orig_y = incoming.y();
    Vector3<float> normal{0.0f, 1.0f, 0.0f};
    
    incoming.reflect_in_place(normal);
    
    REQUIRE(ES::math::approx_equal(incoming.y(), orig_y * -1.0f));
}

TEST_CASE("VectorN reflect_safe - safe reflection with normalization", "[VectorN]") {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 5.0f, 0.0f};
    
    auto reflected = incoming.reflect_safe(normal);
    
    REQUIRE(reflected.magnitude() > 0.0f);
}

TEST_CASE("VectorN reflect_in_place_safe - safe in-place reflection", "[VectorN]") {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 5.0f, 0.0f}; 
    
    incoming.reflect_in_place_safe(normal);
    
    REQUIRE(incoming.magnitude() > 0.0f);
}

TEST_CASE("VectorN refract - refraction across interface", "[VectorN]") {
    Vector3<float> incoming{0.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 1.0f, 0.0f};
    
    auto refracted = incoming.refract(normal, 1.0f, 1.5f);
    
    REQUIRE(refracted.magnitude() > 0.0f);
    
    Vector3<float> steep{0.9f, -0.1f, 0.0f};
    steep.normalize_in_place();
    auto tir = steep.refract(normal, 1.5f, 1.0f);
    REQUIRE(tir.magnitude_squared() >= 0.0f);
}

TEST_CASE("VectorN refract_in_place - in-place refraction", "[VectorN]") {
    Vector3<float> incoming{0.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 1.0f, 0.0f};
    
    incoming.refract_in_place(normal, 1.0f, 1.5f);
    
    REQUIRE(incoming.magnitude() > 0.0f);
}

TEST_CASE("VectorN refract_safe - safe refraction with normalization", "[VectorN]") {
    Vector3<float> incoming{0.0f, -5.0f, 0.0f}; 
    Vector3<float> normal{0.0f, 3.0f, 0.0f}; 
    
    auto refracted = incoming.refract_safe(normal, 1.0f, 1.5f);
    
    REQUIRE(refracted.magnitude() >= 0.0f);
}

TEST_CASE("VectorN refract_in_place_safe - safe in-place refraction", "[VectorN]") {
    Vector3<float> incoming{0.0f, -5.0f, 0.0f}; 
    Vector3<float> normal{0.0f, 3.0f, 0.0f};
    
    incoming.refract_in_place_safe(normal, 1.0f, 1.5f);
    
    REQUIRE(incoming.magnitude() >= 0.0f);
}

TEST_CASE("VectorN distance - distance between vectors", "[VectorN]") {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{3.0f, 4.0f, 0.0f};
    
    float dist = a.distance(b);
    
    REQUIRE(ES::math::approx_equal(dist, 5.0f));
}

TEST_CASE("VectorN distance_squared - squared distance", "[VectorN]") {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{3.0f, 4.0f, 0.0f};
    
    float distSq = a.distance_squared(b);
    
    REQUIRE(ES::math::approx_equal(distSq, 25.0f));
}

TEST_CASE("VectorN project_onto - vector projection", "[VectorN]") {
    Vector3<float> a{3.0f, 4.0f, 0.0f};
    Vector3<float> b{1.0f, 0.0f, 0.0f};
    
    auto proj = a.project_onto(b);
    
    REQUIRE(proj == Vector3<float>{3.0f, 0.0f, 0.0f});
}

TEST_CASE("VectorN project_onto_in_place - in-place projection", "[VectorN]") {
    Vector3<float> a{3.0f, 4.0f, 0.0f};
    Vector3<float> b{1.0f, 0.0f, 0.0f};
    
    a.project_onto_in_place(b);
    
    REQUIRE(a == Vector3<float>{3.0f, 0.0f, 0.0f});
}

TEST_CASE("VectorN slerp - spherical linear interpolation", "[VectorN]") {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};
    
    auto mid = a.slerp(b, 0.5f);
    
    REQUIRE(ES::math::approx_equal(mid.magnitude(), 1.0f));
    REQUIRE(mid.x() > 0.0f);
    REQUIRE(mid.y() > 0.0f);
    
    auto start = a.slerp(b, 0.0f);
    REQUIRE(ES::math::approx_equal(start.x(), a.x()));
    
    auto end = a.slerp(b, 1.0f);
    REQUIRE(ES::math::approx_equal(end.y(), b.y()));
}

TEST_CASE("VectorN zero - static zero vector creation", "[VectorN]") {
    auto zero3 = Vector3<float>::zero();
    
    REQUIRE(zero3.x() == 0.0f);
    REQUIRE(zero3.y() == 0.0f);
    REQUIRE(zero3.z() == 0.0f);
    
    auto zero4 = Vector4<double>::zero();
    
    REQUIRE(zero4.x() == 0.0);
    REQUIRE(zero4.y() == 0.0);
    REQUIRE(zero4.z() == 0.0);
    REQUIRE(zero4.w() == 0.0);
}

TEST_CASE("VectorN comprehensive edge cases", "[VectorN]") {
    Vector3<float> zero{0.0f, 0.0f, 0.0f};
    auto normalized = zero.normalize();
    REQUIRE(normalized == zero);
    
    #ifdef NDEBUG
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    float angle = a.angle(zero);
    REQUIRE(angle == 0.0f);
    #endif
}