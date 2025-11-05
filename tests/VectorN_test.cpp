#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../VectorN.hpp"
#include "../math.hpp"


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
    Vector3<float> v3Int(4,5,6); //construct float from ints

    REQUIRE(v3 == v3Int);

    Vector3<float> v35(v2, 6.0f); // constructor from smaller vector + extra
    VectorN<float,10> v10(v3,4,5,6,7,8,9,10);

    REQUIRE(v35[0] == 4.0f);
    REQUIRE(v35[1] == 5.0f);
    REQUIRE(v35[2] == 6.0f);

    auto v4 = Vector3<float>{1.0f, 2.0f, 3.0f};
    auto v5 = Vector4<float>(v4, 4.0f); 
    REQUIRE(v5[3] == 4.0f);

}


TEST_CASE("VectorN arithmetic operators", "[VectorN]") {
    Vector3<float> a{1.0f, 2.0f, 3.0f};
    Vector3<float> b{3.0f, 2.0f, 1.0f};
    Vector3<float> zero(0,0,0);
    
    auto c = a + b;
    REQUIRE(c[0] == 4.0f);
    REQUIRE(c[1] == 4.0f);
    REQUIRE(c[2] == 4.0f);
    
    
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

    // f/0;  //Should assert, And it does, that is correct 
    #ifdef NDEBUG
    auto g = e/0;
    REQUIRE(g==zero);
    #endif
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
    
    //d.hadamard_divide(zero); //This asserts, that is correct;
    #ifdef NDEBUG
    a.hadamard_divide_in_place(zero);
    REQUIRE(a==zero);
    #endif

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