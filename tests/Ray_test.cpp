#include "../Ray.hpp"
#include <catch2/catch_test_macros.hpp>

namespace ES{

    TEST_CASE("Ray Constructor and accessing", "[Ray]"){
        Ray<float> raw;
        Ray<float> full{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,6)};
        
        REQUIRE(full.origin.x() ==1.0f);
        REQUIRE(full.origin.y() == 2.0f);
        REQUIRE(full.origin.z() == 3.0f);
        REQUIRE(full.direction.x() == 4.0f);
        REQUIRE(full.direction.y() == 5.0f);
        REQUIRE(full.direction.z() == 6.0f);
    }

    TEST_CASE("Ray at", "[Ray]"){
        Ray<float> one_two_three{PointN<float,3>(1,2,3),VectorN<float,3>(1,2,3)};
        PointN<float,3> one(2,4,6);
        PointN<float,3> two(3,6,9);
        PointN<float,3> three(4,8,12);
        PointN<float,3> negative_jit(0,0,0);


        REQUIRE(one_two_three.at(0).almost_equal(one_two_three.origin));
        REQUIRE(one_two_three.at(1).almost_equal(one));
        REQUIRE(one_two_three.at(2).almost_equal(two));
        REQUIRE(one_two_three.at(3).almost_equal(three));
        REQUIRE(one_two_three.at(-1).almost_equal(negative_jit));
    }


    TEST_CASE("ray normalize", "[Ray]"){
        Ray<float> ray{PointN<float,3>(0,0,0),VectorN<float,3>(1,2,2)};
        Ray<float> ray2{PointN<float,3>(0,0,0),VectorN<float,3>(1/3.0f,2/3.0f,2/3.0f)};
        REQUIRE(ray.normalize().almost_equal(ray2));
        ray.normalize_in_place();
        REQUIRE(ray.almost_equal(ray2));
    }

    TEST_CASE("Ray operator==", "[Ray]"){
        Ray<float> r1{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,6)};
        Ray<float> r2{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,6)};
        Ray<float> r3{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,7)};
        REQUIRE(r1 == r2);
        REQUIRE_FALSE(r1 == r3);    
    }

    TEST_CASE("Ray almost_equal", "[Ray]"){
        Ray<float> r1{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,6)};
        Ray<float> r2{PointN<float,3>(1.000000001f,2,3),VectorN<float,3>(4,5,6.000000001f)};
        Ray<float> r3{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,10)};
        REQUIRE(r1.almost_equal(r2));
        REQUIRE_FALSE(r1.almost_equal(r3));
    }

    TEST_CASE("Ray transform", "[Ray]"){
        Ray<float> ray{PointN<float,3>(1,0,0),VectorN<float,3>(1,0,0)};
        Matrix<float,4> identity = Matrix<float,4>::identity();
        Ray<float> transformed = ray.transform(identity);
        REQUIRE(transformed.origin.almost_equal(PointN<float,3>(1,0,0)));
        REQUIRE(transformed.direction.almost_equal(VectorN<float,3>(1,0,0)));
    }

    TEST_CASE("Ray transform_in_place", "[Ray]"){
        Ray<float> ray{PointN<float,3>(1,0,0),VectorN<float,3>(1,0,0)};
        Matrix<float,4> identity = Matrix<float,4>::identity();
        ray.transform_in_place(identity);
        REQUIRE(ray.origin.almost_equal(PointN<float,3>(1,0,0)));
        REQUIRE(ray.direction.almost_equal(VectorN<float,3>(1,0,0)));
    }

    TEST_CASE("Ray translate", "[Ray]"){
        Ray<float> ray{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,6)};
        Ray<float> translated = ray.translate(VectorN<float,3>(1,1,1));
        REQUIRE(translated.origin.almost_equal(PointN<float,3>(2,3,4)));
        REQUIRE(translated.direction.almost_equal(VectorN<float,3>(4,5,6)));
    }

    TEST_CASE("Ray translate_in_place", "[Ray]"){
        Ray<float> ray{PointN<float,3>(1,2,3),VectorN<float,3>(4,5,6)};
        ray.translate_in_place(VectorN<float,3>(1,1,1));
        REQUIRE(ray.origin.almost_equal(PointN<float,3>(2,3,4)));
        REQUIRE(ray.direction.almost_equal(VectorN<float,3>(4,5,6)));
    }

    TEST_CASE("Ray scale", "[Ray]"){
        Ray<float> ray{PointN<float,3>(1,2,3),VectorN<float,3>(2,4,6)};
        Ray<float> scaled = ray.scale(2.0f);
        REQUIRE(scaled.origin.almost_equal(PointN<float,3>(1,2,3)));
        REQUIRE(scaled.direction.almost_equal(VectorN<float,3>(4,8,12)));
    }

    TEST_CASE("Ray scale_in_place", "[Ray]"){
        Ray<float> ray{PointN<float,3>(1,2,3),VectorN<float,3>(2,4,6)};
        ray.scale_in_place(2.0f);
        REQUIRE(ray.origin.almost_equal(PointN<float,3>(1,2,3)));
        REQUIRE(ray.direction.almost_equal(VectorN<float,3>(4,8,12)));
    }

    TEST_CASE("Ray isValid", "[Ray]"){
        Ray<float> valid{PointN<float,3>(0,0,0),VectorN<float,3>(1,0,0)};
        Ray<float> invalid{PointN<float,3>(0,0,0),VectorN<float,3>(0,0,0)};
        REQUIRE(valid.isValid());
        REQUIRE_FALSE(invalid.isValid());
    }

}