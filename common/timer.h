#ifndef TIMER_H
#define TIMER_H

#include "entity.h"

 /// @brief The Timer class is a timer that you can add as a child from an entity
class Timer : public Entity
{
public:
	/// @brief constructor
	Timer();

	/// @brief destructor
	virtual ~Timer();


	/// @brief update function
	/// @return void
	virtual void Update(float deltaTime);


	/// @brief method for starting the timer
	/// @return void
	void StartTimer();

	/// @brief method for pausing the timer
	/// @return void
	void PauseTimer();

	/// @brief method for resetting the timer and setting the second back to 0
	/// @return void
	void StopTimer();

	/// @brief method for starting over
	/// @return void
	void StartOverTimer();

	/// @brief seconds getter
	/// @return seconds
	float GetSeconds() { return _seconds; }

	bool isPlaying;

private:
	/// @brief Seconds in seconds
	float _seconds;

	/// @brief check if timer is paused
	bool _paused;

};

#endif /* TIMER_H */