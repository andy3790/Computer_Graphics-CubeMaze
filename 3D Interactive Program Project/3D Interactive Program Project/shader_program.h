#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "includer.h"

void InitShader();
void make_vertexShaders();
void make_fragmentShaders();
extern GLuint s_program;
extern GLuint vertexShader;
extern GLuint fragmentShader;
char* filetobuf(const char* file);