#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "bus_manager.h"
#include "input_redirection.h"
#include "query.h"
#include "test_runner.h"

bool operator==(const Query& lhs, const Query& rhs);

void Test_Pair_Vector_Map_Set_Ouput();

void Test_InputProcessing();

void Test_AddBus_GetAllBuses();

void Test_OstreamOperator_AllBusesResponse();

void Test_GetBusesForStop();

void Test_OstreamOperator_BusesForStopResponse();

void Test_GetStopsForBus();

void Test_OstreamOperator_StopsForBusResponse();

// функция "TestAll()" должна вызываться в самом начале функции "main"
// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll();
