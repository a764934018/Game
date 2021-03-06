#include "car.h"
#include <algorithm>
#include <thread>
#include <chrono>
#include <graphics.h>
#include <time.h>
#include <conio.h>


int main(int, char**)
{
	initgraph(1024, 768);	
    setorigin(1024/2, 768/2);
    setaspectratio(1, -1);
	setcolor(RGB(0,255,0));

	Car car;

	std::chrono::duration<double> frameTime(0.0);
	std::chrono::duration<double> sleepAdjust(0.0);
 
	while (true)
	{
        BeginBatchDraw();
        setfillcolor(RGB(0,0,0));
        fillrectangle(-1024, -768, 1024, 768);

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		car.Step();

        FlushBatchDraw();

		// Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
		// using mm_pause or equivalent for the last millisecond.
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> target(1.0 / 60.0);
		std::chrono::duration<double> timeUsed = t2 - t1;
		std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
		if (sleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(sleepTime);
		}

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		frameTime = t3 - t1;

		// Compute the sleep adjustment using a low pass filter
		sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - frameTime);
	}

    closegraph();

	return 0;
}
