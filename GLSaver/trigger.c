#include "trigger.h"

typedef struct tagtrigger
{
    double _value;
	int _speed;
    int _delta;
    int _timer;
    int _flip;  //for now all our triggers will flip independently
} _trigger, *_lp_trigger;

_lp_trigger _triggers = 0;
int _max_triggers = 10;
int _current_trigger = 0;

int SetTrigger(int triggerSpeed, int delta)
{
    if (_triggers == 0)
        _triggers = (_lp_trigger)malloc(_max_triggers * sizeof(_trigger));

    if (_current_trigger < _max_triggers)
    {
        _triggers[_current_trigger]._delta = delta;
        _triggers[_current_trigger]._timer = 0;
        _triggers[_current_trigger]._value = (double)0;
        _triggers[_current_trigger]._flip = 0;
		_triggers[_current_trigger]._speed = triggerSpeed;

        int _returnval = _current_trigger;
        _current_trigger++;
        return _returnval;
    }
    return -1;
}

void UpdateTriggerValue(_lp_trigger _trig)
{
	double trigdelta = 0.0;
	switch (_trig->_speed)
	{
		case TRIG_SLOW:
			trigdelta = 0.006;
			break;
		case TRIG_MEDIUM:
			trigdelta = 0.009;
			break;
		case TRIG_FAST:
			trigdelta = 0.06;
			break;
		default:
			trigdelta = 0.05;
			break;
	}
        if (_trig->_flip)
            _trig->_value -= trigdelta;
        else
            _trig->_value += trigdelta;
}

double UpdateTrigger(int _id)
{
    if (_id >= 0 && _id < _max_triggers)
    {
        _triggers[_id]._timer++;
        if (_triggers[_id]._timer > _triggers[_id]._delta)
        {
            _triggers[_id]._flip = (_triggers[_id]._flip == 1 ? 0 : 1);
            _triggers[_id]._timer = 0;
        }

		UpdateTriggerValue(&_triggers[_id]);

        return _triggers[_id]._value;
    }

    return -1;
}

void CleanupTriggers()
{
    free(_triggers);
}
