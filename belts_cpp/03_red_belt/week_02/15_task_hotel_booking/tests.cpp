#include "booking_manager.h"
#include "test_runner.h"

void Test_AddEvent() {
    {
        BookingManager_V2 manager;

        manager.MakeBooking(1, "Marriott"s, 1, 2);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("Marriott"s), 1u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("Marriott"s), 2u);

        manager.MakeBooking(1, "Marriott"s, 3, 1);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("Marriott"s), 2u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("Marriott"s), 3u);

        manager.MakeBooking(1, "Marriott"s, 7, 3);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("Marriott"s), 3u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("Marriott"s), 6u);

        manager.MakeBooking(1, "FourSeasons"s, 2, 4);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("FourSeasons"s), 1u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("FourSeasons"s), 4u);

        manager.MakeBooking(1, "FourSeasons"s, 3, 1);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("FourSeasons"s), 2u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("FourSeasons"s), 5u);

        manager.MakeBooking(1, "Azimut"s, 7, 2);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("Azimut"s), 1u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("Azimut"s), 2u);

        manager.MakeBooking(2, "Marriott"s, 3, 1);
        ASSERT_EQUAL(manager.GetNumberClientsBookedTimeAgo("Marriott"s), 3u);
        ASSERT_EQUAL(manager.GetNumberRoomsBookedTimeAgo("Marriott"s), 7u);
    }
}

void Test_RemoveRedundantEvents() {
}

void Test_UpdateSummaryStatisticsWithinTimeAgo() {
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, Test_AddEvent);
    RUN_TEST(tr, Test_RemoveRedundantEvents);
    RUN_TEST(tr, Test_UpdateSummaryStatisticsWithinTimeAgo);
}