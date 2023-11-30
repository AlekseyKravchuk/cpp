#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility> // std::move
#include <vector>

#include "profile.h"

std::string MakeString();
void ProfileCopyAssignment_vs_Moving();
void ProfileInsertingToSet_Copying_vs_Moving();
void ProfileAddingKeyValuePairToEmptyMap_Copying_vs_Moving();