// Ted Meyer
// Bathroom.h
// License: GPL v2

#ifndef BATHROOM_H
#define BATHROOM_H

typedef enum 
{
	MALE,
	FEMALE,
	NONE
} gender;


void initialize();
void finalize();

void enter(gender g);
void leave();





#endif