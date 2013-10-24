#ifndef TRIGGER_H_INCLUDED
#define TRIGGER_H_INCLUDED

//confirm that enums are allowed in C code
enum { TRIG_SLOW = 0, TRIG_MEDIUM, TRIG_FAST, TRIG_FASTEST };

int SetTrigger(int triggerSpeed, int delta);
double UpdateTrigger(int triggerid);
void CleanupTriggers();
#endif // TRIGGER_H_INCLUDED
