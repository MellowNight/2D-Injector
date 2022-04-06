#pragma once
// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "d3d_hook.h"
#include "utils.h"

extern Hooks::JmpRipCode* present_hk;

void StartCheat();