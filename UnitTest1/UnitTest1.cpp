#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab_11_5/Main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
    TEST_CLASS(TestDeleteCar)
    {
    public:
        TEST_METHOD(TestDDeleteCar)
        {
            vector<Car> cars;

            Car testCar1;
            testCar1.brand = "Ford";
            testCar1.type = "car";
            testCar1.licensePlate = "ABC123";
            testCar1.serviceYears = 3;
            testCar1.details.color = "Red";
            cars.push_back(testCar1);

            Car testCar2;
            testCar2.brand = "Volvo";
            testCar2.type = "truck";
            testCar2.licensePlate = "XYZ789";
            testCar2.serviceYears = 6;
            testCar2.details.payload = 5.7;
            cars.push_back(testCar2);

            deleteFromFile("cars.bin", 5);
        }
    };
}