// Ted Meyer
// Bathroom.h
// License: GPL v2

#ifndef BATHROOM_H
#define BATHROOM_H

typedef enum 
{
	MALE,
	FEMALE
} gender;


void initialize();
void finalize();

void enter(gender g);
void leave();





#endif
