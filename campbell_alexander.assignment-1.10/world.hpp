
#pragma once

#include <vector>
using namespace std;

#include <string.h>
#include <assert.h>

#include "model.hpp"
#include "util.hpp"
#include "pc.hpp"
#include "item.hpp"

void world_init(World *w);
void world_push_message(World *w, string text,
		MessageSeverity severity = MessageSeverity::Info);

